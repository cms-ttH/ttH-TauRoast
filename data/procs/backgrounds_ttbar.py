from ttH.TauRoast.processing import BasicProcess, CombinedProcess

wleptonic = (10.71 + 10.63 + 11.38) * 0.01

BasicProcess(
    name="ttjets",
    fullname="t#bar{t}",
    paths=["tt_pow", "tt_pow_ext3"],
    # paths=["ttjets_amc"],
    cross_section=831.76,
    events=97994442
)

BasicProcess(
    name="ttjets_sl",
    fullname="t #bar{t} SL",
    paths=["tt_sl_pow"],
    cross_section=831.76 * wleptonic * (1 - wleptonic),
    events=353216236
)

BasicProcess(
    name="ttjets_dl",
    fullname="t #bar{t} DL",
    paths=["tt_dl_pow"],
    cross_section=831.76 * wleptonic ** 2,
    events=4995600
)

BasicProcess(
    name="ttbar_hadronic",
    fullname="t #bar{t} H",
    paths=["ttbar_hadronic"],
    events=31223821
)

BasicProcess(
    name="ttjets_semilept_tbar",
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
    events=157387260,
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
    events=75383000,
    cutflow='signal'
)

CombinedProcess(
    name="ttjets_pow",
    fullname="t#bar{t}",
    limitname="TT",
    subprocesses=["ttjets_sl_pow", "ttjets_dl_pow"]
)

BasicProcess(
    name="ttjets_sl_pow_train",
    fullname="t #bar{t} SL",
    paths=[
        "TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_faster_v5_v1",
        "TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_faster_v5_v2",
        "TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_faster_v6_v1",
        "TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_faster_v7_v1",
    ],
    # cross_section=831.76 * wleptonic * (1 - wleptonic),
    cross_section=182 * 2,
    events=157387260,
    cutflow='training'
)

BasicProcess(
    name="ttjets_dl_pow_train",
    fullname="t #bar{t} FL",
    paths=[
        "TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_faster_v5_v1",
        "TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_faster_v6_v1",
        "TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_faster_v7_v1",
    ],
    # cross_section=831.76 * wleptonic ** 2,
    cross_section=87.3,
    events=75383000,
    cutflow='training'
)

CombinedProcess(
    name="ttjets_pow_train",
    fullname="t#bar{t}",
    limitname="ttjets",
    subprocesses=["ttjets_sl_pow_train", "ttjets_dl_pow_train"]
)
