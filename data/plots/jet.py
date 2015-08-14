from ttH.TauRoast.botany import Leaf
from ttH.TauRoast.plotting import Plot

Leaf('best_w_mass', 'f', """
masses = []
for n, j in enumerate(jets):
    for k in jets[n + 1:]:
        if btag(j) or btag(k):
            continue
        masses.append((j.p4() + k.p4()).M())
if len(masses) > 0:
    result = min(masses, key=lambda m: abs(m - 80))
else:
    result = -1.
"""
)

Plot(
        name="jets/Best_W_Mass",
        values=["best_w_mass"],
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
    Leaf('{0}jet_pt'.format(mode.lower()), '[f]', 'result = [j.p4().Pt() for j in {0}]'.format(fltr))
    Leaf('{0}jet_eta'.format(mode.lower()), '[f]', 'result = [j.p4().Eta() for j in {0}]'.format(fltr))
    Leaf('{0}jet_csv'.format(mode.lower()), '[f]', 'result = [j.csv() for j in {0}]'.format(fltr))

    Plot(
            name="jets/kinematic/{0}J_Pt".format(mode),
            values=['{0}jet_pt'.format(mode.lower())],
            labels=["{0} jet P_{{T}}".format(mode), "Events"],
            binning=[20, 30, 250]
    )
    Plot(
            name="jets/kinematic/{0}J_Eta".format(mode),
            values=['{0}jet_eta'.format(mode.lower())],
            labels=["{0} jet #eta".format(mode), "Events"],
            binning=[40, -3, 3]
    )
    Plot(
            name="jets/id/{0}J_CSV".format(mode),
            values=['{0}jet_csv'.format(mode.lower())],
            labels=["{0} jet CSV".format(mode), "Events"],
            binning=[30, 0, 1]
    )

    for n, (short, long) in enumerate(jspace):
        Plot(
                name="jets/kinematic/{0}{1}J_Pt".format(mode, short),
                values=['{0}jet_pt[{1}]'.format(mode.lower(), n)],
                labels=["{0} {1} jet P_{{T}}".format(mode, long), "Events"],
                binning=[20, 30, 250]
        )
        Plot(
                name="jets/kinematic/{0}{1}J_Eta".format(mode, short),
                values=['{0}jet_eta[{1}]'.format(mode.lower(), n)],
                labels=["{0} {1} jet #eta".format(mode, long), "Events"],
                binning=[40, -3, 3]
        )
        Plot(
                name="jets/id/{0}{1}J_CSV".format(mode, short),
                values=['{0}jet_csv[{1}]'.format(mode.lower(), n)],
                labels=["{0} {1} jet CSV".format(mode, long), "Events"],
                binning=[30, 0, 1]
        )
