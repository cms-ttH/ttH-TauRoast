from ttH.TauRoast.processing import BasicProcess, CombinedProcess

BasicProcess(
    name="ttH_120",
    sample=9120,
    fullname="t#bar{t}H(120) ",
    limitname="ttH120",
    paths=["ttH_120"],
    cross_section=0.1470,
    events=999508
    # split: {hbb: 0, hcc: 1, hww: 2, hzz: 3, htt: 4, hgg: 5, hgluglu: 6, hzg: 7}
)

BasicProcess(
    name="ttH_125",
    sample=9125,
    fullname="t#bar{t}H(125) ",
    limitname="ttH125",
    paths=["ttH_125"],
    cross_section=0.8696,
    events=846069
    # split: {hbb: 0, hcc: 1, hww: 2, hzz: 3, htt: 4, hgg: 5, hgluglu: 6, hzg: 7}
    # split: {htt: 4}
)

BasicProcess(
    name="ttH_125_real",
    sample=9125,
    fullname="t#bar{t}H(125) gen-match",
    paths=["ttH_125"],
    cross_section=0.8696,
    events=846069,
    additional_cuts=[('parentage', 'abs(taus[0].parentId()) == 25 and abs(taus[1].parentId()) == 25')]
    # split: {hbb: 0, hcc: 1, hww: 2, hzz: 3, htt: 4, hgg: 5, hgluglu: 6, hzg: 7}
    # split: {htt: 4}
)


BasicProcess(
    name="ttH_130",
    sample=9130,
    fullname="t#bar{t}H(130) ",
    limitname="ttH130",
    paths=["ttH_130"],
    cross_section=0.1157,
    events=933970
    # split: {hbb: 0, hcc: 1, hww: 2, hzz: 3, htt: 4, hgg: 5, hgluglu: 6, hzg: 7}
)

BasicProcess(
    name="ttHTauTau_120",
    sample=7120,
    fullname="t#bar{t}H(120) #rightarrow #tau_{h} #tau_{h}",
    paths=["ttHTauTau_120"],
    cross_section=0.1470*0.0704,
    events=990172
)

BasicProcess(
    name="ttHTauTau_125",
    sample=7125,
    fullname="t#bar{t}H(125) #rightarrow #tau_{h} #tau_{h}",
    paths=["ttHTauTau_125"],
    cross_section=0.1302*0.0632,
    events=1000000
)

BasicProcess(
    name="ttHTauTau_130",
    sample=7130,
    fullname="t#bar{t}H(130) #rightarrow #tau_{h} #tau_{h}",
    paths=["ttHTauTau_130"],
    cross_section=0.1157*0.0545,
    events=812456
)
