from ttH.TauRoast.botany import Leaf
from ttH.TauRoast.plotting import Plot
from ttH.TauRoast.useful import config

if config.taus >= 2:
    Leaf('tt_deltaR', 'f', 'result = dR(taus[0], taus[1])')
    Leaf('tt_visiblemass', 'f', 'result = (taus[0].p4() + taus[1].p4()).M()')
    Leaf('tt_cosDeltaPhi', 'f', 'result = std::cos(taus[0].p4().Phi() - taus[1].p4().Phi())')

    Plot(
        name="taus/TT_DeltaR",
        values=["tt_deltaR"],
        labels=["#DeltaR #tau_{1,2}", "Events"],
        binning=[15, 0, 6.28],
        essential=True
    )
    Plot(
        name="taus/TT_CosDeltaPhi",
        values=["tt_cosDeltaPhi"],
        labels=["cos(#Delta #phi) #tau_{1,2}", "Events"],
        binning=[15, -1, 1]
    )

    Plot(
        name="taus/TT_VisibleMass",
        values=["tt_visiblemass"],
        labels=["Visible Mass #tau_{1,2}", "Events"],
        binning=[30, 0, 200],
        essential=True
    )
    Plot(
        name="taus/TT_VisibleMass_coarse",
        values=["tt_visiblemass"],
        labels=["Visible Mass #tau_{1,2}", "Events"],
        binning=[5, 0, 200]
    )

