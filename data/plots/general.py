import itertools

from ttH.TauRoast.botany import Leaf
from ttH.TauRoast.plotting import Plot

Leaf('run', 'i', 'result = event.run()')
Leaf('lumi', 'i', 'result = event.lumi()')
Leaf('event', 'i', 'result = event.event()')

Leaf('events', 'f', 'result = .5')
Leaf('njets', 'f', 'result = jets.size() - btags(jets)')
Leaf('ntags', 'f', 'result = btags(jets)')
Leaf('npv', 'f', 'result = event.npv()')
Leaf('nelectrons', 'f', 'int n = 0; for (const auto& l: leptons) n += l.electron(); result = n')
Leaf('nmuons', 'f', 'int n = 0; for (const auto& l: leptons) n += l.muon(); result = n')
Leaf('ntaus', 'i', 'result = len(taus)')
Leaf('ntausreal', 'i', 'result = std::count_if(taus.begin(), taus.end(), [](const superslim::Tau& t) { return t.match() == 5; })')
Leaf('ntausjetfake', 'i', 'result = std::count_if(taus.begin(), taus.end(), [](const superslim::Tau& t) { return t.match() == 6; })')
Leaf('nalltaus', 'i', 'result = len(all_taus)')
Leaf('nalltausreal', 'i', 'result = std::count_if(all_taus.begin(), all_taus.end(), [](const superslim::Tau& t) { return t.match() == 5; })')
Leaf('nalltausjetfake', 'i', 'result = std::count_if(all_taus.begin(), all_taus.end(), [](const superslim::Tau& t) { return t.match() == 6; })')
Leaf('category', 'i', 'result = min(max(0, btags(jets) - 1), 1) * 3 + min(max(0, len(jets) - 2), 2)')
Leaf('category_lj', 'i', 'result = min(max(0, btags(jets) - 2), 2) * 3 + min(max(0, len(jets) - 4), 2)')

Leaf('bdt', 'f', 'result = 0.', final=True)
Plot(
    name="general/BDT",
    values=["bdt"],
    labels=["BDT value", "Events"],
    binning=[20, -1, 1]
)

Plot(
    name="general/Events",
    values=["events"],
    labels=["", "Events"],
    binning=[1, 0, 1]
)

Plot(
    name="general/NumInclusiveJets",
    values=["njets+ntags"],
    labels=["Number of inclusive jets", "Events"],
    binning=[10, 0, 10]
)
Plot(
    name="general/NumJets",
    values=["njets"],
    labels=["Number of jets", "Events"],
    binning=[10, 0, 10]
)
Plot(
    name="general/NumTags",
    values=["ntags"],
    labels=["Number of tags", "Events"],
    binning=[10, 0, 10]
)

Plot(
    name="general/Category",
    values=["category"],
    labels=["", "Events"],
    binning=[6, 0, 6],
    binlabels=["2j1t", "3j1t", "4j1t", "2j2t", "3j2t", "4j2t"]
)
Plot(
    name="general/Category_lj",
    values=["category_lj"],
    labels=["", "Events"],
    log=True,
    binning=[9, 0, 9],
    binlabels=["4j2t", "5j2t", "6j2t", "4j3t", "5j3t", "6j3t", "4j4t", "5j4t", "6j4t"]
)

Plot(
    name="general/NumVertices",
    values=['npv'],
    labels=["Number PV", "Events"],
    binning=[40, 0, 40]
)
Plot(
    name="general/NumVertices_coarse",
    values=['npv'],
    labels=["Number PV", "Events"],
    binning=[20, 0, 40]
)
Plot(
    name="general/NumJetsOverview",
    values=["ntags", "njets+ntags"],
    labels=["Inclusive jets", "Tagged jets"],
    binning=[12, 0, 12, 4, 0, 4]
)
Plot(
    name="general/NumLeptons",
    values=["nelectrons + nmuons"],
    labels=["Number of leptons", "Events"],
    binning=[10, 0, 10]
)
Plot(
    name="general/NumElectrons",
    values=["nelectrons"],
    labels=["Number of e", "Events"],
    binning=[3, 0, 3]
)
Plot(
    name="general/NumMuons",
    values=["nmuons"],
    labels=["Number of #mu", "Events"],
    binning=[3, 0, 3]
)

Plot(
    name="general/NumTaus",
    values=["ntaus"],
    labels=["Number of selected #tau", "Events"],
    binning=[10, 0, 10]
)
Plot(
    name="general/NumTausReal",
    values=["ntausreal"],
    labels=["Number of selected real #tau", "Events"],
    binning=[10, 0, 10]
)
Plot(
    name="general/NumTausJetFakes",
    values=["ntausjetfake"],
    labels=["Number of selected fake #tau from jets", "Events"],
    binning=[10, 0, 10]
)
Plot(
    name="general/NumAllTaus",
    values=["nalltaus"],
    labels=["Number of #tau", "Events"],
    binning=[10, 0, 10]
)
Plot(
    name="general/NumAllTausReal",
    values=["nalltausreal"],
    labels=["Number of real #tau", "Events"],
    binning=[10, 0, 10]
)
Plot(
    name="general/NumAllTausJetFakes",
    values=["nalltausjetfake"],
    labels=["Number of fake #tau from jets", "Events"],
    binning=[10, 0, 10]
)

Leaf('vtx_z', 'f', 'result = event.pv()[0].z()')
Leaf('vtx_zerr', 'f', 'result = event.pv()[0].zError() * 1000')

Plot(
    name="vertex/ZCoord",
    values=["vtx_z"],
    labels=["Vertex Z coordinate", "Events"],
    binning=[40, -20, 20]
)
Plot(
    name="vertex/ZCoordError",
    values=["vtx_zerr"],
    labels=["Vertex Z coordinate Error (#mum)", "Events"],
    binning=[40, 0, 2]
)

Leaf('met', 'f', 'result = met.Pt()')
Leaf('ht', 'f', """
float ht = 0.;
for (const auto& j: jets)
    ht += j.p4().Pt();
for (const auto& t: taus)
    ht += t.p4().Pt();
for (const auto& l: leptons)
    ht += l.p4().Pt();
result = ht
""")
Leaf('mht', 'f', """
auto ht = superslim::LorentzVector();
for (const auto& j: jets)
    ht += j.p4();
for (const auto& t: taus)
    ht += t.p4();
for (const auto& l: leptons)
    ht += l.p4();
result = ht.Pt()
""")

Plot(
    name="general/MET",
    values=["met"],
    labels=["MET", "Events"],
    binning=[20, 0, 300]
)
Plot(
    name="general/HT",
    values=["ht"],
    labels=["HT", "Events"],
    binning=[20, 200, 1000]
)
Plot(
    name="general/MHT",
    values=["mht"],
    labels=["MHT", "Events"],
    binning=[20, 0, 200]
)

ws = ["generator", "csvweight", "puweight"]
ws += map(''.join, itertools.product(["tauideff"], ["", "up", "down"]))
ws += map(''.join, itertools.product(["jettaufake", "etaufake"], ["up", "down"]))
ws += map(''.join, itertools.product(["lfcont", "hfcont", "hfstats1",
                                      "hfstats2", "lfstats1", "lfstats2", "charmerr1", "charmerr2"], ["up", "down"]))
for w in ws:
    Leaf('w_' + w, 'f', 'result = weights["{0}"]'.format(w))

Plot(
    name="weights/Generator",
    values=["w_generator"],
    weights=[],
    labels=["Generator weight (unweighted)", "Events"],
    binning=[20, -3, 5]
)
