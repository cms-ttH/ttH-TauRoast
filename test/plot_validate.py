import ROOT as r

r.gROOT.SetBatch()

class Plotter(object):
    def __init__(self, infile, outfile):
        self.__outfile = outfile

        self.__f = r.TFile(infile)
        self.__c = r.TCanvas()
        self.__first = True

    def plot(self, tree, selection, title, bins, xlow, xhigh, last=False):
        for cut, label in (("w", ""), ("w > 0", " +ve weights"), ("w < 0", " -ve weights")):
            if self.__first:
                self.__first = False
                opt1 = "("
            else:
                opt1 = ""
            opt2 = ")" if last and cut == "w < 0" else ""

            h = r.TH1F("I_am_a_histo", title + label, bins, xlow, xhigh)

            if not any(isinstance(selection, cls) for cls in (list, tuple)):
                selection = [selection]

            t = self.__f.Get("taus/" + tree)
            for sel in selection:
                t.Draw(sel + ">>+I_am_a_histo", cut)

            h.Draw()

            self.__c.SetLogy()
            self.__c.SaveAs(self.__outfile + opt1)
            self.__c.SetLogy(False)

            mx = h.GetMaximum()
            for n in range(h.GetNbinsX(), 0, -1):
                if h.GetBinContent(n) > 0.01 * mx:
                    break
            if n < h.GetNbinsX() * 0.66:
                h.GetXaxis().SetRange(1, n + 2)

            self.__c.SaveAs(self.__outfile + opt2)

p = Plotter("validate.root", "validate.pdf")
p.plot("events", "njets", "ak5 genjets count", 80, 0, 80)
p.plot("events", "njets30", "ak5 genjets count p_{T} > 30", 30, 0, 30)
p.plot("events", ["tau1pt", "tau2pt"], "#tau p_{T} (from H)", 100, 0, 1500)
p.plot("events", ["tau1eta", "tau2eta"], "#tau #eta (from H)", 100, -8, 8)
p.plot("events", ["tau1decay", "tau2decay"], "#tau decay (0: hadronic)", 14, 0, 14)
p.plot("events", ["top1pt", "top2pt"], "top p_{T} (status 62)", 100, 0, 2000)
p.plot("events", ["top1eta", "top2eta"], "top #eta (status 62)", 100, -8, 8)
p.plot("events", "ditaumass", "Higgs mass from #tau_{1,2}", 40, 120, 130)
p.plot("events", "ditaupt", "Higgs p_{T} from #tau_{1,2}", 100, 0, 2500)

p.plot("jets", "pt", "jet p_{T} > 30", 100, 0, 2500)
p.plot("jets", "eta", "jet #eta, p_{T} > 30", 100, -8, 8)
p.plot("jets", "phi", "jet #phi, p_{T} > 30", 100, -3.15, 3.15, last=True)
