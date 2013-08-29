// vim: ts=4:sw=4:et:sta
#include "TFile.h"

#include "../interface/GenHelper.h"
#include "../interface/MVABase.h"
#include "../interface/TTLBranches.h"

using namespace std;
using namespace roast;

// Default constructor
ttl::Branches::Branches() : roast::Branches(), conesize(.25), jetIndexCacheCombo(-1) {
    Null();
}

ttl::Branches::Branches(const std::string& tree_name, const vector<string>& paths) :
        roast::Branches(tree_name, paths), conesize(.25), jetIndexCacheCombo(-1)
{
    Null();
    SetBranchAddresses();
}

ttl::Branches::~Branches()
{
    Delete();
    Null();
}

void
ttl::Branches::GetEntry(double i)
{
    fChain->GetEntry(i);
    jetIndexCacheCombo = -1;
}

bool
ttl::Branches::IsGoodGenMatch(const int& idx) const {
    return (abs((*TTL_Tau1GenMatchId)[idx]) == 15 &&
            abs((*TTL_Tau2GenMatchId)[idx]) == 15 &&
            abs((*TTL_Tau1GenMatchMother0Id)[idx]) == 25 &&
            abs((*TTL_Tau2GenMatchMother0Id)[idx]) == 25);
}

unsigned int ttl::Branches::GetTau1IsolationIndex(const unsigned int iCombo) const {
	if((*TTL_Tau1HPSbyTightCombinedIsolationDeltaBetaCorr)[iCombo]){	return 4; }
	if((*TTL_Tau1HPSbyMediumCombinedIsolationDeltaBetaCorr)[iCombo]){	return 3; }
	if((*TTL_Tau1HPSbyLooseCombinedIsolationDeltaBetaCorr)[iCombo]){	return 2; }
	if((*TTL_Tau1HPSbyVLooseCombinedIsolationDeltaBetaCorr)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2IsolationIndex(const unsigned int iCombo) const {
	if((*TTL_Tau2HPSbyTightCombinedIsolationDeltaBetaCorr)[iCombo]){	return 4; }
	if((*TTL_Tau2HPSbyMediumCombinedIsolationDeltaBetaCorr)[iCombo]){	return 3; }
	if((*TTL_Tau2HPSbyLooseCombinedIsolationDeltaBetaCorr)[iCombo]){	return 2; }
	if((*TTL_Tau2HPSbyVLooseCombinedIsolationDeltaBetaCorr)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau1IsolationIndex3Hits(const unsigned int iCombo) const {
	if((*TTL_Tau1HPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){	return 3; }
	if((*TTL_Tau1HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){	return 2; }
	if((*TTL_Tau1HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2IsolationIndex3Hits(const unsigned int iCombo) const {
	if((*TTL_Tau2HPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){	return 3; }
	if((*TTL_Tau2HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){	return 2; }
	if((*TTL_Tau2HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau1IsolationIndexMVA(const unsigned int iCombo) const {
	if((*TTL_Tau1HPSbyTightIsolationMVA)[iCombo]){	return 3; }
	if((*TTL_Tau1HPSbyMediumIsolationMVA)[iCombo]){	return 2; }
	if((*TTL_Tau1HPSbyLooseIsolationMVA)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2IsolationIndexMVA(const unsigned int iCombo) const {
	if((*TTL_Tau2HPSbyTightIsolationMVA)[iCombo]){	return 3; }
	if((*TTL_Tau2HPSbyMediumIsolationMVA)[iCombo]){	return 2; }
	if((*TTL_Tau2HPSbyLooseIsolationMVA)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau1IsolationIndexMVA2(const unsigned int iCombo) const {
	if((*TTL_Tau1HPSbyTightIsolationMVA2)[iCombo]){	return 3; }
	if((*TTL_Tau1HPSbyMediumIsolationMVA2)[iCombo]){	return 2; }
	if((*TTL_Tau1HPSbyLooseIsolationMVA2)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2IsolationIndexMVA2(const unsigned int iCombo) const {
	if((*TTL_Tau2HPSbyTightIsolationMVA2)[iCombo]){	return 3; }
	if((*TTL_Tau2HPSbyMediumIsolationMVA2)[iCombo]){	return 2; }
	if((*TTL_Tau2HPSbyLooseIsolationMVA2)[iCombo]){	return 1; }
	return 0;
}


unsigned int ttl::Branches::GetTau1AntiElectronIndex(const unsigned int iCombo) const {
	if((*TTL_Tau1HPSagainstElectronTight)[iCombo]){	return 3; }
	if((*TTL_Tau1HPSagainstElectronMedium)[iCombo]){	return 2; }
	if((*TTL_Tau1HPSagainstElectronLoose)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2AntiElectronIndex(const unsigned int iCombo) const {
	if((*TTL_Tau2HPSagainstElectronTight)[iCombo]){	return 3; }
	if((*TTL_Tau2HPSagainstElectronMedium)[iCombo]){	return 2; }
	if((*TTL_Tau2HPSagainstElectronLoose)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau1AntiElectronIndexMVA2(const unsigned int iCombo) const {
	if((*TTL_Tau1HPSagainstElectronTightMVA2)[iCombo]){	return 4; }
	if((*TTL_Tau1HPSagainstElectronMediumMVA2)[iCombo]){	return 3; }
	if((*TTL_Tau1HPSagainstElectronLooseMVA2)[iCombo]){	return 2; }
	if((*TTL_Tau1HPSagainstElectronVLooseMVA2)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2AntiElectronIndexMVA2(const unsigned int iCombo) const {
	if((*TTL_Tau2HPSagainstElectronTightMVA2)[iCombo]){	return 4; }
	if((*TTL_Tau2HPSagainstElectronMediumMVA2)[iCombo]){	return 3; }
	if((*TTL_Tau2HPSagainstElectronLooseMVA2)[iCombo]){	return 2; }
	if((*TTL_Tau2HPSagainstElectronVLooseMVA2)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau1AntiElectronIndexMVA3(const unsigned int iCombo) const {
	if((*TTL_Tau1HPSagainstElectronVTightMVA3)[iCombo]){	return 4; }
	if((*TTL_Tau1HPSagainstElectronTightMVA3)[iCombo]){	return 3; }
	if((*TTL_Tau1HPSagainstElectronMediumMVA3)[iCombo]){	return 2; }
	if((*TTL_Tau1HPSagainstElectronLooseMVA3)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2AntiElectronIndexMVA3(const unsigned int iCombo) const {
	if((*TTL_Tau2HPSagainstElectronVTightMVA3)[iCombo]){	return 4; }
	if((*TTL_Tau2HPSagainstElectronTightMVA3)[iCombo]){	return 3; }
	if((*TTL_Tau2HPSagainstElectronMediumMVA3)[iCombo]){	return 2; }
	if((*TTL_Tau2HPSagainstElectronLooseMVA3)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau1AntiMuonIndex(const unsigned int iCombo) const {
	if((*TTL_Tau1HPSagainstMuonTight)[iCombo]){	return 3; }
	if((*TTL_Tau1HPSagainstMuonMedium)[iCombo]){	return 2; }
	if((*TTL_Tau1HPSagainstMuonLoose)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2AntiMuonIndex(const unsigned int iCombo) const {
	if((*TTL_Tau2HPSagainstMuonTight)[iCombo]){	return 3; }
	if((*TTL_Tau2HPSagainstMuonMedium)[iCombo]){	return 2; }
	if((*TTL_Tau2HPSagainstMuonLoose)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau1AntiMuonIndex2(const unsigned int iCombo) const {
	if((*TTL_Tau1HPSagainstMuonTight2)[iCombo]){	return 3; }
	if((*TTL_Tau1HPSagainstMuonMedium2)[iCombo]){	return 2; }
	if((*TTL_Tau1HPSagainstMuonLoose2)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2AntiMuonIndex2(const unsigned int iCombo) const {
	if((*TTL_Tau2HPSagainstMuonTight2)[iCombo]){	return 3; }
	if((*TTL_Tau2HPSagainstMuonMedium2)[iCombo]){	return 2; }
	if((*TTL_Tau2HPSagainstMuonLoose2)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau1MatchIndex(const unsigned int iCombo) const {
	if((*TTL_Tau1GenMatchId)[iCombo] == -99){  return 0; } // no match
	if(abs((*TTL_Tau1GenMatchId)[iCombo]) == 15){	return 2; } // tau
	if(abs((*TTL_Tau1GenMatchId)[iCombo]) == 13){	return 3; } // muon
	if(abs((*TTL_Tau1GenMatchId)[iCombo]) == 11){	return 4; } // electron
	if(abs((*TTL_Tau1GenMatchId)[iCombo]) == 22){	return 5; } // photon
	if(abs((*TTL_Tau1GenMatchId)[iCombo]) == 23){	return 6; } // Z
	if(abs((*TTL_Tau1GenMatchId)[iCombo]) == 24){	return 7; } // W
	if(abs((*TTL_Tau1GenMatchId)[iCombo]) == 25){	return 8; } // Higgs
	return 1; // hadronic
}

unsigned int ttl::Branches::GetTau2MatchIndex(const unsigned int iCombo) const {
	if((*TTL_Tau2GenMatchId)[iCombo] == -99){  return 0; } // no match
	if(abs((*TTL_Tau2GenMatchId)[iCombo]) == 15){	return 2; } // tau
	if(abs((*TTL_Tau2GenMatchId)[iCombo]) == 13){	return 3; } // muon
	if(abs((*TTL_Tau2GenMatchId)[iCombo]) == 11){	return 4; } // electron
	if(abs((*TTL_Tau2GenMatchId)[iCombo]) == 22){	return 5; } // photon
	if(abs((*TTL_Tau2GenMatchId)[iCombo]) == 23){	return 6; } // Z
	if(abs((*TTL_Tau2GenMatchId)[iCombo]) == 24){	return 7; } // W
	if(abs((*TTL_Tau2GenMatchId)[iCombo]) == 25){	return 8; } // Higgs
	return 1; // hadronic
}

unsigned int
ttl::Branches::GetJetIndex(const int idx, const unsigned int num)
{
    if (idx != jetIndexCacheCombo) {
        jetIndexCache.clear();
        for (unsigned int i = 0; i < J_Pt->size(); i++) {
            if ((DeltaR((*J_Eta)[i], (*J_Phi)[i], (*TTL_Tau1Eta)[idx], (*TTL_Tau1Phi)[idx]) > conesize) &&
                    (DeltaR((*J_Eta)[i], (*J_Phi)[i], (*TTL_Tau2Eta)[idx], (*TTL_Tau2Phi)[idx]) > conesize) &&
                    (DeltaR((*J_Eta)[i], (*J_Phi)[i], (*TTL_LeptonEta)[idx], (*TTL_LeptonPhi)[idx]) > conesize)) {
                jetIndexCache.push_back(i);
            }
        }
        jetIndexCacheCombo = idx;
    }
    return jetIndexCache.at(num);
}

void
ttl::Branches::FillHistograms(std::map<std::string, roast::HWrapper*>& histos, int idx, float weight)
{
#ifdef notdefined
    histos["Events"]->Fill(0, weight);
    histos["MomentumRank"]->Fill((*TTL_MomentumRank)[idx], weight);
    histos["MET"]->Fill(Ev_MET, weight);
    histos["HT"]->Fill((*TTL_HT)[idx], weight);

    if (auto mva = MVABase::gMVA["CFMlpANN"]) {
        histos["FinalCFMlpANN"]->Fill(mva->Evaluate(this, idx), weight);
    }

    if (auto mva = MVABase::gMVA["BDTG"]) {
        histos["FinalBDTG"]->Fill(mva->Evaluate(this, idx), weight);
    } else {
        std::cout << "Can't find BDTG" << std::endl;
    }

    if (auto mva = MVABase::gComboMVA["BDT"]) {
        histos["ComboBDT"]->Fill(mva->Evaluate(this, idx), weight);
    }

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
    // histos["ChargeProduct_TT"]->Fill(((*TTL_Tau1Charge)[idx] * (*TTL_Tau2Charge)[idx]), weight);

    // Taus ============================================================================================================================
    histos["NumTaus"]->Fill(TTL_NumTaus, weight);
    histos["VisibleMass_TT"]->Fill((*TTL_DitauVisibleMass)[idx], weight);
    histos["DeltaR_TT"]->Fill((*TTL_DitauDeltaR)[idx], weight);
    histos["Pt_T1"]->Fill((*TTL_Tau1Pt)[idx], weight);
    histos["Pt_T2"]->Fill((*TTL_Tau2Pt)[idx], weight);
    histos["Eta_T1"]->Fill((*TTL_Tau1Eta)[idx], weight);
    histos["Eta_T2"]->Fill((*TTL_Tau2Eta)[idx], weight);
    histos["Phi_T1"]->Fill((*TTL_Tau1Phi)[idx], weight);
    histos["Phi_T2"]->Fill((*TTL_Tau2Phi)[idx], weight);
    histos["DecayMode_T1"]->Fill((*TTL_Tau1DecayMode)[idx], weight);
    histos["DecayMode_T2"]->Fill((*TTL_Tau2DecayMode)[idx], weight);
    histos["LTPt_T1"]->Fill((*TTL_Tau1LTPt)[idx], weight);
    histos["LTPt_T2"]->Fill((*TTL_Tau2LTPt)[idx], weight);
    histos["NProngs_T1"]->Fill((*TTL_Tau1NProngs)[idx], weight);
    histos["NProngs_T2"]->Fill((*TTL_Tau2NProngs)[idx], weight);
    histos["IsolationIndex_T1"]->Fill(GetTau1IsolationIndex(idx), weight);
    histos["IsolationIndex_T2"]->Fill(GetTau2IsolationIndex(idx), weight);
    histos["IsolationIndexMVA_T1"]->Fill(GetTau1IsolationIndexMVA(idx), weight);
    histos["IsolationIndexMVA_T2"]->Fill(GetTau2IsolationIndexMVA(idx), weight);
    histos["IsolationIndexMVA2_T1"]->Fill(GetTau1IsolationIndexMVA2(idx), weight);
    histos["IsolationIndexMVA2_T2"]->Fill(GetTau2IsolationIndexMVA2(idx), weight);
    histos["IsolationIndex3Hits_T1"]->Fill(GetTau1IsolationIndex3Hits(idx), weight);
    histos["IsolationIndex3Hits_T2"]->Fill(GetTau2IsolationIndex3Hits(idx), weight);
    histos["CombIsoDBcorrRaw_T1"]->Fill((*TTL_Tau1HPSbyCombinedIsolationDeltaBetaCorrRaw)[idx], weight);
    histos["CombIsoDBcorrRaw_T2"]->Fill((*TTL_Tau2HPSbyCombinedIsolationDeltaBetaCorrRaw)[idx], weight);
    histos["CombIsoDBcorrRaw3Hits_T1"]->Fill((*TTL_Tau1HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx], weight);
    histos["CombIsoDBcorrRaw3Hits_T2"]->Fill((*TTL_Tau2HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx], weight);
    histos["IsolationMVAraw_T1"]->Fill((*TTL_Tau1HPSbyIsolationMVAraw)[idx], weight);
    histos["IsolationMVAraw_T2"]->Fill((*TTL_Tau2HPSbyIsolationMVAraw)[idx], weight);
    histos["IsolationMVA2raw_T1"]->Fill((*TTL_Tau1HPSbyIsolationMVA2raw)[idx], weight);
    histos["IsolationMVA2raw_T2"]->Fill((*TTL_Tau2HPSbyIsolationMVA2raw)[idx], weight);
    histos["AntiElectronIndex_T1"]->Fill(GetTau1AntiElectronIndex(idx), weight);
    histos["AntiElectronIndex_T2"]->Fill(GetTau2AntiElectronIndex(idx), weight);
    histos["AntiElectronIndexMVA2_T1"]->Fill(GetTau1AntiElectronIndexMVA2(idx), weight);
    histos["AntiElectronIndexMVA2_T2"]->Fill(GetTau2AntiElectronIndexMVA2(idx), weight);
    histos["AntiElectronIndexMVA3_T1"]->Fill(GetTau1AntiElectronIndexMVA3(idx), weight);
    histos["AntiElectronIndexMVA3_T2"]->Fill(GetTau2AntiElectronIndexMVA3(idx), weight);
    histos["AgainstElectronMVA_T1"]->Fill((*TTL_Tau1HPSagainstElectronMVA)[idx], weight);
    histos["AgainstElectronMVA_T2"]->Fill((*TTL_Tau2HPSagainstElectronMVA)[idx], weight);
    histos["AgainstElectronMVA2raw_T1"]->Fill((*TTL_Tau1HPSagainstElectronMVA2raw)[idx], weight);
    histos["AgainstElectronMVA2raw_T2"]->Fill((*TTL_Tau2HPSagainstElectronMVA2raw)[idx], weight);
    histos["AgainstElectronMVA3raw_T1"]->Fill((*TTL_Tau1HPSagainstElectronMVA3raw)[idx], weight);
    histos["AgainstElectronMVA3raw_T2"]->Fill((*TTL_Tau2HPSagainstElectronMVA3raw)[idx], weight);
    // histos["AgainstElectronMVA2category_T1"]->Fill((*TTL_Tau1HPSagainstElectronMVA2category)[idx], weight);
    // histos["AgainstElectronMVA2category_T2"]->Fill((*TTL_Tau2HPSagainstElectronMVA2category)[idx], weight);
    // histos["AgainstElectronMVA3category_T1"]->Fill((*TTL_Tau1HPSagainstElectronMVA3category)[idx], weight);
    // histos["AgainstElectronMVA3category_T2"]->Fill((*TTL_Tau2HPSagainstElectronMVA3category)[idx], weight);
    histos["AgainstElectronDeadECAL_T1"]->Fill((*TTL_Tau1HPSagainstElectronDeadECAL)[idx], weight);
    histos["AgainstElectronDeadECAL_T2"]->Fill((*TTL_Tau2HPSagainstElectronDeadECAL)[idx], weight);
    histos["AntiMuonIndex_T1"]->Fill(GetTau1AntiMuonIndex(idx), weight);
    histos["AntiMuonIndex_T2"]->Fill(GetTau2AntiMuonIndex(idx), weight);
    histos["AntiMuonIndex2_T1"]->Fill(GetTau1AntiMuonIndex2(idx), weight);
    histos["AntiMuonIndex2_T2"]->Fill(GetTau2AntiMuonIndex2(idx), weight);
    histos["Match_T1"]->Fill(GetTau1MatchIndex(idx), weight);
    histos["Match_T2"]->Fill(GetTau2MatchIndex(idx), weight);

    // Lepton ============================================================================================================================
    histos["Pt_L"]->Fill((*TTL_LeptonPt)[idx], weight);
    histos["Eta_L"]->Fill((*TTL_LeptonEta)[idx], weight);
    histos["Phi_L"]->Fill((*TTL_LeptonPhi)[idx], weight);
    // histos["NumTightMuons"]->Fill((*TTL_NumTightMuons)[idx], weight);
    // histos["NumLooseMuons"]->Fill((*TTL_NumLooseMuons)[idx], weight);
    // histos["NumExLooseMuons"]->Fill((*TTL_NumExLooseMuons)[idx], weight);
    // histos["NumTightElectrons"]->Fill((*TTL_NumTightElectrons)[idx], weight);
    // histos["NumLooseElectrons"]->Fill((*TTL_NumLooseElectrons)[idx], weight);
    // histos["NumExLooseElectrons"]->Fill((*TTL_NumExLooseElectrons)[idx], weight);
    // histos["NumTightLeptons"]->Fill((*TTL_NumTightMuons)[idx] + (*TTL_NumTightElectrons)[idx], weight);
    // histos["NumLooseLeptons"]->Fill((*TTL_NumLooseMuons)[idx] + (*TTL_NumLooseElectrons)[idx], weight);
    // histos["NumExLooseLeptons"]->Fill((*TTL_NumExLooseMuons)[idx] + (*TTL_NumExLooseElectrons)[idx], weight);
    histos["DeltaR_LT1"]->Fill(DeltaR((*TTL_LeptonEta)[idx], (*TTL_LeptonPhi)[idx], (*TTL_Tau1Eta)[idx], (*TTL_Tau1Phi)[idx]), weight);
    histos["DeltaR_LT2"]->Fill(DeltaR((*TTL_LeptonEta)[idx], (*TTL_LeptonPhi)[idx], (*TTL_Tau2Eta)[idx], (*TTL_Tau2Phi)[idx]), weight);

    // Jets ============================================================================================================================
    // tau/jet matching con size
    try {
        unsigned int ljet = GetJetIndex(idx, 0);
        histos["LeadingJet_Pt"]->Fill(J_Pt->at(ljet), weight);
        histos["LeadingJet_Eta"]->Fill(J_Eta->at(ljet), weight);
        histos["DeltaR_LJ1"]->Fill(DeltaR((*TTL_LeptonEta)[idx], (*TTL_LeptonPhi)[idx], J_Eta->at(ljet), J_Phi->at(ljet)), weight);
        histos["DeltaR_T1J1"]->Fill(DeltaR((*TTL_Tau1Eta)[idx], (*TTL_Tau1Phi)[idx], J_Eta->at(ljet), J_Phi->at(ljet)), weight);
        histos["DeltaR_T2J1"]->Fill(DeltaR((*TTL_Tau2Eta)[idx], (*TTL_Tau2Phi)[idx], J_Eta->at(ljet), J_Phi->at(ljet)), weight);

        unsigned int sljet = GetJetIndex(idx, 1);
        histos["SubLeadingJet_Pt"]->Fill(J_Pt->at(sljet), weight);
        histos["SubLeadingJet_Eta"]->Fill(J_Eta->at(sljet), weight);
        histos["DeltaR_LJ2"]->Fill(DeltaR((*TTL_LeptonEta)[idx], (*TTL_LeptonPhi)[idx], J_Eta->at(sljet), J_Phi->at(sljet)), weight);
        histos["DeltaR_T1J2"]->Fill(DeltaR((*TTL_Tau1Eta)[idx], (*TTL_Tau1Phi)[idx], J_Eta->at(sljet), J_Phi->at(sljet)), weight);
        histos["DeltaR_T2J2"]->Fill(DeltaR((*TTL_Tau2Eta)[idx], (*TTL_Tau2Phi)[idx], J_Eta->at(sljet), J_Phi->at(sljet)), weight);

        unsigned int ssljet = GetJetIndex(idx, 2);
        histos["SubSubLeadingJet_Pt"]->Fill(J_Pt->at(ssljet), weight);
        histos["SubSubLeadingJet_Eta"]->Fill(J_Eta->at(ssljet), weight);

        unsigned int sssljet = GetJetIndex(idx, 3);
        histos["SubSubLeadingJet_Pt"]->Fill(J_Pt->at(sssljet), weight);
        histos["SubSubLeadingJet_Eta"]->Fill(J_Eta->at(sssljet), weight);
    } catch (...) {}

    float conesize = .25;
    for (unsigned int i = 0, c = 0; c < 2 and i < J_Pt->size(); i++) {
        if ((DeltaR((*J_Eta)[i], (*J_Phi)[i], (*TTL_Tau1Eta)[idx], (*TTL_Tau1Phi)[idx]) > conesize) &&
                (DeltaR((*J_Eta)[i], (*J_Phi)[i], (*TTL_Tau2Eta)[idx], (*TTL_Tau2Phi)[idx]) > conesize) &&
                (DeltaR((*J_Eta)[i], (*J_Phi)[i], (*TTL_LeptonEta)[idx], (*TTL_LeptonPhi)[idx]) > conesize)) {
            if ((*J_combSecVtxMediumBTag)[i]) {
               histos["BTagJet_Pt"]->Fill((*J_Pt)[i], weight);
               histos["BTagJet_Eta"]->Fill((*J_Eta)[i], weight);
            } else {
               histos["NonBTagJet_Pt"]->Fill((*J_Pt)[i], weight);
               histos["NonBTagJet_Eta"]->Fill((*J_Eta)[i], weight);
            }

            ++c;
        }
    }

    histos["NumCSVLbtags"]->Fill((*TTL_NumCSVLbtagJets)[idx], weight);
    histos["NumCSVMbtags"]->Fill((*TTL_NumCSVMbtagJets)[idx], weight);
    histos["NumCSVTbtags"]->Fill((*TTL_NumCSVTbtagJets)[idx], weight);
    histos["NumCSVLnonBtags"]->Fill((*TTL_NumNonCSVLbtagJets)[idx], weight);
    histos["NumCSVMnonBtags"]->Fill((*TTL_NumNonCSVMbtagJets)[idx], weight);
    histos["NumCSVTnonBtags"]->Fill((*TTL_NumNonCSVTbtagJets)[idx], weight);
    histos["NumCSVnonHTauJets"]->Fill(((*TTL_NumNonCSVMbtagJets)[idx] + (*TTL_NumCSVMbtagJets)[idx]), weight);
    histos["NumCleanCSVLbtags"]->Fill((*TTL_NumCleanCSVLbtagJets)[idx], weight);
    histos["NumCleanCSVMbtags"]->Fill((*TTL_NumCleanCSVMbtagJets)[idx], weight);
    histos["NumCleanCSVTbtags"]->Fill((*TTL_NumCleanCSVTbtagJets)[idx], weight);
    histos["NumCleanCSVLnonBtags"]->Fill((*TTL_NumCleanNonCSVLbtagJets)[idx], weight);
    histos["NumCleanCSVMnonBtags"]->Fill((*TTL_NumCleanNonCSVMbtagJets)[idx], weight);
    histos["NumCleanCSVTnonBtags"]->Fill((*TTL_NumCleanNonCSVTbtagJets)[idx], weight);
    histos["NumCleanCSVnonHTauJets"]->Fill(((*TTL_NumCleanNonCSVMbtagJets)[idx] + (*TTL_NumCleanCSVMbtagJets)[idx]), weight);
    histos["NumInclusiveJets"]->Fill(((*TTL_NumCleanNonCSVMbtagJets)[idx] + (*TTL_NumCleanCSVMbtagJets)[idx]) + 2, weight);
#endif
}

ClassImp(roast::ttl::Branches)
