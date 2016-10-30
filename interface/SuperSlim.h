#ifndef __SuperSlim_h
#define __SuperSlim_h

#include <map>
#include <vector>

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "Math/LorentzVector.h"
#include "TH1F.h"

namespace edm {
   class TriggerNames;
   class TriggerResults;
}

namespace pat {
   class Electron;
   class Jet;
   class Muon;
   class Tau;
}

namespace reco {
   class BeamSpot;
   class GenJet;
   typedef std::vector<GenJet> GenJetCollection;
   class GenParticle;
   typedef std::vector<GenParticle> GenParticleCollection;
   class Vertex;
}

namespace superslim {
   namespace id {
      enum value {
         None = 0,
         Preselected = 1,
         VLoose = 2,
         Loose = 3,
         Medium = 4,
         Tight = 5,
         VTight = 6
      };

      extern const std::vector<std::pair<std::string, id::value>> values;
   }

   typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;

   class CutHistogram {
      public:
         CutHistogram() : h_("cuts", "Cut counts", 64, -0.5, 63.5) {};

         TH1* operator->() { return &h_; };
         TH1& operator*() { return h_; };
         const TH1* operator->() const { return &h_; };
         const TH1& operator*() const { return h_; };
         operator const TH1*() const { return &h_; };

         const TH1* product() const { return &h_; };

         bool mergeProduct(const CutHistogram& other) { h_.Add(other); return true; };
      private:
         TH1F h_;
   };

   class Vertex {
      public:
         Vertex() {};
         Vertex(const reco::Vertex&);
         virtual ~Vertex() {};

         float x() const { return x_; };
         float y() const { return y_; };
         float z() const { return z_; };

         float xError() const { return x_error_; };
         float yError() const { return y_error_; };
         float zError() const { return z_error_; };

         float rho() const { return rho_; };
         float normalizedChi2() const { return normalized_chi2_; };
         int ndof() const { return ndof_; };

      private:
         float x_;
         float y_;
         float z_;

         float x_error_;
         float y_error_;
         float z_error_;

         float rho_;
         float normalized_chi2_;
         int ndof_;
   };

   class GenObject {
      public:
         GenObject() : p_() {};
         virtual ~GenObject() {};

         template<typename T>
         GenObject(const T& c) :
            p_(c.p4()),
            charge_(c.charge()),
            pdg_id_(c.pdgId()) {};

         const float pt() const { return p_.pt(); };
         const float eta() const { return p_.eta(); };
         const float phi() const { return p_.phi(); };
         const LorentzVector& p4() const { return p_; };

         int charge() const { return charge_; };
         int pdgId() const { return pdg_id_; };
      protected:
         LorentzVector p_;
         int charge_;
         int pdg_id_;
   };

   class GenJet : public GenObject {
      public:
         GenJet() : GenObject(), closest_particle_(0), closest_jet_(0) {};
         GenJet(const GenJet& other);
         virtual ~GenJet();

         template<typename T>
         GenJet(const T& j) :
            GenObject(j),
            constituents_(j.numberOfDaughters()),
            closest_particle_(0),
            closest_jet_(0)
         {
            for (unsigned i = 0; i < j.numberOfDaughters(); ++i) {
               auto cand = j.daughterPtr(i);
               if (cand.isNonnull()) {
                  auto dR = deltaR(j.p4(), cand->p4());
                  if (cand->charge() != 0) {
                     charged_p_ += cand->p4();
                     ++charged_constituents_;
                     if (dR < .12) {
                        ++signal_charged_constituents_;
                        signal_charged_p_ += cand->p4();
                     } else if (dR < .3) {
                        ++iso_charged_constituents_;
                        iso_charged_p_ += cand->p4();
                     }
                  }

                  if (dR < .12) {
                     ++signal_constituents_;
                     signal_p_ += cand->p4();
                  } else if (dR < .3) {
                     ++iso_constituents_;
                     iso_p_ += cand->p4();
                  }
               }
            }
         };

         const LorentzVector& chargedP4() const { return charged_p_; };
         int constituents() const { return constituents_; };
         int chargedConstituents() const { return charged_constituents_; };

         const LorentzVector& signalP4() const { return signal_p_; };
         int signalConstituents() const { return signal_constituents_; };
         const LorentzVector& signalChargedP4() const { return signal_charged_p_; };
         int signalChargedConstituents() const { return signal_charged_constituents_; };

