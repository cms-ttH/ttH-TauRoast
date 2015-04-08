from ttH.TauRoast.plotting import Plot

for n in range(1):
    lbl = "l_{{{0}}} ".format(n + 1)
    # Kinematics
    Plot(
        name="leptons/kinematic/L{0}_Pt".format(n + 1),
        code="histo.Fill(leptons[{0}].p4().Pt(), weight)".format(n),
        labels=[lbl + "P_{T}", "Events"],
        binning=[20, 0, 100]
    )
    Plot(
        name="leptons/kinematic/L{0}_Eta".format(n + 1),
        code="histo.Fill(leptons[{0}].p4().Eta(), weight)".format(n),
        labels=[lbl + "#eta", "Events"],
        binning=[20, -3, 3]
    )
    Plot(
        name="leptons/kinematic/L{0}_Phi".format(n + 1),
        code="histo.Fill(leptons[{0}].p4().Phi(), weight)".format(n),
        labels=[lbl + "#Phi", "Events"],
        binning=[20, -3.2, 3.2]
    )

    # Gen matching
    Plot(
            name="leptons/generator/L{0}_Match".format(n + 1),
            code="histo.Fill(abs(leptons[{0}].pdgId()), weight)".format(n),
            labels=[lbl + " id", "Events"],
            binning=[30, 0, 30]
    )
    Plot(
            name="leptons/generator/L{0}_ParentMatch".format(n + 1),
            code="histo.Fill(abs(leptons[{0}].parentId()), weight)".format(n),
            labels=[lbl + "parent id", "Events"],
            binning=[30, 0, 30]
    )

    # ID
    Plot(
        name="leptons/id/L{0}_ImpactParameter".format(n + 1),
        code="histo.Fill(leptons[{0}].impactParameter(), weight)".format(n),
        labels=[lbl + "Impact parameter", "Events"],
        binning=[20, -0.04, 0.04]
    )
    Plot(
        name="leptons/id/L{0}_CorrectedDZ".format(n + 1),
        code="histo.Fill(leptons[{0}].correctedDZ(), weight)".format(n),
        labels=[lbl + "corrected #Delta z", "Events"],
        binning=[20, -0.04, 0.04]
    )
    Plot(
        name="leptons/id/L{0}_RelIso".format(n + 1),
        code="histo.Fill(leptons[{0}].relativeIsolation(), weight)".format(n),
        labels=[lbl + "RelIso", "Events"],
        binning=[20, 0, 0.25]
    )
