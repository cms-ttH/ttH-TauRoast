import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

options = VarParsing.VarParsing('analysis')
options.register("printPreselection", False,
        VarParsing.VarParsing.multiplicity.singleton,
        VarParsing.VarParsing.varType.bool,
        "Print information about preselected leptons")
options.parseArguments()

process = cms.Process("Taus")

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 2000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( options.maxEvents ) )

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'PHYS14_25_V2::All'
process.prefer("GlobalTag")

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring([
            '/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/EC51B40A-0F77-E411-AB65-002590A831AA.root'
        ])
)

process.TFileService = cms.Service("TFileService",
        closeFileFast = cms.untracked.bool(True),
        fileName = cms.string("test/multilepton/ttH2Nonbbgg_125/test.root")
)

import os
dirname = os.path.dirname(process.TFileService.fileName.pythonValue().strip("'"))
if not os.path.isdir(dirname):
    os.makedirs(dirname)

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
        minPreselectedLeptons = cms.uint32(2),
        minLooseLeptons = cms.uint32(0),
        minTightLeptons = cms.uint32(0),
        maxLeptons = cms.int32(-1),
        maxTightLeptons = cms.int32(-1),
        minTaus = cms.uint32(0),
        minTightTaus = cms.uint32(0),
        subtractLeptons = cms.bool(False),
        minLooseLeptonPt = cms.double(5.),
        minTightLeptonPt = cms.double(5.),
        minJetPt = cms.double(25.),
        minTagPt = cms.double(25.),
        printPreselection = cms.bool(options.printPreselection)
)

process.filter = cms.EDFilter("HiggsDecayFilter",
        daughterIds = cms.vuint32(15, 23, 24)
)

# process.p = cms.Path(process.filter * process.ttHLeptons * process.taus)
process.p = cms.Path(process.ttHLeptons * process.taus)
