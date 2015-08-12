import sys
import ROOT as r
from DataFormats.FWLite import Events, Handle

r.gROOT.SetBatch()

events = Events('root://eoscms//eos/cms/store/mc/Phys14DR/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/EC51B40A-0F77-E411-AB65-002590A831AA.root')
handle = Handle('std::vector<reco::GenParticle>')
label = ('prunedGenParticles')

efilter = map(int, sys.argv[1:])

for e in events:
    # print dir(e.eventAuxiliary().event())
    if e.eventAuxiliary().event() not in efilter:
        continue

    e.getByLabel(label, handle)

    for p in handle.product():
        if abs(p.pdgId()) == 25:
            print p.pdgId()
            for i in range(p.numberOfDaughters()):
                print '\t', p.daughter(i).pdgId()
