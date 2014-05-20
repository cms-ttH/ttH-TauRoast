# vim: ts=4:sw=4:et:sta
from contextlib import contextmanager
from glob import glob
import logging
import math
import os
import re
import ROOT as r
import sys

r.gROOT.SetBatch()
r.gSystem.Load("libttHTauRoast")

try:
    from ROOT import roast
except:
    sys.stderr.write("Failed to import 'roast'!\n")
    sys.exit(1)

from ttH.TauRoast.helper import *
from ttH.TauRoast import style

def get_bkg_stack(config, histname, processes):
    res = r.THStack(histname + "_stack", histname + "_stack")

    for p in reversed(processes):
        h = p.GetHistogram(histname).GetHisto()
        h.SetFillColor(config.processes[p].color)
        if 'fake' in p.GetShortName():
            h.SetFillStyle(3244)
        elif 'real' in p.GetShortName():
            h.SetFillStyle(3010)
        else:
            h.SetFillStyle(1001)
        h.SetLineColor(config.processes[p].color)
        h.SetLineWidth(0)
        res.Add(h)

    return res

def get_bkg_sum(histname, processes):
    res = None

    for p in processes:
        if not p.IsBackground():
            continue

        if not res:
            res = roast.HWrapper(p.GetHistogram(histname))
        else:
            res.Add(p.GetHistogram(histname))

    return res

def get_integrals(histname, processes):
    vals = []
    for p in processes:
        if p.GetHistogram(histname).GetHisto():
            vals.append(p.GetHistogram(histname).GetHisto().Integral())
    return vals

def get_maximum(histname, processes, inc_error=False, sig_scale=1.0):
    vals = []
    for p in processes:
        if inc_error:
            vals.append(p.GetHistogram(histname).GetMaximumWithError())
        else:
            vals.append(p.GetHistogram(histname).GetMaximum())

        if p.IsSignal():
            vals[-1] *= sig_scale
    return max(vals)

class SysErrors:
    def __init__(self, config):
        upfilenames = glob(config['paths']['systematics input'].format(s="*Up"))
        downfilenames = glob(config['paths']['systematics input'].format(s="*Down"))

        candidates = filter(lambda s: s != "Collisions", config['analysis']['plot'])

        self.__up = []
        cnt = 1
        max = len(upfilenames)
        for fn in upfilenames:
            logging.info("loading systematic shift {0}/{1} from {2}".format(cnt, max, fn))
            cnt += 1
            procs = load("Roast", fn)
            normalize_processes(config, procs)
            combine_processes(config, procs)
            procs = map(lambda n: get_process(n, procs), candidates)
            self.__up.append((fn, procs))

        self.__down = []
        cnt = 1
        max = len(downfilenames)
        for fn in downfilenames:
            logging.info("loading systematic shift {0}/{1} from {2}".format(cnt, max, fn))
            cnt += 1
            procs = load("Roast", fn)
            normalize_processes(config, procs)
            combine_processes(config, procs)
            procs = get_backgrounds(map(lambda n: get_process(n, procs), candidates))
            self.__down.append((fn, procs))

    def has_errors(self):
        return len(self.__down) > 0 or len(self.__up) > 0

    def get_squared_bkg_shifts(self, procs, histname, histo):
        res = [0] * histo.GetHisto().GetNbinsX()
        for (fn, ps) in procs:
            bkg_sum = get_bkg_sum(histname, ps)

            for i in range(histo.GetHisto().GetNbinsX()):
                res[i] += (bkg_sum.GetHisto().GetBinContent(i + 1) -
                    histo.GetHisto().GetBinContent(i + 1))**2
        return res

    def get_errors(self, histname, histo):
        abs_err = r.TGraphAsymmErrors(histo.GetHisto())
        rel_err = r.TGraphAsymmErrors(histo.GetHisto())

        err_up = self.get_squared_bkg_shifts(self.__up, histname, histo)
        err_down = self.get_squared_bkg_shifts(self.__down, histname, histo)

        for i in range(histo.GetHisto().GetNbinsX()):
            bin_center = histo.GetHisto().GetBinCenter(i + 1)
            bin_content = histo.GetHisto().GetBinContent(i + 1)
            bin_error = histo.GetHisto().GetBinError(i + 1)
            bin_width = histo.GetHisto().GetBinWidth(i + 1)

            if bin_content > 0.001:
                rel_up = math.sqrt(err_up[i] + bin_error**2) / bin_content
                rel_down = math.sqrt(err_down[i] + bin_error**2) / bin_content
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

