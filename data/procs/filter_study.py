from ttH.TauRoast.processing import BasicProcess

for suffix in ['', '_filtered', '_filtered_more', '_fast']:
    BasicProcess(
        name="ttH2Nonbb_tranche3" + suffix,
        fullname="t#bar{t}H(125) #rightarrow #tau_{h} #tau_{h}" + suffix.replace('_', ' '),
        paths=['ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_v1' + suffix],
        cross_section=0.2586,  # 0.5085 * (1 - 0.577),
        events=994998 if suffix == '_fast' else 3975854
    )

    BasicProcess(
        name="ttjets_DL_tranche3" + suffix,
        fullname="t #bar{t} FL" + suffix.replace('_', ' '),
        paths=[
            "TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_v1" + suffix,
        ],
        # cross_section=831.76 * wleptonic ** 2,
        cross_section=87.3,
        events=1000000 if suffix == '_fast' else 75383000
    )

    BasicProcess(
        name="ttjets_SL_tranche3" + suffix,
        fullname="t #bar{t} SL" + suffix.replace('_', ' '),
        paths=[
            "TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_v1" + suffix,
        ],
        # cross_section=831.76 * wleptonic * (1 - wleptonic),
        cross_section=182,
        events=1000000 if suffix == '_fast' else 157387260
    )
