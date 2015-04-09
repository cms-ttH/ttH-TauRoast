import FWCore.ParameterSet.Config as cms

process = cms.Process("Taus")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 2000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1 ) )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'PHYS14_25_V2::All'
process.prefer("GlobalTag")

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring([
            '/store/mc/RunIIWinter15GS/ttHJetToTT_M125_13TeV_amcatnloFXFX_madspin_pythia8/GEN-SIM/MCRUN2_71_V1-v1/20000/023BAAAB-D0D8-E411-B7EF-0002C90EB9D8.root'
        ])
)

process.TFileService = cms.Service("TFileService",
        closeFileFast = cms.untracked.bool(True),
        fileName = cms.string("validate.root")
)

process.taus = cms.EDAnalyzer("TauGeneratorValidation")

process.p = cms.Path(process.taus)
