from ttH.TauRoast.processing import BasicProcess, CombinedProcess

BasicProcess(
    name="Collisions_DoubleElectron",
    fullname="Collisions",
    limitname="data_obs_ee",
    # add cuts:
    #   - NumLooseElectrons = 2
    paths=["data_DoubleElectron_2012A_06Aug", "data_DoubleElectron_2012A_12Jul", "data_DoubleElectron_2012B_13Jul", "data_DoubleElectron_2012C_24Aug", "data_DoubleElectron_2012C_PR", "data_DoubleElectron_2012D_PR"],
    events=105139786
)

BasicProcess(
    name="Collisions_DoubleMuon",
    # add cuts:
    #   - NumLooseMuons = 2
    fullname="Collisions",
    limitname="data_obs_mumu",
    paths=["data_DoubleMu_2012A_06Aug", "data_DoubleMu_2012A_13Jul", "data_DoubleMu_2012B_PR", "data_DoubleMu_2012C_24Aug", "data_DoubleMu_2012C_PR", "data_DoubleMu_2012D_PR"],
    events=88169930
)

BasicProcess(
    name="Collisions_ElectronMuon",
    # add cuts:
    #   - NumLooseElectrons = 1 and NumLooseMuons = 1
    fullname="Collisions",
    limitname="data_obs_emu",
    paths=["data_MuEG_2012A_06Aug", "data_MuEG_2012A_13Jul", "data_MuEG_2012B_13Jul", "data_MuEG_2012C_24Aug", "data_MuEG_2012C_PR", "data_MuEG_2012D_PR"],
    events=60158786
)

BasicProcess(
    name="Collisions_SingleElectron",
    # add cuts:
    #   - NumLooseElectrons = 1
    fullname="Collisions",
    limitname="data_obs_e",
    paths=["data_SingleElectron_2012A_06Aug", "data_SingleElectron_2012A_13July", "data_SingleElectron_2012B_13Jul", "data_SingleElectron_2012C_24Aug", "data_SingleElectron_2012C_PR", "data_SingleElectron_2012D_PR"],
    events=297904375
)

BasicProcess(
    name="Collisions_SingleMuon",
    # add cuts:
    #   - NumLooseMuons = 1
    fullname="Collisions",
    limitname="data_obs_mu",
    paths=["data_SingleMu_2012A_06Aug", "data_SingleMu_2012A_13July", "data_SingleMu_2012B_13July", "data_SingleMu_2012C_24Aug", "data_SingleMu_2012C_PR", "data_SingleMu_2012D_PR"],
    events=262813074
)

CombinedProcess(
    name="Collisions",
    fullname="Collisions",
    limitname="data_obs",
    subprocesses=["Collisions_SingleElectron", "Collisions_SingleMuon"]
)
