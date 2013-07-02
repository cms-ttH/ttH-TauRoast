import os
import ROOT as r
import sys

r.gROOT.SetBatch()
r.gSystem.Load("libTTHTauTauRoast")

try:
    from ROOT import roast
except:
    sys.stderr.write("Failed to import 'roast'!\n")
    sys.exit(1)

from TTHTauTau.Roast.helper import *

def get_bkg_stack(histname, processes):
    res = r.THStack(histname + "_stack", histname + "_stack")

    for p in processes:
        h = p.GetHContainerForSignal()[histname].GetHisto()
        h.SetFillStyle(1001)
        h.SetFillColor(p.GetColor())
        h.SetLineWidth(0)
        res.Add(h)

    return res

def get_bkg_sum(histname, processes):
    res = None

    for p in processes:
        if not p.IsBackground():
            continue

        if not res:
            res = roast.HWrapper(p.GetHContainerForSignal()[histname])
        else:
            res.Add(p.GetHContainerForSignal()[histname])

    return res

def get_integrals(histname, processes):
    vals = []
    for p in processes:
        if p.GetHContainerForSignal()[histname].GetHisto():
            vals.append(p.GetHContainerForSignal()[histname].GetHisto().Integral())
    return vals

def get_maximum(histname, processes, inc_error=False):
    vals = []
    for p in processes:
        if inc_error:
            vals.append(p.GetHContainerForSignal()[histname].GetMaximumWithError())
        else:
            vals.append(p.GetHContainerForSignal()[histname].GetMaximum())
    return max(vals)

class Legend:
    def __init__(self, margin, x_offset, x_length, y_offset, y_length, ncols):
        self.__current = 0
        self.__box_dx = 0.06 * x_length
        self.__box_dy = 0.04 * y_length
        self.__legend_x = margin * x_length + x_offset
        self.__legend_dx = (1. - 2 * margin) / ncols * x_length
        self.__legend_y = (1. - 0.5 * margin) * y_length + y_offset
        self.__legend_dy = 1.4 * self.__box_dy
        self.__pos_x = self.__legend_x
        self.__pos_y = self.__legend_y

        self.__ncols = ncols

        self.__box = r.TBox()
        self.__tex = r.TLatex()
        self.__tex.SetTextSize(0.035)
        self.__line = r.TLine()
        self.__marker = r.TMarker()

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
        if line:
            self.__box.SetLineWidth(1)
            self.__box.SetLineColor(color)
            self.__box.SetLineStyle(1)
            self.__box.SetFillStyle(0)
            self.__box.SetFillColor(color)
            self.__box.DrawBox(
                    self.__pos_x,
                    self.__pos_y,
                    self.__pos_x + self.__box_dx,
                    self.__pos_y - self.__box_dy)
        else:
            self.__box.SetLineWidth(0)
            self.__box.SetLineStyle(0)

        self.__box.SetFillStyle(pattern)
        self.__box.SetFillColor(color)
        self.__box.DrawBox(
                self.__pos_x,
                self.__pos_y,
                self.__pos_x + self.__box_dx,
                self.__pos_y - self.__box_dy)
        self.__tex.DrawLatex(
                self.__pos_x + 1.2 * self.__box_dx,
                self.__pos_y - 0.8 * self.__box_dy,
                label)
        self.advance()

    def draw_marker(self, marker, color, label):
        self.__line.SetLineColor(color)
        self.__line.SetLineWidth(1)
        self.__line.DrawLine(
                self.__pos_x,
                self.__pos_y - self.__box_dy / 2,
                self.__pos_x + self.__box_dx,
                self.__pos_y - self.__box_dy / 2)
        self.__line.DrawLine(
                self.__pos_x + self.__box_dx / 2,
                self.__pos_y,
                self.__pos_x + self.__box_dx / 2,
                self.__pos_y - self.__box_dy)
        self.__marker.SetMarkerStyle(marker)
        self.__marker.SetMarkerColor(color)
        self.__marker.DrawMarker(
                self.__pos_x + self.__box_dx / 2,
                self.__pos_y - self.__box_dy / 2)
        self.__tex.DrawLatex(
                self.__pos_x + 1.2 * self.__box_dx,
                self.__pos_y - 0.8 * self.__box_dy,
                label)
        self.advance()

    def draw_line(self, width, color, label):
        self.__line.SetLineColor(color)
        self.__line.SetLineWidth(width)
        self.__line.DrawLine(
                self.__pos_x,
                self.__pos_y - self.__box_dy / 2,
                self.__pos_x + self.__box_dx,
                self.__pos_y - self.__box_dy / 2)
        self.__tex.DrawLatex(
                self.__pos_x + 1.2 * self.__box_dx,
                self.__pos_y - 0.8 * self.__box_dy,
                label)
        self.advance()

