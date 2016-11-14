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
options.register("takeAll", False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Save as many events as possible")
options.register("dump", False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Dump event content and quit after one event.")
options.register("data", False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Indicate if data is being used (or MC)")
options.register("genFilter", False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Filter the input dataset")
options.register("genFilterWithFakes", False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Use fake taus when filtering the input dataset")
options.register("saveGenInfo", False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Indicate if gen collections should be saved")
options.register("reHLT", False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Indicate if the HLT has been re-run.")
options.register("globalTag", "80X_mcRun2_asymptotic_2016_miniAODv2_v1",
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
)

if len(options.events) > 0:
    process.source.eventsToProcess = cms.untracked.VEventRange(options.events)

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

process.load("RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("ttH.LeptonID.ttHLeptons_cfi")
process.load('ttH.TauMCGeneration.eventFilterMAOD_cfi')
process.load("ttH.TauRoast.genHadronMatching_cfi")

if options.genFilterWithFakes:
    process.ttHGenFilter.useFakeTaus = cms.bool(True)

process.lepPath = cms.Path(
    process.electronMVAValueMapProducer *
    process.ttHLeptons
)

process.ttHLeptons.rhoParam = "fixedGridRhoFastjetCentralNeutral"
# process.ttHLeptons.jets = cms.InputTag("patJetsReapplyJEC")
# process.ttHLeptons.useReappliedJEC = cms.bool(False)

if options.dump:
    process.dump = cms.EDAnalyzer("EventContentAnalyzer")
    process.dpath = cms.Path(process.dump)
    process.maxEvents.input = cms.untracked.int32(1)

if not options.data:
    process.matchPath = cms.Path(
        process.genParticlesForJetsNoNu *
        process.ak4GenJetsCustom *
        process.selectedHadronsAndPartons *
        process.genJetFlavourInfos *
        process.matchGenBHadron *
        process.matchGenCHadron
    )

process.end = cms.EndPath()

for channel in options.channels:
    if channel == 'ttl':
        taus = (2, 2)
        leptons = (1, 1)
        jets = (2,)
        tags = (1, -1)

        build_combinations = True
    elif channel == 'tll':
        taus = (0, 1)
        leptons = (2, 2)
        jets = (4,)
        tags = (1, -1)

        build_combinations = False
    else:
        raise ValueError("channel needs to be either tll or ttl")

    prod = cms.EDProducer("TauProcessor",
                          data=cms.bool(options.data),
                          electrons=cms.InputTag("ttHLeptons"),
                          muons=cms.InputTag("ttHLeptons"),
                          taus=cms.InputTag("ttHLeptons"),
                          # jets=cms.InputTag("patJetsReapplyJEC"),
                          jets=cms.InputTag("slimmedJets"),
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
                          tauCombinatorics=cms.bool(build_combinations),
                          printPreselection=cms.bool(False),
                          reHLT=cms.bool(options.reHLT),
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
    if options.genFilter:
        path *= process.ttHfilter
    path *= getattr(process, channel + "Taus")
    path *= getattr(process, channel + "Selector")

    setattr(process, channel + "Path", path)

    process.end *= getattr(process, channel + "Output")

convertToUnscheduled(process)
