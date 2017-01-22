import pickle

from root_numpy import root2array, rec2array

import ROOT as r

r.gROOT.SetBatch()

ntuple = '/afs/crc.nd.edu/user/m/mwolf3/www/ttH/v2/ttl_train.02/ntuple.root'
save = '/afs/crc.nd.edu/user/m/mwolf3/www/ttH/v2/ttl_train.02/sklearn/bdt-0/bdt.pkl'

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

with open(save, 'rd') as fd:
    bdt = pickle.load(fd)

sig = r.TH1F('sig', '', 40, 0, 1)
bkg = r.TH1F('bkg', '', 40, 0, 1)

infile = r.TFile(ntuple)

for s in signals:
    data = rec2array(root2array(ntuple, str(s), variables))
    for v in bdt.predict_proba(data)[:, 1]:
        sig.Fill(v)
for b in backgrounds:
    data = rec2array(root2array(ntuple, str(b), variables))
    for v in bdt.predict_proba(data)[:, 1]:
        bkg.Fill(v)

c = r.TCanvas()
sig.SetLineColor(r.kBlue)
sig.Scale(1. / sig.Integral())
sig.Draw()
bkg.SetLineColor(r.kRed)
bkg.Scale(1. / bkg.Integral())
bkg.Draw("same")
c.SaveAs('output.png')
