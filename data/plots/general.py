from ttH.TauRoast.plotting import Plot

Plot(
        name="general/Events",
        code="histo.Fill(0, weight)",
        labels=["", "Events"],
        binning=[1, 0, 1]
)

Plot(
        name="general/Combos",
        code="histo.Fill(len(combos), weight)",
        labels=["Number of passed #tau combinations", "Events"],
        binning=[5, 0, 5]
)
Plot(
        name="general/AllCombos",
        code="histo.Fill(event.combos().size(), weight)",
        labels=["Number of all #tau combinations", "Events"],
        binning=[5, 0, 5]
)

Plot(
        name="general/NumInclusiveJets",
        code="histo.Fill(jets.size(), weight)",
        labels=["Number of inclusive jets", "Events"],
        binning=[10, 0, 10]
)
Plot(
        name="general/NumJets",
        code="histo.Fill(len([1 for jet in jets if not btag(jet)]), weight)",
        labels=["Number of jets", "Events"],
        binning=[10, 0, 10]
)
Plot(
        name="general/NumTags",
        code="histo.Fill(len(filter(btag, jets)), weight)",
        labels=["Number of tags", "Events"],
        binning=[10, 0, 10]
)
Plot(
        name="general/NumVertices",
        code="histo.Fill(event.npv(), weight)",
        labels=["Number PV", "Events"],
        binning=[40, 0, 40]
)
Plot(
        name="general/NumJetsOverview",
        code="histo.Fill(jets.size(), len(filter(btag, jets)), weight)",
        labels=["Inclusive jets", "Tagged jets"],
        binning=[10, 0, 10, 5, 0, 4],
        dimensions=2
)
Plot(
        name="general/NumElectrons",
        code="histo.Fill(len([l for l in leptons if l.electron()]), weight)",
        labels=["Number of e", "Events"],
        binning=[3, 0, 3]
)
Plot(
        name="general/NumMuons",
        code="histo.Fill(len([l for l in leptons if l.muon()]), weight)",
        labels=["Number of #mu", "Events"],
        binning=[3, 0, 3]
)

Plot(
        name="vertex/ZCoord",
        code="histo.Fill(pv[0].z(), weight)",
        labels=["Vertex Z coordinate", "Events"],
        binning=[40, -20, 20]
)
Plot(
        name="vertex/ZCoordError",
        code="histo.Fill(pv[0].zError() * 1000, weight)",
        labels=["Vertex Z coordinate Error (#mum)", "Events"],
        binning=[40, 0, 2]
)

Plot(
        name="general/MET",
        code="histo.Fill(event.met().Pt(), weight)",
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
histo.Fill(ht, weight)
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
histo.Fill(ht.Pt(), weight)
        """,
        labels=["MHT", "Events"],
        binning=[20, 0, 200]
)

Plot(
        name="weights/Generator",
        code="""
ws = event.weights()
histo.Fill(ws['generator'])
        """,
        labels=["Generator weight (unweighted)", "Events"],
        binning=[20, -3, 5]
)