         const LorentzVector& isoP4() const { return iso_p_; };
         int isoConstituents() const { return iso_constituents_; };
         const LorentzVector& isoChargedP4() const { return iso_charged_p_; };
         int isoChargedConstituents() const { return iso_charged_constituents_; };

         const GenJet* closestGenJet() const { return closest_jet_; };
         void findClosestGenJet(const reco::GenJetCollection& jets);

         const GenObject* closestGenParticle() const { return closest_particle_; };
         void findClosestGenParticle(const reco::GenParticleCollection& particles);

      private:
         LorentzVector charged_p_;
         int constituents_ = 0;
         int charged_constituents_ = 0;

         int iso_constituents_ = 0;
         int iso_charged_constituents_ = 0;
         int signal_constituents_ = 0;
         int signal_charged_constituents_ = 0;

         LorentzVector iso_p_;
         LorentzVector iso_charged_p_;
         LorentzVector signal_p_;
         LorentzVector signal_charged_p_;

         GenObject* closest_particle_ = 0;
         GenJet* closest_jet_ = 0;
   };

   class PhysObject : public GenObject {
      public:
         PhysObject() : GenObject() {};
         template<typename T>
         PhysObject(const T& c, int rank=-666) :
            GenObject(c),
            dxy_(-9999.),
            dz_(-9999.),
            match_(6),
            gen_pdg_id_(0),
            rank_(rank)
         {
            if (c.hasUserFloat("dxy") and c.hasUserFloat("dz")) {
               dxy_ = c.userFloat("dxy");
               dz_ = c.userFloat("dz");
            }
         };
         virtual ~PhysObject() {};

         const LorentzVector& genP4() const { return gen_p_; };
         const std::vector<superslim::PhysObject>& parents() const { return parents_; };
         int parentId() const;
         int grandParentId() const;
         int rank() const { return rank_; };
         void rank(int r) { rank_ = r; };
         float dxy() const { return dxy_; };
         float dz() const { return dz_; };
         float dR(const superslim::PhysObject& o) const { return reco::deltaR(*this, o); };
         int genPdgId() const { return gen_pdg_id_; };
         // As per https://twiki.cern.ch/twiki/bin/viewauth/CMS/HiggsToTauTauWorking2016#MC_Matching
         // 1: prompt e
         // 2: prompt μ
         // 3: τ → e
         // 4: τ → μ
         // 5: τ → τ had
         // 6: jet/pu fake
         int match() const { return match_; };

      protected:
         const reco::Candidate* getFinal(const reco::Candidate* c);
         const reco::Candidate* getMother(const reco::Candidate* c);
         const reco::GenParticle* getMatch(const reco::Candidate& c, const reco::GenParticleCollection& coll);
         void setGenInfo(const reco::GenParticle* p, int level=2);

         float dxy_;
         float dz_;

      private:
         LorentzVector gen_p_;
         std::vector<superslim::PhysObject> parents_;

         int match_;
         int gen_pdg_id_;
         int rank_;
   };

   class Jet : public PhysObject {
      public:
         Jet() {};
         Jet(const pat::Jet& j, const reco::GenParticleCollection& particles, int rank=-666);
         virtual ~Jet() {};

         float csv() const { return csv_; };
         int flavor() const { return flavor_; };
      private:
         float csv_;
         int flavor_;
   };

   class Lepton : public PhysObject {
      public:
         enum id { All, Fakeable, MVA, LJ };

         Lepton() {};
         Lepton(const pat::Electron& e, const reco::Vertex& pv, const reco::BeamSpot& bs, const reco::GenParticleCollection& particles, int rank);
         Lepton(const pat::Muon& m, const reco::Vertex& pv, const reco::BeamSpot& bs, const reco::GenParticleCollection& particles, int rank);
         virtual ~Lepton() {};

         bool electron() const { return type_ == e; };
         bool muon() const { return type_ == mu; };

         bool chargeConsistent() const { return charge_check_; };

         float impactParameter() const { return impact_parameter_; };
         float impactParameterError() const { return impact_parameter_error_; };
         float relativeIsolation() const { return iso_rel_; };
         float chargedIsolation() const { return iso_charged_; };
         float neutralIsolation() const { return iso_neutral_; };

