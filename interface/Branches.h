// vim: ts=4:sw=4:et:sta
#ifndef Branches_h
#define Branches_h

#include <map>
#include <string>
#include <vector>

#include "TChain.h"

#include "CutFlow.h"
#include "HWrapper.h"

namespace roast {
    class Branches {
        protected:

            void RegenerateCaches();
            void SetUp(std::map<std::string,std::string> const &, std::vector<std::string> const &);
            virtual void Null();
            virtual void Delete();
            virtual void Clear();
            virtual void SetBranchAddresses();
            virtual void SetChain(TChain*);

            std::map<std::string,std::string>		params;
            TChain* fChain;

            bool caches_dirty;
            std::vector<std::vector<int> > clean_btag_cache;
            std::vector<std::vector<int> > clean_nonbtag_cache;

        public:
            Branches();
            Branches(const std::vector<std::string>&);
            ~Branches();

            void Init();
            void GetEntry(double);
            Long64_t GetEntries();
            unsigned int GetNumCombos() { return NumCombos; };
            virtual bool IsGoodGenMatch(const int&) const = 0;

            float GetCleanJetBTagEta(unsigned int, unsigned int);
            float GetCleanJetBTagPhi(unsigned int, unsigned int);
            float GetCleanJetBTagPt(unsigned int, unsigned int);
            float GetCleanJetNonBTagEta(unsigned int, unsigned int);
            float GetCleanJetNonBTagPhi(unsigned int, unsigned int);
            float GetCleanJetNonBTagPt(unsigned int, unsigned int);

            unsigned int TranslateMatchIndex(int, int=-99) const;

