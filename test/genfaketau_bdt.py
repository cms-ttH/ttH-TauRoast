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

# From https://dbaumgartel.wordpress.com/tag/tmva/

Histo_training_S = r.TH1D('Histo_training_S', 'S (Train)', 40, -1.0, 1.0)
Histo_training_B = r.TH1D('Histo_training_B', 'B (Train)', 40, -1.0, 1.0)
Histo_testing_S = r.TH1D('Histo_testing_S', 'S (Test)', 40, -1.0, 1.0)
Histo_testing_B = r.TH1D('Histo_testing_B', 'B (Test)', 40, -1.0, 1.0)

# Fetch the trees of events from the root file
TrainTree = outfile.Get("TrainTree")
TestTree = outfile.Get("TestTree")

# Cutting on these objects in the trees will allow to separate true S/B SCut_Tree = 'classID>0.5'
BCut_Tree = 'classID<0.5'
SCut_Tree = 'classID>0.5'

r.TH1.SetDefaultSumw2()

# Now lets project the tree information into those histograms
TrainTree.Project("Histo_training_S", "BDTG", SCut_Tree)
TrainTree.Project("Histo_training_B", "BDTG", BCut_Tree)
TestTree.Project("Histo_testing_S", "BDTG", SCut_Tree)
TestTree.Project("Histo_testing_B", "BDTG", BCut_Tree)

Histo_training_S.Scale(1. / Histo_training_S.Integral())
Histo_training_B.Scale(1. / Histo_training_B.Integral())
Histo_testing_S.Scale(1. / Histo_testing_S.Integral())
Histo_testing_B.Scale(1. / Histo_testing_B.Integral())

# Create the color styles
Histo_training_S.SetLineColor(2)
Histo_training_S.SetMarkerColor(2)
Histo_training_S.SetFillColor(2)
Histo_testing_S.SetLineColor(2)
Histo_testing_S.SetMarkerColor(2)
Histo_testing_S.SetFillColor(2)

Histo_training_B.SetLineColor(4)
Histo_training_B.SetMarkerColor(4)
Histo_training_B.SetFillColor(4)
Histo_testing_B.SetLineColor(4)
Histo_testing_B.SetMarkerColor(4)
Histo_testing_B.SetFillColor(4)

# Histogram fill styles
Histo_training_S.SetFillStyle(3001)
Histo_training_B.SetFillStyle(3001)
Histo_testing_S.SetFillStyle(0)
Histo_testing_B.SetFillStyle(0)

# Histogram marker styles
Histo_testing_S.SetMarkerStyle(20)
Histo_testing_B.SetMarkerStyle(20)

# Set titles
Histo_training_S.GetXaxis().SetTitle("Classifier, BDT")
Histo_training_S.GetYaxis().SetTitle("Counts/Bin")

r.gROOT.SetBatch()

# Draw the objects
c1 = r.TCanvas("c1", "", 800, 600)
r.gStyle.SetOptStat(0)
r.gStyle.SetOptTitle(0)
Histo_training_S.Draw("HIST")
Histo_training_B.Draw("HISTSAME")
Histo_testing_S.Draw("EPSAME")
Histo_testing_B.Draw("EPSAME")

# Reset the y-max of the plot
ymax = max([h.GetMaximum() for h in [Histo_training_S, Histo_training_B, Histo_testing_S, Histo_testing_B]])
ymax *= 1.2
Histo_training_S.SetMaximum(ymax)

# Create Legend
c1.cd(1).BuildLegend(0.42, 0.72, 0.57, 0.88).SetFillColor(0)

# Add custom title
l1 = r.TLatex()
l1.SetNDC()
l1.DrawLatex(0.26, 0.93, "Classification with TMVA (r)")

# Finally, draw the figure
c1.Print('training.png')
