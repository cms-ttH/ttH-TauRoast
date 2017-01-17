from array import array

import ROOT as r

r.gROOT.SetBatch()

ntuple = '/afs/crc.nd.edu/user/m/mwolf3/www/ttH/v2/ttl_train/ntuple.root'
weights = '/afs/crc.nd.edu/user/m/mwolf3/www/ttH/v2/ttl_train/sklearn/bdt-1/weights.xml'

signals = 'ttH2Nonbb_125_train'.split()
backgrounds = 'ttjets_dl_pow_train ttjets_sl_pow_train'.split()

variables = """
ht
met
tt_cosDeltaPhi
tt_deltaR
tt_visiblemass
tau1_pt
tau1_abseta
tau1_leadingtrackpt
tau1jet_deltaRmin
tau2_pt
tau2_abseta
tau2_leadingtrackpt
tau2jet_deltaRmin
lep1_pt
lep1_abseta
lep1jet_deltaRmin
lep1tauOS_deltaR
lep1tauSS_deltaR
lep1tauOS_cosDeltaPhi
lep1tauSS_cosDeltaPhi
lep1_mt
lep1_mva
jet_deltaRavg
""".split()

stack = {}

reader = r.TMVA.Reader()
for v in variables:
    stack[v] = array('f', [0.])
    reader.AddVariable(v, stack[v])

reader.BookMVA('BDT', weights)

sig = r.TH1F('sig', '', 40, -1, 1)
bkg = r.TH1F('bkg', '', 40, -1, 1)

infile = r.TFile(ntuple)

for s in signals:
    t = infile.Get(s)
    for v, obj in stack.items():
        t.SetBranchAddress(v, obj)
    for n in xrange(t.GetEntries()):
        t.GetEntry(n)
        sig.Fill(reader.EvaluateMVA('BDT'))

for b in backgrounds:
    t = infile.Get(b)
    for v, obj in stack.items():
        t.SetBranchAddress(v, obj)
    for n in xrange(t.GetEntries()):
        t.GetEntry(n)
        bkg.Fill(reader.EvaluateMVA('BDT'))

c = r.TCanvas()
sig.SetLineColor(r.kBlue)
sig.Scale(1. / sig.Integral())
sig.Draw()
bkg.SetLineColor(r.kRed)
bkg.Scale(1. / bkg.Integral())
bkg.Draw("same")
c.SaveAs('output.png')
