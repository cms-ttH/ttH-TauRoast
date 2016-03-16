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
Leaf('n_presel_mu', 'i', 'result = muons(leptons)')
Leaf('n_presel_ele', 'i', 'result = electrons(leptons)')
Leaf('n_presel_tau', 'i', 'result = len(all_taus)')
Leaf('n_presel_jet', 'i', 'result = len(jets)')
