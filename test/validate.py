import FWCore.ParameterSet.Config as cms

process = cms.Process("Taus")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 2000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( 4000 ) )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'PHYS14_25_V2::All'
process.prefer("GlobalTag")

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
            '/store/mc/RunIISpring15DR74/ttHJetToTT_M125_13TeV_amcatnloFXFX_madspin_pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/00D1004A-3B03-E511-8FE3-0025905A60B4.root',
] )

process.TFileService = cms.Service("TFileService",
        closeFileFast = cms.untracked.bool(True),
        fileName = cms.string("validate.root")
)

process.taus = cms.EDAnalyzer("TauGeneratorValidation")

process.p = cms.Path(process.taus)
