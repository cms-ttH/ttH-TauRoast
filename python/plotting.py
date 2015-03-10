import os

import ROOT as r

from ttH.TauRoast import stylish
from ttH.TauRoast.legendary import Legend
from ttH.TauRoast.processing import Process
from ttH.TauRoast.utils import Snippet

class Plot(Snippet):
    __plots__ = []

    def __init__(self, code, name, labels, binning, dim=1, draw=""):
        super(Plot, self).__init__(code)
        self.__name = name
        self.__opt = draw

        self.__args = [name, ";".join([""] + labels)] + binning
        self.__hists = {}

        if dim == 1:
            self.__class = r.TH1F
        elif dim == 2:
            self.__class = r.TH2F

        Plot.__plots__.append(self)

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
        res.SetFillStyle(3654)
        res.SetFillColor(r.kBlack)
        res.SetMarkerStyle(0)
        return res

    def _get_backgrounds(self, config):
        res = r.THStack(self.__name + "_stack", self.__args[1])
        for cfg in config['backgrounds']:
            background, color = cfg.items()[0]
            h = self._get_histogram(background)
            h.SetFillColor(eval(color, {}, {'r': r}))
            h.SetFillStyle(1001)
            h.SetLineWidth(0)
            res.Add(h)
        return res

    def _get_signals(self, config):
        res = []
        for cfg in config['signals']:
            signal, color = cfg.items()[0]
            h = self._get_histogram(signal)
            h.SetLineColor(eval(color, {}, {'r': r}))
            h.SetLineWidth(4)
            res.append(h)
        return res

    def _get_scale_factor(self, background, signals):
        bsum = background.Integral()
        ssum = max(sig.Integral() for sig in signals)

        return bsum / float(ssum)

    def _normalize(self, lumi):
        for proc, hist in self.__hists.items():
            factor = lumi * proc.cross_section / float(proc.events)
            hist.Scale(factor)

    def draw(self, *args):
        self.__hists.values()[0].Draw(self.__opt, *args)

    def save(self, config, outdir):
        min_y = 0.002
        max_y = min_y
        scale = 1.15
        factor = config.get("scale factor", "auto")

        # TODO move to somewhere else!
        self._normalize(10000)

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

        max_y = scale * max([bkg_stack.GetMaximum()] + [factor * h.GetMaximum() for h in signals])

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
