from ttH.TauRoast.processing import BasicProcess, CombinedProcess

BasicProcess(
    name="collisions_double_e",
    fullname="Collisions",
    additional_cuts=[('trigger selection', 'len(leptons) == 2 and leptons[0].electron() and leptons[1].electron()')],
    paths=["data_DoubleEG_2015D_Dec16"],
    events=93018487
)

BasicProcess(
    name="collisions_double_mu",
    fullname="Collisions",
    additional_cuts=[('trigger selection', 'len(leptons) == 2 and leptons[0].muon() and leptons[1].muon()')],
    paths=["data_DoubleMu_2015D_Dec16"],
    events=51342919
)

BasicProcess(
    name="collisions_mu_egamma",
    fullname="Collisions",
    additional_cuts=[('trigger selection', 'len(leptons) == 2 and (leptons[0].muon() != leptons[1].muon())')],
    paths=["data_MuonEG_2015D_Dec16"],
    events=13442593,
    cutflow='data signal'
)

BasicProcess(
    name="collisions_single_e",
    fullname="Collisions",
    limitname="data_obs_e",
    additional_cuts=[('trigger selection', 'event.trigger().accepted({"HLT_Ele27_eta2p1_WPLoose_Gsf_v"})')],
    paths=[
        'SingleElectron_Run2016B_PromptReco_v2',
        'SingleElectron_Run2016C_PromptReco_v2',
        'SingleElectron_Run2016D_PromptReco_v2',
        'SingleElectron_Run2016E_PromptReco_v2',
        'SingleElectron_Run2016F_PromptReco_v1',
        'SingleElectron_Run2016G_PromptReco_v1',
        'SingleElectron_Run2016H_PromptReco_v2',
        'SingleElectron_Run2016H_PromptReco_v3',
        # 'SingleElectron_Run2016B_23Sep2016_v3',
        # 'SingleElectron_Run2016C_23Sep2016_v1',
        # 'SingleElectron_Run2016D_23Sep2016_v1',
        # 'SingleElectron_Run2016E_23Sep2016_v1',
        # 'SingleElectron_Run2016F_23Sep2016_v1',
        # 'SingleElectron_Run2016G_23Sep2016_v1',
        # 'SingleElectron_Run2016H_PromptReco_v2',
        # 'SingleElectron_Run2016H_PromptReco_v3',
    ],
    events=963278706,
    cutflow='data signal'
)

BasicProcess(
    name="collisions_single_mu",
    fullname="Collisions",
    limitname="data_obs_mu",
    additional_cuts=[('trigger selection', 'event.trigger().accepted({"HLT_IsoMu22_v", "HLT_IsoTkMu22_v"})')],
    paths=[
        'SingleMuon_Run2016B_PromptReco_v2',
        'SingleMuon_Run2016C_PromptReco_v2',
        'SingleMuon_Run2016D_PromptReco_v2',
        'SingleMuon_Run2016E_PromptReco_v2',
        'SingleMuon_Run2016F_PromptReco_v1',
        'SingleMuon_Run2016G_PromptReco_v1',
        'SingleMuon_Run2016H_PromptReco_v2',
        'SingleMuon_Run2016H_PromptReco_v3',
        # 'SingleMuon_Run2016B_23Sep2016_v3',
        # 'SingleMuon_Run2016C_23Sep2016_v1',
        # 'SingleMuon_Run2016D_23Sep2016_v1',
        # 'SingleMuon_Run2016E_23Sep2016_v1',
        # 'SingleMuon_Run2016F_23Sep2016_v1',
        # 'SingleMuon_Run2016G_23Sep2016_v1',
        # 'SingleMuon_Run2016H_PromptReco_v2',
        # 'SingleMuon_Run2016H_PromptReco_v3',
    ],
    events=805524162,
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
        'SingleElectron_Run2016B_PromptReco_v2',
        'SingleElectron_Run2016C_PromptReco_v2',
        'SingleElectron_Run2016D_PromptReco_v2',
        'SingleElectron_Run2016E_PromptReco_v2',
        'SingleElectron_Run2016F_PromptReco_v1',
        'SingleElectron_Run2016G_PromptReco_v1',
        'SingleElectron_Run2016H_PromptReco_v2',
        'SingleElectron_Run2016H_PromptReco_v3',
        # 'SingleElectron_Run2016B_23Sep2016_v3',
        # 'SingleElectron_Run2016C_23Sep2016_v1',
        # 'SingleElectron_Run2016D_23Sep2016_v1',
        # 'SingleElectron_Run2016E_23Sep2016_v1',
        # 'SingleElectron_Run2016F_23Sep2016_v1',
        # 'SingleElectron_Run2016G_23Sep2016_v1',
        # 'SingleElectron_Run2016H_PromptReco_v2',
        # 'SingleElectron_Run2016H_PromptReco_v3',
    ],
    events=963278706,
    cutflow='data fake'
)

BasicProcess(
    name="fakes_single_mu",
    fullname="Fakes",
    limitname="fake_mu",
    additional_cuts=[('trigger selection', 'event.trigger().accepted({"HLT_IsoMu22_v", "HLT_IsoTkMu22_v"})')],
    paths=[
        'SingleMuon_Run2016B_PromptReco_v2',
        'SingleMuon_Run2016C_PromptReco_v2',
        'SingleMuon_Run2016D_PromptReco_v2',
        'SingleMuon_Run2016E_PromptReco_v2',
        'SingleMuon_Run2016F_PromptReco_v1',
        'SingleMuon_Run2016G_PromptReco_v1',
        'SingleMuon_Run2016H_PromptReco_v2',
        'SingleMuon_Run2016H_PromptReco_v3',
        # 'SingleMuon_Run2016B_23Sep2016_v3',
        # 'SingleMuon_Run2016C_23Sep2016_v1',
        # 'SingleMuon_Run2016D_23Sep2016_v1',
        # 'SingleMuon_Run2016E_23Sep2016_v1',
        # 'SingleMuon_Run2016F_23Sep2016_v1',
        # 'SingleMuon_Run2016G_23Sep2016_v1',
        # 'SingleMuon_Run2016H_PromptReco_v2',
        # 'SingleMuon_Run2016H_PromptReco_v3',
    ],
    events=805524162,
    cutflow='data fake'
)

CombinedProcess(
    name="fakes_single",
    fullname="Fakes",
    limitname="fakes",
    subprocesses=["fakes_single_e", "fakes_single_mu"]
)

CombinedProcess(
    name="collisions_double",
    fullname="Collisions",
    limitname="data_obs",
    subprocesses=["collisions_double_e", "collisions_double_mu", "collisions_mu_egamma"]
)
