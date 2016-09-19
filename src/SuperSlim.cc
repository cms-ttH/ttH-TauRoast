#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "ttH/TauRoast/interface/SuperSlim.h"

namespace superslim {
   namespace id {
      const std::vector<std::pair<std::string, id::value>> values = {
         {"VTight", VTight},
         {"Tight", Tight},
         {"Medium", Medium},
         {"Loose", Loose},
         {"VLoose", VLoose},
         {"Preselection", Preselected}
      };
   }

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

   const reco::Candidate *
   PhysObject::getFinal(const reco::Candidate * c)
   {
      auto id = c->pdgId();

      const reco::Candidate *radstate = 0;
      for (unsigned int i = 0; i < c->numberOfDaughters(); ++i) {
         if (c->daughter(i) and c->daughter(i)->pdgId() == id) {
            radstate = c->daughter(i);
            break;
         }
      }

      if (radstate)
         return getFinal(radstate);

      return c;
   }

   const reco::GenParticle*
   PhysObject::getMatch(const reco::Candidate& c, const reco::GenParticleCollection& coll)
   {
      static const auto veto = {11, 12, 13, 14, 16};

      typedef std::pair<double, const reco::GenParticle*> Match;
      std::vector<Match> cands;

      for (auto& p: coll) {
         reco::Candidate::LorentzVector p4;
         if (abs(p.pdgId()) == 15) {
            for (unsigned int i = 0; i < p.numberOfDaughters(); ++i) {
               auto d = getFinal(p.daughter(i));
               if (std::find(veto.begin(), veto.end(), abs(d->pdgId())) != veto.end())
                  continue;
               p4 += d->p4();
            }
         } else {
            p4 = p.p4();
         }
         auto dR = deltaR(p4, c.p4());
         if (dR > 0.2)
            continue;

         if (abs(p.pdgId()) == 11 and p.statusFlags().isPrompt())
            match_ = 1;
         else if (abs(p.pdgId()) == 13 and p.isPromptFinalState())
            match_ = 2;
         else if (abs(p.pdgId()) == 11 and p.statusFlags().isDirectPromptTauDecayProduct())
            match_ = 3;
         else if (abs(p.pdgId()) == 13 and p.isDirectPromptTauDecayProductFinalState())
            match_ = 4;
         else if (abs(p.pdgId()) == 15)
            match_ = 5;
         else
            continue;

         cands.push_back(std::make_pair(dR, &p));
      }
      auto min = std::min_element(cands.begin(), cands.end(), [](const Match& a, const Match& b) { return a.first < b.first; });
      if (min != cands.end() and min->first < 0.5)
         return min->second;
      return 0;
   }

   template<>
   PhysObject::PhysObject(const reco::GenParticle& c) :
         charge_(c.charge()),
         dxy_(-9999.),
         dz_(-9999.),
         p_(c.p4()),
         match_(6),
         pdg_id_(c.pdgId()),
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

      gen_p_ = p->p4();
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

