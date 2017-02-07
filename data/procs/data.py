from ttH.TauRoast.processing import BasicProcess, CombinedProcess

BasicProcess(
    name="collisions_single_e",
    fullname="Collisions",
    limitname="data_obs_e",
    additional_cuts=[('trigger selection', 'event.trigger().accepted({"HLT_Ele27_eta2p1_WPLoose_Gsf_v"})')],
    paths=[
        'SingleElectron_Run2016B_23Sep2016_v3',
        'SingleElectron_Run2016C_23Sep2016_v1',
        'SingleElectron_Run2016D_23Sep2016_v1',
        'SingleElectron_Run2016E_23Sep2016_v1',
        'SingleElectron_Run2016F_23Sep2016_v1',
        'SingleElectron_Run2016G_23Sep2016_v1',
        'SingleElectron_Run2016H_PromptReco_v2',
        'SingleElectron_Run2016H_PromptReco_v3',
    ],
    events=246440440 + 97337279 + 148167727 + 117321545 + 70593532 + 153363109 + 126863489 + 3191585,
    cutflow='data signal'
)

BasicProcess(
    name="collisions_single_mu",
    fullname="Collisions",
    limitname="data_obs_mu",
    additional_cuts=[('trigger selection', 'event.trigger().accepted({"HLT_IsoMu22_v", "HLT_IsoTkMu22_v"})')],
    paths=[
        'SingleMuon_Run2016B_23Sep2016_v3',
        'SingleMuon_Run2016C_23Sep2016_v1',
        'SingleMuon_Run2016D_23Sep2016_v1',
        'SingleMuon_Run2016E_23Sep2016_v1',
        'SingleMuon_Run2016F_23Sep2016_v1',
        'SingleMuon_Run2016G_23Sep2016_v1',
        'SingleMuon_Run2016H_PromptReco_v2',
        'SingleMuon_Run2016H_PromptReco_v3',
    ],
    events=158145722 + 67441308 + 98017996 + 90984718 + 65554000 + 150014043 + 171134793 + 4393222,
    cutflow='data signal'
)

CombinedProcess(
    name="collisions_single",
    fullname="Collisions",
    limitname="data_obs",
    subprocesses=["collisions_single_e", "collisions_single_mu"]
)

BasicProcess(
    name="fakes_single_e",
    fullname="Fakes",
    limitname="fake_e",
    additional_cuts=[('trigger selection', 'event.trigger().accepted({"HLT_Ele27_eta2p1_WPLoose_Gsf_v"})')],
    paths=[
        'SingleElectron_Run2016B_23Sep2016_v3',
        'SingleElectron_Run2016C_23Sep2016_v1',
        'SingleElectron_Run2016D_23Sep2016_v1',
        'SingleElectron_Run2016E_23Sep2016_v1',
        'SingleElectron_Run2016F_23Sep2016_v1',
        'SingleElectron_Run2016G_23Sep2016_v1',
        'SingleElectron_Run2016H_PromptReco_v2',
        'SingleElectron_Run2016H_PromptReco_v3',
    ],
    events=246440440 + 97337279 + 148167727 + 117321545 + 70593532 + 153363109 + 126863489 + 3191585,
    cutflow='data fake'
)

BasicProcess(
    name="fakes_single_mu",
    fullname="Fakes",
    limitname="fake_mu",
    additional_cuts=[('trigger selection', 'event.trigger().accepted({"HLT_IsoMu22_v", "HLT_IsoTkMu22_v"})')],
    paths=[
        'SingleMuon_Run2016B_23Sep2016_v3',
        'SingleMuon_Run2016C_23Sep2016_v1',
        'SingleMuon_Run2016D_23Sep2016_v1',
        'SingleMuon_Run2016E_23Sep2016_v1',
        'SingleMuon_Run2016F_23Sep2016_v1',
        'SingleMuon_Run2016G_23Sep2016_v1',
        'SingleMuon_Run2016H_PromptReco_v2',
        'SingleMuon_Run2016H_PromptReco_v3',
    ],
    events=158145722 + 67441308 + 98017996 + 90984718 + 65554000 + 150014043 + 171134793 + 4393222,
    cutflow='data fake'
)

CombinedProcess(
    name="fakes_single",
    fullname="Fakes",
    limitname="fakes_data",
    subprocesses=["fakes_single_e", "fakes_single_mu"]
)
