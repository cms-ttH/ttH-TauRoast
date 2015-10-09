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

infile = 'root://cmsxrootd.fnal.gov//store/user/shwillia/Spring15_Sync/ttHbb_spring15_25ns_plusboostedjets.root'
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

from JetMETCorrections.Configuration.JetCorrectionServices_cff import *

process.ak4PFCHSL1Fastjet = cms.ESProducer(
        'L1FastjetCorrectionESProducer',
        level       = cms.string('L1FastJet'),
        algorithm   = cms.string('AK4PFchs'),
        srcRho      = cms.InputTag('fixedGridRhoFastjetAll')
)
process.ak4PFchsL2Relative = ak4CaloL2Relative.clone(algorithm = 'AK4PFchs')
process.ak4PFchsL3Absolute = ak4CaloL3Absolute.clone(algorithm = 'AK4PFchs')
process.ak4PFchsL1L2L3 = cms.ESProducer("JetCorrectionESChain",
        correctors = cms.vstring(
            'ak4PFCHSL1Fastjet',
            'ak4PFchsL2Relative',
            'ak4PFchsL3Absolute')
)

process.taus = cms.EDAnalyzer("TauProcessor",
        electrons = cms.InputTag("slimmedElectrons"),
        muons = cms.InputTag("slimmedMuons"),
        minJets = cms.uint32(0),
        minTags = cms.uint32(0),
        maxTags = cms.int32(-1),
        minPreselectedLeptons = cms.uint32(1),
        minLooseLeptons = cms.uint32(1),
        minTightLeptons = cms.uint32(1),
        maxLooseLeptons = cms.int32(1),
        maxTightLeptons = cms.int32(1),
        minTaus = cms.uint32(0),
        minTightTaus = cms.uint32(0),
        subtractLeptons = cms.bool(False),
        minLooseLeptonPt = cms.double(10.),
        minTightLeptonPt = cms.double(30.),
        maxLooseLeptonEta = cms.double(2.4),
        maxTightLeptonEta = cms.double(2.1),
        minJetPt = cms.double(30.),
        minTagPt = cms.double(30.),
        maxJetEta = cms.double(2.4),
        filterPUJets = cms.bool(False),
        printPreselection = cms.bool(False),
        triggerSelection = cms.string("HLT_Ele27_eta2p1_WP85_Gsf_HT200_v1 OR HLT_IsoMu24_eta2p1_v1")
)

process.p = cms.Path(process.taus)
