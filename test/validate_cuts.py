import sys
import ROOT as r

r.gROOT.SetBatch()
r.gStyle.SetOptStat(0)

f = r.TFile(sys.argv[1], "READ")
tree = f.Get("taus/decays")

cuts = [
        "(mother_id==25)",
        "(abs(daughter1_decay)!=11 && abs(daughter1_decay)!=13)",
        "(abs(daughter2_decay)!=11 && abs(daughter2_decay)!=13)",
        "(daughter1_reco_pt>=20 && daughter2_reco_pt>=20)",
        "(daughter1_iso_3hits>=0)",
        "(daughter2_iso_3hits>=0)",
        "(daughter1_iso_3hits>=1)",
        "(daughter2_iso_3hits>=1)",
        "(daughter1_iso_3hits>=3 || daughter2_iso_3hits>=3)"
]

cut = ""
prev = None
for c in cuts:
    if len(cut) > 0:
        cut = " && ".join([cut, c])
    else:
        cut = c

    if prev is not None:
        prev = val

    val = tree.Draw("daughter2_iso_mva", cut)

    if prev is None:
        prev = val

    print c, val, val / float(prev)
    # print c.replace("3hits", "mva"), tree.Draw("daughter2_iso_mva", cut.replace("3hits", "mva").replace("=3", "=5"))
