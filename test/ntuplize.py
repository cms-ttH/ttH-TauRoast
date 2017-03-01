import os.path

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from FWCore.ParameterSet.Utilities import convertToUnscheduled

from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import updatedPatJets
from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import updatedPatJetCorrFactors


def add_channel(channel, path):
    dirname, filename = os.path.split(path)
    return os.path.join(dirname, "{}_{}".format(channel, filename))

options = VarParsing.VarParsing('analysis')
options.outputFile = "test.root"
options.register("channels", "",
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "Which channel(s) to run")
options.register("events", "",
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "Selected events to run over")
options.register("takeVLoose", False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Take very loose taus")
options.register("takeAll", False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Save as many events as possible")
options.register("dump", False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Dump event content and quit after one event.")
options.register("sample", "",
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Sample name to use")
options.register("data", False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Indicate if data is being used (or MC)")
options.register("saveGenInfo", False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Indicate if gen collections should be saved")
options.register("hlt", "HLT",
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Indicate the HLT result label")
options.register("hip", True,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Indicate if HIP safe mu ID is to be used.")
options.register("globalTag", "80X_mcRun2_asymptotic_2016_TrancheIV_v8",
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Global tag to use")
options.setDefault("channels", "ttl,tll")
options.parseArguments()

process = cms.Process("Taus")

process.options = cms.untracked.PSet(
    wantSummary=cms.untracked.bool(True),
    numberOfThreads=cms.untracked.uint32(2),
    numberOfStreams=cms.untracked.uint32(0),
    SkipEvent=cms.untracked.vstring('ProductNotFound')
)


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 2000

process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(options.maxEvents))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = options.globalTag

process.source = cms.Source(
    "PoolSource",
    fileNames=cms.untracked.vstring(options.inputFiles),
    # eventsToProcess=cms.untracked.VEventRange('1:1952714')
)

if len(options.events) > 0:
    process.source.eventsToProcess = cms.untracked.VEventRange(options.events)

# Jets
# ----

process.patJetCorrFactorsReapplyJEC = updatedPatJetCorrFactors.clone(
    src=cms.InputTag("slimmedJets"),
    levels=['L1FastJet', 'L2Relative', 'L3Absolute'],
    payload='AK4PFchs'  # Make sure to choose the appropriate levels and payload here!
)
if options.data:
    process.patJetCorrFactorsReapplyJEC.levels.append('L2L3Residual')

process.patJetsReapplyJEC = updatedPatJets.clone(
    jetSource=cms.InputTag("slimmedJets"),
    jetCorrFactorsSource=cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJEC"))
)

process.jetPath = cms.Path(
    process.patJetCorrFactorsReapplyJEC *
    process.patJetsReapplyJEC
)

from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD

# If you only want to re-correct and get the proper uncertainties
runMetCorAndUncFromMiniAOD(process,
                           isData=options.data)

# If you would like to re-cluster and get the proper uncertainties
# runMetCorAndUncFromMiniAOD(process,
#                        isData=True (or False),
#                        pfCandColl=cms.InputTag("packedPFCandidates"),
#                        recoMetFromPFCs=True,
#                        )

# Bad Muons
# ---------

process.badGlobalMuonTagger = cms.EDFilter(
    "BadGlobalMuonTagger",
    muons=cms.InputTag("slimmedMuons"),
    vtx=cms.InputTag("offlineSlimmedPrimaryVertices"),
    muonPtCut=cms.double(20),
    selectClones=cms.bool(False),
    taggingMode=cms.bool(True),
    verbose=cms.untracked.bool(False)
)
process.cloneGlobalMuonTagger = process.badGlobalMuonTagger.clone(
    selectClones=cms.bool(True)
)

process.removeBadAndCloneGlobalMuons = cms.EDProducer(
    "MuonRefPruner",
    input=cms.InputTag("slimmedMuons"),
    toremove=cms.InputTag("badGlobalMuonTagger", "bad"),
    toremove2=cms.InputTag("cloneGlobalMuonTagger", "bad")
)

process.noBadGlobalMuons = cms.Sequence(
    process.cloneGlobalMuonTagger +
    process.badGlobalMuonTagger +
    process.removeBadAndCloneGlobalMuons
)  # in tagging mode, these modules return always "true"


process.load("RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("ttH.LeptonID.ttHLeptons_cfi")

process.ttHLeptons.LooseCSVWP = cms.double(0.5426)
process.ttHLeptons.MediumCSVWP = cms.double(0.8484)
process.ttHLeptons.IsHIPSafe = cms.bool(options.hip)

process.lepPath = cms.Path(
    process.noBadGlobalMuons *
    process.fullPatMetSequence *
    process.electronMVAValueMapProducer *
    process.ttHLeptons
)

process.ttHLeptons.rhoParam = "fixedGridRhoFastjetCentralNeutral"
process.ttHLeptons.tightObjects = cms.bool(True)
process.ttHLeptons.jets = cms.InputTag("patJetsReapplyJEC")
process.ttHLeptons.JECTag = "patJetCorrFactorsReapplyJEC"

if options.dump:
    process.dump = cms.EDAnalyzer("EventContentAnalyzer")
    process.dpath = cms.Path(process.dump)
    process.maxEvents.input = cms.untracked.int32(1)

process.end = cms.EndPath()

for channel in options.channels:
    if channel == 'ttl':
        taus = (2, 2)
        leptons = (1, 1)
        jets = (3,)
        tags = (1, -1)
    elif channel == 'tll':
        taus = (0, 1)
        leptons = (2, 2)
        jets = (4,)
        tags = (1, -1)
    else:
        raise ValueError("channel needs to be either tll or ttl")

    prod = cms.EDProducer("TauProcessor",
                          sample=cms.string(options.sample),
                          data=cms.bool(options.data),
                          electrons=cms.InputTag("ttHLeptons"),
                          muons=cms.InputTag("ttHLeptons"),
                          taus=cms.InputTag("ttHLeptons"),
                          jets=cms.InputTag("patJetsReapplyJEC"),
                          # jets=cms.InputTag("slimmedJets"),
                          genJets=cms.InputTag("slimmedGenJets"),
                          minTaus=cms.uint32(taus[0]),
                          maxTaus=cms.uint32(taus[1]),
                          minLeptons=cms.uint32(leptons[0]),
                          maxLeptons=cms.uint32(leptons[1]),
                          minJets=cms.uint32(jets[0]),
                          minTags=cms.uint32(tags[0]),
                          maxTags=cms.int32(tags[1]),
                          minJetPt=cms.double(25.),
                          minTagPt=cms.double(25.),
                          maxJetEta=cms.double(2.4),
                          filterPUJets=cms.bool(False),
                          takeAll=cms.bool(options.takeAll),
                          saveGenInfo=cms.bool(options.saveGenInfo),
                          tauVLoose=cms.bool(options.takeVLoose),
                          printPreselection=cms.bool(False),
                          triggerResults=cms.string(options.hlt),
                          debugEvents=cms.vuint32()
                          )

    selector = cms.EDFilter("TauSelector",
                            input=cms.InputTag(channel + "Taus")
                            )

    output = cms.OutputModule(
        "PoolOutputModule",
        fileName=cms.untracked.string(add_channel(channel, options.outputFile)),
        outputCommands=cms.untracked.vstring(['drop *', 'keep *_{}_*_*'.format(channel + "Taus")]),
        SelectEvents=cms.untracked.PSet(SelectEvents=cms.vstring(channel + "Path")),
        dataset=cms.untracked.PSet(filterName=cms.untracked.string(''))
    )

    setattr(process, channel + "Taus", prod)
    setattr(process, channel + "Selector", selector)

    setattr(process, channel + "Output", output)

    path = cms.Path()
    path *= getattr(process, channel + "Taus")
    path *= getattr(process, channel + "Selector")

    setattr(process, channel + "Path", path)

    process.end *= getattr(process, channel + "Output")

convertToUnscheduled(process)
