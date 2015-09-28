import os
import ROOT as r

from ttH.TauRoast.botany import Forest, Leaf

principal = None

def train(ntuple, signals, backgrounds, variables, outfilename, methods=None):
    r.TMVA.gConfig().GetIONames().fWeightFileDir = os.path.join(os.path.dirname(outfilename), 'weights')

    if methods is None:
        methods = [(
            "BDTG",
            "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedGrad:GradBaggingFraction=0.5:nCuts=20:NNodesMax=5"
        )]
    outfile = r.TFile(outfilename, "RECREATE")
    factory = r.TMVA.Factory("TMVAClassification", outfile,
                    "!V:!Silent:Transformations=I;D;P;G,D:AnalysisType=Classification")

    leaves = dict((l.name, l.kind) for l in Leaf.leaves())
    for var in variables:
        factory.AddVariable(var, leaves[var])

    forest = Forest(ntuple)
    for sig in signals:
        factory.AddSignalTree(forest[sig], 1.)
    for bkg in backgrounds:
        factory.AddBackgroundTree(forest[bkg], 1.)

    factory.PrepareTrainingAndTestTree(r.TCut(), "SplitMode=Random:NormMode=NumEvents:!V")

    for m, opt in methods:
        factory.BookMethod(r.TMVA.Types.kBDT, m, opt)

    factory.TrainAllMethods()
    factory.TestAllMethods()
    factory.EvaluateAllMethods()

class Principle(object):
    def __init__(self, wdir, variables, method="BDTG"):
        self.__method = method
        self.__reader = r.TMVA.Reader("!Color:!Silent")
        leaves = dict((l.name, l) for l in Leaf.leaves())
        for var in variables:
            self.__reader.AddVariable(var, leaves[var].value)
        fn = os.path.join(wdir, 'weights', 'TMVAClassification_{0}.weights.xml'.format(self.__method))
        self.__reader.BookMVA(self.__method, fn)

    def evaluate(self):
        return self.__reader.EvaluateMVA(self.__method)

def setup(wdir, variables):
    global principal
    if principal is not None:
        logger.error("MVA already set up!")
        return
    principal = Principle(wdir, variables)
