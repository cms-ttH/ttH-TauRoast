from ttH.TauRoast.botany import Leaf
from ttH.TauRoast.plotting import Plot

# Leaf('best_w_mass', 'f', """
# masses = []
# for n, j in enumerate(jets):
#     for k in jets[n + 1:]:
#         if btag(j) or btag(k):
#             continue
#         masses.append((j.p4() + k.p4()).M())
# if len(masses) > 0:
#     return min(masses, key=lambda m: abs(m - 80))
# else:
#     return -1.
# """
# )

# Plot(
#         name="jets/Best_W_Mass",
#         values=["best_w_mass"],
#         labels=["mass", "count"],
#         binning=[40, 0, 400]
# )

Leaf('jet_deltaRavg', 'f',
     '''std::vector<float> drs;
        for (auto i = 0; i < jets.size(); ++i)
            for (auto j = i + 1; j < jets.size(); ++j)
                drs.push_back(dR(jets[i], jets[j]));
        result = std::accumulate(drs.begin(), drs.end(), 0.) / drs.size()''')
Plot(
    name="jets/JJ_AvgDeltaR",
    values=["jet_deltaRavg"],
    labels=["Avg #DeltaR jet, jet", "Events"],
    binning=[15, 0, 6.28],
    essential=True
)

Leaf('jet_deltaRmin', 'f',
     '''std::vector<float> drs;
        for (auto i = 0; i < jets.size(); ++i)
            for (auto j = i + 1; j < jets.size(); ++j)
                drs.push_back(dR(jets[i], jets[j]));
        result = drs.size() > 0 ? *std::min_element(drs.begin(), drs.end()) : -666.''')
Plot(
    name="jets/JJ_MinDeltaR",
    values=["jet_deltaRmin"],
    labels=["Min #DeltaR jet, jet", "Events"],
    binning=[15, 0, 6.28]
)

Leaf('jet_deltaRmax', 'f',
     '''std::vector<float> drs;
        for (auto i = 0; i < jets.size(); ++i)
            for (auto j = i + 1; j < jets.size(); ++j)
                drs.push_back(dR(jets[i], jets[j]));
        result = drs.size() > 0 ? *std::max_element(drs.begin(), drs.end()) : -666.''')
Plot(
    name="jets/JJ_MaxDeltaR",
    values=["jet_deltaRmin"],
    labels=["Min #DeltaR jet, jet", "Events"],
    binning=[15, 0, 6.28]
)

Leaf('jet_relativejes', '[f]',
     '''
     auto jesup = event.jets("CMS_ttHl_JESUp");
     auto jesdown = event.jets("CMS_ttHl_JESDown");
     for (unsigned int i = 0; i < std::min({jets.size(), jesup.size(), jesdown.size()}); ++i) {
         auto up = std::abs(jets[i].pt() - jesup[i].pt()) / jets[i].pt();
         auto down = std::abs(jets[i].pt() - jesdown[i].pt()) / jets[i].pt();
         result.push_back(up);
         result.push_back(down);
     }''')

Leaf('jet_ideff', '[i]',
     '''
     for (auto j: jets) {
         if (j.flavor() == 5)
             result.push_back(btag(j));
     }''')

Leaf('jet_ideff_loose', '[i]',
     '''
     for (auto j: jets) {
         if (j.flavor() == 5)
             result.push_back(btag(j, true));
     }''')

Leaf('jet_mistag', '[i]',
     '''
     for (auto j: jets) {
         if (j.flavor() != 5)
             result.push_back(btag(j));
     }''')

pspace = [
    ("", "jets"),
    ("Untagged", "notags(jets)"),
    ("Tagged", "tags(jets)")
]

jspace = [
    ("L", "Leading"),
    ("SubL", "Sub-leading"),
    ("SubSubL", "3rd"),
    ("SubSubSubL", "4th")
]

return_list = """
std::vector<float> fs;
for (const auto& j: {0})
    result.push_back(j.{1});
"""

for i in range(2):
    Leaf('jet{}_pt'.format(i + 1), 'f', 'result = jets.at({}).p4().Pt()'.format(i))
    Plot(
        name="jets/kinematic/Jet{}_Pt".format(i + 1),
        values=['jet{}_pt'.format(i + 1)],
        labels=["Jet_{{{}}} P_{{T}}".format(i + 1), "Events"],
        binning=[20, 20, 250]
    )

    Leaf('tag{}_pt'.format(i + 1), 'f', 'result = tags(jets).at({}).p4().Pt()'.format(i))
    Plot(
        name="jets/kinematic/Tag{}_Pt".format(i + 1),
        values=['tag{}_pt'.format(i + 1)],
        labels=["b-Jet_{{{}}} P_{{T}}".format(i + 1), "Events"],
        binning=[20, 20, 250]
    )

    Leaf('notag{}_pt'.format(i + 1), 'f', 'result = notags(jets).at({}).p4().Pt()'.format(i))
    Plot(
        name="jets/kinematic/NTag{}_Pt".format(i + 1),
        values=['notag{}_pt'.format(i + 1)],
        labels=["non b-Jet_{{{}}} P_{{T}}".format(i + 1), "Events"],
        binning=[20, 20, 250]
    )

for mode, fltr in pspace:
    Leaf('{0}jet_pt'.format(mode.lower()), '[f]', return_list.format(fltr, 'p4().Pt()'))
    Leaf('{0}jet_eta'.format(mode.lower()), '[f]', return_list.format(fltr, 'p4().Eta()'))
    Leaf('{0}jet_csv'.format(mode.lower()), '[f]', return_list.format(fltr, 'csv()'))

    Leaf('{0}jet_genpt'.format(mode.lower()), '[f]', return_list.format(fltr, 'genP4().Pt()'))
    Leaf('{0}jet_geneta'.format(mode.lower()), '[f]', return_list.format(fltr, 'genP4().Eta()'))

    Plot(
        name="jets/kinematic/{0}J_Pt".format(mode),
        values=['{0}jet_pt'.format(mode.lower())],
        labels=["{0} jet P_{{T}}".format(mode), "Events"],
        binning=[20, 20, 250]
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

    Plot(
        name="jets/generator/{0}J_GenPt".format(mode),
        values=['{0}jet_genpt'.format(mode.lower())],
        labels=["{0} jet gen P_{{T}}".format(mode), "Events"],
        binning=[20, 0, 100]
    )
    Plot(
        name="jets/generator/{0}J_GenEta".format(mode),
        values=['{0}jet_geneta'.format(mode.lower())],
        labels=["{0} jet gen #eta".format(mode), "Events"],
        binning=[40, -3, 3]
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
