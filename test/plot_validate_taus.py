import ROOT as r

r.gROOT.SetBatch()
r.gStyle.SetOptStat(0)

class Plotter(object):
    def __init__(self, rootfile, pdffile):
        self.__f = r.TFile(rootfile)
        self.__fn = pdffile
        self.__t = self.__f.Get("taus/taus")
        self.__c = r.TCanvas()
        self.__c.SaveAs(self.__fn + "[")

        self.__cuts = "abs(t_decay) != 11 && abs(t_decay) != 13"

    def close(self):
        self.__c.SaveAs(self.__fn + "]")

    def plotiso(self, var, titles):
        self.__t.Draw("t_pt>>hist(15, 0, 150)", self.__cuts)
        base = r.gDirectory.Get("hist")
        base.SetName("basehist")

        hists = []
        for n, title in enumerate(titles):
            cut = self.__cuts + " && {0} >= {1}".format(var, n)
            print cut
            self.__t.Draw("t_pt>>hist(15, 0, 150)", cut)
            h = r.gDirectory.Get("hist")
            h.SetName(var + str(n))
            h.SetTitle(title + ";p_{T}^{#tau,gen} / GeV;Expected #tau efficiency")
            h.Divide(base)
            hists.append(h)

        l = r.TLegend(0.5, 0.9 - 0.05 * len(titles), 0.9, 0.9)
        opt = ""
        colors = [r.kBlack, r.kRed, r.kBlue, r.kGreen, r.kMagenta, r.kCyan]
        markers = [r.kFullCircle, r.kFullSquare, r.kFullTriangleUp, r.kFullTriangleDown, r.kFullDiamond, r.kFullCross]
        for h, color, marker in zip(hists, colors, markers):
            if opt == "":
                h.SetTitle("")
                h.GetYaxis().SetRangeUser(0, 1)
            h.SetLineColor(color)
            h.SetMarkerColor(color)
            h.SetMarkerStyle(marker)
            l.AddEntry(h, h.GetTitle() if h.GetTitle() != "" else "No isolation", "l")
            h.DrawCopy("P " + opt)
            # h.DrawCopy(opt)
            opt = "same"
        l.Draw()
        self.__c.SaveAs(self.__fn)

p = Plotter("validate.root", "validate.pdf")
p.plotiso("t_iso_3hits", ["No isolation", "Loose cut-based", "Medium cut-based", "Tight cut-based"])
p.plotiso("t_iso_mva", ["No isolation", "Very loose mva-based", "Loose mva-based", "Medium mva-based",
    "Tight mva-based", "Very tight mva-based", "Very very tight mva-based"])
p.close()
