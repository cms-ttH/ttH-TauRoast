import logging
import math
import os

import ROOT as r

from ttH.TauRoast import stylish
from ttH.TauRoast.legendary import Legend
from ttH.TauRoast.processing import Process
from ttH.TauRoast.useful import Snippet

class Plot(Snippet):
    __plots__ = []
    __names__ = set()

    def __init__(self, code, name, labels, binning, dim=1, draw="", limitname=None):
        super(Plot, self).__init__(code)
        self.__name = name
        self.__limitname = limitname if limitname else os.path.basename(name)
        self.__opt = draw

        self.__args = [name, ";".join([""] + labels)] + binning
        self.__hists = {}

        if dim == 1:
            self.__class = r.TH1F
        elif dim == 2:
            self.__class = r.TH2F

        if self.__limitname in Plot.__names__:
            raise KeyError("Plot {0} defined twice".format(self.__limitname))
        Plot.__plots__.append(self)
        Plot.__names__.add(self.__limitname)

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
            h.Add(self__hists[proc])
        return h

    def _get_background_sum(self, config):
        res = None
        for cfg in config['backgrounds']:
            background, color = cfg.items()[0]
            h = self._get_histogram(background)
            if res is None:
                res = h
            else:
                res.Add(h)
        if not res:
            args = list(self.__args)
            args[0] += "_bkg_sum"
            res = self.__class(*args)
        res.SetFillStyle(3654)
        res.SetFillColor(r.kBlack)
        res.SetMarkerStyle(0)
        return res

    def _get_backgrounds(self, config):
        res = r.THStack(self.__name + "_stack", self.__args[1])
        for cfg in config['backgrounds']:
            background, color = cfg.items()[0]
            h = self._get_histogram(background)
            h.SetFillColor(self._eval(color))
            h.SetFillStyle(1001)
            h.SetLineWidth(0)
            res.Add(h)
        return res

    def _get_signals(self, config):
        res = []
        for cfg in config['signals']:
            signal, color = cfg.items()[0]
            h = self._get_histogram(signal)
            h.SetLineColor(self._eval(color))
            h.SetLineWidth(4)
            res.append(h)
        return res

    def _get_scale_factor(self, background, signals):
        bsum = background.Integral()
        ssum = max(sig.Integral() for sig in signals)

        return (bsum / float(ssum) if bsum > 0 else 1)

    def _normalize(self, lumi):
        for proc, hist in self.__hists.items():
            logging.debug("normalizing histogram {0}, process {1}".format(
                self.__name, proc))
            factor = lumi * proc.cross_section / float(proc.events)
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

    def write(self, file, lumi, fmt="{p}_{v}"):
        self._normalize(lumi)

        for proc, hist in self.__hists.items():
            logging.debug("writing histogram {0}".format(fmt.format(p=proc.limitname, v=self.__limitname)))
            file.WriteObject(hist, fmt.format(p=proc.limitname, v=self.__limitname))

    def save(self, config, outdir):
        logging.debug("saving histogram {0}".format(self.__name))

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

        bkg_sum.Draw("E2 same")

        base_histo.Draw("axis same")

        # need to keep legend in memory, otherwise legend boxes are not
        # drawn (thank you, ROOT)
        legend = None
        if config.get("legend", True):
            legend = self._add_legend(config, factor)

        canvas.cd(2)

        ratio = base_histo.Clone()
        stylish.setup_lower_axis(ratio)

        ratio.Draw("axis")

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

    def fill(self, process, event, combo):
        try:
            hist = self.__hists[process]
        except KeyError:
            args = list(self.__args)
            args[0] += "_{p}".format(p=process)
            self.__hists[process] = self.__class(*args)
            hist = self.__hists[process]

        self._execute(event, combo, locals={'histo': hist})

    @property
    def name(self):
        return self.__name

    @classmethod
    def plots(cls):
        return cls.__plots__
