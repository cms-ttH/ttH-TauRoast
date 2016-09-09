from ttH.TauRoast.processing import BasicProcess, CombinedProcess

wleptonic = (10.71 + 10.63 + 11.38) * 0.01

BasicProcess(
    name="ttW",
    fullname="t#bar{t}W",
    paths=["TTWJetsToLNu_TuneCUETP8M1_13TeV_amcatnloFXFX_madspin_pythia8_v1"],
    cross_section=0.2043,
    events=252673
)

BasicProcess(
    name="ttZ",
    fullname="t#bar{t}Z",
    paths=["TTZToLLNuNu_M_10_TuneCUETP8M1_13TeV_amcatnlo_pythia8_v1"],
    cross_section=0.2529,
    events=398600
)

BasicProcess(
    name="ttjets",
    sample=2500,
    fullname="t#bar{t}",
    paths=["tt_pow", "tt_pow_ext3"],
    # paths=["ttjets_amc"],
    cross_section=831.76,
    events=97994442
)

BasicProcess(
    name="ttjets_sl",
    sample=2563,
    fullname="t #bar{t} SL",
    paths=["tt_sl_pow"],
    cross_section=831.76 * wleptonic * (1 - wleptonic),
    events=353216236
)

BasicProcess(
    name="ttjets_dl",
    sample=2533,
    fullname="t #bar{t} DL",
    paths=["tt_dl_pow"],
    cross_section=831.76 * wleptonic ** 2,
    events=4995600
)

BasicProcess(
    name="ttbar_hadronic",
    sample=2566,
    fullname="t #bar{t} H",
    paths=["ttbar_hadronic"],
    events=31223821
)

BasicProcess(
    name="ttjets_semilept_tbar",
    sample=2563,
    fullname="t #bar{t} SL",
    paths=[
        "TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_v1",
        "TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_ext1_v1"
    ],
    # cross_section=831.76 * wleptonic * (1 - wleptonic),
    cross_section=182,
    events=11947951 + 48546872
)

BasicProcess(
    name="ttjets_semilept_t",
    sample=2563,
    fullname="t #bar{t} SL",
    paths=[
        "TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_v1",
        "TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_ext1_v1"
    ],
    # cross_section=831.76 * wleptonic * (1 - wleptonic),
    cross_section=182,
    events=11926538 + 43588107
)

CombinedProcess(
    name="ttjets_semilept",
    fullname="t #bar{t} SL",
    subprocesses=["ttjets_semilept_t", "ttjets_semilept_tbar"]
)

BasicProcess(
    name="ttjets_fulllept",
    sample=2533,
    fullname="t #bar{t} FL",
    paths=[
        "TTJets_DiLept_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_v4",
        "TTJets_DiLept_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_ext1_v1"
    ],
    # cross_section=831.76 * wleptonic ** 2,
    cross_section=87.3,
    events=6058236 + 24623997
)

CombinedProcess(
    name="ttjets_mad",
    fullname="t#bar{t}",
    subprocesses=["ttjets_semilept", "ttjets_fulllept"]
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
    events=35079788
)

BasicProcess(
    name="dyjets_50",
    fullname="Z + jets M50+",
    paths=["DYJetsToLL_M_50_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_ext1_v1"],
    cross_section=6025.2,
    events=49877138
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
    events=9908534
)

BasicProcess(
    name="ww",
    fullname="W + W",
    paths=["WWTo2L2Nu_13TeV_powheg_v1"],
    cross_section=10.481,
    events=1996600
)

BasicProcess(
    name="wz",
    fullname="W + Z",
    paths=["WZTo3LNu_TuneCUETP8M1_13TeV_powheg_pythia8_v1"],
    cross_section=4.102,
    events=2000000
)

BasicProcess(
    name="zz",
    fullname="Z + Z",
    paths=["ZZTo4L_13TeV_powheg_pythia8_v1"],
    cross_section=1.256,
    events=6638328
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