         int nearestJetConstituents() const { return jet_nconst_; };
         float nearestJetRelativePt() const { return jet_pt_rel_; };
         float nearestJetRatioPt() const { return jet_pt_ratio_; };
         float nearestJetCSV() const { return jet_csv_; };

         float sip3D() const { return sip3d_; };
         float mvaRaw() const { return lep_mva_; };

         float segmentCompatibility() const { return seg_compat_; };
         float relativePtError() const { return relative_pt_error_; };

         bool consistentCharge() const { return consistent_charge_; };
         bool passesConversionVeto() const { return conversion_veto_; };
         float missingHits() const { return missing_hits_; };
         float nonTriggeringId() const { return non_trig_id_; };

         int fakeable() const { return fakeable_; };
         int mva() const { return mva_; };
         int lj() const { return lj_; };

         bool selected(Lepton::id id_=Lepton::All, superslim::id::value min=superslim::id::Preselected) const;
         bool loose(Lepton::id id_=Lepton::All) const { return selected(id_, superslim::id::Loose); };
         bool preselected(Lepton::id id_=Lepton::All) const { return selected(id_, superslim::id::Preselected); };

      private:
         template<typename T>
         superslim::id::value getID(const std::string&, const T&, bool=false, bool=false) const;

         enum kind { e, mu } type_;

         bool charge_check_;

         superslim::id::value fakeable_;
         superslim::id::value mva_;
         superslim::id::value lj_;

         float impact_parameter_;
         float impact_parameter_error_;

         float iso_rel_;
         float iso_charged_;
         float iso_neutral_;

         int jet_nconst_;
         float jet_pt_rel_;
         float jet_pt_ratio_;
         float jet_csv_;

         float sip3d_;
         float lep_mva_;

         // electron specific
         bool consistent_charge_;
         bool conversion_veto_;
         float missing_hits_;
         float non_trig_id_;

         // muon specific
         float seg_compat_;
         float relative_pt_error_;
         bool medium_;
   };

   class Tau : public PhysObject {
      public:
         enum id { All, Iso3Hits05, Iso3Hits03, IsoMVA05, IsoMVA03 };

         Tau() {};
         Tau(const pat::Tau& t, const reco::Vertex& pv, const reco::GenParticleCollection& particles, const reco::GenJetCollection& jets, int rank);
         virtual ~Tau() {};

         bool selected(Tau::id id_=Tau::All, superslim::id::value min=superslim::id::Preselected) const;
         bool loose(Tau::id id_=Tau::All) const { return selected(id_, superslim::id::Loose); };

         int decayMode() const { return decay_mode_; };
         int isolation3Hits03() const { return isolation_3hits03_; };
         int isolation3Hits05() const { return isolation_3hits05_; };
         int isolationMVA03() const { return isolation_mva03_; };
         int isolationMVA05() const { return isolation_mva05_; };
         int prongs() const { return prongs_; };
         int vetoElectron() const { return veto_electron_; };
         int vetoMuon() const { return veto_muon_; };

         float leadingTrackPt() const { return leading_track_pt_; };
         float rawIsolation3Hits03() const { return raw_isolation_3hits03_; };
         float rawIsolation3Hits05() const { return raw_isolation_3hits05_; };
         float rawIsolationMVA03() const { return raw_isolation_mva03_; };
         float rawIsolationMVA05() const { return raw_isolation_mva05_; };

         const superslim::GenJet& genJet() const { return gen_jet_; };

         LorentzVector genVisibleP4() const { return gen_vis_p_; };
         LorentzVector genJetP4() const { return gen_jet_.p4(); };
         LorentzVector genJetChargedP4() const { return gen_jet_.chargedP4(); };

         int genJetConstituents() const { return gen_jet_.constituents(); };
         int genJetChargedConstituents() const { return gen_jet_.chargedConstituents(); };
      private:
         superslim::id::value getID(const std::string&, const std::string&, const pat::Tau&) const;

         int decay_mode_;
         int prongs_;

         bool selected_;
         superslim::id::value isolation_3hits03_;
         superslim::id::value isolation_3hits05_;
         superslim::id::value isolation_mva03_;
         superslim::id::value isolation_mva05_;
         superslim::id::value veto_electron_;
         superslim::id::value veto_muon_;

         float leading_track_pt_;
         float raw_isolation_3hits03_;
         float raw_isolation_3hits05_;
         float raw_isolation_mva03_;
         float raw_isolation_mva05_;

