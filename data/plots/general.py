import itertools

from ttH.TauRoast.botany import Leaf
from ttH.TauRoast.plotting import Plot

Leaf('events', 'f', 'return 1')
# Leaf('combos', 'f', 'return combos.size()')
Leaf('allcombos', 'f', 'return event.combos().size()')
Leaf('njets', 'f', 'return jets.size() - btags(jets)')
Leaf('ntags', 'f', 'return btags(jets)')
Leaf('npv', 'f', 'return event.npv()')
Leaf('nelectrons', 'f', 'int n = 0; for (const auto& l: leptons) n += l.electron(); return n')
Leaf('nmuons', 'f', 'int n = 0; for (const auto& l: leptons) n += l.muon(); return n')

Leaf('bdt', 'f', 'return 0.', final=True)
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
        name="general/Combos",
        values=["combos"],
        labels=["Number of passed #tau combinations", "Events"],
        binning=[5, 0, 5]
)
Plot(
        name="general/AllCombos",
        values=['allcombos'],
        labels=["Number of all #tau combinations", "Events"],
        binning=[5, 0, 5]
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

Leaf('vtx_z', 'f', 'return event.pv()[0].z()')
Leaf('vtx_zerr', 'f', 'return event.pv()[0].zError() * 1000')

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

Leaf('met', 'f', 'return met.Pt()')
Leaf('ht', 'f', """
float ht = 0.;
for (const auto& j: jets)
    ht += j.p4().Pt();
for (const auto& t: taus)
    ht += t.p4().Pt();
for (const auto& l: leptons)
    ht += l.p4().Pt();
return ht
""")
Leaf('mht', 'f', """
auto ht = superslim::LorentzVector();
for (const auto& j: jets)
    ht += j.p4();
for (const auto& t: taus)
    ht += t.p4();
for (const auto& l: leptons)
    ht += l.p4();
return ht.Pt()
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
ws += map(''.join, itertools.product(["jettaufake", "etaufake", "ideff"], ["up", "down"]))
ws += map(''.join, itertools.product(["lfcont", "hfcont", "hfstats1", "hfstats2", "lfstats1", "lfstats2", "charmerr1", "charmerr2"], ["up", "down"]))
for w in ws:
    Leaf('w_' + w, 'f', 'return weights["{0}"]'.format(w))

Plot(
        name="weights/Generator",
        values=["w_generator"],
        weights=[],
        labels=["Generator weight (unweighted)", "Events"],
        binning=[20, -3, 5]
)
