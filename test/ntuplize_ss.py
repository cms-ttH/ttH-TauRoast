import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

options = VarParsing.VarParsing('analysis')
options.parseArguments()

process = cms.Process("Taus")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 2000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( options.maxEvents ) )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'MCRUN2_74_V9::All'
process.prefer("GlobalTag")

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring([
            '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/EC51B40A-0F77-E411-AB65-002590A831AA.root'
        ])
)

process.TFileService = cms.Service("TFileService",
        closeFileFast = cms.untracked.bool(True),
        fileName = cms.string("test.root")
)

from RecoJets.Configuration.RecoJets_cff import *
from RecoJets.Configuration.RecoPFJets_cff import *
from JetMETCorrections.Configuration.JetCorrectionProducersAllAlgos_cff import *
from JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff import *
from JetMETCorrections.Configuration.JetCorrectionServices_cff import *

process.ak4PFCHSL1Fastjet = cms.ESProducer(
        'L1FastjetCorrectionESProducer',
        level       = cms.string('L1FastJet'),
        algorithm   = cms.string('AK4PFchs'),
        srcRho      = cms.InputTag( 'fixedGridRhoFastjetAll' ),
        useCondDB = cms.untracked.bool(True)
        )
process.ak4PFchsL2Relative   =  ak5PFL2Relative.clone( algorithm = 'AK4PFchs' )
process.ak4PFchsL3Absolute   =  ak5PFL3Absolute.clone( algorithm = 'AK4PFchs' )
process.ak4PFchsL1L2L3 = cms.ESProducer("JetCorrectionESChain",
        correctors = cms.vstring(
            'ak4PFCHSL1Fastjet',
            'ak4PFchsL2Relative',
            'ak4PFchsL3Absolute'),
        useCondDB = cms.untracked.bool(True)
)

process.load("ttH.LeptonID.ttHLeptons_cfi")

process.taus = cms.EDAnalyzer("TauProcessor",
        electrons = cms.InputTag("ttHLeptons"),
        muons = cms.InputTag("ttHLeptons"),
        minJets = cms.uint32(0),
        minTags = cms.uint32(0),
        maxTags = cms.int32(-1),
        minLeptons = cms.uint32(2),
        minTightLeptons = cms.uint32(0),
        maxLeptons = cms.int32(-1),
        maxTightLeptons = cms.int32(-1),
        minTaus = cms.uint32(0),
        minTightTaus = cms.uint32(0),
        ssLeptons = cms.bool(True)
)

process.filter = cms.EDFilter("HiggsDecayFilter",
        daughterIds = cms.vuint32(15, 23, 24)
)

process.p = cms.Path(process.filter * process.ttHLeptons * process.taus)
