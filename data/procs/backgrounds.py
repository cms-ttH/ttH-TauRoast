from ttH.TauRoast.processing import BasicProcess, CombinedProcess

wleptonic = (10.71 + 10.63 + 11.38) * 0.01

BasicProcess(
    name="ttjets",
    sample=2500,
    fullname="t#bar{t}",
    paths=["tt_pow", "tt_pow_ext3"],
    # paths=["ttjets_amc"],
    cross_section=831.76,
    events=97994442
    # events=116591749
    # events=115091972
    # events=42730273
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
    name="ttjets_semilept_t",
    sample=2563,
    fullname="t #bar{t} SL",
    paths=["ttjets_slt_mad", "ttjets_slt_mad_ext"],
    cross_section=831.76 * wleptonic * (1 - wleptonic),
    events=58191090
)

BasicProcess(
    name="ttjets_semilept_tbar",
    sample=2563,
    fullname="t #bar{t} SL",
    paths=["ttjets_sltbar_mad", "ttjets_sltbar_mad_ext"],
    cross_section=831.76 * wleptonic * (1 - wleptonic),
    events=60166355
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
    paths=["ttjets_dl_mad", "ttjets_dl_mad_ext"],
    cross_section=831.76 * wleptonic ** 2,
    events=30426583
)

BasicProcess(
    name="ttW",
    fullname="ttW",
    paths=["ttw"],
    cross_section=0.2043,
    events=250307
)

BasicProcess(
    name="st",
    fullname="t",
    paths=["st"],
    cross_section=35.6,
    events=995600
)

BasicProcess(
    name="stbar",
    fullname="#bar{t}",
    paths=["stbar"],
    cross_section=35.6,
    events=988500
)

CombinedProcess(
    name="singletop",
    fullname="single t/#bar{t}",
    limitname="singlet",
    subprocesses=["st", "stbar"]
)

BasicProcess(
    name="dyjets_10_50",
    fullname="Z + jets M10-50",
    paths=["dyjets_10_50"],
    cross_section=3 * 7545.03,
    events=30021559
)

BasicProcess(
    name="dyjets_50",
    fullname="Z + jets M50+",
    paths=["dyjets_50"],
    cross_section=6025.2,
    events=28747969
)

CombinedProcess(
    name="dyjets",
    fullname="Z + jets",
    subprocesses=["dyjets_10_50", "dyjets_50"]
)

BasicProcess(
    name="wjets",
    fullname="W + jets",
    paths=["wjets"],
    cross_section=61526.7,
    events=24184766
)

BasicProcess(
    name="ww",
    fullname="W + W",
    paths=["ww"],
    cross_section=118.7,
    events=993640
)

BasicProcess(
    name="wz",
    fullname="W + Z",
    paths=["wz"],
    cross_section=47.13,
    events=978512
)

BasicProcess(
    name="zz",
    fullname="Z + Z",
    paths=["zz"],
    cross_section=16.523,
    events=996944
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