def stack(config, processes):
    procs = map(lambda n: get_process(n, processes), config['analysis']['plot'])

    plot_ratio = True

    padding = 0.001
    y_divide = 0.25
    ratio_plot_max = 2.5
    bottom_margin = 0.35
    small_number = 0.0001

    for histname in procs[0].GetHistogramNames():
        if all(map(lambda v: v <= 0, get_integrals(histname, procs))):
            # FIXME log this properly
            print "Empty histogram:", histname
            continue

        print "Saving", histname

        max_y = 1.3 * get_maximum(histname, procs, True)

        base_histo = roast.HWrapper(procs[0].GetHContainerForSignal()[histname])
        base_histo.ScaleBy(0)
        base_histo.GetHisto().SetTitle("")

        if plot_ratio:
            min_y = 0.002
            canvas = r.TCanvas(histname, histname, 800, 1000)
            canvas.Divide(1, 2)
            canvas.GetPad(1).SetPad(padding, y_divide + padding, 1 - padding, 1 - padding)
            canvas.GetPad(1).SetTopMargin(0.065)
            canvas.GetPad(1).SetBottomMargin(0)
            canvas.GetPad(2).SetPad(padding, padding, 1 - padding, y_divide - padding)
            canvas.GetPad(2).SetBottomMargin(bottom_margin)
            canvas.cd(1)
        else:
            min_y = 0.001
            canvas = r.TCanvas(histname, histname, 800, 800)
            canvas.cd()

        base_histo.GetHisto().GetYaxis().SetRangeUser(min_y, max_y)

        bkg_procs = get_backgrounds(procs)
        bkg_stack = get_bkg_stack(histname, bkg_procs)
        bkg_stack.Draw("hist")
        bkg_stack.GetXaxis().SetRangeUser(base_histo.GetMinXVis(), base_histo.GetMaxXVis())
        bkg_stack.GetXaxis().SetTitle(base_histo.GetXTitle())
        bkg_stack.GetYaxis().SetTitle(base_histo.GetYTitle())
        bkg_stack.SetMinimum(min_y)
        bkg_stack.SetMaximum(max_y)
        bkg_stack.GetYaxis().SetRangeUser(min_y, max_y)

        base_histo.GetHisto().Draw("hist")

        bkg_stack.Draw("same")

        bkg_sum = get_bkg_sum(histname, procs)
        bkg_sum.SetFillStyle(3654)
        bkg_sum.SetFillColor(r.kBlack)
        bkg_sum.SetMarkerStyle(0)
        bkg_sum.GetHisto().Draw("E2 same")

        sig_procs = get_signals(procs)
        for p in sig_procs:
            h = p.GetHContainerForSignal()[histname].GetHisto()
            h.SetFillStyle(0)
            h.SetLineWidth(3)
            h.SetLineColor(p.GetColor())
            h.GetYaxis().SetRangeUser(min_y, max_y)

            # FIXME implement stacking signals on top of bkg

            h.Draw("hist same")

        try:
            coll = get_collisions(procs)
            h = coll.GetHContainerForSignal()[histname].GetHisto()
            h.SetMarkerStyle(20)
            h.GetYaxis().SetRangeUser(min_y, max_y)
            h.SetLineWidth(2)
            h.Draw("E1 P same")
        except:
            coll = None
            # FIXME do something more sensible
            pass

        x0 = base_histo.GetMinXVis()
        dx = base_histo.GetMaxXVis() - base_histo.GetMinXVis()
        y0 = min_y
        dy = max_y - min_y

        l = Legend(0.05, x0, dx, y0, dy, 3)

        for p in bkg_procs:
            l.draw_box(1001, p.GetColor(), p.GetLabelForLegend())

        l.draw_box(3654, r.kBlack, "Bkg. err.", True)

        if coll:
            l.draw_marker(20, r.kBlack, coll.GetLabelForLegend())

        l.new_row()
        for p in sig_procs:
            l.draw_line(2, p.GetColor(), p.GetLabelForLegend())

        base_histo.GetHisto().Draw("axis same")

        if plot_ratio:
            max_ratio = 2.5

            canvas.cd(2)

            ratio = coll.GetHContainerForSignal()[histname].GetHisto().Clone()
            ratio.Divide(bkg_sum.GetHisto())
            ratio.GetXaxis().SetRangeUser(
                    bkg_sum.GetMinXVis(), bkg_sum.GetMaxXVis())

            ratio.GetXaxis().SetTitleSize(0.15)
            ratio.GetXaxis().SetLabelSize(0.1)
            ratio.GetYaxis().SetTitle("Data/MC")
            ratio.GetYaxis().CenterTitle()
            ratio.GetYaxis().SetTitleSize(0.15)
            ratio.GetYaxis().SetTitleOffset(0.4)
            ratio.GetYaxis().SetLabelSize(0.08)
            ratio.GetYaxis().SetRangeUser(min_y, max_ratio)
            ratio.Draw("axis")

            bkg_err = ratio.Clone()
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
            bkg_err.Draw("E2 same")

            ratio_err = r.TGraphAsymmErrors(ratio)
            for i in range(ratio_err.GetN()):
                x_coord = ratio.GetBinCenter(i + 1)
                width = ratio.GetBinWidth(i + 1)
                y_ratio = ratio.GetBinContent(i + 1)
                y_data = coll.GetHContainerForSignal()[histname].GetHisto().GetBinContent(i + 1)
                y_data_err = coll.GetHContainerForSignal()[histname].GetHisto().GetBinError(i + 1)
                y_bkg = bkg_sum.GetHisto().GetBinContent(i + 1)

                if y_ratio > small_number and y_ratio < ratio_plot_max * 0.99:
                    if y_bkg > small_number:
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
            ratio_err.SetMarkerStyle(8)
            ratio_err.SetLineWidth(2)
            ratio_err.Draw("P same")
            ratio.Draw("axis same")

            line = r.TLine()
            line.SetLineColor(1)
            line.SetLineWidth(2)
            line.DrawLine(bkg_sum.GetMinXVis(), 1, bkg_sum.GetMaxXVis(), 1)

        filename = config['paths']['stack format'].format(t="png",
                d=base_histo.GetSubDir(), n=histname, m="")

        dirname = os.path.dirname(filename)
        if not os.path.exists(dirname):
            os.makedirs(dirname)

        canvas.SaveAs(filename)