def to_ndc(x, y):
    new_x = r.gPad.GetLeftMargin() + x * (1 - r.gPad.GetLeftMargin() - r.gPad.GetRightMargin())
    new_y = r.gPad.GetBottomMargin() + y * (1 - r.gPad.GetBottomMargin() - r.gPad.GetTopMargin())
    return (new_x, new_y)

class Legend:
    def __init__(self, margin, ncols, topmargin=0):
        self.__current = 0
        self.__box_dx = 0.06
        self.__box_dy = 0.04
        self.__legend_x = margin
        self.__legend_dx = (1. - 2 * margin) / ncols
        self.__legend_y = 1. - 0.5 * margin - topmargin
        self.__legend_dy = 1.4 * self.__box_dy
        self.__pos_x = self.__legend_x
        self.__pos_y = self.__legend_y

        self.__ncols = ncols

        self.__markers = []
        self.__paves = []
        self.__tex = r.TLatex()
        self.__tex.SetNDC()
        self.__tex.SetTextSize(0.035)
        self.__line = r.TLine()

    def advance(self):
        self.__current += 1
        self.__pos_x += self.__legend_dx
        if self.__current % self.__ncols == 0:
            self.__pos_x = self.__legend_x
            self.__pos_y -= self.__legend_dy

    def new_row(self):
        if self.__current % self.__ncols == 0:
            return
        self.__current = 0
        self.__pos_x = self.__legend_x
        self.__pos_y -= self.__legend_dy

    def draw_box(self, pattern, color, label, line=False):
        (x1, y1) = to_ndc(self.__pos_x, self.__pos_y)
        (x2, y2) = to_ndc(self.__pos_x + self.__box_dx, self.__pos_y - self.__box_dy)
        pave = r.TPave(x1, y1, x2, y2, 0, "NDC")
        pave.SetFillStyle(pattern)
        pave.SetFillColor(color)
        pave.SetBorderSize(0 if not line else 1)
        pave.Draw()
        self.__paves.append(pave)

        (text_x, text_y) = to_ndc(
                    self.__pos_x + 1.2 * self.__box_dx,
                    self.__pos_y - 0.8 * self.__box_dy)
        self.__tex.DrawLatex(text_x, text_y, label)
        self.advance()

    def draw_marker(self, style, color, label):
        self.__line.SetLineColor(color)
        self.__line.SetLineWidth(1)
        self.__line.DrawLineNDC(
                *(to_ndc(
                    self.__pos_x,
                    self.__pos_y - self.__box_dy / 2)
                + to_ndc(
                    self.__pos_x + self.__box_dx,
                    self.__pos_y - self.__box_dy / 2)))
        self.__line.DrawLineNDC(
                *(to_ndc(
                    self.__pos_x + self.__box_dx / 2,
                    self.__pos_y)
                + to_ndc(
                    self.__pos_x + self.__box_dx / 2,
                    self.__pos_y - self.__box_dy)))
        (marker_x, marker_y) = to_ndc(self.__pos_x + self.__box_dx / 2, self.__pos_y - self.__box_dy / 2)
        marker = r.TMarker(marker_x, marker_y, style)
        marker.SetMarkerStyle(style)
        marker.SetMarkerColor(color)
        marker.SetNDC()
        marker.Draw()
        self.__markers.append(marker)
        (text_x, text_y) = to_ndc(
                    self.__pos_x + 1.2 * self.__box_dx,
                    self.__pos_y - 0.8 * self.__box_dy)
        self.__tex.DrawLatex(text_x, text_y, label)
        self.advance()

    def draw_line(self, width, color, label):
        self.__line.SetLineColor(color)
        self.__line.SetLineWidth(width)
        self.__line.DrawLineNDC(
                *(to_ndc(
                    self.__pos_x,
                    self.__pos_y - self.__box_dy / 2)
                + to_ndc(
                    self.__pos_x + self.__box_dx,
                    self.__pos_y - self.__box_dy / 2)))
        (text_x, text_y) = to_ndc(
                    self.__pos_x + 1.2 * self.__box_dx,
                    self.__pos_y - 0.8 * self.__box_dy)
        self.__tex.DrawLatex(text_x, text_y, label)
        self.advance()

