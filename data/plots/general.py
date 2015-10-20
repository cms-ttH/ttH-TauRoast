import itertools

from ttH.TauRoast.botany import Leaf
from ttH.TauRoast.plotting import Plot

Leaf('events', 'f', 'result = 1')
Leaf('combos', 'f', 'result = len(combos)')
Leaf('allcombos', 'f', 'result = event.combos().size()')
Leaf('njets', 'f', 'result = len([1 for jet in jets if not btag(jet)])')
Leaf('ntags', 'f', 'result = len([1 for jet in jets if btag(jet)])')
Leaf('npv', 'f', 'result = event.npv()')
Leaf('nelectrons', 'f', 'result = len([l for l in leptons if l.electron()])')
Leaf('nmuons', 'f', 'result = len([l for l in leptons if l.muon()])')

Leaf('bdt', 'f', 'result = mva()', final=True)
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
        name="general/NumJetsOverview",
        values=["njets+ntags", "ntags"],
        labels=["Inclusive jets", "Tagged jets"],
        binning=[10, 0, 10, 5, 0, 5]
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

Leaf('vtx_z', 'f', 'result = pv[0].z()')
Leaf('vtx_zerr', 'f', 'result = pv[0].zError() * 1000')

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
ht = 0
for j in jets:
    ht += j.p4().Pt()
for t in taus:
    ht += t.p4().Pt()
for l in leptons:
    ht += l.p4().Pt()
result = ht
""")
Leaf('mht', 'f', """
ht = superslim.LorentzVector()
for j in jets:
    ht += j.p4()
for t in taus:
    ht += t.p4()
for l in leptons:
    ht += l.p4()
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

ws = ["generator"]
ws += map(''.join, itertools.product(["jettaufake", "etaufake", "ideff"], ["up", "down"]))
for w in ws:
    Leaf('w_' + w, 'f', 'result = weights["{0}"]'.format(w))

Plot(
        name="weights/Generator",
        values=["w_generator"],
        weights=[],
        labels=["Generator weight (unweighted)", "Events"],
        binning=[20, -3, 5]
)
