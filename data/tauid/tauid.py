import itertools

from ttH.TauRoast.botany import Leaf
from ttH.TauRoast.plotting import Plot
from ttH.TauRoast.processing import BasicProcess, CombinedProcess

BasicProcess(
    name="ttH2Nonbb_125_amc",
    fullname="t#bar{t}H(125)",
    limitname="ttH2Nonbb125",
    paths=['ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_mWCutfix_ext1_v1'],
    cross_section=0.2151,  # 0.5085 * (1 - 0.577),
    events=10045633,
    relativesys=[.025, 0.03, 0.02, 0.103, 0.07, 0.036]
)

BasicProcess(
    name="ttjets_sl_pow",
    fullname="t #bar{t} SL",
    paths=[
        "TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_v1",
    ],
    # cross_section=831.76 * wleptonic * (1 - wleptonic),
    cross_section=182 * 2,
    events=152720952,
)

BasicProcess(
    name="ttjets_dl_pow",
    fullname="t #bar{t} FL",
    paths=[
        "TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV_powheg_pythia8_v1",
    ],
    # cross_section=831.76 * wleptonic ** 2,
    cross_section=87.3,
    events=79092400,
)

CombinedProcess(
    name="ttjets_pow",
    fullname="t#bar{t}",
    limitname="TT",
    subprocesses=["ttjets_sl_pow", "ttjets_dl_pow"]
)


Leaf('events', 'f', 'result = .5')
Plot(
    name="general/Events",
    values=["events"],
    labels=["", "Events"],
    binning=[1, 0, 1]
)

count_jets = """
result = std::count_if(std::begin(jets), std::end(jets), [](const superslim::Jet& j) {
    return abs(j.eta()) < 2.3 and j.pt() > 20;
})
"""

count_gentau = """
result = std::count_if(std::begin(event.genParticles()), std::end(event.genParticles()), [](const superslim::GenObject& p) {
    return abs(p.eta()) < 2.3 and p.pt() > 20 and abs(p.pdgId()) == 15;
})
"""

count_realtau = """
result = std::count_if(std::begin(taus), std::end(taus), [](const superslim::Tau& t) {{
    return abs(t.eta()) < 2.3 and t.pt() > 20 and t.match() == 5 and t.decayMode() > 0 and t.isolation{0}() >= superslim::id::{1};
}})
"""

count_recotau = """
result = std::count_if(std::begin(taus), std::end(taus), [](const superslim::Tau& t) {{
    return abs(t.eta()) < 2.3 and t.pt() > 20 and t.decayMode() > 0 and t.isolation{0}() >= superslim::id::{1};
}})
"""

pt_jets = """
for (const auto& j: jets) {{
    if (abs(j.eta()) < 2.3 and j.pt() > 20)
        result.push_back(j.pt());
}}
"""

pt_recotau = """
for (const auto& t: taus) {{
    if (abs(t.eta()) < 2.3 and t.pt() > 20 and t.decayMode() > 0 and t.isolation{0}() >= superslim::id::{1})
        result.push_back(t.pt());
}}
"""

Leaf('njets', 'i', count_jets)
Leaf('ngentaus', 'i', count_gentau)
Leaf('jetpt', '[f]', pt_jets)

isos = "3Hits03 3Hits05 MVA03 MVA05".split()
cuts = "VLoose Loose Medium Tight VTight VVTight".split()
for iso, cut in itertools.product(isos, cuts):
    Leaf("nrealtaus{}{}".format(iso, cut), 'i', count_realtau.format(iso, cut))
    Leaf("nrecotaus{}{}".format(iso, cut), 'i', count_recotau.format(iso, cut))
    Leaf("ptrecotaus{}{}".format(iso, cut), '[f]', pt_recotau.format(iso, cut))
