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
                std::vector<float>* LL_Lepton2Mt;
                std::vector<float>* LL_Lepton2Mt2;
                std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* LL_Lepton2P;
                std::vector<float>* LL_Lepton2Phi;
                std::vector<float>* LL_Lepton2Pt;
                std::vector<float>* LL_Lepton2RelIso;
                std::vector<bool>* LL_PassZMask;
                std::vector<bool>* LL_PassZMask2;
                std::vector<float>* LL_TauCharge;
                std::vector<unsigned int>* LL_TauDecayMode;
                std::vector<float>* LL_TauEmFraction;
                std::vector<float>* LL_TauEta;
                std::vector<int>* LL_TauGenMatchDaughter0Id;
                std::vector<int>* LL_TauGenMatchDaughter0Status;
                std::vector<int>* LL_TauGenMatchDaughter1Id;
                std::vector<int>* LL_TauGenMatchDaughter1Status;
                std::vector<int>* LL_TauGenMatchGrandmother00Id;
                std::vector<int>* LL_TauGenMatchGrandmother00Status;
                std::vector<int>* LL_TauGenMatchGrandmother01Id;
                std::vector<int>* LL_TauGenMatchGrandmother01Status;
                std::vector<int>* LL_TauGenMatchGrandmother10Id;
                std::vector<int>* LL_TauGenMatchGrandmother10Status;
                std::vector<int>* LL_TauGenMatchGrandmother11Id;
                std::vector<int>* LL_TauGenMatchGrandmother11Status;
                std::vector<int>* LL_TauGenMatchId;
                std::vector<int>* LL_TauGenMatchMother0Id;
                std::vector<int>* LL_TauGenMatchMother0Status;
                std::vector<int>* LL_TauGenMatchMother1Id;
                std::vector<int>* LL_TauGenMatchMother1Status;
                std::vector<int>* LL_TauGenMatchStatus;
                std::vector<bool>* LL_TauHPSagainstElectronDeadECAL;
                std::vector<bool>* LL_TauHPSagainstElectronLoose;
                std::vector<bool>* LL_TauHPSagainstElectronLooseMVA2;
                std::vector<bool>* LL_TauHPSagainstElectronLooseMVA3;
                std::vector<bool>* LL_TauHPSagainstElectronMVA;
                std::vector<bool>* LL_TauHPSagainstElectronMVA2category;
                std::vector<float>* LL_TauHPSagainstElectronMVA2raw;
                std::vector<bool>* LL_TauHPSagainstElectronMVA3category;
                std::vector<float>* LL_TauHPSagainstElectronMVA3raw;
                std::vector<bool>* LL_TauHPSagainstElectronMedium;
                std::vector<bool>* LL_TauHPSagainstElectronMediumMVA2;
                std::vector<bool>* LL_TauHPSagainstElectronMediumMVA3;
                std::vector<bool>* LL_TauHPSagainstElectronTight;
                std::vector<bool>* LL_TauHPSagainstElectronTightMVA2;
                std::vector<bool>* LL_TauHPSagainstElectronTightMVA3;
                std::vector<bool>* LL_TauHPSagainstElectronVLooseMVA2;
                std::vector<bool>* LL_TauHPSagainstElectronVTightMVA3;
                std::vector<bool>* LL_TauHPSagainstMuonLoose;
                std::vector<bool>* LL_TauHPSagainstMuonLoose2;
                std::vector<bool>* LL_TauHPSagainstMuonMedium;
                std::vector<bool>* LL_TauHPSagainstMuonMedium2;
                std::vector<bool>* LL_TauHPSagainstMuonTight;
                std::vector<bool>* LL_TauHPSagainstMuonTight2;
                std::vector<float>* LL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw;
                std::vector<float>* LL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw3Hits;
                std::vector<float>* LL_TauHPSbyIsolationMVA2raw;
                std::vector<float>* LL_TauHPSbyIsolationMVAraw;
                std::vector<bool>* LL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* LL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr3Hits;
                std::vector<bool>* LL_TauHPSbyLooseIsolationMVA;
                std::vector<bool>* LL_TauHPSbyLooseIsolationMVA2;
                std::vector<bool>* LL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* LL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr3Hits;
                std::vector<bool>* LL_TauHPSbyMediumIsolationMVA;
                std::vector<bool>* LL_TauHPSbyMediumIsolationMVA2;
                std::vector<bool>* LL_TauHPSbyTightCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* LL_TauHPSbyTightCombinedIsolationDeltaBetaCorr3Hits;
                std::vector<bool>* LL_TauHPSbyTightIsolationMVA;
                std::vector<bool>* LL_TauHPSbyTightIsolationMVA2;
                std::vector<bool>* LL_TauHPSbyVLooseCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* LL_TauHPSdecayModeFinding;
                std::vector<bool>* LL_TauIsInTheCracks;
                std::vector<float>* LL_TauLTIpVtdxy;
                std::vector<float>* LL_TauLTIpVtdxyError;
                std::vector<float>* LL_TauLTIpVtdz;
                std::vector<float>* LL_TauLTIpVtdzError;
                std::vector<float>* LL_TauLTNormChiSqrd;
                std::vector<float>* LL_TauLTPt;
                std::vector<unsigned int>* LL_TauLTValidHits;
                std::vector<bool>* LL_TauLTvalid;
                std::vector<float>* LL_TauLTvx;
                std::vector<float>* LL_TauLTvy;
                std::vector<float>* LL_TauLTvz;
                std::vector<float>* LL_TauLepton1CosDeltaPhi;
                std::vector<float>* LL_TauLepton1DeltaR;
                std::vector<float>* LL_TauLepton1METMass;
                std::vector<float>* LL_TauLepton1VisibleMass;
                std::vector<float>* LL_TauLepton2CosDeltaPhi;
                std::vector<float>* LL_TauLepton2DeltaR;
                std::vector<float>* LL_TauLepton2METMass;
                std::vector<float>* LL_TauLepton2VisibleMass;
                std::vector<int>* LL_TauMomentumRank;
                std::vector<unsigned int>* LL_TauNProngs;
                std::vector<unsigned int>* LL_TauNSignalGammas;
                std::vector<unsigned int>* LL_TauNSignalNeutrals;
                std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* LL_TauP;
                std::vector<float>* LL_TauPhi;
                std::vector<float>* LL_TauPt;
                std::vector<float>* LL_TriggerEventWeight;
                // >>> End declarations <<<

                ClassDef(Branches, 1);
        };
    }
}

#endif
