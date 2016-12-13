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
Leaf('tau_antiElectron', '[f]', return_list.format('taus', 'vetoElectron()'))
Leaf('tau_antiMuon', '[f]', return_list.format('taus', 'vetoMuon()'))
Leaf('tau_genjet_pt', '[f]', return_list.format('taus', 'genJetP4().Pt()'))
Leaf('tau_genjet_lpt', '[f]', return_list.format('taus', 'genJet().leadingP4().Pt()'))
Leaf('tau_genjet_eta', '[f]', return_list.format('taus', 'genJetP4().Eta()'))
Leaf('tau_genjet_chargedpt', '[f]', return_list.format('taus', 'genJetChargedP4().Pt()'))
Leaf('tau_genjet_chargedeta', '[f]', return_list.format('taus', 'genJetChargedP4().Eta()'))

Leaf('tau_genjet_signalpt', '[f]', return_list.format('taus', 'genJet().signalP4().Pt()'))
Leaf('tau_genjet_signalconstituents', '[i]', return_list.format('taus', 'genJet().signalConstituents()'))
Leaf('tau_genjet_signalchargedpt', '[f]', return_list.format('taus', 'genJet().signalChargedP4().Pt()'))
Leaf('tau_genjet_signalchargedconstituents', '[i]', return_list.format('taus', 'genJet().signalChargedConstituents()'))

Leaf('tau_genjet_isopt', '[f]', return_list.format('taus', 'genJet().isoP4().Pt()'))
Leaf('tau_genjet_isoconstituents', '[i]', return_list.format('taus', 'genJet().isoConstituents()'))
Leaf('tau_genjet_isochargedpt', '[f]', return_list.format('taus', 'genJet().isoChargedP4().Pt()'))
Leaf('tau_genjet_isochargedconstituents', '[i]', return_list.format('taus', 'genJet().isoChargedConstituents()'))

Leaf('tau_genjet_closestpt', '[f]', """
     for (const auto& tau: taus) {
         auto j = tau.genJet().closestGenJet();
         if (j) result.push_back(j->pt());
         else result.push_back(-666.);
     }
     """)
Leaf('tau_genjet_closestdr', '[f]', """
     for (const auto& tau: taus) {
         auto j = tau.genJet().closestGenJet();
         if (j) result.push_back(dR(j->p4(), tau.genJet().p4()));
         else result.push_back(-666.);
     }
     """)
Leaf('tau_genjet_closestparticlept', '[f]', """
     for (const auto& tau: taus) {
         auto j = tau.genJet().closestGenParticle();
         if (j) result.push_back(j->pt());
         else result.push_back(-666.);
     }
     """)
Leaf('tau_genjet_closestparticledr', '[f]', """
     for (const auto& tau: taus) {
         auto j = tau.genJet().closestGenParticle();
         if (j) result.push_back(dR(j->p4(), tau.genJet().p4()));
         else result.push_back(-666.);
     }
     """)

Leaf('tau_match', '[f]', return_list.format('taus', 'match()'))
Leaf('tau_genjet_constituents', '[f]', return_list.format('taus', 'genJetConstituents()'))
Leaf('tau_genjet_chargedconstituents', '[f]', return_list.format('taus', 'genJetChargedConstituents()'))
Leaf('tau_genjet_neutralconstituents', '[f]', """
     for (const auto& tau: taus) {
         result.push_back(tau.genJetConstituents() - tau.genJetChargedConstituents());
     }
     """)

Leaf('genjet_pt', '[f]', return_list.format('event.genJets()', 'p4().Pt()'))
Leaf('genjet_lpt', '[f]', return_list.format('event.genJets()', 'leadingP4().Pt()'))
Leaf('genjet_eta', '[f]', return_list.format('event.genJets()', 'p4().Eta()'))
Leaf('genjet_constituents', '[i]', return_list.format('event.genJets()', 'constituents()'))
Leaf('genjet_chargedpt', '[f]', return_list.format('event.genJets()', 'chargedP4().Pt()'))
Leaf('genjet_chargedconstituents', '[i]', return_list.format('event.genJets()', 'chargedConstituents()'))
Leaf('genjet_neutralconstituents', '[f]', """
     for (const auto& jet: event.genJets()) {
         result.push_back(jet.constituents() - jet.chargedConstituents());
     }
     """)

Leaf('genjet_signalpt', '[f]', return_list.format('event.genJets()', 'signalP4().Pt()'))
Leaf('genjet_signalconstituents', '[i]', return_list.format('event.genJets()', 'signalConstituents()'))
Leaf('genjet_signalchargedpt', '[f]', return_list.format('event.genJets()', 'signalChargedP4().Pt()'))
Leaf('genjet_signalchargedconstituents', '[i]', return_list.format('event.genJets()', 'signalChargedConstituents()'))

Leaf('genjet_isopt', '[f]', return_list.format('event.genJets()', 'isoP4().Pt()'))
Leaf('genjet_isoconstituents', '[i]', return_list.format('event.genJets()', 'isoConstituents()'))
Leaf('genjet_isochargedpt', '[f]', return_list.format('event.genJets()', 'isoChargedP4().Pt()'))
Leaf('genjet_isochargedconstituents', '[i]', return_list.format('event.genJets()', 'isoChargedConstituents()'))

Leaf('genjet_closestpt', '[f]', """
     for (const auto& jet: event.genJets()) {
         auto j = jet.closestGenJet();
         if (j) result.push_back(j->pt());
         else result.push_back(-666.);
     }
     """)
Leaf('genjet_closestdr', '[f]', """
     for (const auto& jet: event.genJets()) {
         auto j = jet.closestGenJet();
         if (j) result.push_back(dR(j->p4(), jet.p4()));
         else result.push_back(-666.);
     }
     """)
Leaf('genjet_closestparticlept', '[f]', """
     for (const auto& jet: event.genJets()) {
         auto j = jet.closestGenParticle();
         if (j) result.push_back(j->pt());
         else result.push_back(-666.);
     }
     """)
Leaf('genjet_closestparticledr', '[f]', """
     for (const auto& jet: event.genJets()) {
         auto j = jet.closestGenParticle();
         if (j) result.push_back(dR(j->p4(), jet.p4()));
         else result.push_back(-666.);
     }
     """)
