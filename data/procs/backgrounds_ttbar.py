from ttH.TauRoast.processing import BasicProcess, CombinedProcess

wleptonic = (10.71 + 10.63 + 11.38) * 0.01

BasicProcess(
    name="ttjets_semilept_tbar",
    fullname="t #bar{t} SL",
    paths=[
        "TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_v1",
        "TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_ext1_v1"
    ],
    # cross_section=831.76 * wleptonic * (1 - wleptonic),
    cross_section=182,
    events=11944041 + 48266353
)

BasicProcess(
    name="ttjets_semilept_t",
    fullname="t #bar{t} SL",
    paths=[
        "TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_v1",
        "TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_ext1_v1"
    ],
    # cross_section=831.76 * wleptonic * (1 - wleptonic),
    cross_section=182,
    events=11957043 + 50016934
)

CombinedProcess(
    name="ttjets_semilept",
    fullname="t #bar{t} SL",
    subprocesses=["ttjets_semilept_t", "ttjets_semilept_tbar"]
)

BasicProcess(
    name="ttjets_fulllept",
    fullname="t #bar{t} FL",
    paths=[
        "TTJets_DiLept_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_v4",
        "TTJets_DiLept_TuneCUETP8M1_13TeV_madgraphMLM_pythia8_ext1_v1"
    ],
    # cross_section=831.76 * wleptonic ** 2,
    cross_section=87.3,
    events=6094476 + 24350202
)

CombinedProcess(
    name="ttjets_mad",
    fullname="t#bar{t}",
    limitname="ttjets",
    subprocesses=["ttjets_semilept", "ttjets_fulllept"]
)

BasicProcess(
    name="ttjets_sl_pow",
    fullname="t #bar{t} SL",
    paths=[
        "TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_v1",
    ],
    # cross_section=831.76 * wleptonic * (1 - wleptonic),
    cross_section=182 * 2,
    events=152720952,
    cutflow='signal'
)

BasicProcess(
    name="ttjets_dl_pow",
    fullname="t #bar{t} FL",
    paths=[
        "TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_v1",
    ],
    # cross_section=831.76 * wleptonic ** 2,
    cross_section=87.3,
    events=79092400,
    cutflow='signal'
)

CombinedProcess(
    name="ttjets_pow",
    fullname="t#bar{t}",
    limitname="TT",
    subprocesses=["ttjets_sl_pow", "ttjets_dl_pow"]
)