            // >>> Begin declarations <<<
            std::vector<int>* E_ElectronGenMatchDaughter0Id;
            std::vector<int>* E_ElectronGenMatchDaughter0Status;
            std::vector<int>* E_ElectronGenMatchDaughter1Id;
            std::vector<int>* E_ElectronGenMatchDaughter1Status;
            std::vector<int>* E_ElectronGenMatchGrandmother00Id;
            std::vector<int>* E_ElectronGenMatchGrandmother00Status;
            std::vector<int>* E_ElectronGenMatchGrandmother01Id;
            std::vector<int>* E_ElectronGenMatchGrandmother01Status;
            std::vector<int>* E_ElectronGenMatchGrandmother10Id;
            std::vector<int>* E_ElectronGenMatchGrandmother10Status;
            std::vector<int>* E_ElectronGenMatchGrandmother11Id;
            std::vector<int>* E_ElectronGenMatchGrandmother11Status;
            std::vector<int>* E_ElectronGenMatchId;
            std::vector<int>* E_ElectronGenMatchMother0Id;
            std::vector<int>* E_ElectronGenMatchMother0Status;
            std::vector<int>* E_ElectronGenMatchMother1Id;
            std::vector<int>* E_ElectronGenMatchMother1Status;
            std::vector<int>* E_ElectronGenMatchStatus;
            std::vector<float>* E_Eta;
            std::vector<bool>* E_IsLooseElectron;
            std::vector<bool>* E_IsTightElectron;
            std::vector<float>* E_MomentumRank;
            Int_t E_NumElectrons;
            std::vector<float>* E_Phi;
            std::vector<float>* E_Pt;
            Bool_t Ev_IsTauEvent;
            Double_t Ev_MET;
            Double_t Ev_METphi;
            Int_t Ev_bQuarkCount;
            Int_t Ev_cQuarkCount;
            Double_t Ev_eventNumber;
            Int_t Ev_higgsDecayMode;
            Double_t Ev_lumiBlock;
            Double_t Ev_puWeight;
            Double_t Ev_puWeightDown;
            Double_t Ev_puWeightUp;
            Double_t Ev_q2WeightDown;
            Double_t Ev_q2WeightUp;
            Double_t Ev_runNumber;
            Double_t Ev_topPtWeight;
            Double_t Ev_topPtWeightDown;
            Double_t Ev_topPtWeightUp;
            std::vector<float>* GJ_Eta;
            std::vector<bool>* GJ_IsBjet;
            std::vector<float>* GJ_MomentumRank;
            Int_t GJ_NumGenJets;
            std::vector<float>* GJ_Phi;
            std::vector<float>* GJ_Pt;
            std::vector<float>* GT_Eta;
            std::vector<float>* GT_MomentumRank;
            Int_t GT_NumGenTaus;
            std::vector<float>* GT_ParentEta;
            std::vector<int>* GT_ParentId;
            std::vector<float>* GT_ParentP;
            std::vector<float>* GT_ParentPhi;
            std::vector<float>* GT_ParentPt;
            std::vector<float>* GT_Phi;
            std::vector<float>* GT_Pt;
            std::vector<bool>* GT_ToElectron;
            std::vector<bool>* GT_ToHadrons;
            std::vector<bool>* GT_ToMuon;
            std::vector<float>* GT_VisEta;
            std::vector<float>* GT_VisPhi;
            std::vector<float>* GT_VisPt;
            std::vector<float>* J_Eta;
            std::vector<float>* J_MomentumRank;
            Int_t J_NumJets;
            std::vector<float>* J_Phi;
            std::vector<float>* J_Pt;
            std::vector<float>* J_combSecVtxBTag;
            std::vector<bool>* J_combSecVtxLooseBTag;
            std::vector<bool>* J_combSecVtxMediumBTag;
            std::vector<bool>* J_combSecVtxTightBTag;
            std::vector<float>* M_Eta;
            std::vector<bool>* M_IsLooseMuon;
            std::vector<bool>* M_IsTightMuon;
            std::vector<float>* M_MomentumRank;
            std::vector<int>* M_MuonGenMatchDaughter0Id;
            std::vector<int>* M_MuonGenMatchDaughter0Status;
            std::vector<int>* M_MuonGenMatchDaughter1Id;
            std::vector<int>* M_MuonGenMatchDaughter1Status;
            std::vector<int>* M_MuonGenMatchGrandmother00Id;
            std::vector<int>* M_MuonGenMatchGrandmother00Status;
            std::vector<int>* M_MuonGenMatchGrandmother01Id;
            std::vector<int>* M_MuonGenMatchGrandmother01Status;
            std::vector<int>* M_MuonGenMatchGrandmother10Id;
            std::vector<int>* M_MuonGenMatchGrandmother10Status;
            std::vector<int>* M_MuonGenMatchGrandmother11Id;
            std::vector<int>* M_MuonGenMatchGrandmother11Status;
            std::vector<int>* M_MuonGenMatchId;
            std::vector<int>* M_MuonGenMatchMother0Id;
            std::vector<int>* M_MuonGenMatchMother0Status;
            std::vector<int>* M_MuonGenMatchMother1Id;
            std::vector<int>* M_MuonGenMatchMother1Status;
            std::vector<int>* M_MuonGenMatchStatus;
            Int_t M_NumMuons;
            std::vector<float>* M_Phi;
            std::vector<float>* M_Pt;
            std::vector<float>* M_RelIso;
            std::vector<float>* CSVeventWeight;
            std::vector<float>* CSVeventWeightCErr1down;
            std::vector<float>* CSVeventWeightCErr1up;
            std::vector<float>* CSVeventWeightCErr2down;
            std::vector<float>* CSVeventWeightCErr2up;
            std::vector<float>* CSVeventWeightHFStats1down;
            std::vector<float>* CSVeventWeightHFStats1up;
            std::vector<float>* CSVeventWeightHFStats2down;
            std::vector<float>* CSVeventWeightHFStats2up;
            std::vector<float>* CSVeventWeightHFdown;
            std::vector<float>* CSVeventWeightHFup;
            std::vector<float>* CSVeventWeightLFStats1down;
            std::vector<float>* CSVeventWeightLFStats1up;
            std::vector<float>* CSVeventWeightLFStats2down;
            std::vector<float>* CSVeventWeightLFStats2up;
            std::vector<float>* CSVeventWeightLFdown;
            std::vector<float>* CSVeventWeightLFup;
            std::vector<std::vector<unsigned int> >* CleanJetIndices;
            std::vector<float>* HT;
            std::vector<int>* MomentumRank;
            std::vector<unsigned int>* NumCSVLbtagJets;
            std::vector<unsigned int>* NumCSVMbtagJets;
            std::vector<unsigned int>* NumCSVTbtagJets;
            std::vector<unsigned int>* NumCleanCSVLbtagJets;
            std::vector<unsigned int>* NumCleanCSVMbtagJets;
            std::vector<unsigned int>* NumCleanCSVTbtagJets;
            std::vector<unsigned int>* NumCleanNonCSVLbtagJets;
            std::vector<unsigned int>* NumCleanNonCSVMbtagJets;
            std::vector<unsigned int>* NumCleanNonCSVTbtagJets;
            UInt_t NumCombos;
            std::vector<unsigned int>* NumExLooseElectrons;
            std::vector<unsigned int>* NumExLooseMuons;
            std::vector<unsigned int>* NumLooseElectrons;
            std::vector<unsigned int>* NumLooseMuons;
            std::vector<unsigned int>* NumNonCSVLbtagJets;
            std::vector<unsigned int>* NumNonCSVMbtagJets;
            std::vector<unsigned int>* NumNonCSVTbtagJets;
            UInt_t NumTaus;
            std::vector<unsigned int>* NumTightElectrons;
            std::vector<unsigned int>* NumTightMuons;
            std::vector<float>* T_Charge;
            std::vector<unsigned int>* T_DecayMode;
            std::vector<float>* T_EmFraction;
            std::vector<float>* T_Eta;
            std::vector<bool>* T_HPSagainstElectronDeadECAL;
            std::vector<bool>* T_HPSagainstElectronLoose;
            std::vector<bool>* T_HPSagainstElectronLooseMVA2;
            std::vector<bool>* T_HPSagainstElectronLooseMVA3;
            std::vector<bool>* T_HPSagainstElectronMVA;
            std::vector<bool>* T_HPSagainstElectronMVA2category;
            std::vector<float>* T_HPSagainstElectronMVA2raw;
            std::vector<bool>* T_HPSagainstElectronMVA3category;
            std::vector<float>* T_HPSagainstElectronMVA3raw;
            std::vector<bool>* T_HPSagainstElectronMedium;
            std::vector<bool>* T_HPSagainstElectronMediumMVA2;
            std::vector<bool>* T_HPSagainstElectronMediumMVA3;
            std::vector<bool>* T_HPSagainstElectronTight;
            std::vector<bool>* T_HPSagainstElectronTightMVA2;
            std::vector<bool>* T_HPSagainstElectronTightMVA3;
            std::vector<bool>* T_HPSagainstElectronVLooseMVA2;
            std::vector<bool>* T_HPSagainstElectronVTightMVA3;
            std::vector<bool>* T_HPSagainstMuonLoose;
            std::vector<bool>* T_HPSagainstMuonLoose2;
            std::vector<bool>* T_HPSagainstMuonMedium;
            std::vector<bool>* T_HPSagainstMuonMedium2;
            std::vector<bool>* T_HPSagainstMuonTight;
            std::vector<bool>* T_HPSagainstMuonTight2;
            std::vector<float>* T_HPSbyCombinedIsolationDeltaBetaCorrRaw;
            std::vector<float>* T_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits;
            std::vector<float>* T_HPSbyIsolationMVA2raw;
            std::vector<float>* T_HPSbyIsolationMVAraw;
            std::vector<bool>* T_HPSbyLooseCombinedIsolationDeltaBetaCorr;
            std::vector<bool>* T_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits;
            std::vector<bool>* T_HPSbyLooseIsolationMVA;
            std::vector<bool>* T_HPSbyLooseIsolationMVA2;
            std::vector<bool>* T_HPSbyMediumCombinedIsolationDeltaBetaCorr;
            std::vector<bool>* T_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits;
            std::vector<bool>* T_HPSbyMediumIsolationMVA;
            std::vector<bool>* T_HPSbyMediumIsolationMVA2;
            std::vector<bool>* T_HPSbyTightCombinedIsolationDeltaBetaCorr;
            std::vector<bool>* T_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits;
            std::vector<bool>* T_HPSbyTightIsolationMVA;
            std::vector<bool>* T_HPSbyTightIsolationMVA2;
            std::vector<bool>* T_HPSbyVLooseCombinedIsolationDeltaBetaCorr;
            std::vector<bool>* T_HPSdecayModeFinding;
            std::vector<bool>* T_IsInTheCracks;
            std::vector<float>* T_LTIpVtdxy;
            std::vector<float>* T_LTIpVtdxyError;
            std::vector<float>* T_LTIpVtdz;
            std::vector<float>* T_LTIpVtdzError;
            std::vector<float>* T_LTNormChiSqrd;
            std::vector<float>* T_LTPt;
            std::vector<unsigned int>* T_LTValidHits;
            std::vector<bool>* T_LTvalid;
            std::vector<float>* T_LTvx;
            std::vector<float>* T_LTvy;
            std::vector<float>* T_LTvz;
            std::vector<float>* T_MomentumRank;
            std::vector<unsigned int>* T_NProngs;
            std::vector<unsigned int>* T_NSignalGammas;
            std::vector<unsigned int>* T_NSignalNeutrals;
            Int_t T_NumTaus;
            std::vector<float>* T_Phi;
            std::vector<float>* T_Pt;
            std::vector<int>* T_TauGenMatchDaughter0Id;
            std::vector<int>* T_TauGenMatchDaughter0Status;
            std::vector<int>* T_TauGenMatchDaughter1Id;
            std::vector<int>* T_TauGenMatchDaughter1Status;
            std::vector<int>* T_TauGenMatchGrandmother00Id;
            std::vector<int>* T_TauGenMatchGrandmother00Status;
            std::vector<int>* T_TauGenMatchGrandmother01Id;
            std::vector<int>* T_TauGenMatchGrandmother01Status;
            std::vector<int>* T_TauGenMatchGrandmother10Id;
            std::vector<int>* T_TauGenMatchGrandmother10Status;
            std::vector<int>* T_TauGenMatchGrandmother11Id;
            std::vector<int>* T_TauGenMatchGrandmother11Status;
            std::vector<int>* T_TauGenMatchId;
            std::vector<int>* T_TauGenMatchMother0Id;
            std::vector<int>* T_TauGenMatchMother0Status;
            std::vector<int>* T_TauGenMatchMother1Id;
            std::vector<int>* T_TauGenMatchMother1Status;
            std::vector<int>* T_TauGenMatchStatus;
            std::vector<float>* V_NormChiSquared;
            std::vector<float>* V_NumDOF;
            Double_t V_NumInteractionsBX0;
            Double_t V_NumInteractionsBXm1;
            Double_t V_NumInteractionsBXp1;
            Int_t V_NumVertices;
            std::vector<float>* V_Rho;
            std::vector<float>* V_Xcoord;
            std::vector<float>* V_XcoordErr;
            std::vector<float>* V_Ycoord;
            std::vector<float>* V_YcoordErr;
            std::vector<float>* V_Zcoord;
            std::vector<float>* V_ZcoordErr;
            // >>> End declarations <<<

        ClassDef(Branches, 1);
    };
}

#endif
