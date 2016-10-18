# vim: set sw=4 sta et :

channel: ttl
generator: pow
ntupledir: test/genfilter
outdir: test/genfilter/out
histformat: "{p}_{c}_{v}"
mode: filter

# lumi: 2517
lumi: 30000

plot:
  - ttjets
  - ttjets_filtered

limits: []
discriminants: []

default cuts:
  - "Trigger": >
      (leptons[0].muon() and leptons[0].pt() > 25) or
      (leptons[0].electron() and leptons[0].pt() > 30)
  - "e/μ quality": "len(leptons) == 1 and lepton_id == superslim::Lepton::MVA"
  - "τ quality": "len(taus) == 2 and all_of(taus.begin(), taus.end(), [](const superslim::Tau& t) { return t.isolationMVA03() >= superslim::id::Tight; })"
  - "τ anti-e": "all_of(taus.begin(), taus.end(), [](const superslim::Tau& t) { return t.vetoElectron() >= superslim::id::Loose; })"
  - "τ anti-μ": "all_of(taus.begin(), taus.end(), [](const superslim::Tau& t) { return t.vetoMuon() >= superslim::id::Loose; })"
  - "Jets": "len(jets) >= 2"
  - "Tags": "btags(jets) == 1 or btags(jets, true) == 2"
  - "ΔR(ττ)  > 0.5": "dR(taus[0], taus[1]) > 0.5"
  - "Opposite charge for τ": "taus[0].charge() * taus[1].charge() < 0"

default weights:
  - Generator
  - CSVWeight
  - PUWeight
  - tauIdEff

# List of systematics to run when `-s` is specified
systematics: []

categories:
  - inclusive: ""
