#include <map>
#include <vector>

#include "Math/LorentzVector.h"
#include "TObject.h"

#ifndef __CINT__
class HLTConfigProvider;

namespace edm {
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
   class Candidate;
   class GenParticle;
   typedef std::vector<GenParticle> GenParticleCollection;
   class Vertex;
}
#endif

namespace superslim {
   typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;

   class Vertex {
      public:
         Vertex() {};
#ifndef __CINT__
         Vertex(const reco::Vertex&);
#endif
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

         ClassDef(Vertex, 2);
   };

   class PhysObject {
      public:
         PhysObject() : p_() {};
#ifndef __CINT__
         PhysObject(const reco::Candidate*);
#endif
         virtual ~PhysObject() {};

         const LorentzVector&
            p4() const { return p_; };
         const std::vector<superslim::PhysObject>&
            parents() const { return parents_; };
         int
            parentId() const;
         int
            grandParentId() const;
         int
            charge() const { return charge_; };
         int
            pdgId() const { return pdg_id_; };
         int
            genPdgId() const { return gen_pdg_id_; };
#ifndef __CINT__
         void
            setGenInfo(const reco::GenParticle* p, int level=2);
#endif

      protected:
         int charge_;

      private:
         LorentzVector p_;
         std::vector<superslim::PhysObject> parents_;

         int pdg_id_;
         int gen_pdg_id_;

         ClassDef(PhysObject, 1);
   };

   class Jet : public PhysObject {
      public:
         Jet() {};
#ifndef __CINT__
         Jet(const pat::Jet& j);
#endif
         virtual ~Jet() {};

         float csv() const { return csv_; };
      private:
         float csv_;
         ClassDef(Jet, 1);
   };

   class Lepton : public PhysObject {
      public:
         Lepton() {};
#ifndef __CINT__
         Lepton(const pat::Electron& e, float rel_iso, const reco::Vertex& pv, const reco::BeamSpot& bs);
         Lepton(const pat::Muon& m, float rel_iso, const reco::Vertex& pv, const reco::BeamSpot& bs);
#endif
         virtual ~Lepton() {};

         bool electron() const { return type_ == e; };
         bool muon() const { return type_ == mu; };

         bool chargeConsistent() const { return charge_check_; };

         float correctedD0() const { return corrected_d0_; };
         float correctedDZ() const { return corrected_dz_; };
         float impactParameter() const { return impact_parameter_; };
         float impactParameterError() const { return impact_parameter_error_; };
         float relativeIsolation() const { return rel_iso_; };
         int cut() const { return cut_; };
         int mva() const { return mva_; };
      private:
         enum kind { e, mu } type_;

         bool charge_check_;

         int cut_;
         int mva_;

         float corrected_d0_ = 0.;
         float corrected_dz_ = 0.;
         float impact_parameter_;
         float impact_parameter_error_;
         float rel_iso_;

         ClassDef(Lepton, 3);
   };

   class Tau : public PhysObject {
      public:
         Tau() {};
#ifndef __CINT__
         Tau(const pat::Tau& t);
#endif
         virtual ~Tau() {};

         int decayMode() const { return decay_mode_; };
         int isolation3Hits() const { return isolation_3hits_; };
         int prongs() const { return prongs_; };
         int vetoElectron() const { return veto_electron_; };
         int vetoMuon() const { return veto_muon_; };

         float leadingTrackPt() const { return leading_track_pt_; };
      private:
         int decay_mode_;
         int isolation_3hits_;
         int prongs_;
         int veto_electron_;
         int veto_muon_;

         float leading_track_pt_;

         ClassDef(Tau, 1);
   };

   class Combination {
      public:
         Combination() {};
         Combination(
               const std::vector<superslim::Tau>&,
               const std::vector<superslim::Lepton>&,
               const std::vector<superslim::Jet>&);
         virtual ~Combination() {};

         const std::vector<superslim::Jet>& jets() const { return jets_; };
         const std::vector<superslim::Lepton>& leptons() const { return leptons_; };
         const std::vector<superslim::Tau>& taus() const { return taus_; };
         const std::map<std::string, float>& weights() const { return weights_; };

         void setWeight(const std::string& s, float f) { weights_[s] = f; };
      private:
         std::vector<superslim::Jet> jets_;
         std::vector<superslim::Lepton> leptons_;
         std::vector<superslim::Tau> taus_;

         std::map<std::string, float> weights_;

         ClassDef(Combination, 1);
   };

   class Trigger {
      public:
         Trigger() {};
#ifndef __CINT__
         Trigger(const edm::TriggerResults&, const HLTConfigProvider&);
#endif
         virtual ~Trigger() {};

         const bool single_e() const { return single_e_; };
         const bool single_mu() const { return single_mu_; };
         const bool double_e() const { return double_e_; };
         const bool double_mu() const { return double_mu_; };
         const bool mixed() const { return mixed_; };
      private:
#ifndef __CINT__
         bool fired(const edm::TriggerResults&, const HLTConfigProvider&, const std::vector<std::string>&);
#endif
         bool single_e_;
         bool single_mu_;
         bool double_e_;
         bool double_mu_;
         bool mixed_;

         ClassDef(Trigger, 1);
   };

   class Event {
      public:
         Event() {};
#ifndef __CINT__
         Event(const std::vector<superslim::Combination>& cs,
               long run, long lumi, long event,
               int npv, int ntv,
               const superslim::Trigger& trigger,
               const LorentzVector& met,
               const std::vector<superslim::Vertex>& pv,
               const reco::GenParticleCollection& genparticles);
#endif
         virtual ~Event() {};

         const std::vector<superslim::Combination>& combos() const { return combos_; };
         const std::map<std::string, float>& weights() const { return weights_; };

         long run() const { return run_; };
         long lumi() const { return lumi_; };
         long event() const { return event_; };

         // primary vertices
         int npv() const { return npv_; };
         // true vertices
         int ntv() const { return ntv_; };

         int higgsDecay() const { return hdecay_; };

         const LorentzVector& met() const { return met_; };
         const std::vector<superslim::Vertex>& pv() const { return pv_; };
         const Trigger& trigger() const { return trigger_; };

         void setWeight(const std::string& s, float f) { weights_[s] = f; };
      private:
         std::vector<superslim::Combination> combos_;
         std::map<std::string, float> weights_;

         long run_;
         long lumi_;
         long event_;

         int npv_;
         int ntv_;

         int hdecay_;

         LorentzVector met_;
         std::vector<superslim::Vertex> pv_;
         superslim::Trigger trigger_;

         ClassDef(Event, 4);
   };

   bool operator<(const PhysObject& lhs, const PhysObject& rhs);
}
