from ttH.TauRoast.botany import Leaf
from ttH.TauRoast.plotting import Plot
from ttH.TauRoast.useful import config

if config.taus >= 2:
    Leaf('tt_deltaR', 'f', 'result = dR(taus[0], taus[1])')
    Leaf('tt_visiblemass', 'f', 'result = (taus[0].p4() + taus[1].p4()).M()')

    Plot(
        name="taus/TT_DeltaR",
        values=["tt_deltaR"],
        labels=["#DeltaR #tau_{1,2}", "Events"],
        binning=[15, 0, 6.28]
    )
    Plot(
        name="taus/TT_VisibleMass",
        values=["tt_visiblemass"],
        labels=["Visible Mass #tau_{1,2}", "Events"],
        binning=[30, 0, 200]
    )
    Plot(
        name="taus/TT_VisibleMass_coarse",
        values=["tt_visiblemass"],
        labels=["Visible Mass #tau_{1,2}", "Events"],
        binning=[5, 0, 200]
    )

for n in range(config.taus):
    lbl = "#tau_{{{0}}} ".format(n + 1)

    Leaf('tau{0}lep{0}_deltaR'.format(n + 1, 1), 'f', 'result = dR(leptons[{0}], taus[{1}])'.format(0, n))

    Plot(
        name="taus/T{0}L_DeltaR".format(n + 1),
        values=["tau{0}lep{0}_deltaR".format(n + 1, 1)],
        labels=["#DeltaR " + lbl + ", l", "Events"],
        binning=[15, 0, 6.28]
    )

    Leaf('tau{0}_pt'.format(n + 1), 'f', 'result = taus[{0}].p4().Pt()'.format(n))
    Leaf('tau{0}_eta'.format(n + 1), 'f', 'result = taus[{0}].p4().Eta()'.format(n))
    Leaf('tau{0}_phi'.format(n + 1), 'f', 'result = taus[{0}].p4().Phi()'.format(n))
    Leaf('tau{0}_leadingtrackpt'.format(n + 1), 'f', 'result = taus[{0}].leadingTrackPt()'.format(n))

    # Kinematics
    Plot(
        name="taus/kinematic/T{0}_Pt".format(n + 1),
        values=["tau{0}_pt".format(n + 1)],
        labels=[lbl + "P_{T}", "Events"],
        binning=[10, 20, 100 if n == 0 else 60]
    )
    Plot(
        name="taus/kinematic/T{0}_LTPt".format(n + 1),
        values=["tau{0}_leadingtrackpt".format(n + 1)],
        labels=[lbl + " leading track P_{T}", "Events"],
        binning=[40, 0, 120]
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

    Leaf('tau{0}_genid'.format(n + 1), 'f', 'result = min(abs(taus[{0}].genPdgId()), 29)'.format(n))
    Leaf('tau{0}_genparentid'.format(n + 1), 'f', 'result = min(abs(taus[{0}].parentId()), 29)'.format(n))

    # Gen matching
    Plot(
            name="taus/generator/T{0}_Match".format(n + 1),
            values=["tau{0}_genid".format(n + 1)],
            labels=[lbl + " id", "Events"],
            binning=[30, 0, 30]
    )
    Plot(
            name="taus/generator/T{0}_ParentMatch".format(n + 1),
            values=["tau{0}_genparentid".format(n + 1)],
            labels=[lbl + "parent id", "Events"],
            binning=[30, 0, 30]
    )

    Leaf('tau{0}_decaymode'.format(n + 1), 'f', 'result = taus[{0}].decayMode()'.format(n))
    Leaf('tau{0}_nprongs'.format(n + 1), 'f', 'result = taus[{0}].prongs()'.format(n))
    Leaf('tau{0}_iso3hits'.format(n + 1), 'f', 'result = taus[{0}].isolation3Hits()'.format(n))
    Leaf('tau{0}_vetoelectron'.format(n + 1), 'f', 'result = taus[{0}].vetoElectron()'.format(n))
    Leaf('tau{0}_vetomuon'.format(n + 1), 'f', 'result = taus[{0}].vetoMuon()'.format(n))

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
            name="taus/id/T{0}_Isolation3Hits".format(n + 1),
            values=["tau{0}_iso3hits".format(n + 1)],
            labels=[lbl + " 3 Hit Isolation Index", "Events"],
            binning=[5, 0, 5]
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
