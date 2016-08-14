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
    events=13442593
)

BasicProcess(
    name="collisions_single_e",
    fullname="Collisions",
    limitname="data_obs_e",
    additional_cuts=[('trigger selection', 'len(leptons) == 1 and leptons[0].electron()')],
    paths=["data_SingleE_2015D_Dec16"],
    events=297904375
)

BasicProcess(
    name="collisions_single_mu",
    fullname="Collisions",
    limitname="data_obs_mu",
    additional_cuts=[('trigger selection', 'len(leptons) == 1 and leptons[0].muon()')],
    paths=["data_SingleMu_2015D_Dec16"],
    events=262813074
)

CombinedProcess(
    name="collisions_single",
    fullname="Collisions",
    limitname="data_obs",
    subprocesses=["collisions_single_e", "collisions_single_mu"]
)

CombinedProcess(
    name="collisions_double",
    fullname="Collisions",
    limitname="data_obs",
    subprocesses=["collisions_double_e", "collisions_double_mu", "collisions_mu_egamma"]
)
