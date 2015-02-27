#include "DataFormats/Candidate/interface/Candidate.h"

#include "ttH/TauRoast/interface/SuperSlim.h"

namespace superslim {
   PhysObject::PhysObject(const reco::Candidate& c, int level) :
      p_(c.p4()),
      pdg_id_(c.pdgId())
   {
   }

   Combination::Combination(
         const std::vector<Tau>& taus,
         const std::vector<Lepton>& leptons,
         const std::vector<Jet>& jets) :
      jets_(jets),
      leptons_(leptons),
      taus_(taus)
   {
   }
}

ClassImp(superslim::PhysObject);
ClassImp(superslim::Jet);
ClassImp(superslim::Lepton);
ClassImp(superslim::Tau);
ClassImp(superslim::Combination);
