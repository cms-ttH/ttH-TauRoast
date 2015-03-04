from ttH.TauRoast.plotting import Plot

Plot(
        name="njets",
        code="histo.Fill(jets.size())",
        labels=["jet multiplicity", "count"],
        binning=[30, 0, 30]
)
Plot(
        name="ntags",
        code="histo.Fill(len(filter(btag, jets)))",
        labels=["tag multiplicity", "count"],
        binning=[31, 0, 30]
)
Plot(
        name="tags_vs_jets",
        code="histo.Fill(jets.size(), len(filter(btag, jets)))",
        labels=["jets", "tags"],
        binning=[10, 0, 10, 5, 0, 5],
        dim=2,
        draw="COLZ"
)
Plot(
        name="jetpt",
        code="for j in jets: histo.Fill(j.p4().Pt())",
        labels=["P_{T}", "count"],
        binning=[40, 0, 160]
)
Plot(
        name="leppt",
        code="for l in leptons: histo.Fill(l.p4().Pt())",
        labels=["P_{T}", "count"],
        binning=[40, 0, 160]
)
Plot(
        name="tau1pt",
        code="""
histo.Fill(taus[0].p4().Pt())
print taus[0].p4().Pt()
print taus.at(0).p4().Pt()
print taus[1].p4().Pt()
print taus[0].p4().Eta()
print taus[0].p4().Pt()
for tau in taus:
  print "t", tau.p4().Pt()
for lep in leptons:
  print "l", lep.p4().Pt()
  break
print '---'""",
        labels=["P_{T}", "count"],
        binning=[40, 0, 160]
)
Plot(
        name="tau2pt",
        code="histo.Fill(taus[1].p4().Pt())",
        labels=["P_{T}", "count"],
        binning=[40, 0, 160]
)
Plot(
        name="jj_m",
        code="""
masses = []
for n, j in enumerate(jets):
    for k in jets[n + 1:]:
        if btag(j) or btag(k):
            continue
        masses.append((j.p4() + k.p4()).M())
if len(masses) > 0:
    histo.Fill(min(masses, key=lambda m: abs(m - 80)))
""",
        labels=["mass", "count"],
        binning=[40, 0, 400]
)
Plot(
        name="tau1id",
        code="histo.Fill(abs(taus[0].pdgId()))",
        labels=["id", "count"],
        binning=[30, 0, 30]
)
Plot(
        name="tau2id",
        code="histo.Fill(abs(taus[1].pdgId()))",
        labels=["id", "count"],
        binning=[30, 0, 30]
)
Plot(
        name="tau1pid",
        code="histo.Fill(abs(taus[0].parentId()))",
        labels=["id", "count"],
        binning=[60, -29.5, 29.5]
)
Plot(
        name="tau2pid",
        code="histo.Fill(abs(taus[1].parentId()))",
        labels=["id", "count"],
        binning=[60, -29.5, 29.5]
)
Plot(
        name="npv",
        code="histo.Fill(event.npv())",
        labels=["npv", "count"],
        binning=[60, 0, 60]
)
Plot(
        name="combos",
        code="histo.Fill(event.combos().size())",
        labels=["combos", "count"],
        binning=[10, 0, 10]
)
