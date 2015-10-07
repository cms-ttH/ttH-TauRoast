#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

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
      pdg_id_(c->pdgId()),
      gen_pdg_id_(0)
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
   PhysObject::setGenInfo(const reco::GenParticle* p, int level)
   {
      if (!p)
         return;

      gen_pdg_id_ = p->pdgId();

      if (level <= 0)
         return;

      for (unsigned int i = 0; i < p->numberOfMothers(); ++i) {
         auto mother = dynamic_cast<const reco::GenParticle*>(p->mother(i));

         while (mother and mother->pdgId() == p->pdgId()) {
            if (mother->numberOfMothers() > 0) {
               mother = dynamic_cast<const reco::GenParticle*>(mother->mother(0));
            } else {
               mother = 0;
               break;
            }
         }

         if (!mother)
            continue;

         PhysObject o(mother);
         o.setGenInfo(mother, level - 1);
         parents_.push_back(o);
      }
   }

   Jet::Jet(const pat::Jet& j) :
      PhysObject(&j),
      csv_(j.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"))
   {
      setGenInfo(j.genParton());
   }

   Lepton::Lepton(const pat::Electron& e, float rel_iso, const reco::Vertex& pv, const reco::BeamSpot& bs) :
      PhysObject(&e),
      type_(Lepton::e),
      charge_check_(e.isGsfCtfScPixChargeConsistent()),
      impact_parameter_(e.dB(pat::Electron::PV3D)),
      impact_parameter_error_(e.edB(pat::Electron::PV3D)),
      rel_iso_(rel_iso)
   {
      setGenInfo(e.genParticle());

      if (e.gsfTrack().isAvailable()) {
         corrected_d0_ = e.gsfTrack()->dxy(bs.position());
         corrected_dz_ = e.gsfTrack()->dz(pv.position());
      }

      cut_ = (e.userFloat("idTightCut") > .5) + (e.userFloat("idLooseCut") > .5);
      mva_ = (e.userFloat("idTightMVA") > .5) + (e.userFloat("idLooseMVA") > .5);
   }

   Lepton::Lepton(const pat::Muon& m, float rel_iso, const reco::Vertex& pv, const reco::BeamSpot& bs) :
      PhysObject(&m),
      type_(Lepton::mu),
      charge_check_(false),
      impact_parameter_(m.dB(pat::Muon::PV3D)),
      impact_parameter_error_(m.edB(pat::Muon::PV3D)),
      rel_iso_(rel_iso)
   {
      setGenInfo(m.genParticle());

      if (m.innerTrack().isAvailable()) {
         charge_check_ = m.innerTrack()->ptError() / m.innerTrack()->pt() < .2;
         corrected_d0_ = m.innerTrack()->dxy(bs.position());
         corrected_dz_ = m.innerTrack()->dz(pv.position());
      }

      cut_ = (m.userFloat("idTightCut") > .5) + (m.userFloat("idLooseCut") > .5);
      mva_ = (m.userFloat("idTightMVA") > .5) + (m.userFloat("idLooseMVA") > .5);
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

   Trigger::Trigger(const edm::TriggerResults& trig, const HLTConfigProvider& hlt) :
      single_e_(false),
      single_mu_(false),
      double_e_(false),
      double_mu_(false),
      mixed_(false)
   {
      single_e_ = fired(trig, hlt, {"HLT_Ele27_eta2p1_WP85_Gsf_HT200_v1"});
      single_mu_ = fired(trig, hlt, {"HLT_IsoMu24_eta2p1_v1"});
   }

   bool
   Trigger::fired(const edm::TriggerResults& res, const HLTConfigProvider& hlt, const std::vector<std::string>& paths)
   {
      for (const auto& path: paths) {
         auto idx = hlt.triggerIndex(path);
         if (idx < res.size() and res.accept(idx))
            return true;
      }
      return false;
   }

   Event::Event(const std::vector<superslim::Combination>& cs,
         long run, long lumi, long event,
         int npv, int ntv,
         const superslim::Trigger& trigger,
         const LorentzVector& met,
         const std::vector<superslim::Vertex>& pv,
         const reco::GenParticleCollection& genparticles) :
      combos_(cs),
      run_(run), lumi_(lumi), event_(event),
      npv_(npv), ntv_(ntv),
      hdecay_(-1),
      met_(met), pv_(pv),
      trigger_(trigger)
   {
      for (const auto& p: genparticles) {
         if (abs(p.pdgId()) == 25) {
            bool use = true;
            int id = -1;
            for (unsigned int i = 0; i < p.numberOfDaughters() and use; ++i) {
               int did = abs(p.daughter(i)->pdgId());
               if (did == 25) {
                  use = false;
               } else if (id > 0 and id != did) {
                  if (did > id) {
                     id = (id * 1000) + did;
                  } else {
                     id = (did * 1000) + id;
                  }
               } else {
                  id = did;
               }
            }

            if (id > 0 and use) {
               hdecay_ = id;
               break;
            }
         }
      }
   }
}

ClassImp(superslim::Vertex);
ClassImp(superslim::PhysObject);
ClassImp(superslim::Jet);
ClassImp(superslim::Lepton);
ClassImp(superslim::Tau);
ClassImp(superslim::Combination);
ClassImp(superslim::Trigger);
ClassImp(superslim::Event);
