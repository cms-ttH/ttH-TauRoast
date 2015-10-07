import os
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

infile = 'root://xrootd-cms.infn.it:1094//store/user/shwillia/Spring15_HbbSync/ttbar_Spring15_HbbSync.root'
if os.path.isfile(os.path.basename(infile)):
    infile = 'file:' + os.path.basename(infile)

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring([
            infile
        ])
)

process.TFileService = cms.Service("TFileService",
        closeFileFast = cms.untracked.bool(True),
        fileName = cms.string("test/leptonplusjets/ttH2bb_125/test.root")
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

process.taus = cms.EDAnalyzer("TauProcessor",
        electrons = cms.InputTag("slimmedElectrons"),
        muons = cms.InputTag("slimmedMuons"),
        minJets = cms.uint32(2),
        minTags = cms.uint32(1),
        maxTags = cms.int32(2),
        minPreselectedLeptons = cms.uint32(1),
        minLooseLeptons = cms.uint32(1),
        minTightLeptons = cms.uint32(1),
        maxLooseLeptons = cms.int32(1),
        maxTightLeptons = cms.int32(1),
        minTaus = cms.uint32(0),
        minTightTaus = cms.uint32(0),
        subtractLeptons = cms.bool(False),
        minLooseLeptonPt = cms.double(10.),
        minTightLeptonPt = cms.double(20.),
        minJetPt = cms.double(30.),
        minTagPt = cms.double(20.),
        printPreselection = cms.bool(False)
)

process.p = cms.Path(process.taus)
