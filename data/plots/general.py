from ttH.TauRoast.plotting import Plot

Plot(
        name="general/Events",
        code="histo.Fill(0)",
        labels=["", "Events"],
        binning=[1, 0, 1]
)

Plot(
        name="general/NumInclusiveJets",
        code="histo.Fill(jets.size())",
        labels=["Number of inclusive jets", "Events"],
        binning=[10, 0, 10]
)
Plot(
        name="general/NumJets",
        code="histo.Fill(len([1 for jet in jets if not btag(jet)]))",
        labels=["Number of jets", "Events"],
        binning=[10, 0, 10]
)
Plot(
        name="general/NumTags",
        code="histo.Fill(len(filter(btag, jets)))",
        labels=["Number of tags", "Events"],
        binning=[10, 0, 10]
)
Plot(
        name="general/NumVertices",
        code="histo.Fill(event.npv())",
        labels=["Number PV", "Events"],
        binning=[40, 0, 40]
)
Plot(
        name="general/NumJetsOverview",
        code="histo.Fill(jets.size(), len(filter(btag, jets)))",
        labels=["Inclusive jets", "Tagged jets"],
        binning=[10, 0, 10, 5, 0, 4],
        dim=2,
        draw="COLZ"
)


Plot(
        name="general/MET",
        code="histo.Fill(event.met().Pt())",
        labels=["MET", "Events"],
        binning=[20, 0, 300]
)

# Plot(
#         name="leppt",
#         code="for l in leptons: histo.Fill(l.p4().Pt())",
#         labels=["P_{T}", "count"],
#         binning=[40, 0, 160]
# )
# Plot(
#         name="combos",
#         code="histo.Fill(event.combos().size())",
#         labels=["combos", "count"],
#         binning=[10, 0, 10]
# )
