import FWCore.ParameterSet.Config as cms

process = cms.Process("Taus")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 2000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1 ) )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'PHYS14_25_V2::All'
process.prefer("GlobalTag")

process.load("files_ttjets")
# process.source = cms.Source("PoolSource",
#         fileNames = cms.untracked.vstring([
#             '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/00000/08B36E8F-5E7F-E411-9D5A-002590200AE4.root',
#             '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/00000/FC4E6E16-5C7F-E411-8843-002590200AE4.root',
#             '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/10000/629CE4D5-687F-E411-BF71-001E673969FA.root',
#             '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/20000/14587980-CB7E-E411-A0F4-001E67397701.root',
#             '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/30000/9E314FC0-067F-E411-9500-001E67397B11.root',
#             '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/40000/78722DCF-E57E-E411-B437-002590A4FFB8.root',
#             '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v2/40000/F87FB415-E57E-E411-B7CF-002590A4FFB8.root'
#         ])
# )

process.TFileService = cms.Service("TFileService",
        closeFileFast = cms.untracked.bool(True),
        fileName = cms.string("test.root")
)

process.taus = cms.EDAnalyzer("TauProcessor",
        minJets = cms.uint32(2),
        minTags = cms.uint32(1),
        maxTags = cms.uint32(2),
        minLeptons = cms.uint32(1),
        minTightLeptons = cms.uint32(1),
        minTaus = cms.uint32(2),
        minTightTaus = cms.uint32(1)
)

process.p = cms.Path(process.taus)
