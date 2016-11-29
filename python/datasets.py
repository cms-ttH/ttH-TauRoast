import re

mctag = re.compile(r'RunIISpring16MiniAODv2.*80X_mcRun2_asymptotic_(v14|2016_miniAODv2_v0).|matze-.*_([0-9a-f]+).')

datasets_tranche3 = """
/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISpring16MiniAODv2-premix_withHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM
/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISpring16MiniAODv2-premix_withHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM
/TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISpring16MiniAODv2-premix_withHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM
/ttHTobb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISpring16MiniAODv2-premix_withHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM
"""

datasets_tranche3_fast = """
/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/matze-fast_ttjets_dl_MiniAOD_6b57d231e28e4ebd8065fc7621fa1f5b-v1/USER
/TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/matze-fast_ttjets_sl_MiniAOD_6b57d231e28e4ebd8065fc7621fa1f5b-v1/USER
/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/matze-fast_ttH_MiniAOD_6b57d231e28e4ebd8065fc7621fa1f5b-v1/USER
"""

datasets_ttH = """
/ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_mWCutfix/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext1-v1/MINIAODSIM
/ttHJetTobb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext3-v1/MINIAODSIM
/ttHJetToTT_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM

/ttHToNonbb_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM
/ttHTobb_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM
/ttHToTT_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM
"""

datasets_mc = """
/WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/WpWpJJ_EWK-QCD_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM

/WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/WW_DoubleScattering_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/tZq_ll_4f_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM

/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM

/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM
/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM
/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v4/MINIAODSIM
/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM

#
# SCALE TEST!!!
#
/TTToSemiLeptonic_13TeV-powheg/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v2/MINIAODSIM
/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext4-v1/MINIAODSIM
/TTTo2L2Nu_13TeV-powheg/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM
/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/TTToSemiLeptonic_TuneCUETP8M1_alphaS01273_13TeV-powheg-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
#
# SCALE TEST END!!!
#

/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
# /DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-BS2016_BSandPUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1/MINIAODSIM
/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v2/MINIAODSIM
/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/WWTo2L2Nu_13TeV-powheg/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/ZZTo4L_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
"""

datasets_collisions = """
/SingleElectron/Run2016B-23Sep2016-v3/MINIAOD
/SingleElectron/Run2016C-23Sep2016-v1/MINIAOD
/SingleElectron/Run2016D-23Sep2016-v1/MINIAOD
/SingleElectron/Run2016E-23Sep2016-v1/MINIAOD
/SingleElectron/Run2016F-23Sep2016-v1/MINIAOD
/SingleElectron/Run2016G-23Sep2016-v1/MINIAOD
/SingleElectron/Run2016H-PromptReco-v2/MINIAOD
/SingleElectron/Run2016H-PromptReco-v3/MINIAOD

/SingleMuon/Run2016B-23Sep2016-v3/MINIAOD
/SingleMuon/Run2016C-23Sep2016-v1/MINIAOD
/SingleMuon/Run2016D-23Sep2016-v1/MINIAOD
/SingleMuon/Run2016E-23Sep2016-v1/MINIAOD
/SingleMuon/Run2016F-23Sep2016-v1/MINIAOD
/SingleMuon/Run2016G-23Sep2016-v1/MINIAOD
/SingleMuon/Run2016H-PromptReco-v2/MINIAOD
/SingleMuon/Run2016H-PromptReco-v3/MINIAOD

/DoubleEG/Run2016B-23Sep2016-v3/MINIAOD
/DoubleEG/Run2016C-23Sep2016-v1/MINIAOD
/DoubleEG/Run2016D-23Sep2016-v1/MINIAOD
/DoubleEG/Run2016E-23Sep2016-v1/MINIAOD
/DoubleEG/Run2016F-23Sep2016-v1/MINIAOD
/DoubleEG/Run2016G-23Sep2016-v1/MINIAOD
/DoubleEG/Run2016H-PromptReco-v2/MINIAOD
/DoubleEG/Run2016H-PromptReco-v3/MINIAOD

/DoubleMuon/Run2016B-23Sep2016-v3/MINIAOD
/DoubleMuon/Run2016C-23Sep2016-v1/MINIAOD
/DoubleMuon/Run2016D-23Sep2016-v1/MINIAOD
/DoubleMuon/Run2016E-23Sep2016-v1/MINIAOD
/DoubleMuon/Run2016F-23Sep2016-v1/MINIAOD
/DoubleMuon/Run2016G-23Sep2016-v1/MINIAOD
/DoubleMuon/Run2016H-PromptReco-v2/MINIAOD
/DoubleMuon/Run2016H-PromptReco-v3/MINIAOD

/MuonEG/Run2016B-23Sep2016-v3/MINIAOD
/MuonEG/Run2016C-23Sep2016-v1/MINIAOD
/MuonEG/Run2016D-23Sep2016-v1/MINIAOD
/MuonEG/Run2016E-23Sep2016-v1/MINIAOD
/MuonEG/Run2016F-23Sep2016-v1/MINIAOD
/MuonEG/Run2016G-23Sep2016-v1/MINIAOD
/MuonEG/Run2016H-PromptReco-v2/MINIAOD
/MuonEG/Run2016H-PromptReco-v3/MINIAOD
"""


def datasets(tag):
    if tag == 'all':
        dsets = datasets_ttH + datasets_mc + datasets_collisions + datasets_tranche3
    elif tag == 'ttH':
        dsets = datasets_ttH
    elif tag == 'mc':
        dsets = datasets_ttH + datasets_mc
    elif tag == 'data':
        dsets = datasets_collisions
    elif tag == 'bkgs':
        dsets = datasets_mc
    elif tag == 'tranche3':
        dsets = datasets_tranche3 + datasets_tranche3_fast
    elif tag == 'tranche3_fast':
        dsets = datasets_tranche3_fast
    elif tag == 'tranche3_full':
        dsets = datasets_tranche3
    else:
        raise ValueError("invalid tag '{}'".format(tag))
    return [ds.strip() for ds in dsets.split('\n') if len(ds) > 0 and not ds.startswith('#')]
