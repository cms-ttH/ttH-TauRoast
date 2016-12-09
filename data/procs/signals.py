from ttH.TauRoast.processing import BasicProcess, CombinedProcess

BasicProcess(
    name="ttH2TT_125_amc",
    fullname="t#bar{t}H(125) #rightarrow #tau_{h} #tau_{h}",
    limitname="ttHTT125_amc",
    paths=['ttH2TT_125', 'ttH2TT_125_ext1', 'ttH2TT_125_ext2', 'ttH2TT_125_ext3'],
    cross_section=0.5085 * 6.32e-2,
    events=8378353
    # split: {hbb: 0, hcc: 1, hww: 2, hzz: 3, htt: 4, hgg: 5, hgluglu: 6, hzg: 7}
    # split: {htt: 4}
)

BasicProcess(
    name="ttH2TT_125_real_amc",
    fullname="t#bar{t}H(125) #rightarrow #tau_{h} #tau_{h} gen-match",
    paths=['ttH2TT_125', 'ttH2TT_125_ext1', 'ttH2TT_125_ext2', 'ttH2TT_125_ext3'],
    cross_section=0.5085 * 6.32e-2,
    events=8378353,
    additional_cuts=[('parentage', 'abs(taus[0].parentId()) == 25 and abs(taus[1].parentId()) == 25')]
    # split: {hbb: 0, hcc: 1, hww: 2, hzz: 3, htt: 4, hgg: 5, hgluglu: 6, hzg: 7}
    # split: {htt: 4}
)

BasicProcess(
    name="ttH2bb_125_amc",
    fullname="t#bar{t}H(125) #rightarrow b#bar{b}",
    limitname="ttHbb125",
    paths=['ttH2bb_125', 'ttH2bb_125_ext1', 'ttH2bb_125_ext2', 'ttH2bb_125_ext3'],
    cross_section=0.5085 * 0.577,
    events=20605280
)

BasicProcess(
    name="ttH2Nonbb_125_amc",
    fullname="t#bar{t}H(125) #rightarrow b#bar{b}",
    limitname="ttH2Nonbb125",
    paths=['ttH2Nonbb_125', 'ttH2Nonbb_125_ext1'],
    cross_section=0.2586,  # 0.5085 * (1 - 0.577),
    events=17594172
)

BasicProcess(
    name="ttH2Nonbbgg_125_amc",
    fullname="t#bar{t}H(125) w/o b#bar{b}, gg",
    limitname="ttH2Nonbbgg125",
    paths=["ttH2Nonbbgg_125"],
    cross_section=0.5085 * (1 - (0.577 + 0.2)),
    events=36000
)

BasicProcess(
    name="ttH2TT_125_pow",
    fullname="t#bar{t}H(125) #rightarrow #tau_{h} #tau_{h}",
    limitname="ttHTT125",
    paths=['ttHToTT_M125_13TeV_powheg_pythia8_v1'],
    cross_section=0.5085 * 6.32e-2,
    events=2187600
)

BasicProcess(
    name="ttH2TT_125_real_pow",
    fullname="t#bar{t}H(125) #rightarrow #tau_{h} #tau_{h} gen-match",
    paths=['ttHToTT_M125_13TeV_powheg_pythia8_v1'],
    cross_section=0.5085 * 6.32e-2,
    events=2187600,
    additional_cuts=[
        ('parentage', 'abs(taus[0].parentId()) == 25 and abs(taus[1].parentId()) == 25'),
        ('id', 'abs(taus[0].match()) == 5 and abs(taus[1].match()) == 5')
    ]
)

BasicProcess(
    name="ttH2TT_125_realtaus_pow",
    fullname="t#bar{t}H(125) #rightarrow #tau_{h} #tau_{h} gen-match",
    paths=['ttHToTT_M125_13TeV_powheg_pythia8_v1'],
    cross_section=0.5085 * 6.32e-2,
    events=2187600,
    additional_cuts=[
        ('id', 'abs(taus[0].match()) == 5 and abs(taus[1].match()) == 5')
    ]
)

BasicProcess(
    name="ttH2Nonbb_125_real_pow",
    fullname="t#bar{t}H(125) #rightarrow #tau_{h} #tau_{h} gen-match",
    paths=['ttHToNonbb_M125_13TeV_powheg_pythia8_v1'],
    # cross_section=0.5085 * (1 - 0.577),
    cross_section=0.2586,
    events=3860872,
    additional_cuts=[
        (
            'parentage',
            '(abs(taus[0].parentId()) == 25 and abs(taus[1].parentId()) == 25) or ' +
            '(abs(taus[0].parentId()) == 24 and abs(taus[1].parentId()) == 24)'
        ),
        ('id', 'abs(taus[0].match()) == 5 and abs(taus[1].match()) == 5')
    ]
)

BasicProcess(
    name="ttH2Nonbb_125_realtaus_pow",
    fullname="t#bar{t}H(125) #rightarrow #tau_{h} #tau_{h} gen-match",
    paths=['ttHToNonbb_M125_13TeV_powheg_pythia8_v1'],
    # cross_section=0.5085 * (1 - 0.577),
    cross_section=0.2586,
    events=3860872,
    additional_cuts=[('id', 'abs(taus[0].match()) == 5 and abs(taus[1].match()) == 5')]
)

BasicProcess(
    name="ttH2bb_125_signal",
    fullname="t#bar{t}H(125) #rightarrow b#bar{b}",
    paths=['ttHTobb_M125_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_v1'],
    cross_section=0.5085 * 0.577,
    events=3993304,
    cutflow='signal'
)

BasicProcess(
    name="ttH2Nonbb_125_signal",
    fullname="t#bar{t}H(125) #rightarrow b#bar{b}",
    paths=['ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_v1'],
    # cross_section=0.5085 * (1 - 0.577),
    cross_section=0.2586,
    events=3975854,
    cutflow='signal'
)

BasicProcess(
    name="ttH2bb_125_fake",
    fullname="t#bar{t}H(125) #rightarrow b#bar{b}",
    paths=['ttHTobb_M125_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_v1'],
    cross_section=0.5085 * 0.577,
    events=3993304,
    cutflow='fake'
)

BasicProcess(
    name="ttH2Nonbb_125_fake",
    fullname="t#bar{t}H(125) #rightarrow b#bar{b}",
    paths=['ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_v1'],
    # cross_section=0.5085 * (1 - 0.577),
    cross_section=0.2586,
    events=3975854,
    cutflow='fake'
)

CombinedProcess(
    name="ttH_125_signal",
    fullname="t#bar{t}H(125)",
    subprocesses=["ttH2bb_125_signal", "ttH2Nonbb_125_signal"]
)

CombinedProcess(
    name="ttH_125_fake",
    fullname="t#bar{t}H(125)",
    subprocesses=["ttH2bb_125_fake", "ttH2Nonbb_125_fake"]
)

CombinedProcess(
    name="ttH_125",
    fullname="t#bar{t}H(125)",
    limitname="ttH125",
    subprocesses=["ttH_125_signal", "ttH_125_fake"]
)

CombinedProcess(
    name="ttH_125_amc",
    fullname="t#bar{t}H(125)",
    limitname="ttH125_amc",
    subprocesses=["ttH2bb_125_amc", "ttH2Nonbb_125_amc"]
)

CombinedProcess(
    name="ttH_125_pow",
    fullname="t#bar{t}H(125)",
    limitname="ttH125",
    subprocesses=["ttH2bb_125_pow", "ttH2Nonbb_125_pow"]
)
