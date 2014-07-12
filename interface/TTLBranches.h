// vim: ts=4:sw=4:et:sta
#ifndef TTLBranches_h
#define TTLBranches_h

#include "Math/LorentzVector.h"

#include "Branches.h"

namespace roast {
    namespace ttl {
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

                unsigned int	GetTau1IsolationIndex(const unsigned int) const;
                unsigned int	GetTau2IsolationIndex(const unsigned int) const;
                unsigned int	GetTau1IsolationIndexMVA(const unsigned int) const;
                unsigned int	GetTau2IsolationIndexMVA(const unsigned int) const;
                unsigned int	GetTau1IsolationIndexMVA2(const unsigned int) const;
                unsigned int	GetTau2IsolationIndexMVA2(const unsigned int) const;
                unsigned int	GetTau1IsolationIndex3Hits(const unsigned int) const;
                unsigned int	GetTau2IsolationIndex3Hits(const unsigned int) const;
                unsigned int	GetTau1AntiElectronIndex(const unsigned int) const;
                unsigned int	GetTau2AntiElectronIndex(const unsigned int) const;
                unsigned int	GetTau1AntiElectronIndexMVA2(const unsigned int) const;
                unsigned int	GetTau2AntiElectronIndexMVA2(const unsigned int) const;
                unsigned int	GetTau1AntiElectronIndexMVA3(const unsigned int) const;
                unsigned int	GetTau2AntiElectronIndexMVA3(const unsigned int) const;
                unsigned int	GetTau1AntiMuonIndex(const unsigned int) const;
                unsigned int	GetTau2AntiMuonIndex(const unsigned int) const;
                unsigned int	GetTau1AntiMuonIndex2(const unsigned int) const;
                unsigned int	GetTau2AntiMuonIndex2(const unsigned int) const;

