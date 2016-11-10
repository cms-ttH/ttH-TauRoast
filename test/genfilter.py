import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

options = VarParsing.VarParsing('analysis')
options.outputFile = "test.root"
options.register("globalTag", "80X_mcRun2_asymptotic_2016_miniAODv2_v1",
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Global tag to use")
options.register("events", "",
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "Selected events to run over")
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

if len(options.events) > 0:
    process.source.eventsToProcess = cms.untracked.VEventRange(options.events)

process.load('ttH.TauMCGeneration.eventFilterMAOD_cfi')
process.ttHGenFilter.useFakeTaus = cms.bool(True)
process.ttHGenFilter.minTaus = cms.int32(2)
process.ttHGenFilter.fakeCut = cms.double(0.)

process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.evpath = cms.Path(process.ttHfilter)

process.output = cms.OutputModule(
    "PoolOutputModule",
    fileName=cms.untracked.string(options.outputFile),
    outputCommands=cms.untracked.vstring(['keep *']),
    # SelectEvents=cms.untracked.PSet(SelectEvents=cms.vstring('epath', 'mpath')),
    SelectEvents=cms.untracked.PSet(SelectEvents=cms.vstring('evpath')),
    dataset=cms.untracked.PSet(filterName=cms.untracked.string(''))
)

process.end = cms.EndPath(process.output)
