import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

options = VarParsing.VarParsing('analysis')
options.outputFile = "test.root"
options.register("globalTag", "80X_mcRun2_asymptotic_2016_miniAODv2_v1",
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Global tag to use")
options.parseArguments()

process = cms.Process("Filter")

process.options = cms.untracked.PSet(
    wantSummary=cms.untracked.bool(True),
    numberOfThreads=cms.untracked.uint32(2),
    numberOfStreams=cms.untracked.uint32(0),
    SkipEvent=cms.untracked.vstring('ProductNotFound')
)
process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(options.maxEvents))

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 2000

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = options.globalTag

process.source = cms.Source(
    "PoolSource",
    fileNames=cms.untracked.vstring(options.inputFiles)
)

process.njets = cms.EDFilter(
    "GenJetFilter",
    genTag=cms.InputTag('slimmedGenJets'),
    nJets=cms.int32(2),
    minPt=cms.double(20),
    maxEta=cms.double(2.7)
)

process.electron = cms.EDFilter(
    "GenLeptonFilter",
    genTag=cms.InputTag('prunedGenParticles'),
    minPt=cms.double(9),
    maxEta=cms.double(2.7),
    pdgId=cms.int32(11),
    parentPdgId=cms.int32(24)
)

process.muon = cms.EDFilter(
    "GenLeptonFilter",
    genTag=cms.InputTag('prunedGenParticles'),
    minPt=cms.double(9),
    maxEta=cms.double(2.7),
    pdgId=cms.int32(13),
    parentPdgId=cms.int32(24)
)

process.eventf = cms.EDFilter(
    "GenEventFilter",
    genParticles=cms.InputTag('prunedGenParticles'),
    genJets=cms.InputTag('slimmedGenJets'),
    leptonID=cms.vint32(11, 13),
    leptonPt=cms.vdouble(9, 9),
    leptonEta=cms.vdouble(2.7, 2.7),
    jetPt=cms.double(20),
    jetEta=cms.double(2.7),
    tauPt=cms.double(18),
    tauEta=cms.double(2.7),
    minLeptons=cms.int32(1),
    minJets=cms.int32(2),
    minTaus=cms.int32(0),
    minTotal=cms.int32(7),
    minTotalLeptons=cms.int32(1)
)

process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.evpath = cms.Path(process.eventf)

# process.epath = cms.Path(
#     process.njets *
#     process.electron
# )

# process.mpath = cms.Path(
#     process.njets *
#     process.muon
# )

process.output = cms.OutputModule(
    "PoolOutputModule",
    fileName=cms.untracked.string(options.outputFile),
    outputCommands=cms.untracked.vstring(['keep *']),
    # SelectEvents=cms.untracked.PSet(SelectEvents=cms.vstring('epath', 'mpath')),
    SelectEvents=cms.untracked.PSet(SelectEvents=cms.vstring('evpath')),
    dataset=cms.untracked.PSet(filterName=cms.untracked.string(''))
)

process.end = cms.EndPath(process.output)