         PhysObject o(*mother);
         o.setGenInfo(mother, level - 1);
         parents_.push_back(o);
      }
   }

   Jet::Jet(const pat::Jet& j, const reco::GenParticleCollection& particles) :
      PhysObject(j),
      csv_(j.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags")),
      flavor_(j.hadronFlavour())
   {
      getMatch(j, particles);
      setGenInfo(j.genParton());
   }

   Lepton::Lepton(const pat::Electron& e, const reco::Vertex& pv, const reco::BeamSpot& bs, const reco::GenParticleCollection& particles) :
      PhysObject(e),
      type_(Lepton::e),
      charge_check_(e.isGsfCtfScPixChargeConsistent()),
      impact_parameter_(e.dB(pat::Electron::PV3D)),
      impact_parameter_error_(e.edB(pat::Electron::PV3D)),
      seg_compat_(-666.)
   {
      getMatch(e, particles);
      setGenInfo(e.genParticle());

      fakeable_ = getID("Fakeable", e, false, true);
      mva_ = getID("MVABased", e, false, true);
      lj_ = getID("LJ", e, true);

      iso_rel_ = e.userFloat("miniIso");
      iso_charged_ = e.userFloat("miniAbsIsoCharged");
      iso_neutral_ = e.userFloat("miniAbsIsoNeutralcorr");

      jet_nconst_ = e.userFloat("nearestJetNDauCharged");
      jet_pt_rel_ = e.userFloat("nearestJetPtRel");
      jet_pt_ratio_ = e.userFloat("nearestJetPtRatio");
      jet_csv_ = e.userFloat("nearestJetCsv");

      sip3d_ = e.userFloat("sip3D");
      lep_mva_ = e.userFloat("leptonMVA");

      consistent_charge_ = e.isGsfCtfScPixChargeConsistent();
      conversion_veto_ = e.passConversionVeto();
      missing_hits_ = e.userFloat("numMissingHits");
      non_trig_id_ = e.userFloat("eleMvaId");
   }

   Lepton::Lepton(const pat::Muon& m, const reco::Vertex& pv, const reco::BeamSpot& bs, const reco::GenParticleCollection& particles) :
      PhysObject(m),
      type_(Lepton::mu),
      charge_check_(false),
      impact_parameter_(m.dB(pat::Muon::PV3D)),
      impact_parameter_error_(m.edB(pat::Muon::PV3D)),
      seg_compat_(m.segmentCompatibility()),
      relative_pt_error_(-666.)
   {
      getMatch(m, particles);
      setGenInfo(m.genParticle());

      if (m.innerTrack().isAvailable()) {
         charge_check_ = m.innerTrack()->ptError() / m.innerTrack()->pt() < .2;
      }

      if (m.bestTrack()) {
         relative_pt_error_ = m.bestTrack()->ptError() / m.bestTrack()->pt();
      }

      fakeable_ = getID("Fakeable", m, false, true);
      mva_ = getID("MVABased", m, false, true);
      lj_ = getID("LJ", m, true);

      iso_rel_ = m.userFloat("miniIso");
      iso_charged_ = m.userFloat("miniAbsIsoCharged");
      iso_neutral_ = m.userFloat("miniAbsIsoNeutralcorr");

      jet_nconst_ = m.userFloat("nearestJetNDauCharged");
      jet_pt_rel_ = m.userFloat("nearestJetPtRel");
      jet_pt_ratio_ = m.userFloat("nearestJetPtRatio");
      jet_csv_ = m.userFloat("nearestJetCsv");

      sip3d_ = m.userFloat("sip3D");
      lep_mva_ = m.userFloat("leptonMVA");
      bool global = (m.isGlobalMuon() && m.userFloat("normalizedChiSq") < 3 && m.userFloat("localChiSq") < 12 && m.userFloat("trackKink") < 20);
      medium_ = (m.userFloat("validFraction") >= 0.8 && m.segmentCompatibility() >= (global ? 0.303 : 0.451));
   }

   template<typename T>
   id::value Lepton::getID(const std::string& suffix, const T& t, bool range, bool preselected) const {
      if (range) {
         for (const auto& _id: superslim::id::values) {
            if (_id.first == "Preselection" and not preselected)
               continue;
            std::string name = "id" + _id.first + (_id.first == "Preselection" ? "" : suffix);
            if (t.hasUserFloat(name) and t.userFloat(name) > .5)
               return _id.second;
         }
      } else {
         std::string name = "id" + suffix;
         if (t.hasUserFloat(name) and t.userFloat(name) > .5)
            return superslim::id::Loose;
         name = "idPreselection";
         if (t.hasUserFloat(name) and t.userFloat(name) > .5)
            return superslim::id::Preselected;
      }
      return superslim::id::None;
   };

   bool Lepton::selected(Lepton::id id_, superslim::id::value min) const {
      switch (id_) {
         case Lepton::All:
            return mva_ >= min or lj_ >= min;
            break;
         case Lepton::Fakeable:
            return fakeable_ >= min;
            break;
         case Lepton::MVA:
            return mva_ >= min;
            break;
         case Lepton::LJ:
            return lj_ >= min;
            break;
      };
      return false;
   };

   Tau::Tau(const pat::Tau& t, const reco::Vertex& pv, const reco::GenParticleCollection& particles, const reco::GenJetCollection& jets) :
      PhysObject(t),
      decay_mode_(t.decayMode()),
      prongs_(t.signalChargedHadrCands().size()),
      selected_(t.userFloat("idNonIsolated")),
      leading_track_pt_(-1.0)
   {
      auto match = getMatch(t, particles);
      if (match) {
         for (unsigned int i = 0; i < match->numberOfDaughters(); ++i) {
            auto d = getFinal(match->daughter(i));
            // Ignore neutrinos
            if (abs(d->pdgId()) % 2 == 0 and abs(d->pdgId()) > 10 and abs(d->pdgId()) < 20)
               continue;
            gen_vis_p_ += d->p4();
         }
         setGenInfo(match);
      } else {
         std::vector<std::pair<float, const reco::GenJet*>> js;
         for (const auto& j: jets)
            js.push_back(std::make_pair(deltaR(p4(), j.p4()), &j));
         std::sort(std::begin(js), std::end(js), [](const auto& a, const auto& b) { return a.first < b.first; });

         if (js.size() > 0 and js[0].first < .4)
            setGenJetInfo(*(js[0].second));
      }

      // for (auto& pair: t.tauIDs()) {
      //    std::cout << pair.first << std::endl;
      // }
      // std::cout << "=== THE END ===" << std::endl;

      if (t.leadChargedHadrCand().isNonnull()) {
         leading_track_pt_ = t.leadChargedHadrCand()->pt();
         charge_ = t.leadChargedHadrCand()->charge();
      }

      raw_isolation_3hits03_ = 0.; // NOT PRESENT: t.tauID("byCombinedIsolationDeltaBetaCorrRaw3HitsdR03");
      raw_isolation_3hits05_ = t.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");

      isolation_3hits03_ = getID("by", "CombinedIsolationDeltaBetaCorr3HitsdR03", t);
      isolation_3hits05_ = getID("by", "CombinedIsolationDeltaBetaCorr3Hits", t);
      isolation_mva03_ = getID("by", "IsolationMVArun2v1DBdR03oldDMwLT", t);
      isolation_mva05_ = getID("by", "IsolationMVArun2v1DBoldDMwLT", t);

      veto_electron_ = getID("againstElectron", "MVA6", t);
      veto_muon_ = getID("againstMuon", "3", t);
   }

   void
   Tau::setGenJetInfo(const reco::GenJet& j)
   {
      gen_jet_p_ = j.p4();
      gen_jet_constituents_ = j.numberOfDaughters();

      for (unsigned i = 0; i < j.numberOfDaughters(); ++i) {
         const reco::Candidate* cand = j.daughter(i);
         if (cand->charge() != 0) {
            gen_jet_charged_p_ += cand->p4();
            ++gen_jet_charged_constituents_;
         }
      }
   }

   superslim::id::value Tau::getID(const std::string& prefix, const std::string& suffix, const pat::Tau& t) const {
      for (const auto& _id: superslim::id::values) {
         std::string name = prefix + _id.first + suffix;
         if (t.isTauIDAvailable(name) and t.tauID(name) > .5)
            return _id.second;
      }
      return superslim::id::None;
   };

   bool Tau::selected(Tau::id id_, superslim::id::value min) const {
      if (not selected_)
         return false;

      switch (id_) {
         case Tau::All:
            return isolation_3hits03_ >= min or isolation_3hits05_ >= min or isolation_mva03_ >= min or isolation_mva05_ >= min;
            break;
         case Tau::Iso3Hits05:
            return isolation_3hits05_ >= min;
            break;
         case Tau::Iso3Hits03:
            return isolation_3hits03_ >= min;
            break;
         case Tau::IsoMVA05:
            return isolation_mva05_ >= min;
            break;
         case Tau::IsoMVA03:
            return isolation_mva03_ >= min;
            break;
      };
      return false;
   };


   Combination::Combination(
         const std::vector<Tau>& taus,
         superslim::Tau::id tau_id,
         const std::vector<Lepton>& leptons,
         superslim::Lepton::id lepton_id,
         const std::map<std::string, std::vector<Jet>>& jets,
         const std::map<std::string, LorentzVector>& met) :
      jets_(jets),
      leptons_(leptons),
      taus_(taus),
      met_(met),
      tau_id_(tau_id),
      lepton_id_(lepton_id)
   {
      for (const auto& l: leptons_) {
         if (l.electron())
            electrons_.push_back(l);
         else
            muons_.push_back(l);
      }
   }

   bool
   operator<(const PhysObject& lhs, const PhysObject& rhs)
   {
      return rhs.p4().Pt() < lhs.p4().Pt();
   }

   Trigger::Trigger(const edm::TriggerResults& trig, const edm::TriggerNames& names)
   {
      for (auto& name: names.triggerNames()) {
         auto idx = names.triggerIndex(name);
         if (idx < trig.size() and trig.accept(idx))
            accepted_.push_back(name);
      }
      std::sort(accepted_.begin(), accepted_.end());
   }

   Event::Event(const std::vector<superslim::Combination>& cs,
         const std::vector<superslim::Tau>& taus,
         const std::vector<superslim::Lepton>& leps,
         long run, long lumi, long event,
         int npv, int ntv,
         const std::vector<superslim::Vertex>& pv,
         int category,
         const superslim::Trigger& trigger,
         const reco::GenParticleCollection& genparticles) :
      combos_(cs),
      taus_(taus), leptons_(leps),
      run_(run), lumi_(lumi), event_(event),
      npv_(npv), ntv_(ntv),
      hfcat_(category),
      hdecay_(-1),
      pv_(pv),
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
