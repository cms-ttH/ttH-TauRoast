#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "ttH/TauRoast/interface/SuperSlim.h"

namespace superslim {
   Vertex::Vertex(const reco::Vertex& v) :
      x_(v.x()),
      y_(v.y()),
      z_(v.z()),
      x_error_(v.xError()),
      y_error_(v.yError()),
      z_error_(v.zError()),
      rho_(v.position().rho()),
      normalized_chi2_(v.normalizedChi2()),
      ndof_(v.ndof())
   {
   }

   PhysObject::PhysObject(const reco::Candidate* c) :
      charge_(c->charge()),
      p_(c->p4()),
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
      PhysObject(&e),
      type_(Lepton::e)
   {
      setGenInfo(e.genParticle());
   }

   Lepton::Lepton(const pat::Muon& m) :
      PhysObject(&m),
      type_(Lepton::mu)
   {
      setGenInfo(m.genParticle());
   }

   Tau::Tau(const pat::Tau& t) :
      PhysObject(&t),
      decay_mode_(t.decayMode()),
      prongs_(t.signalChargedHadrCands().size()),
      leading_track_pt_(-1.0)
   {
      setGenInfo(t.genParticle());

      if (t.leadChargedHadrCand().isNonnull()) {
         leading_track_pt_ = t.leadChargedHadrCand()->pt();
         charge_ = t.leadChargedHadrCand()->charge();
      }

      if (t.tauID("byTightCombinedIsolationDeltaBetaCorr3Hits") > .5)
         isolation_3hits_ = 3;
      else if (t.tauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") > .5)
         isolation_3hits_ = 2;
      else if (t.tauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") > .5)
         isolation_3hits_ = 1;
      else
         isolation_3hits_ = 0;

      if (t.tauID("againstMuonTight3") > .5)
         veto_muon_ = 2;
      else if (t.tauID("againstMuonLoose3") > .5)
         veto_muon_ = 1;
      else
         veto_muon_ = 0;

      if (t.tauID("againstElectronMediumMVA5") > .5)
         veto_electron_ = 3;
      if (t.tauID("againstElectronLooseMVA5") > .5)
         veto_electron_ = 2;
      if (t.tauID("againstElectronVLooseMVA5") > .5)
         veto_electron_ = 1;
      else
         veto_electron_ = 0;
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

   bool
   operator<(const PhysObject& lhs, const PhysObject& rhs)
   {
      return rhs.p4().Pt() < lhs.p4().Pt();
   }
}

ClassImp(superslim::Vertex);
ClassImp(superslim::PhysObject);
ClassImp(superslim::Jet);
ClassImp(superslim::Lepton);
ClassImp(superslim::Tau);
ClassImp(superslim::Combination);
ClassImp(superslim::Event);
