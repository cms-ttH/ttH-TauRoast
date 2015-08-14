import logging
import math
import os
import random

import ROOT as r

from ttH.TauRoast import stylish
from ttH.TauRoast.botany import Forest
from ttH.TauRoast.decorative import savetime
from ttH.TauRoast.legendary import Legend
from ttH.TauRoast.processing import Process

class Plot(object):
    __plots = {}

    def __init__(self, name, values, labels, binning, limitname=None, weights=None):
        self.__name = name
        self.__limitname = limitname if limitname else os.path.basename(name)

        self.__values = values
        self.__weights = weights

        self.__args = [self.__limitname, ";".join([""] + labels)] + binning
        self.__hists = {}

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

    def _add_legend(self, config, factor):
        l = Legend(0.05, 3, 0.08)
        for cfg in config['backgrounds']:
            bkg, color = cfg.items()[0]
            l.draw_box(1001, self._eval(color), Process.get(bkg).fullname)
        l.draw_box(3654, r.kBlack, "Bkg. err.", True)
        # TODO add collisions
        l.new_row()
        for cfg in config['signals']:
            sig, color = cfg.items()[0]
            label = Process.get(sig).fullname
            if factor != 1:
                label += " (#times {0:.1f})".format(factor)
            l.draw_line(2, self._eval(color), label)
        return l

    def _eval(self, color):
        return eval(color, {}, {'r': r})

    def _get_histogram(self, process):
        procs = Process.expand(process)
        h = self.__hists[procs[0]].Clone()
        for proc in procs[1:]:
            h.Add(self.__hists[proc])
        return h

    def _get_background_sum(self, config):
        res = None
        for cfg in config['backgrounds']:
            background, color = cfg.items()[0]
            try:
                h = self._get_histogram(background)
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
        res.SetFillStyle(3654)
        res.SetFillColor(r.kBlack)
        res.SetMarkerStyle(0)
        return res

    def _get_backgrounds(self, config):
        res = r.THStack(self.__name + "_stack", self.__args[1])
        for cfg in config['backgrounds']:
            background, color = cfg.items()[0]
            try:
                h = self._get_histogram(background)
                h.SetFillColor(self._eval(color))
                h.SetFillStyle(1001)
                h.SetLineWidth(0)
                res.Add(h)
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
                h.SetLineWidth(4)
                h.SetMarkerStyle(20)
                h.SetMarkerSize(1)
                res.append(h)
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
            except KeyError:
                pass
        return res

    def _get_scale_factor(self, background, signals):
        bsum = background.Integral()
        ssum = max(sig.Integral() for sig in signals)

        return (bsum / float(ssum) if ssum > 0 and bsum > 0 else 1)

    def _normalize(self, cuts):
        for proc, hist in self.__hists.items():
            logging.debug("normalizing histogram {0}, process {1}".format(
                self.__name, proc))
            factor = cuts[-1][proc] / float(cuts[-3][proc])
            hist.Scale(factor)

    def read(self, file, procs, fmt="{p}_{v}"):
        for proc in procs:
            logging.debug("reading histogram {0}".format(fmt.format(p=proc.limitname, v=self.__limitname)))
            h = file.Get(fmt.format(p=proc.limitname, v=self.__limitname))
            if h is None:
                logging.warning("histogram {0} not found in file".format(
                    fmt.format(p=proc.limitname, v=self.__limitname)))
            else:
                h.SetDirectory(0)
                self.__hists[proc] = h

    def write(self, file, cuts, fmt="{p}_{v}"):
        self._normalize(cuts)

        for proc, hist in self.__hists.items():
            logging.debug("writing histogram {0}".format(fmt.format(p=proc.limitname, v=self.__limitname)))
            file.WriteObject(hist, fmt.format(p=proc.limitname, v=self.__limitname))

    def save(self, config, outdir):
        logging.debug("saving histogram {0}".format(self.__name))

        if self.__class == r.TH1F:
            self._save1d(config, outdir)
        else:
            self._save2d(config, outdir)

    def _save2d(self, config, outdir):
        bkg_sum = self._get_background_sum(config)
        signals = zip((cfg.keys()[0] for cfg in config['signals']), self._get_signals(config))

        for label, hist in signals + [('background', bkg_sum)]:
            canvas = r.TCanvas(self.__name + label, self.__name, 600, 600)

            canvas.SetTopMargin(.18)
            canvas.SetLeftMargin(0.13)
            canvas.SetRightMargin(0.14)

            stylish.setup_upper_axis(hist, scale=False, is2d=True)
            hist.Draw("COLZ")

            subdir = os.path.dirname(os.path.join(outdir, self.__name))
            if not os.path.exists(subdir) and subdir != '':
                os.makedirs(subdir)
            canvas.SaveAs(os.path.join(outdir, "{0}_{1}.pdf".format(self.__name, label)))

    def _build_background_errors(self, background):
        abs_err = r.TGraphAsymmErrors(background)
        rel_err = r.TGraphAsymmErrors(background)

        # err_up = self.get_squared_bkg_shifts(self.__up, histname, histo)
        # err_down = self.get_squared_bkg_shifts(self.__down, histname, histo)
        err_up = [0] * background.GetNbinsX()
        err_down = [0] * background.GetNbinsX()

        for i in range(background.GetNbinsX()):
            bin_center = background.GetBinCenter(i + 1)
            bin_content = background.GetBinContent(i + 1)
            bin_error = background.GetBinError(i + 1)
            bin_width = background.GetBinWidth(i + 1)

            if bin_content > 0.001:
                rel_up = math.sqrt(err_up[i] + bin_error**2) / bin_content
                rel_down = math.sqrt(err_down[i] + bin_error**2) / bin_content
            else:
                rel_up = 0
                rel_down = 0

            abs_err.SetPoint(i, bin_center, bin_content)
            abs_err.SetPointEXlow(i, bin_width / 2)
            abs_err.SetPointEXhigh(i, bin_width / 2)
            abs_err.SetPointEYlow(i, math.sqrt(err_down[i]))
            abs_err.SetPointEYhigh(i, math.sqrt(err_up[i]))

            rel_err.SetPoint(i, bin_center, 1)
            rel_err.SetPointEXlow(i, bin_width / 2)
            rel_err.SetPointEXhigh(i, bin_width / 2)
            rel_err.SetPointEYlow(i, rel_down)
            rel_err.SetPointEYhigh(i, rel_up)

        return (abs_err, rel_err)

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
        graph.SetMarkerStyle(0)
        graph.SetLineWidth(4)
        graph.SetLineColor(r.kBlack)

        return graph

    def _draw_ratio(self, config):
        background = self._get_background_sum(config)
        collisions = self._get_data(config)

        (_, rel_err) = self._build_background_errors(background)
        rel_err.SetMarkerSize(0)
        rel_err.SetFillColor(r.kGreen)
        rel_err.SetFillStyle(1001)
        rel_err.Draw("2 same")

        if len(collisions) > 0:
            ratio = collisions[0].Clone()
            ratio.Divide(background)
            err = self._build_ratio_errors(ratio, collisions[0], background)
            err.Draw("P same")
        else:
            err = None

        return err, rel_err

    def _save1d(self, config, outdir):
        min_y = 0.002
        max_y = min_y
        scale = 1.15
        factor = config.get("scale factor", "auto")

        canvas = r.TCanvas(self.__name, self.__name, 600, 700)
        canvas.Divide(1, 2)

        stylish.setup_upper_pad(canvas.GetPad(1))
        stylish.setup_lower_pad(canvas.GetPad(2))

        args = list(self.__args)
        args[0] += "_base"
        base_histo = self.__class(*args)
        stylish.setup_upper_axis(base_histo)

        canvas.cd(1)

        bkg_sum = self._get_background_sum(config)
        bkg_stack = self._get_backgrounds(config)
        bkg_stack.Draw()

        signals = self._get_signals(config)
        collisions = self._get_data(config)

        if factor == "auto":
            factor = self._get_scale_factor(bkg_sum, signals)

        if config.get("legend", True):
            scale = 1.175 + 0.05 * (
                    math.ceil(len(config['backgrounds'] + config.get('data', [])) / 3. + 1)
                    + math.ceil(len(config['signals']) / 3.))

        max_y = scale * max([bkg_stack.GetMaximum()] + [factor * h.GetMaximum() for h in signals])

        if max_y == 0:
            return

        base_histo.GetYaxis().SetRangeUser(min_y, max_y)
        base_histo.Draw("hist")

        bkg_stack.SetMinimum(min_y)
        bkg_stack.SetMaximum(max_y)
        bkg_stack.Draw("hist same")

        for sig in signals:
            sig.Scale(factor)
            sig.DrawCopy("hist same")

        for data in collisions:
            data.DrawCopy("E1 P same")

        bkg_sum.Draw("E2 same")

        base_histo.Draw("axis same")

        # need to keep legend in memory, otherwise legend boxes are not
        # drawn (thank you, ROOT)
        legend = None
        if config.get("legend", True):
            legend = self._add_legend(config, factor)

        canvas.cd(2)

        lower = base_histo.Clone()
        stylish.setup_lower_axis(lower)

        lower.Draw("axis")

        err = self._draw_ratio(config)

        lower.Draw("axis same")

        line = r.TLine()
        line.SetLineColor(1)
        line.SetLineWidth(1)
        line.DrawLineNDC(
                r.gPad.GetLeftMargin(),
                r.gPad.GetBottomMargin() +
                    (1 / stylish.ratio_plot_max) *
                    (1 - r.gPad.GetBottomMargin() - r.gPad.GetTopMargin()),
                1 - r.gPad.GetRightMargin(),
                r.gPad.GetBottomMargin() +
                    (1 / stylish.ratio_plot_max) *
                    (1 - r.gPad.GetBottomMargin() - r.gPad.GetTopMargin()))

        subdir = os.path.dirname(os.path.join(outdir, self.__name))
        if not os.path.exists(subdir) and subdir != '':
            os.makedirs(subdir)
        canvas.SaveAs(os.path.join(outdir, self.__name + ".pdf"))

    @savetime
    def fill(self, process, weights):
        try:
            hist = self.__hists[process]
        except KeyError:
            args = list(self.__args)
            args[0] += "_{p}".format(p=process)
            self.__hists[process] = self.__class(*args)
            hist = self.__hists[process]

        weights = ["w_" + w.lower() for w in weights]

        drw = '{0}>>{1}'.format(":".join(self.__values), hist.GetName())
        sel = '*'.join(self.__weights if self.__weights else weights)
        opt = '' if len(self.__values) == 1 else 'COLZ'
        Forest.draw(str(process), drw, sel, opt)
        # has to happen after the draw, otherwise ROOT won't find the
        # histo!
        self.__hists[process].SetDirectory(0)

    @property
    def name(self):
        return self.__name

    @classmethod
    def plots(cls):
        return cls.__plots.values()
