import re


def mctag(dset):
    dset = re.sub(r'RunII(?:Spring|Summer)16MiniAODv2.*80X_mcRun2_asymptotic_(?:v14|2016_miniAODv2_v0|2016_TrancheIV_v6).', '', dset)
    dset = re.sub(r'matze-(faster_v[0-9]+_).*_(?:[0-9a-f]+).', r'\1', dset)
    return dset

datasets_tranche3 = """
/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
/TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
/ttHTobb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
"""

datasets_train = """
/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/matze-faster_v5_ttjets_dl_maod_777fc0041cce4fac803f2dbb53a837f5-v1/USER
/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/matze-faster_v6_ttjets_dl_maod_5f67423d861446de8e222e7a6ba53e0e-v1/USER
/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/matze-faster_v7_ttjets_dl_maod_74cbe421a2e94aaeb1e9d71c9d1364bd-v1/USER
/TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/matze-faster_v5_ttjets_sl_maod_777fc0041cce4fac803f2dbb53a837f5-v1/USER
/TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/matze-faster_v5_ttjets_sl_maod_777fc0041cce4fac803f2dbb53a837f5-v2/USER
/TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/matze-faster_v6_ttjets_sl_maod_5f67423d861446de8e222e7a6ba53e0e-v1/USER
/TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/matze-faster_v7_ttjets_sl_maod_74cbe421a2e94aaeb1e9d71c9d1364bd-v1/USER
/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/matze-faster_v5_ttH_maod_777fc0041cce4fac803f2dbb53a837f5-v1/USER
/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/matze-faster_v6_ttH_maod_5f67423d861446de8e222e7a6ba53e0e-v1/USER
/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/matze-faster_v7_ttH_maod_74cbe421a2e94aaeb1e9d71c9d1364bd-v1/USER
"""

datasets_ttH = """
# /ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_mWCutfix/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext1-v1/MINIAODSIM
# /ttHJetTobb_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext3-v1/MINIAODSIM
/ttHJetToTT_M120_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
"""

datasets_mc = """
/WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM
# /WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM
/TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
/TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM
/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM
/WpWpJJ_EWK-QCD_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM

/WZZ_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
# /WW_DoubleScattering_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/tZq_ll_4f_13TeV-amcatnlo-herwigpp/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
/tZq_ll_4f_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM
/TTTT_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM

/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/MINIAODSIM
/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM
/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM

/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
/TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM
/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
/TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM
/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
/TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM

# /DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM
/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_HCALDebug_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/MINIAODSIM
/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM
/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM
/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
/WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
/WWTo2L2Nu_13TeV-powheg/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
/ZZTo4L_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM
"""

datasets_collisions_prompt = """
/SingleElectron/Run2016B-PromptReco-v2/MINIAOD
/SingleElectron/Run2016C-PromptReco-v2/MINIAOD
/SingleElectron/Run2016D-PromptReco-v2/MINIAOD
/SingleElectron/Run2016E-PromptReco-v2/MINIAOD
/SingleElectron/Run2016F-PromptReco-v1/MINIAOD
/SingleElectron/Run2016G-PromptReco-v1/MINIAOD
/SingleElectron/Run2016H-PromptReco-v2/MINIAOD
/SingleElectron/Run2016H-PromptReco-v3/MINIAOD

/SingleMuon/Run2016B-PromptReco-v2/MINIAOD
/SingleMuon/Run2016C-PromptReco-v2/MINIAOD
/SingleMuon/Run2016D-PromptReco-v2/MINIAOD
/SingleMuon/Run2016E-PromptReco-v2/MINIAOD
/SingleMuon/Run2016F-PromptReco-v1/MINIAOD
/SingleMuon/Run2016G-PromptReco-v1/MINIAOD
/SingleMuon/Run2016H-PromptReco-v2/MINIAOD
/SingleMuon/Run2016H-PromptReco-v3/MINIAOD
"""

datasets_collisions_rereco = """
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

# /DoubleEG/Run2016B-23Sep2016-v3/MINIAOD
# /DoubleEG/Run2016C-23Sep2016-v1/MINIAOD
# /DoubleEG/Run2016D-23Sep2016-v1/MINIAOD
# /DoubleEG/Run2016E-23Sep2016-v1/MINIAOD
# /DoubleEG/Run2016F-23Sep2016-v1/MINIAOD
# /DoubleEG/Run2016G-23Sep2016-v1/MINIAOD
# /DoubleEG/Run2016H-PromptReco-v2/MINIAOD
# /DoubleEG/Run2016H-PromptReco-v3/MINIAOD

# /DoubleMuon/Run2016B-23Sep2016-v3/MINIAOD
# /DoubleMuon/Run2016C-23Sep2016-v1/MINIAOD
# /DoubleMuon/Run2016D-23Sep2016-v1/MINIAOD
# /DoubleMuon/Run2016E-23Sep2016-v1/MINIAOD
# /DoubleMuon/Run2016F-23Sep2016-v1/MINIAOD
# /DoubleMuon/Run2016G-23Sep2016-v1/MINIAOD
# /DoubleMuon/Run2016H-PromptReco-v2/MINIAOD
# /DoubleMuon/Run2016H-PromptReco-v3/MINIAOD

# /MuonEG/Run2016B-23Sep2016-v3/MINIAOD
# /MuonEG/Run2016C-23Sep2016-v1/MINIAOD
# /MuonEG/Run2016D-23Sep2016-v1/MINIAOD
# /MuonEG/Run2016E-23Sep2016-v1/MINIAOD
# /MuonEG/Run2016F-23Sep2016-v1/MINIAOD
# /MuonEG/Run2016G-23Sep2016-v1/MINIAOD
# /MuonEG/Run2016H-PromptReco-v2/MINIAOD
# /MuonEG/Run2016H-PromptReco-v3/MINIAOD
"""


def datasets(tag):
    if tag == 'all':
        dsets = datasets_ttH + datasets_mc + datasets_collisions_rereco + datasets_tranche3
    elif tag == 'ttH':
        dsets = datasets_ttH
    elif tag == 'mc':
        dsets = datasets_ttH + datasets_mc
    elif tag == 'data':
        dsets = datasets_collisions_prompt
    elif tag == 'bkgs':
        dsets = datasets_mc
    elif tag == 'tranche3':
        dsets = datasets_tranche3
    elif tag == 'tranche3_full':
        dsets = datasets_tranche3
    elif tag == 'train':
        dsets = datasets_train
    else:
        raise ValueError("invalid tag '{}'".format(tag))
    return [ds.strip() for ds in dsets.split('\n') if len(ds) > 0 and not ds.startswith('#')]
