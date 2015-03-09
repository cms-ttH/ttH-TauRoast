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

    def draw(self, *args):
        self.__hists.values()[0].Draw(self.__opt, *args)

    def save(self, config, outdir):
        min_y = 0.002
        max_y = min_y

        canvas = r.TCanvas(self.__name, self.__name, 600, 700)
        canvas.Divide(1, 2)

        stylish.setup_upper_pad(canvas.GetPad(1))
        stylish.setup_lower_pad(canvas.GetPad(2))

        args = list(self.__args)
        args[0] += "_base"
        base_histo = self.__class(*args)

        canvas.cd(1)

        bkg_stack = self._get_backgrounds(config)
        bkg_stack.Draw()

        signals = self._get_signals(config)
        max_y = max(h.GetMaximum() for h in [bkg_stack] + signals)

        bkg_stack.SetMinimum(min_y)
        bkg_stack.SetMaximum(max_y)

        for sig in signals:
            sig.DrawCopy("hist same")

        bkg_stack.Draw("axis same")

        canvas.cd(2)

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
