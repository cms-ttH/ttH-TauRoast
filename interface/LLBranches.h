// vim: ts=4:sw=4:et:sta
#ifndef LLBranches_h
#define LLBranches_h

#include "Math/LorentzVector.h"

#include "Branches.h"

namespace roast {
    namespace ll {
        class Branches : public roast::Branches {
            private:
                void	Null();
                void	Delete();
                void	Clear();
                void	SetBranchAddresses();

            public:
                Branches();
                Branches(const std::vector<std::string>&);
                ~Branches();

                virtual bool IsGoodGenMatch(const int&) const { return false; };

                // >>> Begin declarations <<<
                std::vector<float>* J_Lepton1DeltaR;
                std::vector<float>* J_Lepton2DeltaR;
                std::vector<float>* LL_Lepton1Charge;
                std::vector<float>* LL_Lepton1CorrectedD0;
                std::vector<float>* LL_Lepton1CorrectedDZ;
                std::vector<float>* LL_Lepton1Eta;
                std::vector<float>* LL_Lepton1EventWeight;
                std::vector<int>* LL_Lepton1GenMatchDaughter0Id;
                std::vector<int>* LL_Lepton1GenMatchDaughter0Status;
                std::vector<int>* LL_Lepton1GenMatchDaughter1Id;
                std::vector<int>* LL_Lepton1GenMatchDaughter1Status;
                std::vector<int>* LL_Lepton1GenMatchGrandmother00Id;
                std::vector<int>* LL_Lepton1GenMatchGrandmother00Status;
                std::vector<int>* LL_Lepton1GenMatchGrandmother01Id;
                std::vector<int>* LL_Lepton1GenMatchGrandmother01Status;
                std::vector<int>* LL_Lepton1GenMatchGrandmother10Id;
                std::vector<int>* LL_Lepton1GenMatchGrandmother10Status;
                std::vector<int>* LL_Lepton1GenMatchGrandmother11Id;
                std::vector<int>* LL_Lepton1GenMatchGrandmother11Status;
                std::vector<int>* LL_Lepton1GenMatchId;
                std::vector<int>* LL_Lepton1GenMatchMother0Id;
                std::vector<int>* LL_Lepton1GenMatchMother0Status;
                std::vector<int>* LL_Lepton1GenMatchMother1Id;
                std::vector<int>* LL_Lepton1GenMatchMother1Status;
                std::vector<int>* LL_Lepton1GenMatchStatus;
                std::vector<float>* LL_Lepton1ImpactParameter;
                std::vector<float>* LL_Lepton1IsElectron;
                std::vector<bool>* LL_Lepton1IsLoose;
                std::vector<float>* LL_Lepton1IsMuon;
                std::vector<bool>* LL_Lepton1IsTight;
                std::vector<float>* LL_Lepton1Lepton2CosDeltaPhi;
                std::vector<float>* LL_Lepton1Lepton2DeltaR;
                std::vector<float>* LL_Lepton1Lepton2VisibleMass;
                std::vector<float>* LL_Lepton1METCosDeltaPhi;
                std::vector<float>* LL_Lepton1Mt;
                std::vector<float>* LL_Lepton1Mt2;
                std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* LL_Lepton1P;
                std::vector<float>* LL_Lepton1Phi;
                std::vector<float>* LL_Lepton1Pt;
                std::vector<float>* LL_Lepton1RelIso;
                std::vector<float>* LL_Lepton2Charge;
                std::vector<float>* LL_Lepton2CorrectedD0;
                std::vector<float>* LL_Lepton2CorrectedDZ;
                std::vector<float>* LL_Lepton2Eta;
                std::vector<float>* LL_Lepton2EventWeight;
                std::vector<int>* LL_Lepton2GenMatchDaughter0Id;
                std::vector<int>* LL_Lepton2GenMatchDaughter0Status;
                std::vector<int>* LL_Lepton2GenMatchDaughter1Id;
                std::vector<int>* LL_Lepton2GenMatchDaughter1Status;
                std::vector<int>* LL_Lepton2GenMatchGrandmother00Id;
                std::vector<int>* LL_Lepton2GenMatchGrandmother00Status;
                std::vector<int>* LL_Lepton2GenMatchGrandmother01Id;
                std::vector<int>* LL_Lepton2GenMatchGrandmother01Status;
                std::vector<int>* LL_Lepton2GenMatchGrandmother10Id;
                std::vector<int>* LL_Lepton2GenMatchGrandmother10Status;
                std::vector<int>* LL_Lepton2GenMatchGrandmother11Id;
                std::vector<int>* LL_Lepton2GenMatchGrandmother11Status;
                std::vector<int>* LL_Lepton2GenMatchId;
                std::vector<int>* LL_Lepton2GenMatchMother0Id;
                std::vector<int>* LL_Lepton2GenMatchMother0Status;
                std::vector<int>* LL_Lepton2GenMatchMother1Id;
                std::vector<int>* LL_Lepton2GenMatchMother1Status;
                std::vector<int>* LL_Lepton2GenMatchStatus;
                std::vector<float>* LL_Lepton2ImpactParameter;
                std::vector<float>* LL_Lepton2IsElectron;
                std::vector<bool>* LL_Lepton2IsLoose;
                std::vector<float>* LL_Lepton2IsMuon;
                std::vector<bool>* LL_Lepton2IsTight;
                std::vector<float>* LL_Lepton2METCosDeltaPhi;
                std::vector<float>* LL_Lepton2Mt;
                std::vector<float>* LL_Lepton2Mt2;
                std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* LL_Lepton2P;
                std::vector<float>* LL_Lepton2Phi;
                std::vector<float>* LL_Lepton2Pt;
                std::vector<float>* LL_Lepton2RelIso;
                std::vector<bool>* LL_PassZMask;
                std::vector<bool>* LL_PassZMask2;
                std::vector<float>* LL_TriggerEventWeight;
                // >>> End declarations <<<

                ClassDef(Branches, 1);
        };
    }
}

#endif
