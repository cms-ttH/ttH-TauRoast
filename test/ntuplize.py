import os.path


def add_channel(channel, path):
    dirname, filename = os.path.split(path)
    return os.path.join(dirname, "{}_{}".format(channel, filename))

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

options = VarParsing.VarParsing('analysis')
options.outputFile = "test.root"
options.register("channels", "",
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "Which channel(s) to run")
options.register("dataset", None,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Which dataset is being processed")
options.register("takeAll", False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Save as many events as possible")
options.register("data", False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Indicate if data is being used (or MC)")
options.register("globalTag", "76X_mcRun2_asymptotic_v12",
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Global tag to use")
options.setDefault("channels", "ttl,tll")
options.parseArguments()

process = cms.Process("Taus")

process.options = cms.untracked.PSet(
    wantSummary=cms.untracked.bool(True),
    numberOfThreads=cms.untracked.uint32(2),
    numberOfStreams=cms.untracked.uint32(0)
)


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 2000

process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(options.maxEvents))

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = options.globalTag

process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring(options.inputFiles)
                            )

# process.TFileService = cms.Service("TFileService",
#         closeFileFast = cms.untracked.bool(True),
#         fileName = cms.string(options.outputFile)
# )

# from JetMETCorrections.Configuration.JetCorrectionServices_cff import *

# process.ak4PFCHSL1Fastjet = cms.ESProducer(
#     'L1FastjetCorrectionESProducer',
#     level=cms.string('L1FastJet'),
#     algorithm=cms.string('AK4PFchs'),
#     srcRho=cms.InputTag('fixedGridRhoFastjetAll')
# )
# process.ak4PFchsL2Relative = ak4CaloL2Relative.clone(algorithm='AK4PFchs')
# process.ak4PFchsL3Absolute = ak4CaloL3Absolute.clone(algorithm='AK4PFchs')
# process.ak4PFchsL1L2L3 = cms.ESProducer("JetCorrectionESChain",
#                                         correctors=cms.vstring(
#                                             'ak4PFCHSL1Fastjet',
#                                             'ak4PFchsL2Relative',
#                                             'ak4PFchsL3Absolute')
#                                         )

# if options.data:
#     process.ak4PFchsResidual = ak4CaloResidual.clone(algorithm='AK4PFchs')
#     process.ak4PFchsL1L2L3.correctors.append('ak4PFchsResidual')

process.selector = cms.EDFilter("TauSelector",
                                inputs=cms.VInputTag()
                                )

from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetCorrFactorsUpdated
process.patJetCorrFactorsReapplyJEC = patJetCorrFactorsUpdated.clone(
    src=cms.InputTag("slimmedJets"),
    levels=['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual'],
    payload='AK4PFchs'  # Make sure to choose the appropriate levels and payload here!
)


from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetsUpdated
process.patJetsReapplyJEC = patJetsUpdated.clone(
    jetSource=cms.InputTag("slimmedJets"),
    jetCorrFactorsSource=cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJEC"))
)

process.jetPath = cms.Path(
    process.patJetCorrFactorsReapplyJEC
    * process.patJetsReapplyJEC
)

process.load("RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("ttH.LeptonID.ttHLeptons_cfi")
process.load("ttH.TauRoast.genHadronMatching_cfi")

process.ttHLeptons.jets = cms.InputTag("patJetsReapplyJEC")

process.dump = cms.EDAnalyzer("EventContentAnalyzer")

trig_single_e = []
trig_single_mu = []
trig_double_eg = []
trig_double_mu = []
trig_mixed = []

process.end = cms.EndPath()

for channel in options.channels:
    if channel == 'ttl':
        taus = (2, 2)
        leptons = (1, 1)
        jets = (2,)
        tags = (1, -1)

        build_combinations = True

        trig_single_e = "HLT_Ele27_eta2p1_WPLoose_Gsf_v" if options.data else "HLT_Ele27_WPLoose_Gsf_v"
        trig_single_mu = "HLT_IsoMu18_v" if options.data else "HLT_IsoMu17_eta2p1_v"
    elif channel == 'tll':
        taus = (0, 1)
        leptons = (2, 2)
        jets = (4,)
        tags = (1, -1)

        build_combinations = False

        trig_double_eg = [
            "HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v",
            ("HLT_Ele23_WPLoose_Gsf_v" if options.data else "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v"),
        ]
        trig_double_mu = [
            "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
            "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v",
            "HLT_IsoMu20_v",
            "HLT_IsoTkMu20_v"
        ]
        trig_mixed = [
            "HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v",
            "HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v",
            "HLT_IsoMu20_v",
            "HLT_IsoTkMu20_v",
            ("HLT_Ele23_WPLoose_Gsf_v" if options.data else "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v")
        ]
    else:
        raise ValueError("channel needs to be either tll or ttl")

    prod = cms.EDProducer("TauProcessor",
                          data=cms.bool(options.data),
                          electrons=cms.InputTag("ttHLeptons"),
                          muons=cms.InputTag("ttHLeptons"),
                          taus=cms.InputTag("ttHLeptons"),
                          jets=cms.InputTag("patJetsReapplyJEC"),
                          # jets=cms.InputTag("slimmedJets"),
                          minTaus=cms.uint32(taus[0]),
                          maxTaus=cms.uint32(taus[1]),
                          minLeptons=cms.uint32(leptons[0]),
                          maxLeptons=cms.uint32(leptons[1]),
                          minJets=cms.uint32(jets[0]),
                          minTags=cms.uint32(tags[0]),
                          maxTags=cms.int32(tags[1]),
                          minJetPt=cms.double(25.),
                          minTagPt=cms.double(25.),
                          maxJetEta=cms.double(2.4),
                          filterPUJets=cms.bool(False),
                          takeAll=cms.bool(options.takeAll),
                          tauCombinatorics=cms.bool(build_combinations),
                          printPreselection=cms.bool(False),
                          triggerSingleE=cms.vstring(trig_single_e),
                          triggerSingleMu=cms.vstring(trig_single_mu),
                          triggerDoubleE=cms.vstring(trig_double_eg),
                          triggerDoubleMu=cms.vstring(trig_double_mu),
                          triggerMixed=cms.vstring(trig_mixed),
                          debugEvents=cms.vuint32()
                          )

    selector = cms.EDFilter("TauSelector",
                            input=cms.InputTag(channel + "Taus")
                            )

    output = cms.OutputModule("PoolOutputModule",
                              fileName=cms.untracked.string(
                                  add_channel(channel, options.outputFile)),
                              outputCommands=cms.untracked.vstring(
                                  ['drop *', 'keep *_{}_*_*'.format(channel + "Taus")]),
                              SelectEvents=cms.untracked.PSet(
                                  SelectEvents=cms.vstring(channel + "Path")),
                              )

    setattr(process, channel + "Taus", prod)
    setattr(process, channel + "Selector", selector)

    setattr(process, channel + "Output", output)

    # writer = cms.EDAnalyzer("TauWriter",
    #         input = cms.InputTag("taus")
    # )

    if not options.data:
        path = cms.Path(
            process.genParticlesForJetsNoNu
            * process.ak4GenJetsCustom
            * process.selectedHadronsAndPartons
            * process.genJetFlavourInfos
            * process.matchGenBHadron
            * process.matchGenCHadron
        )
    else:
        path = cms.Path()

    path *= process.electronMVAValueMapProducer
    path *= process.ttHLeptons
    path *= getattr(process, channel + "Taus")
    path *= getattr(process, channel + "Selector")

    setattr(process, channel + "Path", path)

    process.end *= getattr(process, channel + "Output")
