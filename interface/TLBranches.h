// vim: ts=4:sw=4:et:sta
#ifndef TLBranches_h
#define TLBranches_h

#include "Math/LorentzVector.h"

#include "Branches.h"

namespace roast {
    namespace tl {
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
                std::vector<std::vector<float> >* J_LeptonDeltaR;
                std::vector<std::vector<float> >* J_TauDeltaR;
                std::vector<float>* TL_LeptonCharge;
                std::vector<float>* TL_LeptonCorrectedD0;
                std::vector<float>* TL_LeptonCorrectedDZ;
                std::vector<float>* TL_LeptonEta;
                std::vector<float>* TL_LeptonEventWeight;
                std::vector<int>* TL_LeptonGenMatchDaughter0Id;
                std::vector<int>* TL_LeptonGenMatchDaughter0Status;
                std::vector<int>* TL_LeptonGenMatchDaughter1Id;
                std::vector<int>* TL_LeptonGenMatchDaughter1Status;
                std::vector<int>* TL_LeptonGenMatchGrandmother00Id;
                std::vector<int>* TL_LeptonGenMatchGrandmother00Status;
                std::vector<int>* TL_LeptonGenMatchGrandmother01Id;
                std::vector<int>* TL_LeptonGenMatchGrandmother01Status;
                std::vector<int>* TL_LeptonGenMatchGrandmother10Id;
                std::vector<int>* TL_LeptonGenMatchGrandmother10Status;
                std::vector<int>* TL_LeptonGenMatchGrandmother11Id;
                std::vector<int>* TL_LeptonGenMatchGrandmother11Status;
                std::vector<int>* TL_LeptonGenMatchId;
                std::vector<int>* TL_LeptonGenMatchMother0Id;
                std::vector<int>* TL_LeptonGenMatchMother0Status;
                std::vector<int>* TL_LeptonGenMatchMother1Id;
                std::vector<int>* TL_LeptonGenMatchMother1Status;
                std::vector<int>* TL_LeptonGenMatchStatus;
                std::vector<float>* TL_LeptonImpactParameter;
                std::vector<float>* TL_LeptonIsElectron;
                std::vector<bool>* TL_LeptonIsLoose;
                std::vector<float>* TL_LeptonIsMuon;
                std::vector<bool>* TL_LeptonIsTight;
                std::vector<float>* TL_LeptonMt;
                std::vector<float>* TL_LeptonMt2;
                std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* TL_LeptonP;
                std::vector<float>* TL_LeptonPhi;
                std::vector<float>* TL_LeptonPt;
                std::vector<float>* TL_LeptonRelIso;
                std::vector<float>* TL_TauCharge;
                std::vector<unsigned int>* TL_TauDecayMode;
                std::vector<float>* TL_TauEmFraction;
                std::vector<float>* TL_TauEta;
                std::vector<int>* TL_TauGenMatchDaughter0Id;
                std::vector<int>* TL_TauGenMatchDaughter0Status;
                std::vector<int>* TL_TauGenMatchDaughter1Id;
                std::vector<int>* TL_TauGenMatchDaughter1Status;
                std::vector<int>* TL_TauGenMatchGrandmother00Id;
                std::vector<int>* TL_TauGenMatchGrandmother00Status;
                std::vector<int>* TL_TauGenMatchGrandmother01Id;
                std::vector<int>* TL_TauGenMatchGrandmother01Status;
                std::vector<int>* TL_TauGenMatchGrandmother10Id;
                std::vector<int>* TL_TauGenMatchGrandmother10Status;
                std::vector<int>* TL_TauGenMatchGrandmother11Id;
                std::vector<int>* TL_TauGenMatchGrandmother11Status;
                std::vector<int>* TL_TauGenMatchId;
                std::vector<int>* TL_TauGenMatchMother0Id;
                std::vector<int>* TL_TauGenMatchMother0Status;
                std::vector<int>* TL_TauGenMatchMother1Id;
                std::vector<int>* TL_TauGenMatchMother1Status;
                std::vector<int>* TL_TauGenMatchStatus;
                std::vector<bool>* TL_TauHPSagainstElectronDeadECAL;
                std::vector<bool>* TL_TauHPSagainstElectronLoose;
                std::vector<bool>* TL_TauHPSagainstElectronLooseMVA2;
                std::vector<bool>* TL_TauHPSagainstElectronLooseMVA3;
                std::vector<bool>* TL_TauHPSagainstElectronMVA;
                std::vector<bool>* TL_TauHPSagainstElectronMVA2category;
                std::vector<float>* TL_TauHPSagainstElectronMVA2raw;
                std::vector<bool>* TL_TauHPSagainstElectronMVA3category;
                std::vector<float>* TL_TauHPSagainstElectronMVA3raw;
                std::vector<bool>* TL_TauHPSagainstElectronMedium;
                std::vector<bool>* TL_TauHPSagainstElectronMediumMVA2;
                std::vector<bool>* TL_TauHPSagainstElectronMediumMVA3;
                std::vector<bool>* TL_TauHPSagainstElectronTight;
                std::vector<bool>* TL_TauHPSagainstElectronTightMVA2;
                std::vector<bool>* TL_TauHPSagainstElectronTightMVA3;
                std::vector<bool>* TL_TauHPSagainstElectronVLooseMVA2;
                std::vector<bool>* TL_TauHPSagainstElectronVTightMVA3;
                std::vector<bool>* TL_TauHPSagainstMuonLoose;
                std::vector<bool>* TL_TauHPSagainstMuonLoose2;
                std::vector<bool>* TL_TauHPSagainstMuonMedium;
                std::vector<bool>* TL_TauHPSagainstMuonMedium2;
                std::vector<bool>* TL_TauHPSagainstMuonTight;
                std::vector<bool>* TL_TauHPSagainstMuonTight2;
                std::vector<float>* TL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw;
                std::vector<float>* TL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw3Hits;
                std::vector<float>* TL_TauHPSbyIsolationMVA2raw;
                std::vector<float>* TL_TauHPSbyIsolationMVAraw;
                std::vector<bool>* TL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* TL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr3Hits;
                std::vector<bool>* TL_TauHPSbyLooseIsolationMVA;
                std::vector<bool>* TL_TauHPSbyLooseIsolationMVA2;
                std::vector<bool>* TL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* TL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr3Hits;
                std::vector<bool>* TL_TauHPSbyMediumIsolationMVA;
                std::vector<bool>* TL_TauHPSbyMediumIsolationMVA2;
                std::vector<bool>* TL_TauHPSbyTightCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* TL_TauHPSbyTightCombinedIsolationDeltaBetaCorr3Hits;
                std::vector<bool>* TL_TauHPSbyTightIsolationMVA;
                std::vector<bool>* TL_TauHPSbyTightIsolationMVA2;
                std::vector<bool>* TL_TauHPSbyVLooseCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* TL_TauHPSdecayModeFinding;
                std::vector<bool>* TL_TauIsInTheCracks;
                std::vector<float>* TL_TauLTIpVtdxy;
                std::vector<float>* TL_TauLTIpVtdxyError;
                std::vector<float>* TL_TauLTIpVtdz;
                std::vector<float>* TL_TauLTIpVtdzError;
                std::vector<float>* TL_TauLTNormChiSqrd;
                std::vector<float>* TL_TauLTPt;
                std::vector<unsigned int>* TL_TauLTValidHits;
                std::vector<bool>* TL_TauLTvalid;
                std::vector<float>* TL_TauLTvx;
                std::vector<float>* TL_TauLTvy;
                std::vector<float>* TL_TauLTvz;
                std::vector<float>* TL_TauLeptonCosDeltaPhi;
                std::vector<float>* TL_TauLeptonDeltaR;
                std::vector<float>* TL_TauLeptonMETMass;
                std::vector<float>* TL_TauLeptonVisibleMass;
                std::vector<int>* TL_TauMomentumRank;
                std::vector<unsigned int>* TL_TauNProngs;
                std::vector<unsigned int>* TL_TauNSignalGammas;
                std::vector<unsigned int>* TL_TauNSignalNeutrals;
                std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* TL_TauP;
                std::vector<float>* TL_TauPhi;
                std::vector<float>* TL_TauPt;
                std::vector<float>* TL_TriggerEventWeight;
                // >>> End declarations <<<

                ClassDef(Branches, 1);
        };
    }
}

#endif
