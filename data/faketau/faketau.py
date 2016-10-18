from ttH.TauRoast.botany import Leaf
from ttH.TauRoast.plotting import Plot
from ttH.TauRoast.processing import BasicProcess

BasicProcess(
    name="ttjets",
    paths=['ttjets'],
    cross_section=45140,
    events=45140
)

Leaf('events', 'f', 'result = .5')
Plot(
    name="general/Events",
    values=["events"],
    labels=["", "Events"],
    binning=[1, 0, 1]
)

Leaf('nelectrons', 'f', 'result = leptons.size() > 1 ? leptons[0].electron() + leptons[1].electron() : 0')
Leaf('nmuons', 'f', 'result =  leptons.size() > 1 ? leptons[0].muon() + leptons[1].muon() : 0')
Leaf('nloosetaus', 'i', 'result = std::count_if(std::begin(taus), std::end(taus), [](const superslim::Tau& t) { return t.isolationMVA03() >= superslim::id::Loose; })')
Leaf('ntighttaus', 'i', 'result = std::count_if(std::begin(taus), std::end(taus), [](const superslim::Tau& t) { return t.isolationMVA03() >= superslim::id::Tight; })')
Leaf('nrealtaus', 'i', 'result = std::count_if(std::begin(taus), std::end(taus), [](const superslim::Tau& t) { return t.match() == 5; })')
Leaf('nrealtighttaus', 'i', 'result = std::count_if(std::begin(taus), std::end(taus), [](const superslim::Tau& t) { return t.isolationMVA03() >= superslim::id::Tight and t.match() == 5; })')
Leaf('nfaketaus', 'i', 'result = std::count_if(std::begin(taus), std::end(taus), [](const superslim::Tau& t) { return t.match() == 6; })')
Leaf('nfaketighttaus', 'i', 'result = std::count_if(std::begin(taus), std::end(taus), [](const superslim::Tau& t) { return t.isolationMVA03() >= superslim::id::Tight and t.match() == 6; })')

Leaf('nEvent', 'i', 'result = event.event()')
Leaf('n_presel_mu', 'i', 'result = len(muons)')
Leaf('n_presel_ele', 'i', 'result = len(electrons)')
Leaf('n_presel_tau', 'i', 'result = len(taus)')
Leaf('n_presel_jet', 'i', 'result = len(jets)')

return_list = """
for (const auto& o: {0})
    result.push_back(o.{1});
"""

map_list = """
for (const auto& o: {0})
    result.push_back({1}(o))
"""

Leaf('tau_pt', '[f]', return_list.format('taus', 'p4().Pt()'))
Leaf('tau_eta', '[f]', return_list.format('taus', 'p4().Eta()'))
Leaf('tau_isoMVA03', '[f]', return_list.format('taus', 'isolationMVA03()'))
Leaf('tau_genjet_pt', '[f]', return_list.format('taus', 'genJetP4().Pt()'))
Leaf('tau_genjet_eta', '[f]', return_list.format('taus', 'genJetP4().Eta()'))
Leaf('tau_genjetcharged_pt', '[f]', return_list.format('taus', 'genJetChargedP4().Pt()'))
Leaf('tau_genjetcharged_eta', '[f]', return_list.format('taus', 'genJetChargedP4().Eta()'))

Leaf('tau_match', '[f]', return_list.format('taus', 'match()'))
Leaf('tau_genjet_constituents', '[f]', return_list.format('taus', 'genJetConstituents()'))
Leaf('tau_genjetcharged_constituents', '[f]', return_list.format('taus', 'genJetChargedConstituents()'))

Leaf('tau_pjet', '[f]', map_list.format('taus', 'fakeable::pjet'))
Leaf('tau_pfake', '[f]', map_list.format('taus', 'fakeable::pfake'))

Leaf('genjet_pt', '[f]', return_list.format('event.genJets()', 'p4().Pt()'))
Leaf('genjet_eta', '[f]', return_list.format('event.genJets()', 'p4().Eta()'))
Leaf('genjet_constituents', '[i]', return_list.format('event.genJets()', 'constituents()'))
Leaf('genjet_chargedpt', '[f]', return_list.format('event.genJets()', 'chargedP4().Pt()'))
Leaf('genjet_chargedconstituents', '[i]', return_list.format('event.genJets()', 'chargedConstituents()'))

Leaf('genjet_pjet', '[f]', map_list.format('event.genJets()', 'fakeable::pjet'))
Leaf('genjet_pfake', '[f]', map_list.format('event.genJets()', 'fakeable::pfake'))