         LorentzVector gen_vis_p_;
         GenJet gen_jet_;
   };

   class Combination {
      public:
         Combination() {};
         Combination(
               const std::vector<superslim::Tau>&,
               superslim::Tau::id,
               const std::vector<superslim::Lepton>&,
               superslim::Lepton::id,
               const std::map<std::string, std::vector<superslim::Jet>>&,
               const std::map<std::string, LorentzVector>& met);
         virtual ~Combination() {};

         const std::vector<superslim::Jet>& jets(const std::string& s="NA") const { return jets_.find(s)->second; };
         const std::vector<superslim::Lepton>& leptons() const { return leptons_; };
         const std::vector<superslim::Lepton>& electrons() const { return electrons_; };
         const std::vector<superslim::Lepton>& muons() const { return muons_; };
         const std::vector<superslim::Tau>& taus() const { return taus_; };
         const LorentzVector& met(const std::string& s="NA") const { return met_.find(s)->second; };
         const std::map<std::string, float>& weights() const { return weights_; };

         superslim::Tau::id tauId() const { return tau_id_; };
         superslim::Lepton::id leptonId() const { return lepton_id_; };

         void setWeight(const std::string& s, float f) { weights_[s] = f; };
      private:
         std::map<std::string, std::vector<superslim::Jet>> jets_;
         std::vector<superslim::Lepton> leptons_;
         std::vector<superslim::Lepton> electrons_;
         std::vector<superslim::Lepton> muons_;
         std::vector<superslim::Tau> taus_;
         std::map<std::string, LorentzVector> met_;

         superslim::Tau::id tau_id_;
         superslim::Lepton::id lepton_id_;

         std::map<std::string, float> weights_;
   };

   class Trigger {
      public:
         Trigger() {};
         Trigger(const edm::TriggerResults&, const edm::TriggerNames&);
         virtual ~Trigger() {};

         bool accepted(const std::string& s) const;

      private:
         std::vector<std::string> accepted_;
   };

   class Event {
      public:
         Event() {};
         Event(const std::vector<superslim::Combination>& cs,
               const std::vector<superslim::Tau>&,
               const std::vector<superslim::Lepton>&,
               long run, long lumi, long event,
               int npv, int ntv,
               const std::vector<superslim::Vertex>& pv,
               int catergory,
               const superslim::Trigger& trigger,
               const reco::GenParticleCollection& genparticles);
         virtual ~Event() {};

         const std::vector<superslim::Combination>& combos() const { return combos_; };
         const std::vector<superslim::Lepton>& leptons() const { return leptons_; };
         const std::vector<superslim::Tau>& taus() const { return taus_; };
         const std::map<std::string, float>& weights() const { return weights_; };

         const std::vector<superslim::GenObject>& genParticles() const { return gen_particles_; };
         const std::vector<superslim::GenJet>& genJets() const { return gen_jets_; };

         long run() const { return run_; };
         long lumi() const { return lumi_; };
         long event() const { return event_; };

         // primary vertices
         int npv() const { return npv_; };
         // true vertices
         int ntv() const { return ntv_; };

         int hfCategory() const { return hfcat_; };
         int higgsDecay() const { return hdecay_; };

         const std::vector<superslim::Vertex>& pv() const { return pv_; };
         const Trigger& trigger() const { return trigger_; };

         void setWeight(const std::string& s, float f) { weights_[s] = f; };

         void setGenParticles(const std::vector<GenObject>& v) { gen_particles_ = v; };
         void setGenJets(const std::vector<GenJet>& v) { gen_jets_ = v; };
      private:
         std::vector<superslim::Combination> combos_;
         std::vector<superslim::Tau> taus_;
         std::vector<superslim::Lepton> leptons_;

         std::vector<superslim::GenObject> gen_particles_;
         std::vector<superslim::GenJet> gen_jets_;

         std::map<std::string, float> weights_;

         long run_;
         long lumi_;
         long event_;

         int npv_;
         int ntv_;

         int hfcat_;
         int hdecay_;

         std::vector<superslim::Vertex> pv_;
         superslim::Trigger trigger_;
   };

   bool operator<(const PhysObject& lhs, const PhysObject& rhs);

   template<> PhysObject::PhysObject(const reco::GenParticle&, int rank);
}

#endif
