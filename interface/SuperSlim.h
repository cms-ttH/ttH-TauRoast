#include <map>
#include <vector>

#include "Math/LorentzVector.h"
#include "TObject.h"

namespace reco {
   class Candidate;
   class GenParticle;
}

namespace superslim {
   typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;

   class PhysObject {
      public:
         PhysObject() {};
         PhysObject(const reco::Candidate*);
         virtual ~PhysObject() {};

         LorentzVector
            p4() const { return p_; };
         const std::vector<superslim::PhysObject>
            parents() const { return parents_; };
         int
            parentId() const;
         int
            grandParentId() const;
         int
            charge() const { return charge_; };
         int
            pdgId() const { return pdg_id_; };
         void
            setGenInfo(const reco::Candidate* p, int level=2);
      private:
         LorentzVector p_;
         std::vector<superslim::PhysObject> parents_;

         int charge_;
         int pdg_id_;

         ClassDef(PhysObject, 1);
   };

   class Jet : public PhysObject {
      public:
         Jet() {};
         Jet(const reco::Candidate* c, float csv) : PhysObject(c), csv_(csv) {};
         virtual ~Jet() {};

         float csv() const { return csv_; };
      private:
         float csv_;
         ClassDef(Jet, 1);
   };

   class Lepton : public PhysObject {
      public:
         Lepton() {};
         Lepton(const reco::Candidate* c) : PhysObject(c) {};
         virtual ~Lepton() {};
      private:
         ClassDef(Lepton, 1);
   };

   class Tau : public PhysObject {
      public:
         Tau() {};
         Tau(const reco::Candidate* c) : PhysObject(c) {};
         virtual ~Tau() {};
      private:
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

         const std::vector<superslim::Jet> jets() const { return jets_; };
         const std::vector<superslim::Lepton> leptons() const { return leptons_; };
         const std::vector<superslim::Tau> taus() const { return taus_; };
      private:
         std::vector<superslim::Jet> jets_;
         std::vector<superslim::Lepton> leptons_;
         std::vector<superslim::Tau> taus_;

         std::map<std::string, float> weights_;

         ClassDef(Combination, 1);
   };

   class Event {
      public:
         Event() {};
         Event(const std::vector<superslim::Combination>& cs,
               long run, long lumi, long event,
               int npv, int ntv) :
            combos_(cs),
            run_(run), lumi_(lumi), event_(event),
            npv_(npv), ntv_(ntv) {};
         virtual ~Event() {};

         const std::vector<superslim::Combination> combos() const { return combos_; };
         const std::map<std::string, float> weights() const { return weights_; };

         long run() const { return run_; };
         long lumi() const { return lumi_; };
         long event() const { return event_; };

         // primary vertices
         int npv() const { return npv_; };
         // true vertices
         int ntv() const { return ntv_; };
      private:
         std::vector<superslim::Combination> combos_;
         std::map<std::string, float> weights_;

         long run_;
         long lumi_;
         long event_;

         int npv_;
         int ntv_;

         ClassDef(Event, 1);
   };
}
