from ttH.TauRoast.plotting import Plot

Plot(
        name="jets/Best_W_Mass",
        code="""
masses = []
for n, j in enumerate(jets):
    for k in jets[n + 1:]:
        if btag(j) or btag(k):
            continue
        masses.append((j.p4() + k.p4()).M())
if len(masses) > 0:
    histo.Fill(min(masses, key=lambda m: abs(m - 80)), weight)
""",
        labels=["mass", "count"],
        binning=[40, 0, 400]
)

pspace = [
        ("", "jets"),
        ("Untagged", "notags"),
        ("Tagged", "tags")
]

jspace = [
        ("L", "Leading"),
        ("SubL", "Sub-leading"),
        ("SubSubL", "3rd"),
        ("SubSubSubL", "4th")
]

for mode, fltr in pspace:
    Plot(
            name="jets/kinematic/{0}J_Pt".format(mode),
            code="for j in {0}: histo.Fill(j.p4().Pt(), weight)".format(fltr),
            labels=["{0} jet P_{{T}}".format(mode), "Events"],
            binning=[20, 30, 250]
    )
    Plot(
            name="jets/kinematic/{0}J_Eta".format(mode),
            code="for j in {0}: histo.Fill(j.p4().Eta(), weight)".format(fltr),
            labels=["{0} jet #eta".format(mode), "Events"],
            binning=[40, -3, 3]
    )
    Plot(
            name="jets/id/{0}J_CSV".format(mode),
            code="for j in {0}: histo.Fill(j.csv(), weight)".format(fltr),
            labels=["{0} jet CSV".format(mode), "Events"],
            binning=[30, 0, 1]
    )
    for n, (short, long) in enumerate(jspace):
        Plot(
                name="jets/kinematic/{0}{1}J_Pt".format(mode, short),
                code="if len({0}) > {1}: histo.Fill({0}[{1}].p4().Pt(), weight)".format(fltr, n),
                labels=["{0} {1} jet P_{{T}}".format(mode, long), "Events"],
                binning=[20, 30, 250]
        )
        Plot(
                name="jets/kinematic/{0}{1}J_Eta".format(mode, short),
                code="if len({0}) > {1}: histo.Fill({0}[{1}].p4().Eta(), weight)".format(fltr, n),
                labels=["{0} {1} jet #eta".format(mode, long), "Events"],
                binning=[40, -3, 3]
        )
        Plot(
                name="jets/id/{0}{1}J_CSV".format(mode, short),
                code="if len({0}) > {1}: histo.Fill({0}[{1}].csv(), weight)".format(fltr, n),
                labels=["{0} {1} jet CSV".format(mode, long), "Events"],
                binning=[30, 0, 1]
        )
