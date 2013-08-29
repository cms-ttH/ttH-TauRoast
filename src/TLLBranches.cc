// vim: ts=4:sw=4:et:sta
#include "TFile.h"

#include "../interface/Helper.h"
#include "../interface/MVABase.h"
#include "../interface/TLLBranches.h"

using namespace std;
using namespace roast;

// Default constructor
tll::Branches::Branches() : roast::Branches(), conesize(.25), jetIndexCacheCombo(-1) {
    Null();
}

tll::Branches::Branches(const std::string& tree_name, const vector<string>& paths) :
    roast::Branches(tree_name, paths), conesize(.25), jetIndexCacheCombo(-1)
{
    Null();
    SetBranchAddresses();
}

tll::Branches::~Branches()
{
    Delete();
    Null();
}

    void
tll::Branches::GetEntry(double i)
{
    fChain->GetEntry(i);
    jetIndexCacheCombo = -1;
}

bool
tll::Branches::IsGoodGenMatch(const int& idx) const {
    return (abs((*TLL_TauGenMatchId)[idx]) == 15 &&
            abs((*TLL_TauGenMatchMother0Id)[idx]) == 25);
}

unsigned int tll::Branches::GetTauIsolationIndex(const unsigned int iCombo) const {
    if((*TLL_TauHPSbyTightCombinedIsolationDeltaBetaCorr)[iCombo]){ return 4; }
    if((*TLL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr)[iCombo]){ return 3; }
    if((*TLL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr)[iCombo]){ return 2; }
    if((*TLL_TauHPSbyVLooseCombinedIsolationDeltaBetaCorr)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauIsolationIndex3Hits(const unsigned int iCombo) const {
    if((*TLL_TauHPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){ return 3; }
    if((*TLL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){ return 2; }
    if((*TLL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauIsolationIndexMVA(const unsigned int iCombo) const {
    if((*TLL_TauHPSbyTightIsolationMVA)[iCombo]){ return 3; }
    if((*TLL_TauHPSbyMediumIsolationMVA)[iCombo]){ return 2; }
    if((*TLL_TauHPSbyLooseIsolationMVA)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauIsolationIndexMVA2(const unsigned int iCombo) const {
    if((*TLL_TauHPSbyTightIsolationMVA2)[iCombo]){ return 3; }
    if((*TLL_TauHPSbyMediumIsolationMVA2)[iCombo]){ return 2; }
    if((*TLL_TauHPSbyLooseIsolationMVA2)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauAntiElectronIndex(const unsigned int iCombo) const {
    if((*TLL_TauHPSagainstElectronTight)[iCombo]){ return 3; }
    if((*TLL_TauHPSagainstElectronMedium)[iCombo]){ return 2; }
    if((*TLL_TauHPSagainstElectronLoose)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauAntiElectronIndexMVA2(const unsigned int iCombo) const {
    if((*TLL_TauHPSagainstElectronTightMVA2)[iCombo]){ return 4; }
    if((*TLL_TauHPSagainstElectronMediumMVA2)[iCombo]){ return 3; }
    if((*TLL_TauHPSagainstElectronLooseMVA2)[iCombo]){ return 2; }
    if((*TLL_TauHPSagainstElectronVLooseMVA2)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauAntiElectronIndexMVA3(const unsigned int iCombo) const {
    if((*TLL_TauHPSagainstElectronVTightMVA3)[iCombo]){ return 4; }
    if((*TLL_TauHPSagainstElectronTightMVA3)[iCombo]){ return 3; }
    if((*TLL_TauHPSagainstElectronMediumMVA3)[iCombo]){ return 2; }
    if((*TLL_TauHPSagainstElectronLooseMVA3)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauAntiMuonIndex(const unsigned int iCombo) const {
    if((*TLL_TauHPSagainstMuonTight)[iCombo]){ return 3; }
    if((*TLL_TauHPSagainstMuonMedium)[iCombo]){ return 2; }
    if((*TLL_TauHPSagainstMuonLoose)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauAntiMuonIndex2(const unsigned int iCombo) const {
    if((*TLL_TauHPSagainstMuonTight2)[iCombo]){ return 3; }
    if((*TLL_TauHPSagainstMuonMedium2)[iCombo]){ return 2; }
    if((*TLL_TauHPSagainstMuonLoose2)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauMatchIndex(const unsigned int iCombo) const {
    if((*TLL_TauGenMatchId)[iCombo] == -99){ return 0; } // no match
    if(abs((*TLL_TauGenMatchId)[iCombo]) == 15){ return 2; } // tau
    if(abs((*TLL_TauGenMatchId)[iCombo]) == 13){ return 3; } // muon
    if(abs((*TLL_TauGenMatchId)[iCombo]) == 11){ return 4; } // electron
    if(abs((*TLL_TauGenMatchId)[iCombo]) == 22){ return 5; } // photon
    if(abs((*TLL_TauGenMatchId)[iCombo]) == 23){ return 6; } // Z
    if(abs((*TLL_TauGenMatchId)[iCombo]) == 24){ return 7; } // W
    if(abs((*TLL_TauGenMatchId)[iCombo]) == 25){ return 8; } // Higgs
    return 1; // hadronic
}

void
tll::Branches::FillHistograms(std::map<std::string, roast::HWrapper*>& histos, int idx, float weight)
{
#ifdef notdefined
    histos["Events"]->Fill(0, weight);
    histos["MomentumRank"]->Fill((*TLL_MomentumRank)[idx], weight);
    histos["MET"]->Fill(Ev_MET, weight);
    histos["HT"]->Fill((*TLL_HT)[idx], weight);
    if (auto mva = MVABase::gMVA["CFMlpANN"])
        histos["FinalCFMlpANN"]->Fill(mva->Evaluate(this, idx), weight);
    if (auto mva = MVABase::gMVA["BDTG"])
        histos["FinalBDTG"]->Fill(mva->Evaluate(this, idx), weight);
    if (auto mva = MVABase::gComboMVA["BDT"])
        histos["ComboBDT"]->Fill(mva->Evaluate(this, idx), weight);
    histos["SelectedComboRank"]->Fill(idx, weight);

    histos["NumberPV_noPURW"]->Fill(V_NumVertices, weight/Ev_puWeight);
    histos["NumberBXm1_noPURW"]->Fill(V_NumInteractionsBXm1, weight/Ev_puWeight);
    histos["NumberBX0_noPURW"]->Fill(V_NumInteractionsBX0, weight/Ev_puWeight);
    histos["NumberBXp1_noPURW"]->Fill(V_NumInteractionsBXp1, weight/Ev_puWeight);
    histos["PUweights_noPURW"]->Fill(Ev_puWeight, weight/Ev_puWeight);

    histos["NumberPV_afterPURW"]->Fill(V_NumVertices, weight);
    histos["NumberBXm1_afterPURW"]->Fill(V_NumInteractionsBXm1, weight);
    histos["NumberBX0_afterPURW"]->Fill(V_NumInteractionsBX0, weight);
    histos["NumberBXp1_afterPURW"]->Fill(V_NumInteractionsBXp1, weight);
    histos["PUweights_afterPURW"]->Fill(Ev_puWeight, weight);

    for( vector<float>::const_iterator vertexIt = V_Zcoord->begin(); vertexIt != V_Zcoord->end(); ++vertexIt ) {
        histos["VertexZcoord_afterPURW"]->Fill(*vertexIt,weight);
    }

    // Taus ============================================================================================================================
    histos["NumTaus"]->Fill(TLL_NumTaus, weight);
    histos["Pt_T"]->Fill((*TLL_TauPt)[idx], weight);
    histos["Eta_T"]->Fill((*TLL_TauEta)[idx], weight);
    histos["Phi_T"]->Fill((*TLL_TauPhi)[idx], weight);
    histos["DecayMode_T"]->Fill((*TLL_TauDecayMode)[idx], weight);
    histos["LTPt_T"]->Fill((*TLL_TauLTPt)[idx], weight);
    histos["NProngs_T"]->Fill((*TLL_TauNProngs)[idx], weight);
    histos["IsolationIndex_T"]->Fill(GetTauIsolationIndex(idx), weight);
    histos["IsolationIndexMVA_T"]->Fill(GetTauIsolationIndexMVA(idx), weight);
    histos["IsolationIndexMVA2_T"]->Fill(GetTauIsolationIndexMVA2(idx), weight);
    histos["IsolationIndex3Hits_T"]->Fill(GetTauIsolationIndex3Hits(idx), weight);
    histos["CombIsoDBcorrRaw_T"]->Fill((*TLL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw)[idx], weight);
    histos["CombIsoDBcorrRaw3Hits_T"]->Fill((*TLL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx], weight);
    histos["IsolationMVAraw_T"]->Fill((*TLL_TauHPSbyIsolationMVAraw)[idx], weight);
    histos["IsolationMVA2raw_T"]->Fill((*TLL_TauHPSbyIsolationMVA2raw)[idx], weight);
    histos["AntiElectronIndex_T"]->Fill(GetTauAntiElectronIndex(idx), weight);
    histos["AntiElectronIndexMVA2_T"]->Fill(GetTauAntiElectronIndexMVA2(idx), weight);
    histos["AntiElectronIndexMVA3_T"]->Fill(GetTauAntiElectronIndexMVA3(idx), weight);
    histos["AgainstElectronMVA_T"]->Fill((*TLL_TauHPSagainstElectronMVA)[idx], weight);
    histos["AgainstElectronMVA2raw_T"]->Fill((*TLL_TauHPSagainstElectronMVA2raw)[idx], weight);
    histos["AgainstElectronMVA3raw_T"]->Fill((*TLL_TauHPSagainstElectronMVA3raw)[idx], weight);
    histos["AgainstElectronMVA2category_T"]->Fill((*TLL_TauHPSagainstElectronMVA2category)[idx], weight);
    histos["AgainstElectronMVA3category_T"]->Fill((*TLL_TauHPSagainstElectronMVA3category)[idx], weight);
    histos["AgainstElectronDeadECAL_T"]->Fill((*TLL_TauHPSagainstElectronDeadECAL)[idx], weight);
    histos["AntiMuonIndex_T"]->Fill(GetTauAntiMuonIndex(idx), weight);
    histos["AntiMuonIndex2_T"]->Fill(GetTauAntiMuonIndex2(idx), weight);
    histos["Match_T"]->Fill(GetTauMatchIndex(idx), weight);

    // Lepton1 ============================================================================================================================
    histos["Pt_L1"]->Fill((*TLL_Lepton1Pt)[idx], weight);
    histos["Eta_L1"]->Fill((*TLL_Lepton1Eta)[idx], weight);
    histos["Phi_L1"]->Fill((*TLL_Lepton1Phi)[idx], weight);
    histos["NumTightMuons"]->Fill((*TLL_NumTightMuons)[idx], weight);
    histos["NumLooseMuons"]->Fill((*TLL_NumLooseMuons)[idx], weight);
    histos["NumExLooseMuons"]->Fill((*TLL_NumExLooseMuons)[idx], weight);
    histos["NumTightElectrons"]->Fill((*TLL_NumTightElectrons)[idx], weight);
    histos["NumLooseElectrons"]->Fill((*TLL_NumLooseElectrons)[idx], weight);
    histos["NumExLooseElectrons"]->Fill((*TLL_NumExLooseElectrons)[idx], weight);
    histos["NumTightLeptons"]->Fill((*TLL_NumTightMuons)[idx] + (*TLL_NumTightElectrons)[idx] , weight);
    histos["NumLooseLeptons"]->Fill((*TLL_NumLooseMuons)[idx] + (*TLL_NumLooseElectrons)[idx] , weight);
    histos["NumExLooseLeptons"]->Fill((*TLL_NumExLooseMuons)[idx] + (*TLL_NumExLooseElectrons)[idx] , weight);
    histos["DeltaR_L1T"]->Fill(DeltaR((*TLL_Lepton1Eta)[idx], (*TLL_Lepton1Phi)[idx], (*TLL_TauEta)[idx], (*TLL_TauPhi)[idx]), weight);

    // Lepton2 ============================================================================================================================
    histos["Pt_L2"]->Fill((*TLL_Lepton2Pt)[idx], weight);
    histos["Eta_L2"]->Fill((*TLL_Lepton2Eta)[idx], weight);
    histos["Phi_L2"]->Fill((*TLL_Lepton2Phi)[idx], weight);
    histos["NumTightMuons"]->Fill((*TLL_NumTightMuons)[idx], weight);
    histos["NumLooseMuons"]->Fill((*TLL_NumLooseMuons)[idx], weight);
    histos["NumExLooseMuons"]->Fill((*TLL_NumExLooseMuons)[idx], weight);
    histos["NumTightElectrons"]->Fill((*TLL_NumTightElectrons)[idx], weight);
    histos["NumLooseElectrons"]->Fill((*TLL_NumLooseElectrons)[idx], weight);
    histos["NumExLooseElectrons"]->Fill((*TLL_NumExLooseElectrons)[idx], weight);
    histos["NumTightLeptons"]->Fill((*TLL_NumTightMuons)[idx] + (*TLL_NumTightElectrons)[idx] , weight);
    histos["NumLooseLeptons"]->Fill((*TLL_NumLooseMuons)[idx] + (*TLL_NumLooseElectrons)[idx] , weight);
    histos["NumExLooseLeptons"]->Fill((*TLL_NumExLooseMuons)[idx] + (*TLL_NumExLooseElectrons)[idx] , weight);
    histos["DeltaR_L2T"]->Fill(DeltaR((*TLL_Lepton2Eta)[idx], (*TLL_Lepton2Phi)[idx], (*TLL_TauEta)[idx], (*TLL_TauPhi)[idx]), weight);
#endif
}

ClassImp(roast::tll::Branches)
