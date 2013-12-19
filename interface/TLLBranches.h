// vim: ts=4:sw=4:et:sta
#ifndef TLLBranches_h
#define TLLBranches_h

#include "Branches.h"

namespace roast {
    namespace tll {
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

                virtual bool IsGoodGenMatch(const int&) const;

                unsigned int GetTauIsolationIndex(const unsigned int) const;
                unsigned int GetTauIsolationIndexMVA(const unsigned int) const;
                unsigned int GetTauIsolationIndexMVA2(const unsigned int) const;
                unsigned int GetTauIsolationIndex3Hits(const unsigned int) const;
                unsigned int GetTauAntiElectronIndex(const unsigned int) const;
                unsigned int GetTauAntiElectronIndexMVA2(const unsigned int) const;
                unsigned int GetTauAntiElectronIndexMVA3(const unsigned int) const;
                unsigned int GetTauAntiMuonIndex(const unsigned int) const;
                unsigned int GetTauAntiMuonIndex2(const unsigned int) const;

                // >>> Begin declarations <<<
                std::vector<float>* TLL_Lepton1Charge;
                std::vector<float>* TLL_Lepton1CorrectedD0;
                std::vector<float>* TLL_Lepton1CorrectedDZ;
                std::vector<float>* TLL_Lepton1Eta;
                std::vector<float>* TLL_Lepton1EventWeight;
                std::vector<int>* TLL_Lepton1GenMatchDaughter0Id;
                std::vector<int>* TLL_Lepton1GenMatchDaughter0Status;
                std::vector<int>* TLL_Lepton1GenMatchDaughter1Id;
                std::vector<int>* TLL_Lepton1GenMatchDaughter1Status;
                std::vector<int>* TLL_Lepton1GenMatchGrandmother00Id;
                std::vector<int>* TLL_Lepton1GenMatchGrandmother00Status;
                std::vector<int>* TLL_Lepton1GenMatchGrandmother01Id;
                std::vector<int>* TLL_Lepton1GenMatchGrandmother01Status;
                std::vector<int>* TLL_Lepton1GenMatchGrandmother10Id;
                std::vector<int>* TLL_Lepton1GenMatchGrandmother10Status;
                std::vector<int>* TLL_Lepton1GenMatchGrandmother11Id;
                std::vector<int>* TLL_Lepton1GenMatchGrandmother11Status;
                std::vector<int>* TLL_Lepton1GenMatchId;
                std::vector<int>* TLL_Lepton1GenMatchMother0Id;
                std::vector<int>* TLL_Lepton1GenMatchMother0Status;
                std::vector<int>* TLL_Lepton1GenMatchMother1Id;
                std::vector<int>* TLL_Lepton1GenMatchMother1Status;
                std::vector<int>* TLL_Lepton1GenMatchStatus;
                std::vector<float>* TLL_Lepton1ImpactParameter;
                std::vector<float>* TLL_Lepton1IsElectron;
                std::vector<bool>* TLL_Lepton1IsLoose;
                std::vector<float>* TLL_Lepton1IsMuon;
                std::vector<bool>* TLL_Lepton1IsTight;
                std::vector<float>* TLL_Lepton1Lepton2CosDeltaPhi;
                std::vector<float>* TLL_Lepton1Lepton2DeltaR;
                std::vector<float>* TLL_Lepton1Lepton2VisibleMass;
                std::vector<float>* TLL_Lepton1Mt;
                std::vector<float>* TLL_Lepton1Mt2;
                std::vector<float>* TLL_Lepton1Phi;
                std::vector<float>* TLL_Lepton1Pt;
                std::vector<float>* TLL_Lepton1RelIso;
                std::vector<float>* TLL_Lepton2Charge;
                std::vector<float>* TLL_Lepton2CorrectedD0;
                std::vector<float>* TLL_Lepton2CorrectedDZ;
                std::vector<float>* TLL_Lepton2Eta;
                std::vector<float>* TLL_Lepton2EventWeight;
                std::vector<int>* TLL_Lepton2GenMatchDaughter0Id;
                std::vector<int>* TLL_Lepton2GenMatchDaughter0Status;
                std::vector<int>* TLL_Lepton2GenMatchDaughter1Id;
                std::vector<int>* TLL_Lepton2GenMatchDaughter1Status;
                std::vector<int>* TLL_Lepton2GenMatchGrandmother00Id;
                std::vector<int>* TLL_Lepton2GenMatchGrandmother00Status;
                std::vector<int>* TLL_Lepton2GenMatchGrandmother01Id;
                std::vector<int>* TLL_Lepton2GenMatchGrandmother01Status;
                std::vector<int>* TLL_Lepton2GenMatchGrandmother10Id;
                std::vector<int>* TLL_Lepton2GenMatchGrandmother10Status;
                std::vector<int>* TLL_Lepton2GenMatchGrandmother11Id;
                std::vector<int>* TLL_Lepton2GenMatchGrandmother11Status;
                std::vector<int>* TLL_Lepton2GenMatchId;
                std::vector<int>* TLL_Lepton2GenMatchMother0Id;
                std::vector<int>* TLL_Lepton2GenMatchMother0Status;
                std::vector<int>* TLL_Lepton2GenMatchMother1Id;
                std::vector<int>* TLL_Lepton2GenMatchMother1Status;
                std::vector<int>* TLL_Lepton2GenMatchStatus;
                std::vector<float>* TLL_Lepton2ImpactParameter;
                std::vector<float>* TLL_Lepton2IsElectron;
                std::vector<bool>* TLL_Lepton2IsLoose;
                std::vector<float>* TLL_Lepton2IsMuon;
                std::vector<bool>* TLL_Lepton2IsTight;
                std::vector<float>* TLL_Lepton2Mt;
                std::vector<float>* TLL_Lepton2Mt2;
                std::vector<float>* TLL_Lepton2Phi;
                std::vector<float>* TLL_Lepton2Pt;
                std::vector<float>* TLL_Lepton2RelIso;
                std::vector<bool>* TLL_PassZMask;
                std::vector<bool>* TLL_PassZMask2;
                std::vector<float>* TLL_TauCharge;
                std::vector<unsigned int>* TLL_TauDecayMode;
                std::vector<float>* TLL_TauEmFraction;
                std::vector<float>* TLL_TauEta;
                std::vector<int>* TLL_TauGenMatchDaughter0Id;
                std::vector<int>* TLL_TauGenMatchDaughter0Status;
                std::vector<int>* TLL_TauGenMatchDaughter1Id;
                std::vector<int>* TLL_TauGenMatchDaughter1Status;
                std::vector<int>* TLL_TauGenMatchGrandmother00Id;
                std::vector<int>* TLL_TauGenMatchGrandmother00Status;
                std::vector<int>* TLL_TauGenMatchGrandmother01Id;
                std::vector<int>* TLL_TauGenMatchGrandmother01Status;
                std::vector<int>* TLL_TauGenMatchGrandmother10Id;
                std::vector<int>* TLL_TauGenMatchGrandmother10Status;
                std::vector<int>* TLL_TauGenMatchGrandmother11Id;
                std::vector<int>* TLL_TauGenMatchGrandmother11Status;
                std::vector<int>* TLL_TauGenMatchId;
                std::vector<int>* TLL_TauGenMatchMother0Id;
                std::vector<int>* TLL_TauGenMatchMother0Status;
                std::vector<int>* TLL_TauGenMatchMother1Id;
                std::vector<int>* TLL_TauGenMatchMother1Status;
                std::vector<int>* TLL_TauGenMatchStatus;
                std::vector<bool>* TLL_TauHPSagainstElectronDeadECAL;
                std::vector<bool>* TLL_TauHPSagainstElectronLoose;
                std::vector<bool>* TLL_TauHPSagainstElectronLooseMVA2;
                std::vector<bool>* TLL_TauHPSagainstElectronLooseMVA3;
                std::vector<bool>* TLL_TauHPSagainstElectronMVA;
                std::vector<bool>* TLL_TauHPSagainstElectronMVA2category;
                std::vector<float>* TLL_TauHPSagainstElectronMVA2raw;
                std::vector<bool>* TLL_TauHPSagainstElectronMVA3category;
                std::vector<float>* TLL_TauHPSagainstElectronMVA3raw;
                std::vector<bool>* TLL_TauHPSagainstElectronMedium;
                std::vector<bool>* TLL_TauHPSagainstElectronMediumMVA2;
                std::vector<bool>* TLL_TauHPSagainstElectronMediumMVA3;
                std::vector<bool>* TLL_TauHPSagainstElectronTight;
                std::vector<bool>* TLL_TauHPSagainstElectronTightMVA2;
                std::vector<bool>* TLL_TauHPSagainstElectronTightMVA3;
                std::vector<bool>* TLL_TauHPSagainstElectronVLooseMVA2;
                std::vector<bool>* TLL_TauHPSagainstElectronVTightMVA3;
                std::vector<bool>* TLL_TauHPSagainstMuonLoose;
                std::vector<bool>* TLL_TauHPSagainstMuonLoose2;
                std::vector<bool>* TLL_TauHPSagainstMuonMedium;
                std::vector<bool>* TLL_TauHPSagainstMuonMedium2;
                std::vector<bool>* TLL_TauHPSagainstMuonTight;
                std::vector<bool>* TLL_TauHPSagainstMuonTight2;
                std::vector<float>* TLL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw;
                std::vector<float>* TLL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw3Hits;
                std::vector<float>* TLL_TauHPSbyIsolationMVA2raw;
                std::vector<float>* TLL_TauHPSbyIsolationMVAraw;
                std::vector<bool>* TLL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* TLL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr3Hits;
                std::vector<bool>* TLL_TauHPSbyLooseIsolationMVA;
                std::vector<bool>* TLL_TauHPSbyLooseIsolationMVA2;
                std::vector<bool>* TLL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* TLL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr3Hits;
                std::vector<bool>* TLL_TauHPSbyMediumIsolationMVA;
                std::vector<bool>* TLL_TauHPSbyMediumIsolationMVA2;
                std::vector<bool>* TLL_TauHPSbyTightCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* TLL_TauHPSbyTightCombinedIsolationDeltaBetaCorr3Hits;
                std::vector<bool>* TLL_TauHPSbyTightIsolationMVA;
                std::vector<bool>* TLL_TauHPSbyTightIsolationMVA2;
                std::vector<bool>* TLL_TauHPSbyVLooseCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* TLL_TauHPSdecayModeFinding;
                std::vector<bool>* TLL_TauIsInTheCracks;
                std::vector<float>* TLL_TauLTIpVtdxy;
                std::vector<float>* TLL_TauLTIpVtdxyError;
                std::vector<float>* TLL_TauLTIpVtdz;
                std::vector<float>* TLL_TauLTIpVtdzError;
                std::vector<float>* TLL_TauLTNormChiSqrd;
                std::vector<float>* TLL_TauLTPt;
                std::vector<unsigned int>* TLL_TauLTValidHits;
                std::vector<bool>* TLL_TauLTvalid;
                std::vector<float>* TLL_TauLTvx;
                std::vector<float>* TLL_TauLTvy;
                std::vector<float>* TLL_TauLTvz;
                std::vector<float>* TLL_TauLepton1CosDeltaPhi;
                std::vector<float>* TLL_TauLepton1DeltaR;
                std::vector<float>* TLL_TauLepton1METMass;
                std::vector<float>* TLL_TauLepton1VisibleMass;
                std::vector<float>* TLL_TauLepton2CosDeltaPhi;
                std::vector<float>* TLL_TauLepton2DeltaR;
                std::vector<float>* TLL_TauLepton2METMass;
                std::vector<float>* TLL_TauLepton2VisibleMass;
                std::vector<int>* TLL_TauMomentumRank;
                std::vector<unsigned int>* TLL_TauNProngs;
                std::vector<unsigned int>* TLL_TauNSignalGammas;
                std::vector<unsigned int>* TLL_TauNSignalNeutrals;
                std::vector<float>* TLL_TauPhi;
                std::vector<float>* TLL_TauPt;
                std::vector<float>* TLL_TriggerEventWeight;
                // >>> End declarations <<<

                ClassDef(Branches, 1);
        };
    }
}

#endif
