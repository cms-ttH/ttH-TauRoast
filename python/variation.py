import ROOT as r

from ttH.TauRoast.botany import Forest, Leaf

def train_mva(ntuple, signals, backgrouns, variables, outfilename, methods=None):
    if methods is None:
        methods = [(
            "BDTG",
            "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedGrad:GradBaggingFraction=0.5:nCuts=20:NNodesMax=5"
        )]
    outfile = r.TFile(outfilename, "RECREATE")
    factory = r.TMVA.Factory("TMVAClassification", outfile,
                    "!V:!Silent:Transformations=I;D;P;G,D:AnalysisType=Classification")

    leaves = dict(l.name, l.kind for l in Leaf.leaves())
    for var in variables:
        factory.AddVariable(var, leaves[var])

    forest = Forest(ntuple)
    for sig in signals:
        factory.AddSignalTree(forest[sig], 1.)
    for bkg in backgrounds:
        factory.AddBackgroundTree(forest[bkg], 1.)

    factory.PrepareTrainingAndTestTree("", "", "SplitMode=Random:NormMode=NumEvents:!V")

    for m, opt in methods:
        factory.BookMethod(r.TMVA.Types.kBDT, m, opt)

    factory.TrainAllMethods()
    factory.TestAllMethods()
    factory.EvaluateAllMethods()
