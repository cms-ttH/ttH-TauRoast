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
        name="vertex/ZCoord",
        code="histo.Fill(pv[0].z())",
        labels=["Vertex Z coordinate", "Events"],
        binning=[40, -20, 20]
)
Plot(
        name="vertex/ZCoordError",
        code="histo.Fill(pv[0].zError() * 1000)",
        labels=["Vertex Z coordinate Error (#mum)", "Events"],
        binning=[40, 0, 2]
)

Plot(
        name="general/MET",
        code="histo.Fill(event.met().Pt())",
        labels=["MET", "Events"],
        binning=[20, 0, 300]
)
Plot(
        name="general/HT",
        code="""
ht = 0
for j in jets:
    ht += j.p4().Pt()
for t in taus:
    ht += t.p4().Pt()
for l in leptons:
    ht += l.p4().Pt()
histo.Fill(ht)
        """,
        labels=["HT", "Events"],
        binning=[20, 200, 1000]
)
Plot(
        name="general/MHT",
        code="""
ht = None
for j in jets:
    if ht is None:
        ht = j.p4()
    else:
        ht += j.p4()
for t in taus:
    ht += t.p4()
for l in leptons:
    ht += l.p4()
histo.Fill(ht.Pt())
        """,
        labels=["MHT", "Events"],
        binning=[20, 0, 200]
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
