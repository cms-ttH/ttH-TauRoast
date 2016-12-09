from ttH.TauRoast.processing import BasicProcess, CombinedProcess

BasicProcess(
    name="ttW",
    fullname="t#bar{t}W",
    limitname="ttw",
    paths=["TTWJetsToLNu_TuneCUETP8M1_13TeV_amcatnloFXFX_madspin_pythia8_v1"],
    cross_section=0.2043,
    events=252673,
    cutflow='signal'
)

BasicProcess(
    name="ttZ",
    fullname="t#bar{t}Z",
    limitname="ttz",
    paths=["TTZToLLNuNu_M_10_TuneCUETP8M1_13TeV_amcatnlo_pythia8_v1"],
    cross_section=0.2529,
    events=398600,
    cutflow='signal'
)

BasicProcess(
    name="tttt",
    fullname="tttt",
    paths=["TTTT_TuneCUETP8M1_13TeV_amcatnlo_pythia8_ext1_v1"],
    cross_section=0.009103,
    events=989025
)

BasicProcess(
    name="tZq",
    fullname="tZq",
    paths=["tZq_ll_4f_13TeV_amcatnlo_pythia8_TuneCUETP8M1_v1"],
    cross_section=0.0758,
    events=2973639
)

BasicProcess(
    name="ww_ds",
    fullname="WW Double Scattering",
    paths=["WW_DoubleScattering_13TeV_pythia8_v1"],
    cross_section=1.64,
    events=844954
)

BasicProcess(
    name="wzz",
    fullname="WZZ",
    paths=["WZZ_TuneCUETP8M1_13TeV_amcatnlo_pythia8_v1"],
    cross_section=0.05565,
    events=249800
)

CombinedProcess(
    name="rares",
    fullname="Rares",
    subprocesses=["tttt", "tZq", "ww_ds", "wzz"]
)

BasicProcess(
    name="st",
    fullname="t",
    paths=["ST_tW_top_5f_inclusiveDecays_13TeV_powheg_pythia8_TuneCUETP8M1_v2"],
    cross_section=35.6,
    events=998400
)

BasicProcess(
    name="stbar",
    fullname="#bar{t}",
    paths=["ST_tW_antitop_5f_inclusiveDecays_13TeV_powheg_pythia8_TuneCUETP8M1_v1"],
    cross_section=35.6,
    events=985000
)

BasicProcess(
    name="st_s",
    fullname="t s-channel",
    paths=["ST_s_channel_4f_leptonDecays_13TeV_amcatnlo_pythia8_TuneCUETP8M1_v1"],
    cross_section=3.75,
    events=1000000
)

CombinedProcess(
    name="singletop",
    fullname="single t/#bar{t}",
    limitname="singlet",
    subprocesses=["st", "stbar", "st_s"]
)

BasicProcess(
    name="dyjets_10_50",
    fullname="Z + jets M10-50",
    paths=["DYJetsToLL_M_10to50_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_v1"],
    # cross_section=3 * 7545.03,
    cross_section=18610,
    events=35079788,
    cutflow='signal'
)

BasicProcess(
    name="dyjets_50",
    fullname="Z + jets M50+",
    paths=["DYJetsToLL_M_50_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_ext1_v1"],
    cross_section=6025.2,
    events=49877138,
    cutflow='signal'
)

CombinedProcess(
    name="dyjets",
    fullname="Z + jets",
    subprocesses=["dyjets_10_50", "dyjets_50"]
)

BasicProcess(
    name="wjets",
    fullname="W + jets",
    paths=["WJetsToLNu_TuneCUETP8M1_13TeV_amcatnloFXFX_pythia8_v1"],
    cross_section=61526.7,
    events=9908534,
    cutflow='signal'
)

BasicProcess(
    name="ww",
    fullname="W + W",
    paths=["WWTo2L2Nu_13TeV_powheg_v1"],
    cross_section=10.481,
    events=1996600,
    cutflow='signal'
)

BasicProcess(
    name="wz",
    fullname="W + Z",
    paths=["WZTo3LNu_TuneCUETP8M1_13TeV_powheg_pythia8_v1"],
    cross_section=4.102,
    events=2000000,
    cutflow='signal'
)

BasicProcess(
    name="zz",
    fullname="Z + Z",
    paths=["ZZTo4L_13TeV_powheg_pythia8_v1"],
    cross_section=1.256,
    events=6638328,
    cutflow='signal'
)

CombinedProcess(
    name="ewk",
    fullname="EWK",
    subprocesses=["dyjets", "wjets", "diboson"]
)

CombinedProcess(
    name="diboson",
    fullname="ww, wz, zz",
    limitname="diboson",
    subprocesses=["ww", "wz", "zz"]
)
