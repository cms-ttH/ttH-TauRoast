#!/usr/bin/env python

import ROOT as r

from root_pandas import read_root

quants = 'eta pt chargedPt constituents chargedConstituents'.split()
quants += 'closestdr closestpt closestparticledr closestparticlept'.split()
quants += 'signalPt signalChargedPt signalConstituents signalChargedConstituents'.split()
quants += 'isoPt isoChargedPt isoConstituents isoChargedConstituents'.split()

taus_in = ['genjet_' + q.lower() for q in quants] + ['isoMVA03', 'antiElectron', 'antiMuon', 'match', 'pt']
taus_in = ['tau_' + v for v in taus_in]
taus = read_root("test/genfaketau/out/ntuple.root", "ttjets", columns=taus_in, flatten=True)

fakes = taus[(taus.tau_match == 6)]
selection = taus[
    (taus.tau_match == 6)
    & (taus.tau_isoMVA03 >= 3)
    & (taus.tau_pt >= 20.)
]

gen_in = [q.lower() for q in quants]
gen_in = ['genjet_' + v for v in gen_in]
alljets = read_root("test/genfaketau/out/ntuple.root", "ttjets", columns=gen_in, flatten=True)
jets = alljets[
    (alljets.genjet_pt > 18)
    & (alljets.genjet_eta > -2.5)
    & (alljets.genjet_eta < 2.5)
    & (alljets.genjet_closestparticledr > 0.1)
    & (alljets.genjet_constituents <= 22)
    & (alljets.genjet_chargedconstituents <= 10)
    & (alljets.genjet_isochargedpt < 10)
    & (alljets.genjet_signalpt > 16)
    & (alljets.genjet_signalchargedconstituents <= 5)
]

for q in quants:
    selection[q] = selection['tau_genjet_' + q.lower()]
    jets[q] = jets['genjet_' + q.lower()]

count = 300000

selection.head(count).to_root("test/genfaketau/out/train.root", "signal", "w")
jets.head(count).to_root("test/genfaketau/out/train.root", "background", "a")

r.TMVA.gConfig().GetIONames().fWeightFileDir = "test/genfaketau/out/weights"

infile = r.TFile("test/genfaketau/out/train.root")
outfile = r.TFile("test/genfaketau/out/tmva.root", "RECREATE")
factory = r.TMVA.Factory("TMVAClassification", outfile,
                         "!V:!Silent:Transformations=I;D;P;G,D:AnalysisType=Classification")

factory.AddVariable("pt", "f")
factory.AddVariable("chargedPt", "f")
factory.AddVariable("constituents", "i")
factory.AddVariable("chargedConstituents", "i")
factory.AddVariable("signalPt", "f")
factory.AddVariable("signalChargedPt", "f")
factory.AddVariable("signalConstituents", "i")
factory.AddVariable("signalChargedConstituents", "i")
factory.AddVariable("isoPt", "f")
factory.AddVariable("isoChargedPt", "f")
factory.AddVariable("isoConstituents", "i")
factory.AddVariable("isoChargedConstituents", "i")

factory.AddSignalTree(infile.Get("signal"))
factory.AddBackgroundTree(infile.Get("background"))

factory.PrepareTrainingAndTestTree(r.TCut(), "SplitMode=Random:NormMode=NumEvents:!V")

result = factory.BookMethod(
    r.TMVA.Types.kBDT,
    "BDTG",
    "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:GradBaggingFraction=0.5:nCuts=20:MaxDepth=2"
)

factory.TrainAllMethods()
factory.TestAllMethods()
factory.EvaluateAllMethods()