@contextmanager
def create_plot(config, histcfg, plot_ratio, is2d=False, procname="", hist=None, max=None, scale=None):
    histname = histcfg['filename']
    subdir = histcfg['dir']

    small_number = 1e-5

    if plot_ratio:
        min_y = 0.002
        canvas = r.TCanvas(histname, histname, 600, 700)
        canvas.Divide(1, 2)
        canvas.cd(1)
        style.setup_upper_pad(canvas.GetPad(1))
        style.setup_lower_pad(canvas.GetPad(2))

        pads = (scale, canvas.GetPad(1), canvas.GetPad(2))
    else:
        min_y = 0.001
        canvas = r.TCanvas(histname + procname, histname, 600, 600)

        if is2d:
            r.gPad.SetTopMargin(.18)
            r.gPad.SetLeftMargin(0.13)
            r.gPad.SetRightMargin(0.14)

        pads = (scale, canvas,)

    yield pads

    if plot_ratio:
        canvas.cd(1)

    style.draw_channel_info(config, plot_ratio, proc=procname, is2d=True)

    if plot_ratio:
        canvas.cd(2)
        line = r.TLine()
        line.SetLineColor(1)
        line.SetLineWidth(1)
        line.DrawLineNDC(
                r.gPad.GetLeftMargin(),
                r.gPad.GetBottomMargin() + (1 / style.ratio_plot_max) * (1 - r.gPad.GetBottomMargin() - r.gPad.GetTopMargin()),
                1 - r.gPad.GetRightMargin(),
                r.gPad.GetBottomMargin() + (1 / style.ratio_plot_max) * (1 - r.gPad.GetBottomMargin() - r.gPad.GetTopMargin()))

    if procname != "":
        procname = "_" + procname

    for t in ("png", "pdf"):
        filename = config['paths']['stack format'].format(t=t,
                d=subdir, n=histname, m="", p=procname)

        dirname = os.path.dirname(filename)
        if not os.path.exists(dirname):
            logging.debug("creating directory %s", dirname)
            os.makedirs(dirname)

            php = os.path.join(os.environ["LOCALRT"], 'src/ttHMultileptonAnalysis/DrawPlots/python/utilities/index.php')
            if os.path.isfile(php):
                os.symlink(php, os.path.join(dirname, 'index.php'))

        canvas.SaveAs(filename)

    if plot_ratio:
        canvas.cd(1)
    else:
        canvas.cd()

    # Reset log-scale maximum y-value
    if max and hist:
        new_max = 10 ** (((scale - 1) * 2 + 1) * math.log10(max / scale))
        hist.GetYaxis().SetRangeUser(0.002, new_max)
        r.gPad.Update()

    if is2d:
        r.gPad.SetLogz()
    else:
        r.gPad.SetLogy()

    canvas.Update()

    for t in ("png", "pdf"):
        filename = config['paths']['stack format'].format(t=t,
                d=subdir, n=histname, m="_log", p=procname)

        dirname = os.path.dirname(filename)
        if not os.path.exists(dirname):
            logging.debug("creating directory %s", dirname)
            os.makedirs(dirname)

            php = os.path.join(os.environ["LOCALRT"], 'src/ttHMultileptonAnalysis/DrawPlots/python/utilities/index.php')
            if os.path.isfile(php):
                os.symlink(php, os.path.join(dirname, 'index.php'))

        canvas.SaveAs(filename)

