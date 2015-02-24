#include <vector>

#include "Math/LorentzVector.h"
#include "TObject.h"

namespace superslim {
   typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > LorentzVector;

   class PhysObject {
      public:
         PhysObject() {};
         PhysObject(const LorentzVector& l) : p_(l) {};
         virtual ~PhysObject() {};

         LorentzVector p4() const { return p_; };
      private:
         LorentzVector p_;

         ClassDef(PhysObject, 1);
   };

   class Jet : public PhysObject {
      public:
         Jet() {};
         Jet(const LorentzVector& l, float csv) : PhysObject(l), csv_(csv) {};
         virtual ~Jet() {};

         float csv() const { return csv_; };
      private:
         float csv_;
         ClassDef(Jet, 1);
   };

   class Lepton : public PhysObject {
      public:
         Lepton() {};
         Lepton(const LorentzVector& l) : PhysObject(l) {};
         virtual ~Lepton() {};
      private:
         ClassDef(Lepton, 1);
   };

   class Tau : public PhysObject {
      public:
         Tau() {};
         Tau(const LorentzVector& l) : PhysObject(l) {};
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

         ClassDef(Combination, 1);
   };
}
