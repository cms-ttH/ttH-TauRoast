from ttH.TauRoast.processing import BasicProcess, CombinedProcess

BasicProcess(
    name="ttH2TT_125",
    sample=9125,
    fullname="t#bar{t}H(125) #rightarrow #tau_{h} #tau_{h}",
    limitname="ttHTT125",
    paths=["ttH2TT_125"],
    cross_section=0.5085 * 6.32e-2,
    events=1427478
    # split: {hbb: 0, hcc: 1, hww: 2, hzz: 3, htt: 4, hgg: 5, hgluglu: 6, hzg: 7}
    # split: {htt: 4}
)

BasicProcess(
    name="ttH2TT_125_real",
    sample=9125,
    fullname="t#bar{t}H(125) #rightarrow #tau_{h} #tau_{h} gen-match",
    paths=["ttH2TT_125"],
    cross_section= 0.5085 * 6.32e-2,
    events=1427478,
    additional_cuts=[('parentage', 'abs(taus[0].parentId()) == 25 and abs(taus[1].parentId()) == 25')]
    # split: {hbb: 0, hcc: 1, hww: 2, hzz: 3, htt: 4, hgg: 5, hgluglu: 6, hzg: 7}
    # split: {htt: 4}
)

BasicProcess(
    name="ttH2bb_125",
    sample=9125,
    fullname="t#bar{t}H(125) #rightarrow b#bar{b}",
    limitname="ttHbb125",
    paths=["ttH2bb_125"],
    cross_section=0.5085 * 0.577,
    events=2588639
)

BasicProcess(
    name="ttH2Nonbb_125",
    sample=9125,
    fullname="t#bar{t}H(125) #rightarrow b#bar{b}",
    limitname="ttH2Nonbb125",
    paths=["ttH2Nonbb_125"],
    cross_section=0.5085 * (1 - 0.577),
    events=2576712
)

BasicProcess(
    name="ttH2Nonbbgg_125",
    sample=9125,
    fullname="t#bar{t}H(125) w/o b#bar{b}, gg",
    limitname="ttH2Nonbbgg125",
    paths=["ttH2Nonbbgg_125"],
    cross_section=0.5085 * (1 - (0.577 + 0.2)),
    events=2576712
)

CombinedProcess(
        name="ttH_125",
        fullname="t#bar{t}H(125)",
        limitname="ttH125",
        subprocesses=["ttH2bb_125", "ttH2Nonbb_125"]
)