                // >>> Begin declarations <<<
                std::vector<float>* L_Charge;
                std::vector<float>* L_CorrectedD0;
                std::vector<float>* L_CorrectedDZ;
                std::vector<float>* L_EventWeight;
                std::vector<int>* L_GenMatchDaughter0Id;
                std::vector<int>* L_GenMatchDaughter0Status;
                std::vector<int>* L_GenMatchDaughter1Id;
                std::vector<int>* L_GenMatchDaughter1Status;
                std::vector<int>* L_GenMatchGrandmother00Id;
                std::vector<int>* L_GenMatchGrandmother00Status;
                std::vector<int>* L_GenMatchGrandmother01Id;
                std::vector<int>* L_GenMatchGrandmother01Status;
                std::vector<int>* L_GenMatchGrandmother10Id;
                std::vector<int>* L_GenMatchGrandmother10Status;
                std::vector<int>* L_GenMatchGrandmother11Id;
                std::vector<int>* L_GenMatchGrandmother11Status;
                std::vector<int>* L_GenMatchId;
                std::vector<int>* L_GenMatchMother0Id;
                std::vector<int>* L_GenMatchMother0Status;
                std::vector<int>* L_GenMatchMother1Id;
                std::vector<int>* L_GenMatchMother1Status;
                std::vector<int>* L_GenMatchStatus;
                std::vector<float>* L_ImpactParameter;
                std::vector<float>* L_IsElectron;
                std::vector<bool>* L_IsLoose;
                std::vector<float>* L_IsMuon;
                std::vector<bool>* L_IsTight;
                std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* L_P;
                std::vector<float>* L_RelIso;
                std::vector<float>* T1_Charge;
                std::vector<unsigned int>* T1_DecayMode;
                std::vector<float>* T1_EmFraction;
                std::vector<int>* T1_GenMatchDaughter0Id;
                std::vector<int>* T1_GenMatchDaughter0Status;
                std::vector<int>* T1_GenMatchDaughter1Id;
                std::vector<int>* T1_GenMatchDaughter1Status;
                std::vector<int>* T1_GenMatchGrandmother00Id;
                std::vector<int>* T1_GenMatchGrandmother00Status;
                std::vector<int>* T1_GenMatchGrandmother01Id;
                std::vector<int>* T1_GenMatchGrandmother01Status;
                std::vector<int>* T1_GenMatchGrandmother10Id;
                std::vector<int>* T1_GenMatchGrandmother10Status;
                std::vector<int>* T1_GenMatchGrandmother11Id;
                std::vector<int>* T1_GenMatchGrandmother11Status;
                std::vector<int>* T1_GenMatchId;
                std::vector<int>* T1_GenMatchMother0Id;
                std::vector<int>* T1_GenMatchMother0Status;
                std::vector<int>* T1_GenMatchMother1Id;
                std::vector<int>* T1_GenMatchMother1Status;
                std::vector<int>* T1_GenMatchStatus;
                std::vector<bool>* T1_HPSagainstElectronDeadECAL;
                std::vector<bool>* T1_HPSagainstElectronLoose;
                std::vector<bool>* T1_HPSagainstElectronLooseMVA2;
                std::vector<bool>* T1_HPSagainstElectronLooseMVA3;
                std::vector<bool>* T1_HPSagainstElectronMVA;
                std::vector<bool>* T1_HPSagainstElectronMVA2category;
                std::vector<float>* T1_HPSagainstElectronMVA2raw;
                std::vector<bool>* T1_HPSagainstElectronMVA3category;
                std::vector<float>* T1_HPSagainstElectronMVA3raw;
                std::vector<bool>* T1_HPSagainstElectronMedium;
                std::vector<bool>* T1_HPSagainstElectronMediumMVA2;
                std::vector<bool>* T1_HPSagainstElectronMediumMVA3;
                std::vector<bool>* T1_HPSagainstElectronTight;
                std::vector<bool>* T1_HPSagainstElectronTightMVA2;
                std::vector<bool>* T1_HPSagainstElectronTightMVA3;
                std::vector<bool>* T1_HPSagainstElectronVLooseMVA2;
                std::vector<bool>* T1_HPSagainstElectronVTightMVA3;
                std::vector<bool>* T1_HPSagainstMuonLoose;
                std::vector<bool>* T1_HPSagainstMuonLoose2;
                std::vector<bool>* T1_HPSagainstMuonMedium;
                std::vector<bool>* T1_HPSagainstMuonMedium2;
                std::vector<bool>* T1_HPSagainstMuonTight;
                std::vector<bool>* T1_HPSagainstMuonTight2;
                std::vector<float>* T1_HPSbyCombinedIsolationDeltaBetaCorrRaw;
                std::vector<float>* T1_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits;
                std::vector<float>* T1_HPSbyIsolationMVA2raw;
                std::vector<float>* T1_HPSbyIsolationMVAraw;
                std::vector<bool>* T1_HPSbyLooseCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* T1_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits;
                std::vector<bool>* T1_HPSbyLooseIsolationMVA;
                std::vector<bool>* T1_HPSbyLooseIsolationMVA2;
                std::vector<bool>* T1_HPSbyMediumCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* T1_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits;
                std::vector<bool>* T1_HPSbyMediumIsolationMVA;
                std::vector<bool>* T1_HPSbyMediumIsolationMVA2;
                std::vector<bool>* T1_HPSbyTightCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* T1_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits;
                std::vector<bool>* T1_HPSbyTightIsolationMVA;
                std::vector<bool>* T1_HPSbyTightIsolationMVA2;
                std::vector<bool>* T1_HPSbyVLooseCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* T1_HPSdecayModeFinding;
                std::vector<bool>* T1_IsInTheCracks;
                std::vector<float>* T1_JetCSV;
                std::vector<float>* T1_LTIpVtdxy;
                std::vector<float>* T1_LTIpVtdxyError;
                std::vector<float>* T1_LTIpVtdz;
                std::vector<float>* T1_LTIpVtdzError;
                std::vector<float>* T1_LTNormChiSqrd;
                std::vector<float>* T1_LTPt;
                std::vector<unsigned int>* T1_LTValidHits;
                std::vector<bool>* T1_LTvalid;
                std::vector<float>* T1_LTvx;
                std::vector<float>* T1_LTvy;
                std::vector<float>* T1_LTvz;
                std::vector<int>* T1_MomentumRank;
                std::vector<unsigned int>* T1_NProngs;
                std::vector<unsigned int>* T1_NSignalGammas;
                std::vector<unsigned int>* T1_NSignalNeutrals;
                std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* T1_P;
                std::vector<float>* T2_Charge;
                std::vector<unsigned int>* T2_DecayMode;
                std::vector<float>* T2_EmFraction;
                std::vector<int>* T2_GenMatchDaughter0Id;
                std::vector<int>* T2_GenMatchDaughter0Status;
                std::vector<int>* T2_GenMatchDaughter1Id;
                std::vector<int>* T2_GenMatchDaughter1Status;
                std::vector<int>* T2_GenMatchGrandmother00Id;
                std::vector<int>* T2_GenMatchGrandmother00Status;
                std::vector<int>* T2_GenMatchGrandmother01Id;
                std::vector<int>* T2_GenMatchGrandmother01Status;
                std::vector<int>* T2_GenMatchGrandmother10Id;
                std::vector<int>* T2_GenMatchGrandmother10Status;
                std::vector<int>* T2_GenMatchGrandmother11Id;
                std::vector<int>* T2_GenMatchGrandmother11Status;
                std::vector<int>* T2_GenMatchId;
                std::vector<int>* T2_GenMatchMother0Id;
                std::vector<int>* T2_GenMatchMother0Status;
                std::vector<int>* T2_GenMatchMother1Id;
                std::vector<int>* T2_GenMatchMother1Status;
                std::vector<int>* T2_GenMatchStatus;
                std::vector<bool>* T2_HPSagainstElectronDeadECAL;
                std::vector<bool>* T2_HPSagainstElectronLoose;
                std::vector<bool>* T2_HPSagainstElectronLooseMVA2;
                std::vector<bool>* T2_HPSagainstElectronLooseMVA3;
                std::vector<bool>* T2_HPSagainstElectronMVA;
                std::vector<bool>* T2_HPSagainstElectronMVA2category;
                std::vector<float>* T2_HPSagainstElectronMVA2raw;
                std::vector<bool>* T2_HPSagainstElectronMVA3category;
                std::vector<float>* T2_HPSagainstElectronMVA3raw;
                std::vector<bool>* T2_HPSagainstElectronMedium;
                std::vector<bool>* T2_HPSagainstElectronMediumMVA2;
                std::vector<bool>* T2_HPSagainstElectronMediumMVA3;
                std::vector<bool>* T2_HPSagainstElectronTight;
                std::vector<bool>* T2_HPSagainstElectronTightMVA2;
                std::vector<bool>* T2_HPSagainstElectronTightMVA3;
                std::vector<bool>* T2_HPSagainstElectronVLooseMVA2;
                std::vector<bool>* T2_HPSagainstElectronVTightMVA3;
                std::vector<bool>* T2_HPSagainstMuonLoose;
                std::vector<bool>* T2_HPSagainstMuonLoose2;
                std::vector<bool>* T2_HPSagainstMuonMedium;
                std::vector<bool>* T2_HPSagainstMuonMedium2;
                std::vector<bool>* T2_HPSagainstMuonTight;
                std::vector<bool>* T2_HPSagainstMuonTight2;
                std::vector<float>* T2_HPSbyCombinedIsolationDeltaBetaCorrRaw;
                std::vector<float>* T2_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits;
                std::vector<float>* T2_HPSbyIsolationMVA2raw;
                std::vector<float>* T2_HPSbyIsolationMVAraw;
                std::vector<bool>* T2_HPSbyLooseCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* T2_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits;
                std::vector<bool>* T2_HPSbyLooseIsolationMVA;
                std::vector<bool>* T2_HPSbyLooseIsolationMVA2;
                std::vector<bool>* T2_HPSbyMediumCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* T2_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits;
                std::vector<bool>* T2_HPSbyMediumIsolationMVA;
                std::vector<bool>* T2_HPSbyMediumIsolationMVA2;
                std::vector<bool>* T2_HPSbyTightCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* T2_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits;
                std::vector<bool>* T2_HPSbyTightIsolationMVA;
                std::vector<bool>* T2_HPSbyTightIsolationMVA2;
                std::vector<bool>* T2_HPSbyVLooseCombinedIsolationDeltaBetaCorr;
                std::vector<bool>* T2_HPSdecayModeFinding;
                std::vector<bool>* T2_IsInTheCracks;
                std::vector<float>* T2_JetCSV;
                std::vector<float>* T2_LTIpVtdxy;
                std::vector<float>* T2_LTIpVtdxyError;
                std::vector<float>* T2_LTIpVtdz;
                std::vector<float>* T2_LTIpVtdzError;
                std::vector<float>* T2_LTNormChiSqrd;
                std::vector<float>* T2_LTPt;
                std::vector<unsigned int>* T2_LTValidHits;
                std::vector<bool>* T2_LTvalid;
                std::vector<float>* T2_LTvx;
                std::vector<float>* T2_LTvy;
                std::vector<float>* T2_LTvz;
                std::vector<int>* T2_MomentumRank;
                std::vector<unsigned int>* T2_NProngs;
                std::vector<unsigned int>* T2_NSignalGammas;
                std::vector<unsigned int>* T2_NSignalNeutrals;
                std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >* T2_P;
                // >>> End declarations <<<

                ClassDef(Branches, 1);
        };
    }
}

#endif
