from ttH.TauRoast.plotting import Plot

Plot(
    name="taus/TT_DeltaR",
    code="histo.Fill(dR(taus[0], taus[1]), weight)",
    labels=["#DeltaR #tau_{1,2}", "Events"],
    binning=[15, 0, 6.28]
)
Plot(
    name="taus/TT_VisibleMass",
    code="histo.Fill((taus[0].p4() + taus[1].p4()).M(), weight)",
    labels=["Visible Mass #tau_{1,2}", "Events"],
    binning=[30, 0, 200]
)

for n in range(2):
    lbl = "#tau_{{{0}}} ".format(n + 1)

    Plot(
        name="taus/T{0}L_DeltaR".format(n + 1),
        code="histo.Fill(dR(leptons[0], taus[{0}]), weight)".format(n),
        labels=["#DeltaR " + lbl + ", l", "Events"],
        binning=[15, 0, 6.28]
    )

    # Kinematics
    Plot(
        name="taus/kinematic/T{0}_Pt".format(n + 1),
        code="histo.Fill(taus[{0}].p4().Pt(), weight)".format(n),
        labels=[lbl + "P_{{T}}", "Events"],
        binning=[40, 0, 160]
    )
    Plot(
        name="taus/kinematic/T{0}_Eta".format(n + 1),
        code="histo.Fill(taus[{0}].p4().Eta(), weight)".format(n),
        labels=[lbl + "#eta", "Events"],
        binning=[20, -3, 3]
    )
    Plot(
        name="taus/kinematic/T{0}_Phi".format(n + 1),
        code="histo.Fill(taus[{0}].p4().Phi(), weight)".format(n),
        labels=[lbl + "#Phi", "Events"],
        binning=[20, -3.2, 3.2]
    )

    # Gen matching
    Plot(
            name="taus/generator/T{0}_Match".format(n + 1),
            code="histo.Fill(abs(taus[{0}].pdgId()), weight)".format(n),
            labels=[lbl + " id", "Events"],
            binning=[30, 0, 30]
    )
    Plot(
            name="taus/generator/T{0}_ParentMatch".format(n + 1),
            code="histo.Fill(abs(taus[{0}].parentId()), weight)".format(n),
            labels=[lbl + "parent id", "Events"],
            binning=[30, 0, 30]
    )

    # ID
    Plot(
            name="taus/id/T{0}_DecayMode".format(n + 1),
            code="histo.Fill(taus[{0}].decayMode(), weight)".format(n),
            labels=[lbl + " Decay Mode", "Events"],
            binning=[5, 0, 5]
    )
    Plot(
            name="taus/id/T{0}_NumProngs".format(n + 1),
            code="histo.Fill(taus[{0}].prongs(), weight)".format(n),
            labels=[lbl + " Num Prongs", "Events"],
            binning=[5, 0, 5]
    )
    Plot(
            name="taus/id/T{0}_Isolation3Hits".format(n + 1),
            code="histo.Fill(taus[{0}].isolation3Hits(), weight)".format(n),
            labels=[lbl + " 3 Hit Isolation Index", "Events"],
            binning=[5, 0, 5]
    )
    Plot(
            name="taus/id/T{0}_VetoElectron".format(n + 1),
            code="histo.Fill(taus[{0}].vetoElectron(), weight)".format(n),
            labels=[lbl + " Electron Veto (MVA5)", "Events"],
            binning=[5, 0, 5]
    )
    Plot(
            name="taus/id/T{0}_VetoMuon".format(n + 1),
            code="histo.Fill(taus[{0}].vetoMuon(), weight)".format(n),
            labels=[lbl + " Muon Veto (##3)", "Events"],
            binning=[5, 0, 5]
    )
