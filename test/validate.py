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
            '/store/mc/RunIISpring15MiniAODv2/ttHJetToTT_M125_13TeV_amcatnloFXFX_madspin_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/50000/787D9C97-C26D-E511-8A94-001E67E6F80F.root',
] )

process.TFileService = cms.Service("TFileService",
        closeFileFast = cms.untracked.bool(True),
        fileName = cms.string("validate.root")
)

process.taus = cms.EDAnalyzer("TauGeneratorValidation")

process.p = cms.Path(process.taus)