def stack(config, processes):
    procs = map(lambda n: get_process(n, processes), config['analysis']['plot'])

    err = SysErrors(config)

    plot_ratio = True

    for cfg in config['histograms']:
        histname = cfg['filename']
        try:
            if all(map(lambda v: v <= 0, get_integrals(histname, procs))):
                logging.warn("empty histogram: %s", histname)
                continue
        except:
            logging.warn("unfilled histogram: %s", histname)
            continue

        logging.info("plotting %s", histname)

        if procs[0].GetHistogram(histname).ClassName().startswith("TH2"):
            bkg_sum = get_bkg_sum(histname, procs)
            zmax = bkg_sum.GetMaximum()
            axes = [r.TH1.GetXaxis, r.TH1.GetYaxis]

            try:
                coll = get_collisions(procs)
                ratio = coll.GetHistogram(histname).Clone()
                zmax = max(zmax, ratio.GetMaximum())
                ratio.Divide(bkg_sum.GetHisto())
                ratio.SetMarkerColor(r.kWhite)
                ratio.SetMarkerSize(1.5)
                ratio.SetMarkerSize(0.8)
                with create_plot(config, cfg, is2d=True, plot_ratio=False, procname="Data-MC") as (scale, pad1,):
                    pad1.cd()
                    style.setup_upper_axis(ratio, scale=False, is2d=True)
                    r.gStyle.SetPaintTextFormat("4.2f");

                    if 'visible' in cfg:
                        for (axis, urange) in zip(axes, cfg['visible']):
                            axis(ratio).SetRangeUser(*map(float, urange))

                    ratio.GetZaxis().SetRangeUser(0.33, 3.0)
                    ratio.GetZaxis().SetTitle("Data/MC")
                    ratio.Draw("COLZ TEXT")
            except:
                pass

            with create_plot(config, cfg, is2d=True, plot_ratio=False, procname="Background") as (scale, pad1,):
                pad1.cd()
                h = bkg_sum.GetHisto()

                if 'visible' in cfg:
                    for (axis, urange) in zip(axes, cfg['visible']):
                        axis(h).SetRangeUser(*map(float, urange))

                if len(cfg['axis labels']) == 3:
                    h.SetTitle(";{0};{1};{2}".format(*cfg['axis labels']))
                else:
                    h.SetTitle(";{0};{1}".format(*cfg['axis labels']))
                style.setup_upper_axis(h, scale=False, is2d=True)
                h.GetZaxis().SetRangeUser(0., zmax)
                h.Draw("COLZ")

            for p in procs:
                with create_plot(config, cfg, is2d=True, plot_ratio=False, procname=p.GetShortName()) as (scale, pad1,):
                    pad1.cd()
                    h = p.GetHistogram(histname).GetHisto()

                    if 'visible' in cfg:
                        for (axis, urange) in zip(axes, cfg['visible']):
                            axis(h).SetRangeUser(*map(float, urange))

                    if len(cfg['axis labels']) == 3:
                        h.SetTitle(";{0};{1};{2}".format(*cfg['axis labels']))
                    else:
                        h.SetTitle(";{0};{1}".format(*cfg['axis labels']))
                    style.setup_upper_axis(h, scale=False, is2d=True)
                    h.GetZaxis().SetRangeUser(0., zmax)
                    h.Draw("COLZ")
        else:
            base_histo = roast.HWrapper(procs[0].GetHistogram(histname))

            bkg_procs = get_backgrounds(procs)
            bkg_stack = get_bkg_stack(config, histname, bkg_procs)
            bkg_stack.Draw("hist")

            sig_scale = config['display']['signal scale factor']
            scale = 1.15
            if config['display']['legend']:
                scale = 1.45

            max_y = scale * max(
                    get_maximum(histname, procs, True, sig_scale),
                    bkg_stack.GetMaximum())

            with create_plot(config, cfg, plot_ratio, hist=base_histo, max=max_y, scale=scale) as (scale, pad1, pad2):
                pad1.cd()

                bkg_stack.Draw("hist")
                bkg_stack.SetMinimum(0.002)
                bkg_stack.SetMaximum(max_y)
                bkg_stack.GetYaxis().SetRangeUser(0.002, max_y)

                bkg_stack.Draw("hist")

                style.setup_upper_axis(base_histo)
                base_histo.GetYaxis().SetRangeUser(0.002, max_y)
                # base_histo.GetXaxis().SetRangeUser(base_histo.GetMinXVis(), base_histo.GetMaxXVis())
                base_histo.SetTitle(";{0};{1}".format(*cfg['axis labels']))

                if 'visible' in cfg:
                    base_histo.GetXaxis().SetRangeUser(*map(float, cfg['visible']))

                base_histo.Draw("hist")

                bkg_stack.Draw("hist same")

                bkg_sum = get_bkg_sum(histname, procs)
                bkg_sum.SetFillStyle(3654)
                bkg_sum.SetFillColor(r.kBlack)
                bkg_sum.SetMarkerStyle(0)

                (abs_err, rel_err) = err.get_errors(histname, bkg_sum)
                abs_err.SetFillStyle(3654)
                abs_err.SetFillColor(r.kBlack)
                abs_err.SetMarkerStyle(0)

                if err.has_errors():
                    abs_err.Draw("2 same")
                else:
                    bkg_sum.GetHisto().Draw("E2 same")

                sig_procs = get_signals(procs)
                for p in sig_procs:
                    h = p.GetHistogram(histname).GetHisto().Clone()
                    h.Scale(config['display']['signal scale factor'])
                    h.SetFillStyle(0)
                    h.SetLineWidth(4)
                    h.SetLineColor(config.processes[p.GetShortName()].color)
                    h.GetYaxis().SetRangeUser(0.002, max_y)

                    # FIXME implement stacking signals on top of bkg

                    h.DrawCopy("hist same")

                try:
                    coll = get_collisions(procs)
                    h = coll.GetHistogram(histname).GetHisto()
                    h.SetMarkerStyle(20)
                    h.SetMarkerSize(1)
                    h.GetYaxis().SetRangeUser(0.002, max_y)
                    h.SetLineWidth(4)
                    h.SetLineColor(r.kBlack)
                    h.Draw("E1 P same")
                except:
                    coll = None
                    # FIXME do something more sensible
                    pass

                if config['display']['legend']:
                    pad1.cd()
                    l = Legend(0.05, 3, 0.08)
                    for p in bkg_procs:
                        l.draw_box(1001, config.processes[p].color, p.GetLabelForLegend())
                    l.draw_box(3654, r.kBlack, "Bkg. err.", True)
                    if coll:
                        l.draw_marker(20, r.kBlack, coll.GetLabelForLegend())
                    l.new_row()
                    for p in sig_procs:
                        suffix = "" if sig_scale == 1 else " (#times {0})".format(sig_scale)
                        l.draw_line(2, config.processes[p].color, p.GetLabelForLegend() + suffix)

                base_histo.GetHisto().Draw("axis same")

                if plot_ratio:
                    pad2.cd()

                    try:
                        ratio = coll.GetHistogram(histname).GetHisto().Clone()
                        ratio.Divide(bkg_sum.GetHisto())
                    except:
                        ratio = base_histo.GetHisto().Clone()

                    # ratio.GetXaxis().SetRangeUser(
                            # bkg_sum.GetMinXVis(), bkg_sum.GetMaxXVis())
                    ratio.SetTitle(";{0};{1}".format(*cfg['axis labels']))
                    style.setup_lower_axis(ratio)

                    if 'visible' in cfg:
                        ratio.GetXaxis().SetRangeUser(*map(float, cfg['visible']))

                    ratio.Draw("axis")

                    if 'dump' in cfg and cfg['dump']:
                        bins = [ratio.GetBinLowEdge(b) for b in xrange(1, ratio.GetNbinsX() + 2)]
                        content = [ratio.GetBinContent(b) for b in xrange(ratio.GetNbinsX() + 2)]
                        print bins
                        print content
                        if 'dump file' in config['paths']:
                            f = r.TFile(config['paths']['dump file'], 'UPDATE')
                            f.WriteObject(ratio, "data_mc_ratio")
                            f.Close()

                    bkg_err = base_histo.GetHisto().Clone()
                    for i in range(bkg_err.GetNbinsX()):
                        bkg_err.SetBinContent(i + 1, 1)

                        if bkg_sum.GetHisto().GetBinContent(i + 1) > 0.001:
                            bkg_err.SetBinError(i + 1,
                                    bkg_sum.GetHisto().GetBinError(i + 1) /
                                    bkg_sum.GetHisto().GetBinContent(i + 1))
                        else:
                            bkg_err.SetBinError(i + 1, 0)
                    bkg_err.SetMarkerSize(0)
                    bkg_err.SetFillColor(r.kGreen)
                    rel_err.SetMarkerSize(0)
                    rel_err.SetFillColor(r.kGreen)
                    rel_err.SetFillStyle(1001)
                    if ratio:
                        # bkg_err.Draw("E2 same")
                        rel_err.Draw("2 same")
                    else:
                        # bkg_err.Draw("E2")
                        rel_err.Draw("2")

                    if coll:
                        ratio_err = r.TGraphAsymmErrors(ratio)
                        for i in range(ratio_err.GetN()):
                            x_coord = ratio.GetBinCenter(i + 1)
                            width = ratio.GetBinWidth(i + 1)
                            y_ratio = ratio.GetBinContent(i + 1)
                            y_data = coll.GetHistogram(histname).GetHisto().GetBinContent(i + 1)
                            y_data_err = coll.GetHistogram(histname).GetHisto().GetBinError(i + 1)
                            y_bkg = bkg_sum.GetHisto().GetBinContent(i + 1)

                            if y_ratio > style.small_number and y_ratio < style.ratio_plot_max * 0.99:
                                if y_bkg > style.small_number:
                                    y_up = (y_data + y_data_err) / y_bkg
                                    y_down = (y_data - y_data_err) / y_bkg
                                else:
                                    y_up = 0
                                    y_down = 0
                                ratio_err.SetPoint(i, x_coord, y_ratio)
                                ratio_err.SetPointEYhigh(i, y_up - y_ratio)
                                ratio_err.SetPointEYlow(i, y_ratio - y_down)
                                ratio_err.SetPointEXhigh(i, width / 2)
                                ratio_err.SetPointEXlow(i, width / 2)
                            else:
                                ratio_err.SetPoint(i, x_coord, 999)

                        ratio_err.SetMarkerSize(1.)
                        ratio_err.SetMarkerStyle(0)
                        ratio_err.SetLineWidth(4)
                        ratio_err.SetLineColor(r.kBlack)
                        ratio_err.Draw("P same")
                        ratio.Draw("axis same")
