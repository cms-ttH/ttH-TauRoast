import logging
import math
import os
import random

import ROOT as r

from ttH.TauRoast import stylish
from ttH.TauRoast.botany import Forest
from ttH.TauRoast.decorative import savetime
from ttH.TauRoast.legendary import Legend
from ttH.TauRoast.processing import BasicProcess, Process


class Plot(object):
    __plots = {}

    def __init__(self, name, values, labels, binning, binlabels=None, limitname=None, weights=None, blind=False, essential=False):
        self.__name = name
        self.__limitname = limitname if limitname else os.path.basename(name)
        self.__normalized = False
        self.__blind = blind
        self.__essential = essential

        self.__values = values
        self.__weights = weights

        self.__args = [self.__limitname, ";".join([""] + labels)] + binning
        self.__labels = binlabels
        self.__hists = {}

        self.__backgrounds_present = set()
        self.__signals_present = set()
        self.__collisions_present = set()

        if len(values) == 1:
            self.__class = r.TH1F
        elif len(values) == 2:
            self.__class = r.TH2F
        else:
            raise ValueError("Need to provide 1 or 2 values to '{0}'!".format(self.__limitname))

        if self.__limitname in Plot.__plots:
            raise KeyError("Plot {0} defined twice".format(self.__limitname))
        Plot.__plots[self.__limitname] = self

    def __str__(self):
        return self.__name

    def blind(self, b=None):
        if b is not None:
            self.__blind = b
        return self.__blind

    def essential(self):
        return self.__essential

    @property
    def limitname(self):
        return self.__limitname

    def _add_binlabels(self, hist):
        if self.__labels:
            if isinstance(self.__labels[0], list) or isinstance(self.__labels[0], tuple):
                for n, label in enumerate(self.__labels[0], 1):
                    hist.GetXaxis().SetBinLabel(n, label)
                for n, label in enumerate(self.__labels[1], 1):
                    hist.GetYaxis().SetBinLabel(n, label)
            else:
                for n, label in enumerate(self.__labels, 1):
                    hist.GetXaxis().SetBinLabel(n, label)

    def _add_legend(self, config, factor):
        l = Legend(0.05, 4, 0.03)
        if len(self.__backgrounds_present) > 0:
            l.draw_marker(20, r.kBlack, "Data")
            for cfg in config['backgrounds']:
                props = {'SetFillStyle': 1001}
                props.update(cfg)
                bkg = props.pop('process')
                if bkg not in self.__backgrounds_present:
                    continue
                l.draw_box({k: self._eval(v) for (k, v) in props.items()}, Process.get(bkg).fullname, centerline=True)
        if len(self.__signals_present) > 0:
            for cfg in config['signals']:
                sig, color = cfg.items()[0]
                if sig not in self.__signals_present:
                    continue
                label = Process.get(sig).fullname
                if factor != 1:
                    label += " (#times {0:.1f})".format(factor)
                l.draw_line(2, self._eval(color), label)
                l.new_row()
        return l

    def _eval(self, color):
        if not isinstance(color, basestring):
            return color
        return eval(color, {}, {'r': r})

    def _get_histogram(self, process, systematic=None):
        suffix = '_' + systematic if systematic else ''
        if isinstance(process, Process):
            proc = process
            process = str(process)
        else:
            proc = Process.get(process)
        if isinstance(proc, BasicProcess):
            return self.__hists[process + suffix].Clone()
        hist = None
        for p in proc.subprocesses:
            h = self._get_histogram(p, systematic)
            if hist:
                hist.Add(h, proc.factor)
            else:
                hist = h.Clone()
        if not hist:
            raise KeyError(process)
        return hist

    def _get_background_shifts(self, config, systematics, direction):
        central = self._get_background_sum(config)
        result = [0] * central.GetNbinsX()
        for systematic in systematics:
            central, error = self._get_background_sum(config, systematic + direction)
            for n in range(len(result)):
                result[n] += (central.GetBinContent(n + 1) -
                              error.GetBinContent(n + 1)) ** 2
        return result

    def _get_background_errors(self, config, systematics):
        central = self._get_background_sum(config)
        abs_err = r.TGraphAsymmErrors(central)
        rel_err = r.TGraphAsymmErrors(central)

        err_up = self._get_background_shifts(config, systematics, 'Up')
        err_down = self._get_background_shifts(config, systematics, 'Down')

        for i in range(central.GetNbinsX()):
            bin_center = central.GetBinCenter(i + 1)
            bin_content = central.GetBinContent(i + 1)
            bin_error = central.GetBinError(i + 1)
            bin_width = central.GetBinWidth(i + 1)

            if bin_content > 0.001:
                rel_up = math.sqrt(err_up[i] + bin_error ** 2) / bin_content
                rel_down = math.sqrt(err_down[i] + bin_error ** 2) / bin_content
                # Systematical errors only
                # rel_up = math.sqrt(err_up[i]) / bin_content
                # rel_down = math.sqrt(err_down[i]) / bin_content
            else:
                rel_up = 0
                rel_down = 0

            abs_err.SetPoint(i, bin_center, bin_content)
            rel_err.SetPoint(i, bin_center, 1)

            abs_err.SetPointEXlow(i, bin_width / 2)
            abs_err.SetPointEXhigh(i, bin_width / 2)
            abs_err.SetPointEYlow(i, math.sqrt(err_down[i]))
            abs_err.SetPointEYhigh(i, math.sqrt(err_up[i]))

            rel_err.SetPointEXlow(i, bin_width / 2)
            rel_err.SetPointEXhigh(i, bin_width / 2)
            rel_err.SetPointEYlow(i, rel_down)
            rel_err.SetPointEYhigh(i, rel_up)
        return (abs_err, rel_err)

    def _get_background_sum(self, config, systematic=None):
        res = None
        err = None
        for cfg in config['backgrounds']:
            try:
                if systematic:
                    e = self._get_histogram(cfg['process'], systematic)
                    if e.Integral() == 0:
                        continue
                    if err is None:
                        err = e
                    else:
                        err.Add(e)
            except KeyError:
                pass
            try:
                h = self._get_histogram(cfg['process'])
                if res is None:
                    res = h
                else:
                    res.Add(h)
            except KeyError:
                pass
        if not res:
            args = list(self.__args)
            args[0] += "_bkg_sum_{0}".format(random.randint(0, 100000))
            res = self.__class(*args)
        if not err:
            err = res.Clone()
        res.SetFillStyle(1001)
        res.SetFillColor(r.kBlack)
        res.SetMarkerStyle(0)
        if systematic:
            return res, err
        return res

    def _get_backgrounds(self, config):
        res = r.THStack(self.__name + "_stack", self.__args[1])
        for cfg in config['backgrounds']:
            cfg = dict(cfg.items())
            background = cfg.pop('process')
            try:
                h = self._get_histogram(background)
                h.SetFillStyle(1001)
                h.SetLineWidth(1)
                h.SetLineColor(r.kBlack)
                for attr, val in cfg.items():
                    getattr(h, attr)(self._eval(val))
                res.Add(h)
                self.__backgrounds_present.add(background)
            except KeyError:
                pass
        return res

    def _get_data(self, config):
        res = []
        for cfg in config['data']:
            data, color = cfg.items()[0]
            try:
                h = self._get_histogram(data)
                h.SetLineColor(self._eval(color))
                h.SetLineWidth(3)
                h.SetMarkerStyle(20)
                h.SetMarkerSize(1)
                res.append(h)
                self.__collisions_present.add(data)
            except KeyError:
                pass
        return res

    def _get_signals(self, config):
        res = []
        for cfg in config['signals']:
            signal, color = cfg.items()[0]
            try:
                h = self._get_histogram(signal)
                h.SetLineColor(self._eval(color))
                h.SetLineWidth(4)
                res.append(h)
                self.__signals_present.add(signal)
            except KeyError:
                pass
        return res

    def _get_scale_factor(self, background, signals):
        bsum = background.Integral()
        ssum = max([sig.Integral() for sig in signals] + [0])

        return (bsum / float(ssum) if ssum > 0 and bsum > 0 else 1)

    def _normalize(self, cutflows):
        if self.__normalized:
            return
        self.__normalized = True
        for fullname, hist in self.__hists.items():
            if fullname.endswith('Up') or fullname.endswith('Down'):
                name, _ = fullname.rsplit('_CMS', 1)
                proc = Process.get(name)
            else:
                proc = Process.get(fullname)

            logging.debug("normalizing histogram {0}, process {1}".format(self.__name, proc))
            denom = float(cutflows[proc.cutflow][-3][proc])
            factor = 0. if denom == 0. else cutflows[proc.cutflow][-1][proc] / denom
            hist.Scale(factor)

    def _normalize_to_unity(self, hists):
        for h in hists:
            norm = h.Integral()
            if norm != 0.:
                h.Scale(1. / h.Integral())

    def read(self, file, category, procs, systematics=None, fmt="{p}_{c}_{v}"):
        if systematics is None or not self.__essential:
            systematics = []
        systematics = set(systematics + ['NA'])

        suffixes = []
        for systematic in systematics:
            if systematic == 'NA':
                suffixes.append('')
            else:
                suffixes.append('_{}Up'.format(systematic))
                suffixes.append('_{}Down'.format(systematic))

        for proc in procs:
            for suffix in suffixes:
                histname = fmt.format(p=proc.limitname, v=self.__limitname, c=category)
                histname += suffix
                logging.debug("reading histogram {0}".format(histname))
                h = file.Get(histname)
                try:
                    h.SetDirectory(0)
                    self.__hists[str(proc) + suffix] = h
                except AttributeError:
                    if suffix == '':
                        logging.warning("histogram {} not found in file {}".format(histname, file.GetName()))

    def write(self, file, cutflows, category, systematics=None, procs=None, fmt="{p}_{c}_{v}"):
        self._normalize(cutflows)

        if systematics is None:
            systematics = []
        systematics = set(systematics + ['NA'])

        uncertainties = []
        for systematic in systematics:
            if systematic == 'NA':
                uncertainties.append((None, ''))
            else:
                uncertainties.append((systematic + 'Up', '_{}Up'.format(systematic)))
                uncertainties.append((systematic + 'Down', '_{}Down'.format(systematic)))

        if procs is None:
            procs = [Process.get(k) for k in self.__hists if
                     (not k.endswith('Up')) and (not k.endswith('Down'))]
        else:
            procs = map(Process.get, procs)

        for proc in procs:
            for uncertainty, suffix in uncertainties:
                histname = fmt.format(p=proc.limitname, v=self.__limitname, c=category)
                histname += suffix
                logging.debug("writing histogram {0}".format(histname))
                try:
                    hist = self._get_histogram(proc, uncertainty)
                    file.WriteObject(hist, histname, "WriteDelete")
                except KeyError:
                    pass

    def save(self, config, outdir, systematics=None):
        logging.debug("saving histogram {0}".format(self.__name))

        if self.__class == r.TH1F:
            self._save1d(config, outdir, systematics=systematics)
        else:
            self._save2d(config, outdir, systematics=systematics)

    def _save2d(self, config, outdir, systematics=None):
        bkg_sum = self._get_background_sum(config)
        signals = zip((cfg.keys()[0] for cfg in config['signals']), self._get_signals(config))

        for label, hist in signals + [('background', bkg_sum)]:
            self._add_binlabels(hist)
            canvas = r.TCanvas(self.__name + label, self.__name, 600, 600)

            canvas.SetTopMargin(.18)
            canvas.SetLeftMargin(0.13)
            canvas.SetRightMargin(0.14)

            stylish.setup_upper_axis(hist, scale=False, is2d=True)
            stylish.draw_channel_info(canvas, plot_ratio=False)
            hist.Draw("COLZ")

            subdir = os.path.dirname(os.path.join(outdir, self.__name))
            if not os.path.exists(subdir) and subdir != '':
                os.makedirs(subdir)

            canvas.SaveAs(os.path.join(outdir, "{0}_{1}.pdf".format(self.__name, label)))
            canvas.SetLogz()
            canvas.SaveAs(os.path.join(outdir, "{0}_{1}_log.pdf".format(self.__name, label)))

    def _build_ratio_errors(self, ratio, nom, div):
        graph = r.TGraphAsymmErrors(ratio)

        for i in range(graph.GetN()):
            x_coord = ratio.GetBinCenter(i + 1)
            width = ratio.GetBinWidth(i + 1)
            y_ratio = ratio.GetBinContent(i + 1)
            y_data = nom.GetBinContent(i + 1)
            y_data_err = nom.GetBinError(i + 1)
            y_bkg = div.GetBinContent(i + 1)

            if y_ratio > stylish.small_number and y_ratio < stylish.ratio_plot_max * 0.99:
                if y_bkg > stylish.small_number:
                    y_up = (y_data + y_data_err) / y_bkg
                    y_down = (y_data - y_data_err) / y_bkg
                else:
                    y_up = 0
                    y_down = 0
                graph.SetPoint(i, x_coord, y_ratio)
                graph.SetPointEYhigh(i, y_up - y_ratio)
                graph.SetPointEYlow(i, y_ratio - y_down)
                graph.SetPointEXhigh(i, width / 2)
                graph.SetPointEXlow(i, width / 2)
            else:
                graph.SetPoint(i, x_coord, 999)

        graph.SetMarkerSize(1.)
        graph.SetMarkerStyle(20)
        graph.SetLineWidth(3)
        graph.SetLineColor(r.kBlack)

        return graph

    def _draw_ratio(self, config, base_histo, rel_err):
        background = self._get_background_sum(config)
        collisions = self._get_data(config)

        lower = base_histo.Clone()
        stylish.setup_lower_axis(lower)
        lower.DrawCopy("axis")

        rel_err.SetMarkerSize(0)
        rel_err.SetFillColor(r.kGray)
        rel_err.SetFillStyle(1001)
        rel_err.Draw("2 same")

        if len(collisions) > 0 and not self.__blind:
            ratio = collisions[0].Clone()
            ratio.Divide(background)
            err = self._build_ratio_errors(ratio, collisions[0], background)
            err.Draw("P same")
        else:
            err = None

        lower.DrawCopy("axis same")

        line = r.TLine()
        line.SetLineColor(1)
        line.SetLineWidth(1)
        line.DrawLineNDC(
            r.gPad.GetLeftMargin(),
            r.gPad.GetBottomMargin() + (1 / stylish.ratio_plot_max) * (1 - r.gPad.GetBottomMargin() - r.gPad.GetTopMargin()),
            1 - r.gPad.GetRightMargin(),
            r.gPad.GetBottomMargin() + (1 / stylish.ratio_plot_max) * (1 - r.gPad.GetBottomMargin() - r.gPad.GetTopMargin()))

        return err, rel_err

    def _save1d(self, config, outdir, systematics=None):
        min_y = 0.002
        max_y = min_y
        scale = 1.15
        factor = config.get("scale factor", "auto")
        split = config.get("show ratio", True)

        canvas = r.TCanvas(self.__name, self.__name, 600, 700)

        if split:
            canvas.Divide(1, 2)
            stylish.setup_upper_pad(canvas.GetPad(1))
            stylish.setup_lower_pad(canvas.GetPad(2))
        else:
            canvas.Divide(1, 1)
            stylish.setup_pad(canvas.GetPad(1))

        args = list(self.__args)
        args[0] += "_base"
        base_histo = self.__class(*args)
        self._add_binlabels(base_histo)

        if self.__labels:
            if isinstance(self.__labels[0], list) or isinstance(self.__labels[0], tuple):
                for n, label in enumerate(self.__labels[0], 1):
                    base_histo.GetXaxis().SetBinLabel(n, label)
                for n, label in enumerate(self.__labels[1], 1):
                    base_histo.GetYaxis().SetBinLabel(n, label)
            else:
                for n, label in enumerate(self.__labels, 1):
                    base_histo.GetXaxis().SetBinLabel(n, label)

        stylish.setup_upper_axis(base_histo, split=split)
        stylish.draw_channel_info(canvas.GetPad(1), plot_ratio=split)

        canvas.cd(1)

        bkg_sum = self._get_background_sum(config)
        err_abs, err_rel = self._get_background_errors(config, systematics)
        bkg_stack = self._get_backgrounds(config)
        bkg_stack.Draw()

        signals = self._get_signals(config)
        collisions = self._get_data(config)

        if factor == "auto":
            factor = self._get_scale_factor(bkg_sum, signals)
        elif factor == "norm":
            factor = 1.
            self._normalize_to_unity(signals)
            base_histo.GetYaxis().SetTitle("")

        if config.get("legend", True):
            scale = 1.175 + 0.05 * (
                math.ceil(len(config['backgrounds'] + config.get('data', [])) / 4. + 1)
                + len(config['signals']))

        max_y = scale * max([bkg_stack.GetMaximum()] + [factor * h.GetMaximum() for h in signals] +
                            [c.GetMaximum() for c in collisions])

        if max_y == 0:
            logging.warning("empty plot: {0}".format(self.__name))
            return

        base_histo.GetYaxis().SetRangeUser(min_y, max_y)
        base_histo.Draw("hist")

        bkg_stack.SetMinimum(min_y)
        bkg_stack.SetMaximum(max_y)
        bkg_stack.Draw("hist same")

        for sig in signals:
            sig.Scale(factor)
            sig.DrawCopy("hist same")

        if not self.__blind:
            for data in collisions:
                data.DrawCopy("E1 P same")

        err_abs.Draw("2 same")

        base_histo.Draw("axis same")

        # need to keep legend in memory, otherwise legend boxes are not
        # drawn (thank you, ROOT)
        legend = None
        if config.get("legend", True):
            legend = self._add_legend(config, factor)

        if split:
            canvas.cd(2)
            err, rel_err = self._draw_ratio(config, base_histo, err_rel)

        subdir = os.path.dirname(os.path.join(outdir, self.__name))
        if not os.path.exists(subdir) and subdir != '':
            os.makedirs(subdir)

        canvas.SaveAs(os.path.join(outdir, self.__name + ".pdf"))
        canvas.GetPad(1).SetLogy()
        base_histo.GetYaxis().SetRangeUser(min_y, max_y * 20)
        canvas.SaveAs(os.path.join(outdir, self.__name + "_log.pdf"))

        if legend:
            del legend

    @savetime
    def fill(self, process, systematics, weights, category=None):
        procname = str(process)
        suffix = ''
        for s in [systematics] + weights:
            if s.endswith('Up') or s.endswith('Down'):
                suffix = '_' + s
                if s not in weights and not (procname.startswith('collisions') or procname.startswith('fakes')):
                    procname += suffix
                break
        fullname = str(process) + suffix

        try:
            hist = self.__hists[fullname]
        except KeyError:
            args = list(self.__args)
            args[0] += "_{p}".format(p=fullname)
            self.__hists[fullname] = self.__class(*args)
            hist = self.__hists[fullname]

        weights = ["w_" + w.lower() for w in weights]

        drw = '{0}>>{1}'.format(":".join(self.__values), hist.GetName())
        sel = '*'.join(self.__weights if self.__weights else weights)
        if category:
            sel += ' * ({})'.format(category) if len(sel) > 0 else category
        opt = '' if len(self.__values) == 1 else 'COLZ'

        # Don't use weights for data
        if str(process).startswith('collisions'):
            sel = category if category else ''

        Forest.draw(procname, drw, sel, opt)
        # has to happen after the draw, otherwise ROOT won't find the
        # histo!
        self.__hists[fullname].SetDirectory(0)

    def clear(self):
        self.__hists.clear()
        self.__normalized = False

    @property
    def name(self):
        return self.__name

    @classmethod
    def plots(cls):
        return cls.__plots.values()

    @classmethod
    def reset(cls):
        for p in cls.__plots.values():
            p.clear()

    @classmethod
    def get_event_count(cls, f, proc, category, fmt, unweighed):
        p = cls.__plots['Events']
        p.clear()
        p.read(f, category, Process.expand(proc), fmt=fmt)
        if unweighed:
            return p._get_histogram(proc).GetEntries()
        return p._get_histogram(proc).GetBinContent(1)

    @classmethod
    def unblind(cls):
        for p in cls.__plots.values():
            p.blind(False)
