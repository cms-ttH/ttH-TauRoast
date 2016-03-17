from ttH.TauRoast.botany import Leaf
from ttH.TauRoast.processing import BasicProcess

BasicProcess(
    name="sync",
    sample=9125,
    fullname="Sync",
    limitname="sync",
    paths=['.'],
    cross_section=1,
    events=45140
)

Leaf('nEvent', 'i', 'result = event.event()')
Leaf('n_presel_mu', 'i', 'result = len(muons)')
Leaf('n_presel_ele', 'i', 'result = len(electrons)')
Leaf('n_presel_tau', 'i', 'result = len(all_taus)')
Leaf('n_presel_jet', 'i', 'result = len(jets)')

for i in range(2):
    Leaf('mu{}_pt'.format(i), 'f', 'result = len(muons) <= {0} ? -9999 : muons[{0}].p4().pt()'.format(i))
    Leaf('mu{}_eta'.format(i), 'f', 'result = len(muons) <= {0} ? -9999 : muons[{0}].p4().eta()'.format(i))
    Leaf('mu{}_phi'.format(i), 'f', 'result = len(muons) <= {0} ? -9999 : muons[{0}].p4().phi()'.format(i))
    Leaf('mu{}_E'.format(i), 'f', 'result = len(muons) <= {0} ? -9999 : muons[{0}].p4().E()'.format(i))
    Leaf('mu{}_charge'.format(i), 'f', 'result = len(muons) <= {0} ? -9999 : muons[{0}].charge()'.format(i))
    Leaf('mu{}_dxy'.format(i), 'f', 'result = len(muons) <= {0} ? -9999 : muons[{0}].dxy()'.format(i))
    Leaf('mu{}_dz'.format(i), 'f', 'result = len(muons) <= {0} ? -9999 : muons[{0}].dz()'.format(i))

for i in range(2):
    Leaf('ele{}_pt'.format(i), 'f', 'result = len(electrons) <= {0} ? -9999 : electrons[{0}].p4().pt()'.format(i))
    Leaf('ele{}_eta'.format(i), 'f', 'result = len(electrons) <= {0} ? -9999 : electrons[{0}].p4().eta()'.format(i))
    Leaf('ele{}_phi'.format(i), 'f', 'result = len(electrons) <= {0} ? -9999 : electrons[{0}].p4().phi()'.format(i))
    Leaf('ele{}_E'.format(i), 'f', 'result = len(electrons) <= {0} ? -9999 : electrons[{0}].p4().E()'.format(i))
    Leaf('ele{}_charge'.format(i), 'f', 'result = len(electrons) <= {0} ? -9999 : electrons[{0}].charge()'.format(i))
    Leaf('ele{}_dxy'.format(i), 'f', 'result = len(electrons) <= {0} ? -9999 : electrons[{0}].dxy()'.format(i))
    Leaf('ele{}_dz'.format(i), 'f', 'result = len(electrons) <= {0} ? -9999 : electrons[{0}].dz()'.format(i))

for i in range(1):
    Leaf('tau{}_pt'.format(i), 'f', 'result = len(taus) <= {0} ? -9999 : taus[{0}].p4().pt()'.format(i))
    Leaf('tau{}_eta'.format(i), 'f', 'result = len(taus) <= {0} ? -9999 : taus[{0}].p4().eta()'.format(i))
    Leaf('tau{}_phi'.format(i), 'f', 'result = len(taus) <= {0} ? -9999 : taus[{0}].p4().phi()'.format(i))
    Leaf('tau{}_E'.format(i), 'f', 'result = len(taus) <= {0} ? -9999 : taus[{0}].p4().E()'.format(i))
    Leaf('tau{}_charge'.format(i), 'f', 'result = len(taus) <= {0} ? -9999 : taus[{0}].charge()'.format(i))
    Leaf('tau{}_dxy'.format(i), 'f', 'result = len(taus) <= {0} ? -9999 : taus[{0}].dxy()'.format(i))
    Leaf('tau{}_dz'.format(i), 'f', 'result = len(taus) <= {0} ? -9999 : taus[{0}].dz()'.format(i))

    Leaf('tau{}_byLooseCombinedIsolationDeltaBetaCorr3Hits'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].isolation3Hits05() >= superslim::id::Loose'.format(i))
    Leaf('tau{}_byMediumCombinedIsolationDeltaBetaCorr3Hits'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].isolation3Hits05() >= superslim::id::Medium'.format(i))
    Leaf('tau{}_byTightCombinedIsolationDeltaBetaCorr3Hits'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].isolation3Hits05() >= superslim::id::Tight'.format(i))
    Leaf('tau{}_byLooseCombinedIsolationDeltaBetaCorr3HitsdR03'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].isolation3Hits03() >= superslim::id::Loose'.format(i))
    Leaf('tau{}_byMediumCombinedIsolationDeltaBetaCorr3HitsdR03'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].isolation3Hits03() >= superslim::id::Medium'.format(i))
    Leaf('tau{}_byTightCombinedIsolationDeltaBetaCorr3HitsdR03'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].isolation3Hits03() >= superslim::id::Tight'.format(i))
    Leaf('tau{}_byLooseIsolationMVArun2v1DBdR03oldDMwLT'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].isolationMVA03() >= superslim::id::Loose'.format(i))
    Leaf('tau{}_byMediumIsolationMVArun2v1DBdR03oldDMwLT'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].isolationMVA03() >= superslim::id::Medium'.format(i))
    Leaf('tau{}_byTightIsolationMVArun2v1DBdR03oldDMwLT'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].isolationMVA03() >= superslim::id::Tight'.format(i))
    Leaf('tau{}_byVTightIsolationMVArun2v1DBdR03oldDMwLT'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].isolationMVA03() >= superslim::id::VTight'.format(i))

    Leaf('tau{}_againstMuonLoose3'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].vetoMuon() >= superslim::id::Loose'.format(i))
    Leaf('tau{}_againstMuonMedium3'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].vetoMuon() >= superslim::id::Medium'.format(i))
    Leaf('tau{}_againstMuonTight3'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].vetoMuon() >= superslim::id::Tight'.format(i))
    Leaf('tau{}_againstElectronVLooseMVA6'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].vetoElectron() >= superslim::id::VLoose'.format(i))
    Leaf('tau{}_againstElectronLooseMVA6'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].vetoElectron() >= superslim::id::Loose'.format(i))
    Leaf('tau{}_againstElectronMediumMVA6'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].vetoElectron() >= superslim::id::Medium'.format(i))
    Leaf('tau{}_againstElectronTightMVA6'.format(i), 'i',
            'result = len(taus) <= {0} ? -9999 : taus[{0}].vetoElectron() >= superslim::id::Tight'.format(i))

for i in range(4):
    Leaf('jet{}_pt'.format(i), 'f', 'result = len(jets) <= {0} ? -9999 : jets[{0}].p4().pt()'.format(i))
    Leaf('jet{}_eta'.format(i), 'f', 'result = len(jets) <= {0} ? -9999 : jets[{0}].p4().eta()'.format(i))
    Leaf('jet{}_phi'.format(i), 'f', 'result = len(jets) <= {0} ? -9999 : jets[{0}].p4().phi()'.format(i))
    Leaf('jet{}_E'.format(i), 'f', 'result = len(jets) <= {0} ? -9999 : jets[{0}].p4().E()'.format(i))
    Leaf('jet{}_CSV'.format(i), 'f', 'result = len(jets) <= {0} ? -9999 : jets[{0}].csv()'.format(i))
