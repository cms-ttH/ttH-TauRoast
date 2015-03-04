from ttH.TauRoast.processing import BasicProcess, CombinedProcess

BasicProcess(
    name="TTbar_Hadronic",
    sample=2566,
    fullname="t #bar{t} H",
    path=["TTbar_Hadronic_Summer12"],
    events=31223821
)

BasicProcess(
    name="TTbar_SemiLept",
    sample=2563,
    fullname="t #bar{t} SL",
    path=["TTbar_SemiLept_Summer12"],
    events=25424818
)

BasicProcess(
    name="TTbar_FullLept",
    sample=2533,
    fullname="t #bar{t} FL",
    path=["TTbar_FullLept_Summer12"],
    cross_section=25.81,
    events=12119013
)

BasicProcess(
    name="WplusJets",
    sample=2400,
    fullname="W + jets",
    path=["WplusJets_Summer12"],
    cross_section=36257,
    events=57709905
)

BasicProcess(
    name="WplusOneJet",
    sample=2401,
    fullname="W + 1 jet",
    path=["WplusOneJet_Summer12"],
    cross_section=6440.4,
    events=23141598
)

BasicProcess(
    name="WplusTwoJets",
    sample=2402,
    fullname="W + 2 jets",
    path=["WplusTwoJets_Summer12"],
    cross_section=2087.2,
    events=34044921
)

BasicProcess(
    name="WplusThreeJets",
    sample=2403,
    fullname="W + 3 jets",
    path=["WplusThreeJets_Summer12"],
    cross_section=619.0,
    events=15539503
)

BasicProcess(
    name="WplusFourJets",
    sample=2404,
    fullname="W + 4 jets",
    path=["WplusFourJets_Summer12"],
    cross_section=255.2,
    events=13382803
)

BasicProcess(
    name="WW",
    sample=2700,
    fullname="W + W",
    path=["WW_Summer12"],
    cross_section=54.8,
    events=10000431
)

BasicProcess(
    name="WZ",
    sample=2701,
    fullname="W + Z",
    path=["WZ_Summer12"],
    cross_section=32.3,
    events=10000283
)

BasicProcess(
    name="ZZ",
    sample=2702,
    fullname="Z + Z",
    path=["ZZ_Summer12"],
    cross_section=7.7,
    events=9799908
)

BasicProcess(
    name="ZplusOneJet",
    sample=2301,
    fullname="Z + 1 jet",
    path=["ZplusOneJet_Summer12"],
    cross_section=666.7,
    events=24045248
)

BasicProcess(
    name="ZplusTwoJets",
    sample=2302,
    fullname="Z + 2 jets",
    path=["ZplusTwoJets_Summer12"],
    cross_section=215.1,
    events=2352304
)

BasicProcess(
    name="ZplusThreeJets",
    sample=2303,
    fullname="Z + 3 jets",
    path=["ZplusThreeJets_Summer12"],
    cross_section=66.07,
    events=11015445
)

BasicProcess(
    name="ZplusFourJets",
    sample=2304,
    fullname="Z + 4 jets",
    path=["ZplusFourJets_Summer12"],
    cross_section=27.38,
    events=6402827
)

BasicProcess(
    name="ZplusJets_LowMass",
    sample=2850,
    fullname="Z + jets M10-50",
    path=["ZplusJets_LowMass_Summer12"],
    events=37835275
)

BasicProcess(
    name="ZplusJets_HighMass",
    sample=2310,
    fullname="Z + jets M50+",
    path=["ZplusJets_HighMass_Summer12"],
    cross_section=3505.7,
    events=30459503
)

BasicProcess(
    name="singleTopPlusW",
    sample=2604,
    fullname="t + W",
    path=["singleTopPlusW_Summer12"],
    events=497658
)

BasicProcess(
    name="singleTopBarPlusW",
    sample=2605,
    fullname="#bar{t} + W",
    path=["singleTopBarPlusW_Summer12"],
    cross_section=7.87,
    events=493460
)

BasicProcess(
    name="singleTopSch",
    sample=2600,
    fullname="t (s-ch)",
    path=["singleTopSch_Summer12"],
    cross_section=3.79,
    events=259961
)

BasicProcess(
    name="singleTopBarSch",
    sample=2601,
    fullname="#bar{t} (s-ch)",
    path=["singleTopBarSch_Summer12"],
    events=139974
)

BasicProcess(
    name="singleTopTch",
    sample=2602,
    fullname="t (t-ch)",
    path=["singleTopTch_Summer12"],
    cross_section=56.4,
    events=3758227
)

BasicProcess(
    name="singleTopBarTch",
    sample=2603,
    fullname="#bar{t} (t-ch)",
    path=["singleTopBarTch_Summer12"],
    cross_section=30.7,
    events=1935072
)

BasicProcess(
    name="ttPlusW",
    sample=2524,
    fullname="#bar{t} + W",
    limitname="ttbarW",
    path=["ttPlusW_Summer12"],
    cross_section=0.249,
    events=196046
)

BasicProcess(
    name="ttPlusZ",
    sample=2523,
    fullname="#bar{t} + Z",
    limitname="ttbarZ",
    path=["ttPlusZ_Summer12"],
    cross_section=0.208,
    events=210160
)

CombinedProcess(
        name="TTbar",
        fullname="t #bar{t}",
        limitname="ttbar",
        subprocesses=["TTbar_Hadronic", "TTbar_SemiLept", "TTbar_FullLept"]
)

CombinedProcess(
        name="Zjets",
        fullname="Z + jets",
        limitname="zjets",
        subprocesses=["ZplusJets_LowMass", "ZplusJets_HighMass", "ZplusOneJet", "ZplusTwoJets", "ZplusThreeJets", "ZplusFourJets"]
)

CombinedProcess(
        name="EWK",
        fullname="EWK",
        subprocesses=["Zjets", "Wjets", "DiBoson"]
)

CombinedProcess(
        name="DiBoson",
        fullname="WW, WZ, ZZ",
        limitname="diboson",
        subprocesses=["WW", "WZ", "ZZ"]
)

CombinedProcess(
        name="SingleTop",
        fullname="single t/#bar{t}",
        limitname="singlet",
        subprocesses=["singleTopSch", "singleTopBarSch", "singleTopTch", "singleTopBarTch", "singleTopPlusW", "singleTopBarPlusW"]
)

CombinedProcess(
        name="ttWZ",
        fullname="t#bar{t} + W/Z",
        subprocesses=["ttPlusW", "ttPlusZ"]
)

CombinedProcess(
        name="Wjets",
        fullname="W + jets",
        limitname="wjets",
        subprocesses=["WplusJets", "WplusOneJet", "WplusTwoJets", "WplusThreeJets", "WplusFourJets"]
)
