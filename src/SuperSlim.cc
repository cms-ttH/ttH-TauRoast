#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "ttH/TauRoast/interface/SuperSlim.h"

namespace superslim {
   PhysObject::PhysObject(const reco::Candidate* c) :
      p_(c->p4()),
      charge_(c->charge()),
      pdg_id_(0)
   {
   }

   int
   PhysObject::parentId() const
   {
      int id = 0;
      if (parents_.size() == 0)
         return id;
      for (const auto& p: parents_) {
         if (id > 0) {
            id *= -1;
            break;
         }
         id = std::abs(p.pdgId());
      }
      return id;
   }

   int
   PhysObject::grandParentId() const
   {
      int id = 0;
      if (parents_.size() == 0)
         return id;
      for (const auto& p: parents_) {
         if (id > 0) {
            id *= -1;
            break;
         }
         id = std::abs(p.parentId());
      }
      return id;
   }

   void
   PhysObject::setGenInfo(const reco::Candidate* p, int level)
   {
      if (!p)
         return;

      pdg_id_ = p->pdgId();

      if (level <= 0)
         return;

      for (unsigned int i = 0; i < p->numberOfMothers(); ++i) {
         auto mother = p->mother(i);

         while (mother and mother->pdgId() == p->pdgId()) {
            if (mother->numberOfMothers() > 0)
               mother = mother->mother(i);
            else
               break;
         }

         if (!mother)
            return;

         PhysObject o(mother);
         o.setGenInfo(mother, level - 1);
         parents_.push_back(o);
      }
   }

   Jet::Jet(const pat::Jet& j) :
      PhysObject(&j),
      csv_(j.bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags"))
   {
      setGenInfo(j.genParton());
   }

   Lepton::Lepton(const pat::Electron& e) :
      PhysObject(&e)
   {
      setGenInfo(e.genParticle());
   }

   Lepton::Lepton(const pat::Muon& m) :
      PhysObject(&m)
   {
      setGenInfo(m.genParticle());
   }

   Tau::Tau(const pat::Tau& t) :
      PhysObject(&t)
   {
      setGenInfo(t.genParticle());
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
ClassImp(superslim::Event);
