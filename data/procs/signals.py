from ttH.TauRoast.processing import BasicProcess, CombinedProcess

BasicProcess(
    name="TTH_120",
    sample=9120,
    fullname="t#bar{t}H(120) ",
    limitname="ttH120",
    path=["TTH_120"],
    cross_section=0.1470,
    events=999508
    # split: {hbb: 0, hcc: 1, hww: 2, hzz: 3, htt: 4, hgg: 5, hgluglu: 6, hzg: 7}
)

BasicProcess(
    name="TTH_125",
    sample=9125,
    fullname="t#bar{t}H(125) ",
    limitname="ttH125",
    path=["TTH_125"],
    cross_section=0.1302,
    events=995697
    # split: {hbb: 0, hcc: 1, hww: 2, hzz: 3, htt: 4, hgg: 5, hgluglu: 6, hzg: 7}
    # split: {htt: 4}
)

BasicProcess(
    name="TTH_130",
    sample=9130,
    fullname="t#bar{t}H(130) ",
    limitname="ttH130",
    path=["TTH_130"],
    cross_section=0.1157,
    events=933970
    # split: {hbb: 0, hcc: 1, hww: 2, hzz: 3, htt: 4, hgg: 5, hgluglu: 6, hzg: 7}
)

BasicProcess(
    name="TTHTauTau_120",
    sample=7120,
    fullname="t#bar{t}H(120) #rightarrow #tau_{h} #tau_{h}",
    path=["TTHTauTau_120"],
    cross_section=0.1470*0.0704,
    events=990172
)

BasicProcess(
    name="TTHTauTau_125",
    sample=7125,
    fullname="t#bar{t}H(125) #rightarrow #tau_{h} #tau_{h}",
    path=["TTHTauTau_125"],
    cross_section=0.1302*0.0632,
    events=1000000
)

BasicProcess(
    name="TTHTauTau_130",
    sample=7130,
    fullname="t#bar{t}H(130) #rightarrow #tau_{h} #tau_{h}",
    path=["TTHTauTau_130"],
    cross_section=0.1157*0.0545,
    events=812456
)