for n in range(config.taus):
    lbl = "#tau_{{{0}}} ".format(n + 1)

    for l in range(config.leptons):
        Leaf('tau{}lep{}_deltaR'.format(n + 1, l + 1), 'f',
             'result = dR(leptons.at({0}), taus.at({1}))'.format(l, n))

        Plot(
            name="taus/T{}L{}_DeltaR".format(n + 1, l + 1),
            values=["tau{}lep{}_deltaR".format(n + 1, l + 1)],
            labels=["#DeltaR {}, l_{}".format(lbl, l + 1), "Events"],
            binning=[15, 0, 6.28]
        )

    Leaf('tau{}jet_deltaRmin'.format(n + 1), 'f',
         '''std::vector<float> drs(jets.size());
            std::transform(jets.begin(), jets.end(), drs.begin(),
                           [&](const superslim::Jet& j) {{ return dR(taus.at({}), j); }});
            result = *std::min_element(drs.begin(), drs.end())'''.format(n))
    Plot(
        name="taus/T{}J_MinDeltaR".format(n + 1),
        values=["tau{}jet_deltaRmin".format(n + 1)],
        labels=["Min #DeltaR {}, jet".format(lbl), "Events"],
        binning=[15, 0, 6.28]
    )

    Leaf('tau{0}_pt'.format(n + 1), 'f', 'result = taus.at({0}).p4().Pt()'.format(n))
    Leaf('tau{0}_eta'.format(n + 1), 'f', 'result = taus.at({0}).p4().Eta()'.format(n))
    Leaf('tau{0}_abseta'.format(n + 1), 'f', 'result = std::abs(taus.at({0}).p4().Eta())'.format(n))
    Leaf('tau{0}_phi'.format(n + 1), 'f', 'result = taus.at({0}).p4().Phi()'.format(n))
    Leaf('tau{0}_leadingtrackpt'.format(n + 1), 'f', 'result = taus.at({0}).leadingTrackPt()'.format(n))

    Leaf('tau{0}_genpt'.format(n + 1), 'f', 'result = taus.at({0}).genP4().Pt()'.format(n))
    Leaf('tau{0}_geneta'.format(n + 1), 'f', 'result = taus.at({0}).genP4().Eta()'.format(n))
    Leaf('tau{0}_genvispt'.format(n + 1), 'f', 'result = taus.at({0}).genVisibleP4().Pt()'.format(n))
    Leaf('tau{0}_genviseta'.format(n + 1), 'f', 'result = taus.at({0}).genVisibleP4().Eta()'.format(n))

    # Kinematics
    Plot(
        name="taus/kinematic/T{0}_Pt".format(n + 1),
        values=["tau{0}_pt".format(n + 1)],
        labels=[lbl + "P_{T}", "Events"],
        binning=[10, 20, 100 if n == 0 else 60],
        essential=True
    )
    Plot(
        name="taus/kinematic/T{0}_LTPt".format(n + 1),
        values=["tau{0}_leadingtrackpt".format(n + 1)],
        labels=[lbl + " leading track P_{T}", "Events"],
        binning=[40, 0, 80 if n > 0 else 120]
    )
    Plot(
        name="taus/kinematic/T{0}_Eta".format(n + 1),
        values=["tau{0}_eta".format(n + 1)],
        labels=[lbl + "#eta", "Events"],
        binning=[20, -3, 3]
    )
    Plot(
        name="taus/kinematic/T{0}_Phi".format(n + 1),
        values=["tau{0}_phi".format(n + 1)],
        labels=[lbl + "#Phi", "Events"],
        binning=[20, -3.2, 3.2]
    )

    Plot(
        name="taus/generator/T{0}_GenPt".format(n + 1),
        values=["tau{0}_genpt".format(n + 1)],
        labels=[lbl + "gen P_{T}", "Events"],
        binning=[10, 0, 100 if n == 0 else 60]
    )
    Plot(
        name="taus/generator/T{0}_GenEta".format(n + 1),
        values=["tau{0}_geneta".format(n + 1)],
        labels=[lbl + "gen #eta", "Events"],
        binning=[20, -3, 3]
    )
    Plot(
        name="taus/generator/T{0}_GenVisPt".format(n + 1),
        values=["tau{0}_genvispt".format(n + 1)],
        labels=[lbl + "gen visible P_{T}", "Events"],
        binning=[10, 0, 100 if n == 0 else 60]
    )
    Plot(
        name="taus/generator/T{0}_GenVisEta".format(n + 1),
        values=["tau{0}_genviseta".format(n + 1)],
        labels=[lbl + "gen visible #eta", "Events"],
        binning=[20, -3, 3]
    )

    Leaf('tau{0}_match'.format(n + 1), 'i', 'result = taus.at({0}).match()'.format(n))
    Leaf('tau{0}_genid'.format(n + 1), 'f',
         'result = min(abs(taus.at({0}).genPdgId()), 29)'.format(n))
    Leaf('tau{0}_genparentid'.format(n + 1), 'f',
         'result = min(abs(taus.at({0}).parentId()), 29)'.format(n))
    Leaf('tau{0}_gen_deltaR'.format(n + 1), 'f',
         'result = dR(taus.at({0}).p4(), taus.at({0}).genP4())'.format(n))

    # Gen matching
    Plot(
        name="taus/generator/T{0}_Match".format(n + 1),
        values=["tau{0}_match".format(n + 1)],
        labels=[lbl + " id", "Events"],
        binning=[6, 1, 7],
        binlabels=["e", "#mu", "#tau #rightarrow e",
                   "#tau #rightarrow #mu", "#tau #rightarrow #tau_{h}", "fake"]
    )
    Plot(
        name="taus/generator/T{0}_GenId".format(n + 1),
        values=["tau{0}_genid".format(n + 1)],
        labels=[lbl + " id", "Events"],
        binning=[30, 0, 30]
    )
    Plot(
        name="taus/generator/T{0}_ParentId".format(n + 1),
        values=["tau{0}_genparentid".format(n + 1)],
        labels=[lbl + "parent id", "Events"],
        binning=[30, 0, 30]
    )
    Plot(
        name="taus/generator/T{0}_GenDeltaR".format(n + 1),
        values=["tau{0}_gen_deltaR".format(n + 1)],
        labels=["#DeltaR_{reco,gen} " + lbl, "Events"],
        binning=[15, 0, 6.28]
    )

    Leaf('tau{0}_decaymode'.format(n + 1), 'f', 'result = taus.at({0}).decayMode()'.format(n))
    Leaf('tau{0}_nprongs'.format(n + 1), 'f', 'result = taus.at({0}).prongs()'.format(n))
    Leaf('tau{0}_iso3hits03'.format(n + 1), 'f', 'result = taus.at({0}).isolation3Hits03()'.format(n))
    Leaf('tau{0}_iso3hits05'.format(n + 1), 'f', 'result = taus.at({0}).isolation3Hits05()'.format(n))
    Leaf('tau{0}_isoMVA03'.format(n + 1), 'f', 'result = taus.at({0}).isolationMVA03()'.format(n))
    Leaf('tau{0}_isoMVA03_raw'.format(n + 1), 'f', 'result = taus.at({0}).rawIsolationMVA03()'.format(n))
    Leaf('tau{0}_isoMVA05'.format(n + 1), 'f', 'result = taus.at({0}).isolationMVA05()'.format(n))
    Leaf('tau{0}_vetoelectron'.format(n + 1), 'f', 'result = taus.at({0}).vetoElectron()'.format(n))
    Leaf('tau{0}_vetomuon'.format(n + 1), 'f', 'result = taus.at({0}).vetoMuon()'.format(n))

    # ID
    Plot(
        name="taus/id/T{0}_DecayMode".format(n + 1),
        values=["tau{0}_decaymode".format(n + 1)],
        labels=[lbl + " Decay Mode", "Events"],
        binning=[5, 0, 5]
    )
    Plot(
        name="taus/id/T{0}_NumProngs".format(n + 1),
        values=["tau{0}_nprongs".format(n + 1)],
        labels=[lbl + " Num Prongs", "Events"],
        binning=[5, 0, 5]
    )
    Plot(
        name="taus/id/T{0}_Isolation3Hits03".format(n + 1),
        values=["tau{0}_iso3hits03".format(n + 1)],
        labels=[lbl + " 3 Hit Isolation Index", "Events"],
        binning=[5, 0, 5]
    )
    Plot(
        name="taus/id/T{0}_Isolation3Hits05".format(n + 1),
        values=["tau{0}_iso3hits05".format(n + 1)],
        labels=[lbl + " 3 Hit Isolation Index", "Events"],
        binning=[5, 0, 5]
    )
    Plot(
        name="taus/id/T{0}_IsolationMVA03".format(n + 1),
        values=["tau{0}_isoMVA03".format(n + 1)],
        labels=[lbl + " Isolation MVA Index", "Events"],
        binning=[7, 0, 7]
    )
    Plot(
        name="taus/id/T{0}_IsolationMVA03_Raw".format(n + 1),
        values=["tau{0}_isoMVA03_raw".format(n + 1)],
        labels=[lbl + " Raw Isolation for MVA, #Delta R = 0.3", "Events"],
        binning=[30, 0, 2]
    )
    Plot(
        name="taus/id/T{0}_IsolationMVA05".format(n + 1),
        values=["tau{0}_isoMVA05".format(n + 1)],
        labels=[lbl + " Isolation MVA Index", "Events"],
        binning=[7, 0, 7]
    )
    Plot(
        name="taus/id/T{0}_VetoElectron".format(n + 1),
        values=["tau{0}_vetoelectron".format(n + 1)],
        labels=[lbl + " Electron Veto (MVA5)", "Events"],
        binning=[5, 0, 5]
    )
    Plot(
        name="taus/id/T{0}_VetoMuon".format(n + 1),
        values=["tau{0}_vetomuon".format(n + 1)],
        labels=[lbl + " Muon Veto (##3)", "Events"],
        binning=[5, 0, 5]
    )
