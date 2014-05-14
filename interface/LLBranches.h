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
                std::vector<float>* L1_Charge;
                std::vector<float>* L1_CorrectedD0;
                std::vector<float>* L1_CorrectedDZ;
                std::vector<float>* L1_EventWeight;
                std::vector<int>* L1_GenMatchDaughter0Id;
                std::vector<int>* L1_GenMatchDaughter0Status;
                std::vector<int>* L1_GenMatchDaughter1Id;
                std::vector<int>* L1_GenMatchDaughter1Status;
                std::vector<int>* L1_GenMatchGrandmother00Id;
                std::vector<int>* L1_GenMatchGrandmother00Status;
                std::vector<int>* L1_GenMatchGrandmother01Id;
                std::vector<int>* L1_GenMatchGrandmother01Status;
                std::vector<int>* L1_GenMatchGrandmother10Id;
                std::vector<int>* L1_GenMatchGrandmother10Status;
                std::vector<int>* L1_GenMatchGrandmother11Id;
                std::vector<int>* L1_GenMatchGrandmother11Status;
                std::vector<int>* L1_GenMatchId;
                std::vector<int>* L1_GenMatchMother0Id;
                std::vector<int>* L1_GenMatchMother0Status;
                std::vector<int>* L1_GenMatchMother1Id;
                std::vector<int>* L1_GenMatchMother1Status;
                std::vector<int>* L1_GenMatchStatus;
                std::vector<float>* L1_ImpactParameter;
                std::vector<float>* L1_IsElectron;
                std::vector<bool>* L1_IsLoose;
                std::vector<float>* L1_IsMuon;
                std::vector<bool>* L1_IsTight;
                std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* L1_P;
                std::vector<float>* L1_RelIso;
                std::vector<float>* L2_Charge;
                std::vector<float>* L2_CorrectedD0;
                std::vector<float>* L2_CorrectedDZ;
                std::vector<float>* L2_EventWeight;
                std::vector<int>* L2_GenMatchDaughter0Id;
                std::vector<int>* L2_GenMatchDaughter0Status;
                std::vector<int>* L2_GenMatchDaughter1Id;
                std::vector<int>* L2_GenMatchDaughter1Status;
                std::vector<int>* L2_GenMatchGrandmother00Id;
                std::vector<int>* L2_GenMatchGrandmother00Status;
                std::vector<int>* L2_GenMatchGrandmother01Id;
                std::vector<int>* L2_GenMatchGrandmother01Status;
                std::vector<int>* L2_GenMatchGrandmother10Id;
                std::vector<int>* L2_GenMatchGrandmother10Status;
                std::vector<int>* L2_GenMatchGrandmother11Id;
                std::vector<int>* L2_GenMatchGrandmother11Status;
                std::vector<int>* L2_GenMatchId;
                std::vector<int>* L2_GenMatchMother0Id;
                std::vector<int>* L2_GenMatchMother0Status;
                std::vector<int>* L2_GenMatchMother1Id;
                std::vector<int>* L2_GenMatchMother1Status;
                std::vector<int>* L2_GenMatchStatus;
                std::vector<float>* L2_ImpactParameter;
                std::vector<float>* L2_IsElectron;
                std::vector<bool>* L2_IsLoose;
                std::vector<float>* L2_IsMuon;
                std::vector<bool>* L2_IsTight;
                std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* L2_P;
                std::vector<float>* L2_RelIso;
                // >>> End declarations <<<

                ClassDef(Branches, 1);
        };
    }
}

#endif
