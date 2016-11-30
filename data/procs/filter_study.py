from ttH.TauRoast.processing import BasicProcess

for suffix in ['', '_fast', '_filtered_0t1l',
               '_filtered_0t2l_mva3', '_filtered_0t3l_mva3',
               '_filtered_1t2l_mva3', '_filtered_1t3l_mva3']:
    for cuts in ['default', 'loose']:
        nsuffix = suffix + ('_' + cuts if cuts != 'default' else '')
        BasicProcess(
            name="ttH2Nonbb_tranche3" + nsuffix,
            fullname="t#bar{t}H(125) #rightarrow #tau_{h} #tau_{h}" + nsuffix.replace('_', ' '),
            paths=['ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_v1' + suffix],
            cross_section=0.2586,  # 0.5085 * (1 - 0.577),
            events=994998 if suffix == '_fast' else 3975854,
            cutflow=cuts
        )

        BasicProcess(
            name="ttjets_DL_tranche3" + nsuffix,
            fullname="t #bar{t} FL" + nsuffix.replace('_', ' '),
            paths=[
                "TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_v1" + suffix,
            ],
            # cross_section=831.76 * wleptonic ** 2,
            cross_section=87.3,
            events=1000000 if suffix == '_fast' else 75383000,
            cutflow=cuts
        )

        BasicProcess(
            name="ttjets_SL_tranche3" + nsuffix,
            fullname="t #bar{t} SL" + nsuffix.replace('_', ' '),
            paths=[
                "TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_v1" + suffix,
            ],
            # cross_section=831.76 * wleptonic * (1 - wleptonic),
            cross_section=182 * 2,
            events=1000000 if suffix == '_fast' else 157387260,
            cutflow=cuts
        )
