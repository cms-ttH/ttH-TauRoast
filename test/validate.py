import FWCore.ParameterSet.Config as cms

process = cms.Process("Taus")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 2000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 40000 ) )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = '74X_mcRun2_asymptotic_v2'

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
    "/store/mc/RunIISpring15MiniAODv2/ttHToTT_M125_13TeV_powheg_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/40000/02FC66B7-CF6D-E511-9B6C-20CF3027A59A.root",
    "/store/mc/RunIISpring15MiniAODv2/ttHToTT_M125_13TeV_powheg_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/40000/10000107-CF6D-E511-98E2-44A8423D7E31.root",
    "/store/mc/RunIISpring15MiniAODv2/ttHToTT_M125_13TeV_powheg_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/40000/10DAD8FF-CE6D-E511-9C21-0CC47A4DEDA2.root",
    "/store/mc/RunIISpring15MiniAODv2/ttHToTT_M125_13TeV_powheg_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/40000/18BB517A-CF6D-E511-9DF6-20CF3027A5D9.root",
    "/store/mc/RunIISpring15MiniAODv2/ttHToTT_M125_13TeV_powheg_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/40000/225296F8-CF6D-E511-9D65-001EC9EAF8B2.root"
] )

process.TFileService = cms.Service("TFileService",
        closeFileFast = cms.untracked.bool(True),
        fileName = cms.string("validate.root")
)

process.taus = cms.EDAnalyzer("TauGeneratorValidation")

process.p = cms.Path(process.taus)
