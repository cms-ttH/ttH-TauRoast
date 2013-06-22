#include "TFile.h"

#include "../interface/Branches.h"
#include "../interface/GenHelper.h"
#include "../interface/MVABase.h"

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

void ttl::Branches::Null(){
	#include "TTLBranches_null.h"
}

void ttl::Branches::Delete(){
	#include "TTLBranches_delete.h"
}

void ttl::Branches::Clear(){
	#include "TTLBranches_clear.h"
}

void ttl::Branches::SetBranchAddresses(){
	#include "TTLBranches_setBranchAddress.h"
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
ttl::Branches::FillHistograms(std::map<std::string, roast::HWrapper*>& histos, int idx, float weight, float pu_weight)
{
    // float weight	= iLeptonWeight*iPuWeight*iTau1TriggerWeight*iTau2TriggerWeight*iTauIdSysWeight*iQ2SysWeight*iTopPtWeight;

    // GenMatching =========================================================================================================================
    //histos["MatchedTo_T1"]->Fill(Tau1MatchedTo(idx) , weight);
    //histos["MatchedTo_T2"]->Fill(Tau2MatchedTo(idx) , weight);

    // General =========================================================================================================================
    auto h = histos.end();
    if ((h = histos.find("Events")) != histos.end()) {
        h->second->Fill(0, weight);
    } else {
        std::cout << "Can't find histogram Events" << std::endl;
    }
    if ((h = histos.find("MomentumRank")) != histos.end()) {
        h->second->Fill((*TTL_MomentumRank)[idx], weight);
    } else {
        std::cout << "Can't find histogram MomentumRank" << std::endl;
    }
    if ((h = histos.find("MET")) != histos.end()) {
        h->second->Fill(Ev_MET, weight);
    } else {
        std::cout << "Can't find histogram MET" << std::endl;
    }
    if ((h = histos.find("HT")) != histos.end()) {
        h->second->Fill((*TTL_HT)[idx], weight);
    } else {
        std::cout << "Can't find histogram HT" << std::endl;
    }

    if (auto mva = MVABase::gMVA["CFMlpANN"]) {
        if ((h = histos.find("FinalCFMlpANN")) != histos.end()) {
            h->second->Fill(mva->Evaluate(this, idx), weight);
        } else {
            std::cout << "Can't find histogram FinalCFMlpANN" << std::endl;
        }
    }

    if (auto mva = MVABase::gMVA["BDTG"]) {
        if ((h = histos.find("FinalBDTG")) != histos.end()) {
            h->second->Fill(mva->Evaluate(this, idx), weight);
        } else {
            std::cout << "Can't find histogram FinalBDTG" << std::endl;
        }
    }

    if (auto mva = MVABase::gComboMVA["BDT"]) {
        if ((h = histos.find("ComboBDT")) != histos.end()) {
            h->second->Fill(mva->Evaluate(this, idx), weight);
        } else {
            std::cout << "Can't find histogram ComboBDT" << std::endl;
        }
    }

    if ((h = histos.find("SelectedComboRank")) != histos.end()) {
        h->second->Fill(idx, weight);
    } else {
        std::cout << "Can't find histogram SelectedComboRank" << std::endl;
    }

    if ((h = histos.find("NumberPV_noPURW")) != histos.end()) {
        h->second->Fill(V_NumVertices, weight/pu_weight);
    } else {
        std::cout << "Can't find histogram NumberPV_noPURW" << std::endl;
    }
    if ((h = histos.find("NumberBXm1_noPURW")) != histos.end()) {
        h->second->Fill(V_NumInteractionsBXm1, weight/pu_weight);
    } else {
        std::cout << "Can't find histogram NumberBXm1_noPURW" << std::endl;
    }
    if ((h = histos.find("NumberBX0_noPURW")) != histos.end()) {
        h->second->Fill(V_NumInteractionsBX0, weight/pu_weight);
    } else {
        std::cout << "Can't find histogram NumberBX0_noPURW" << std::endl;
    }
    if ((h = histos.find("NumberBXp1_noPURW")) != histos.end()) {
        h->second->Fill(V_NumInteractionsBXp1, weight/pu_weight);
    } else {
        std::cout << "Can't find histogram NumberBXp1_noPURW" << std::endl;
    }
    if ((h = histos.find("PUweights_noPURW")) != histos.end()) {
        h->second->Fill(pu_weight, weight/pu_weight);
    } else {
        std::cout << "Can't find histogram PUweights_noPURW" << std::endl;
    }
    if ((h = histos.find("NumberPV_afterPURW")) != histos.end()) {
        h->second->Fill(V_NumVertices, weight);
    } else {
        std::cout << "Can't find histogram NumberPV_afterPURW" << std::endl;
    }
    if ((h = histos.find("NumberBXm1_afterPURW")) != histos.end()) {
        h->second->Fill(V_NumInteractionsBXm1, weight);
    } else {
        std::cout << "Can't find histogram NumberBXm1_afterPURW" << std::endl;
    }
    if ((h = histos.find("NumberBX0_afterPURW")) != histos.end()) {
        h->second->Fill(V_NumInteractionsBX0, weight);
    } else {
        std::cout << "Can't find histogram NumberBX0_afterPURW" << std::endl;
    }
    if ((h = histos.find("NumberBXp1_afterPURW")) != histos.end()) {
        h->second->Fill(V_NumInteractionsBXp1, weight);
    } else {
        std::cout << "Can't find histogram NumberBXp1_afterPURW" << std::endl;
    }
    if ((h = histos.find("PUweights_afterPURW")) != histos.end()) {
        h->second->Fill(pu_weight, weight);
    } else {
        std::cout << "Can't find histogram PUweights_afterPURW" << std::endl;
    }
    
    for( vector<float>::const_iterator vertexIt = V_Zcoord->begin(); vertexIt != V_Zcoord->end(); ++vertexIt ) {
        if ((h = histos.find("VertexZcoord_afterPURW")) != histos.end()) {
            h->second->Fill(*vertexIt,weight);
        } else {
            std::cout << "Can't find histogram VertexZcoord_afterPURW" << std::endl;
        }
    }
    if ((h = histos.find("ChargeProduct_TT")) != histos.end()) {
        h->second->Fill(((*TTL_Tau1Charge)[idx] * (*TTL_Tau2Charge)[idx]), weight);
    } else {
        std::cout << "Can't find histogram ChargeProduct_TT" << std::endl;
    }

    // Taus ============================================================================================================================
    if ((h = histos.find("NumTaus")) != histos.end()) {
        h->second->Fill(TTL_NumTaus, weight);
    } else {
        std::cout << "Can't find histogram NumTaus" << std::endl;
    }
    if ((h = histos.find("VisibleMass_TT")) != histos.end()) {
        h->second->Fill((*TTL_DitauVisibleMass)[idx], weight);
    } else {
        std::cout << "Can't find histogram VisibleMass_TT" << std::endl;
    }
    if ((h = histos.find("DeltaR_TT")) != histos.end()) {
        h->second->Fill((*TTL_DitauDeltaR)[idx], weight);
    } else {
        std::cout << "Can't find histogram DeltaR_TT" << std::endl;
    }
    if ((h = histos.find("Pt_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1Pt)[idx], weight);
    } else {
        std::cout << "Can't find histogram Pt_T1" << std::endl;
    }
    if ((h = histos.find("Pt_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2Pt)[idx], weight);
    } else {
        std::cout << "Can't find histogram Pt_T2" << std::endl;
    }
    if ((h = histos.find("Eta_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1Eta)[idx], weight);
    } else {
        std::cout << "Can't find histogram Eta_T1" << std::endl;
    }
    if ((h = histos.find("Eta_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2Eta)[idx], weight);
    } else {
        std::cout << "Can't find histogram Eta_T2" << std::endl;
    }
    if ((h = histos.find("Phi_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1Phi)[idx], weight);
    } else {
        std::cout << "Can't find histogram Phi_T1" << std::endl;
    }
    if ((h = histos.find("Phi_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2Phi)[idx], weight);
    } else {
        std::cout << "Can't find histogram Phi_T2" << std::endl;
    }
    if ((h = histos.find("DecayMode_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1DecayMode)[idx], weight);
    } else {
        std::cout << "Can't find histogram DecayMode_T1" << std::endl;
    }
    if ((h = histos.find("DecayMode_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2DecayMode)[idx], weight);
    } else {
        std::cout << "Can't find histogram DecayMode_T2" << std::endl;
    }
    if ((h = histos.find("LTPt_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1LTPt)[idx], weight);
    } else {
        std::cout << "Can't find histogram LTPt_T1" << std::endl;
    }
    if ((h = histos.find("LTPt_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2LTPt)[idx], weight);
    } else {
        std::cout << "Can't find histogram LTPt_T2" << std::endl;
    }
    if ((h = histos.find("NProngs_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1NProngs)[idx], weight);
    } else {
        std::cout << "Can't find histogram NProngs_T1" << std::endl;
    }
    if ((h = histos.find("NProngs_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2NProngs)[idx], weight);
    } else {
        std::cout << "Can't find histogram NProngs_T2" << std::endl;
    }
    if ((h = histos.find("IsolationIndex_T1")) != histos.end()) {
        h->second->Fill(GetTau1IsolationIndex(idx), weight);
    } else {
        std::cout << "Can't find histogram IsolationIndex_T1" << std::endl;
    }
    if ((h = histos.find("IsolationIndex_T2")) != histos.end()) {
        h->second->Fill(GetTau2IsolationIndex(idx), weight);
    } else {
        std::cout << "Can't find histogram IsolationIndex_T2" << std::endl;
    }
    if ((h = histos.find("IsolationIndexMVA_T1")) != histos.end()) {
        h->second->Fill(GetTau1IsolationIndexMVA(idx), weight);
    } else {
        std::cout << "Can't find histogram IsolationIndexMVA_T1" << std::endl;
    }
    if ((h = histos.find("IsolationIndexMVA_T2")) != histos.end()) {
        h->second->Fill(GetTau2IsolationIndexMVA(idx), weight);
    } else {
        std::cout << "Can't find histogram IsolationIndexMVA_T2" << std::endl;
    }
    if ((h = histos.find("IsolationIndexMVA2_T1")) != histos.end()) {
        h->second->Fill(GetTau1IsolationIndexMVA2(idx), weight);
    } else {
        std::cout << "Can't find histogram IsolationIndexMVA2_T1" << std::endl;
    }
    if ((h = histos.find("IsolationIndexMVA2_T2")) != histos.end()) {
        h->second->Fill(GetTau2IsolationIndexMVA2(idx), weight);
    } else {
        std::cout << "Can't find histogram IsolationIndexMVA2_T2" << std::endl;
    }
    if ((h = histos.find("IsolationIndex3Hits_T1")) != histos.end()) {
        h->second->Fill(GetTau1IsolationIndex3Hits(idx), weight);
    } else {
        std::cout << "Can't find histogram IsolationIndex3Hits_T1" << std::endl;
    }
    if ((h = histos.find("IsolationIndex3Hits_T2")) != histos.end()) {
        h->second->Fill(GetTau2IsolationIndex3Hits(idx), weight);
    } else {
        std::cout << "Can't find histogram IsolationIndex3Hits_T2" << std::endl;
    }
    if ((h = histos.find("CombIsoDBcorrRaw_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1HPSbyCombinedIsolationDeltaBetaCorrRaw)[idx], weight);
    } else {
        std::cout << "Can't find histogram CombIsoDBcorrRaw_T1" << std::endl;
    }
    if ((h = histos.find("CombIsoDBcorrRaw_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2HPSbyCombinedIsolationDeltaBetaCorrRaw)[idx], weight);
    } else {
        std::cout << "Can't find histogram CombIsoDBcorrRaw_T2" << std::endl;
    }
    if ((h = histos.find("CombIsoDBcorrRaw3Hits_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx], weight);
    } else {
        std::cout << "Can't find histogram CombIsoDBcorrRaw3Hits_T1" << std::endl;
    }
    if ((h = histos.find("CombIsoDBcorrRaw3Hits_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx], weight);
    } else {
        std::cout << "Can't find histogram CombIsoDBcorrRaw3Hits_T2" << std::endl;
    }
    if ((h = histos.find("IsolationMVAraw_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1HPSbyIsolationMVAraw)[idx], weight);
    } else {
        std::cout << "Can't find histogram IsolationMVAraw_T1" << std::endl;
    }
    if ((h = histos.find("IsolationMVAraw_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2HPSbyIsolationMVAraw)[idx], weight);
    } else {
        std::cout << "Can't find histogram IsolationMVAraw_T2" << std::endl;
    }
    if ((h = histos.find("IsolationMVA2raw_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1HPSbyIsolationMVA2raw)[idx], weight);
    } else {
        std::cout << "Can't find histogram IsolationMVA2raw_T1" << std::endl;
    }
    if ((h = histos.find("IsolationMVA2raw_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2HPSbyIsolationMVA2raw)[idx], weight);
    } else {
        std::cout << "Can't find histogram IsolationMVA2raw_T2" << std::endl;
    }
    if ((h = histos.find("AntiElectronIndex_T1")) != histos.end()) {
        h->second->Fill(GetTau1AntiElectronIndex(idx), weight);
    } else {
        std::cout << "Can't find histogram AntiElectronIndex_T1" << std::endl;
    }
    if ((h = histos.find("AntiElectronIndex_T2")) != histos.end()) {
        h->second->Fill(GetTau2AntiElectronIndex(idx), weight);
    } else {
        std::cout << "Can't find histogram AntiElectronIndex_T2" << std::endl;
    }
    if ((h = histos.find("AntiElectronIndexMVA2_T1")) != histos.end()) {
        h->second->Fill(GetTau1AntiElectronIndexMVA2(idx), weight);
    } else {
        std::cout << "Can't find histogram AntiElectronIndexMVA2_T1" << std::endl;
    }
    if ((h = histos.find("AntiElectronIndexMVA2_T2")) != histos.end()) {
        h->second->Fill(GetTau2AntiElectronIndexMVA2(idx), weight);
    } else {
        std::cout << "Can't find histogram AntiElectronIndexMVA2_T2" << std::endl;
    }
    if ((h = histos.find("AntiElectronIndexMVA3_T1")) != histos.end()) {
        h->second->Fill(GetTau1AntiElectronIndexMVA3(idx), weight);
    } else {
        std::cout << "Can't find histogram AntiElectronIndexMVA3_T1" << std::endl;
    }
    if ((h = histos.find("AntiElectronIndexMVA3_T2")) != histos.end()) {
        h->second->Fill(GetTau2AntiElectronIndexMVA3(idx), weight);
    } else {
        std::cout << "Can't find histogram AntiElectronIndexMVA3_T2" << std::endl;
    }
    if ((h = histos.find("AgainstElectronMVA_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1HPSagainstElectronMVA)[idx], weight);
    } else {
        std::cout << "Can't find histogram AgainstElectronMVA_T1" << std::endl;
    }
    if ((h = histos.find("AgainstElectronMVA_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2HPSagainstElectronMVA)[idx], weight);
    } else {
        std::cout << "Can't find histogram AgainstElectronMVA_T2" << std::endl;
    }
    if ((h = histos.find("AgainstElectronMVA2raw_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1HPSagainstElectronMVA2raw)[idx], weight);
    } else {
        std::cout << "Can't find histogram AgainstElectronMVA2raw_T1" << std::endl;
    }
    if ((h = histos.find("AgainstElectronMVA2raw_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2HPSagainstElectronMVA2raw)[idx], weight);
    } else {
        std::cout << "Can't find histogram AgainstElectronMVA2raw_T2" << std::endl;
    }
    if ((h = histos.find("AgainstElectronMVA3raw_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1HPSagainstElectronMVA3raw)[idx], weight);
    } else {
        std::cout << "Can't find histogram AgainstElectronMVA3raw_T1" << std::endl;
    }
    if ((h = histos.find("AgainstElectronMVA3raw_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2HPSagainstElectronMVA3raw)[idx], weight);
    } else {
        std::cout << "Can't find histogram AgainstElectronMVA3raw_T2" << std::endl;
    }
    if ((h = histos.find("AgainstElectronMVA2category_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1HPSagainstElectronMVA2category)[idx], weight);
    } else {
        std::cout << "Can't find histogram AgainstElectronMVA2category_T1" << std::endl;
    }
    if ((h = histos.find("AgainstElectronMVA2category_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2HPSagainstElectronMVA2category)[idx], weight);
    } else {
        std::cout << "Can't find histogram AgainstElectronMVA2category_T2" << std::endl;
    }
    if ((h = histos.find("AgainstElectronMVA3category_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1HPSagainstElectronMVA3category)[idx], weight);
    } else {
        std::cout << "Can't find histogram AgainstElectronMVA3category_T1" << std::endl;
    }
    if ((h = histos.find("AgainstElectronMVA3category_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2HPSagainstElectronMVA3category)[idx], weight);
    } else {
        std::cout << "Can't find histogram AgainstElectronMVA3category_T2" << std::endl;
    }
    if ((h = histos.find("AgainstElectronDeadECAL_T1")) != histos.end()) {
        h->second->Fill((*TTL_Tau1HPSagainstElectronDeadECAL)[idx], weight);
    } else {
        std::cout << "Can't find histogram AgainstElectronDeadECAL_T1" << std::endl;
    }
    if ((h = histos.find("AgainstElectronDeadECAL_T2")) != histos.end()) {
        h->second->Fill((*TTL_Tau2HPSagainstElectronDeadECAL)[idx], weight);
    } else {
        std::cout << "Can't find histogram AgainstElectronDeadECAL_T2" << std::endl;
    }
    if ((h = histos.find("AntiMuonIndex_T1")) != histos.end()) {
        h->second->Fill(GetTau1AntiMuonIndex(idx), weight);
    } else {
        std::cout << "Can't find histogram AntiMuonIndex_T1" << std::endl;
    }
    if ((h = histos.find("AntiMuonIndex_T2")) != histos.end()) {
        h->second->Fill(GetTau2AntiMuonIndex(idx), weight);
    } else {
        std::cout << "Can't find histogram AntiMuonIndex_T2" << std::endl;
    }
    if ((h = histos.find("AntiMuonIndex2_T1")) != histos.end()) {
        h->second->Fill(GetTau1AntiMuonIndex2(idx), weight);
    } else {
        std::cout << "Can't find histogram AntiMuonIndex2_T1" << std::endl;
    }
    if ((h = histos.find("AntiMuonIndex2_T2")) != histos.end()) {
        h->second->Fill(GetTau2AntiMuonIndex2(idx), weight);
    } else {
        std::cout << "Can't find histogram AntiMuonIndex2_T2" << std::endl;
    }
    if ((h = histos.find("Match_T1")) != histos.end()) {
        h->second->Fill(GetTau1MatchIndex(idx), weight);
    } else {
        std::cout << "Can't find histogram Match_T1" << std::endl;
    }
    if ((h = histos.find("Match_T2")) != histos.end()) {
        h->second->Fill(GetTau2MatchIndex(idx), weight);
    } else {
        std::cout << "Can't find histogram Match_T2" << std::endl;
    }

    // Lepton ============================================================================================================================
    if ((h = histos.find("Pt_L")) != histos.end()) {
        h->second->Fill((*TTL_LeptonPt)[idx], weight);
    } else {
        std::cout << "Can't find histogram Pt_L" << std::endl;
    }
    if ((h = histos.find("Eta_L")) != histos.end()) {
        h->second->Fill((*TTL_LeptonEta)[idx], weight);
    } else {
        std::cout << "Can't find histogram Eta_L" << std::endl;
    }
    if ((h = histos.find("Phi_L")) != histos.end()) {
        h->second->Fill((*TTL_LeptonPhi)[idx], weight);
    } else {
        std::cout << "Can't find histogram Phi_L" << std::endl;
    }
    if ((h = histos.find("NumTightMuons")) != histos.end()) {
        h->second->Fill((*TTL_NumTightMuons)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumTightMuons" << std::endl;
    }
    if ((h = histos.find("NumLooseMuons")) != histos.end()) {
        h->second->Fill((*TTL_NumLooseMuons)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumLooseMuons" << std::endl;
    }
    if ((h = histos.find("NumExLooseMuons")) != histos.end()) {
        h->second->Fill((*TTL_NumExLooseMuons)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumExLooseMuons" << std::endl;
    }
    if ((h = histos.find("NumTightElectrons")) != histos.end()) {
        h->second->Fill((*TTL_NumTightElectrons)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumTightElectrons" << std::endl;
    }
    if ((h = histos.find("NumLooseElectrons")) != histos.end()) {
        h->second->Fill((*TTL_NumLooseElectrons)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumLooseElectrons" << std::endl;
    }
    if ((h = histos.find("NumExLooseElectrons")) != histos.end()) {
        h->second->Fill((*TTL_NumExLooseElectrons)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumExLooseElectrons" << std::endl;
    }
    if ((h = histos.find("NumTightLeptons")) != histos.end()) {
        h->second->Fill((*TTL_NumTightMuons)[idx] + (*TTL_NumTightElectrons)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumTightLeptons" << std::endl;
    }
    if ((h = histos.find("NumLooseLeptons")) != histos.end()) {
        h->second->Fill((*TTL_NumLooseMuons)[idx] + (*TTL_NumLooseElectrons)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumLooseLeptons" << std::endl;
    }
    if ((h = histos.find("NumExLooseLeptons")) != histos.end()) {
        h->second->Fill((*TTL_NumExLooseMuons)[idx] + (*TTL_NumExLooseElectrons)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumExLooseLeptons" << std::endl;
    }
    if ((h = histos.find("DeltaR_LT1")) != histos.end()) {
        h->second->Fill(DeltaR((*TTL_LeptonEta)[idx], (*TTL_LeptonPhi)[idx], (*TTL_Tau1Eta)[idx], (*TTL_Tau1Phi)[idx]), weight);
    } else {
        std::cout << "Can't find histogram DeltaR_LT1" << std::endl;
    }
    if ((h = histos.find("DeltaR_LT2")) != histos.end()) {
        h->second->Fill(DeltaR((*TTL_LeptonEta)[idx], (*TTL_LeptonPhi)[idx], (*TTL_Tau2Eta)[idx], (*TTL_Tau2Phi)[idx]), weight);
    } else {
        std::cout << "Can't find histogram DeltaR_LT2" << std::endl;
    }

    // Jets ============================================================================================================================
    // tau/jet matching con size
    try {
        unsigned int ljet = GetJetIndex(idx, 0);
        if ((h = histos.find("LeadingJet_Pt")) != histos.end()) {
            h->second->Fill(J_Pt->at(ljet), weight);
        } else {
            std::cout << "Can't find histogram LeadingJet_Pt" << std::endl;
        }
        if ((h = histos.find("LeadingJet_Eta")) != histos.end()) {
            h->second->Fill(J_Eta->at(ljet), weight);
        } else {
            std::cout << "Can't find histogram LeadingJet_Eta" << std::endl;
        }
        if ((h = histos.find("DeltaR_LJ1")) != histos.end()) {
            h->second->Fill(DeltaR((*TTL_LeptonEta)[idx], (*TTL_LeptonPhi)[idx], J_Eta->at(ljet), J_Phi->at(ljet)), weight);
        } else {
            std::cout << "Can't find histogram DeltaR_LJ1" << std::endl;
        }
        if ((h = histos.find("DeltaR_T1J1")) != histos.end()) {
            h->second->Fill(DeltaR((*TTL_Tau1Eta)[idx], (*TTL_Tau1Phi)[idx], J_Eta->at(ljet), J_Phi->at(ljet)), weight);
        } else {
            std::cout << "Can't find histogram DeltaR_T1J1" << std::endl;
        }
        if ((h = histos.find("DeltaR_T2J1")) != histos.end()) {
            h->second->Fill(DeltaR((*TTL_Tau2Eta)[idx], (*TTL_Tau2Phi)[idx], J_Eta->at(ljet), J_Phi->at(ljet)), weight);
        } else {
            std::cout << "Can't find histogram DeltaR_T2J1" << std::endl;
        }

        unsigned int sljet = GetJetIndex(idx, 1);
        if ((h = histos.find("SubLeadingJet_Pt")) != histos.end()) {
            h->second->Fill(J_Pt->at(sljet), weight);
        } else {
            std::cout << "Can't find histogram SubLeadingJet_Pt" << std::endl;
        }
        if ((h = histos.find("SubLeadingJet_Eta")) != histos.end()) {
            h->second->Fill(J_Eta->at(sljet), weight);
        } else {
            std::cout << "Can't find histogram SubLeadingJet_Eta" << std::endl;
        }
        if ((h = histos.find("DeltaR_LJ2")) != histos.end()) {
            h->second->Fill(DeltaR((*TTL_LeptonEta)[idx], (*TTL_LeptonPhi)[idx], J_Eta->at(sljet), J_Phi->at(sljet)), weight);
        } else {
            std::cout << "Can't find histogram DeltaR_LJ2" << std::endl;
        }
        if ((h = histos.find("DeltaR_T1J2")) != histos.end()) {
            h->second->Fill(DeltaR((*TTL_Tau1Eta)[idx], (*TTL_Tau1Phi)[idx], J_Eta->at(sljet), J_Phi->at(sljet)), weight);
        } else {
            std::cout << "Can't find histogram DeltaR_T1J2" << std::endl;
        }
        if ((h = histos.find("DeltaR_T2J2")) != histos.end()) {
            h->second->Fill(DeltaR((*TTL_Tau2Eta)[idx], (*TTL_Tau2Phi)[idx], J_Eta->at(sljet), J_Phi->at(sljet)), weight);
        } else {
            std::cout << "Can't find histogram DeltaR_T2J2" << std::endl;
        }

        unsigned int ssljet = GetJetIndex(idx, 2);
        if ((h = histos.find("SubSubLeadingJet_Pt")) != histos.end()) {
            h->second->Fill(J_Pt->at(ssljet), weight);
        } else {
            std::cout << "Can't find histogram SubSubLeadingJet_Pt" << std::endl;
        }
        if ((h = histos.find("SubSubLeadingJet_Eta")) != histos.end()) {
            h->second->Fill(J_Eta->at(ssljet), weight);
        } else {
            std::cout << "Can't find histogram SubSubLeadingJet_Eta" << std::endl;
        }

        unsigned int sssljet = GetJetIndex(idx, 3);
        if ((h = histos.find("SubSubLeadingJet_Pt")) != histos.end()) {
            h->second->Fill(J_Pt->at(sssljet), weight);
        } else {
            std::cout << "Can't find histogram SubSubLeadingJet_Pt" << std::endl;
        }
        if ((h = histos.find("SubSubLeadingJet_Eta")) != histos.end()) {
            h->second->Fill(J_Eta->at(sssljet), weight);
        } else {
            std::cout << "Can't find histogram SubSubLeadingJet_Eta" << std::endl;
        }
    } catch (...) {}

    float conesize = .25;
    for (unsigned int i = 0, c = 0; c < 2 and i < J_Pt->size(); i++) {
        if ((DeltaR((*J_Eta)[i], (*J_Phi)[i], (*TTL_Tau1Eta)[idx], (*TTL_Tau1Phi)[idx]) > conesize) &&
                (DeltaR((*J_Eta)[i], (*J_Phi)[i], (*TTL_Tau2Eta)[idx], (*TTL_Tau2Phi)[idx]) > conesize) &&
                (DeltaR((*J_Eta)[i], (*J_Phi)[i], (*TTL_LeptonEta)[idx], (*TTL_LeptonPhi)[idx]) > conesize)) {
            if ((*J_combSecVtxMediumBTag)[i]) {
               if ((h = histos.find("BTagJet_Pt")) != histos.end()) {
                   h->second->Fill((*J_Pt)[i], weight);
               } else {
                   std::cout << "Can't find histogram BTagJet_Pt" << std::endl;
               }
               if ((h = histos.find("BTagJet_Eta")) != histos.end()) {
                   h->second->Fill((*J_Eta)[i], weight);
               } else {
                   std::cout << "Can't find histogram BTagJet_Eta" << std::endl;
               }
            } else {
               if ((h = histos.find("NonBTagJet_Pt")) != histos.end()) {
                   h->second->Fill((*J_Pt)[i], weight);
               } else {
                   std::cout << "Can't find histogram NonBTagJet_Pt" << std::endl;
               }
               if ((h = histos.find("NonBTagJet_Eta")) != histos.end()) {
                   h->second->Fill((*J_Eta)[i], weight);
               } else {
                   std::cout << "Can't find histogram NonBTagJet_Eta" << std::endl;
               }
            }

            ++c;
        }
    }

    if ((h = histos.find("NumCSVLbtags")) != histos.end()) {
        h->second->Fill((*TTL_NumCSVLbtagJets)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumCSVLbtags" << std::endl;
    }
    if ((h = histos.find("NumCSVMbtags")) != histos.end()) {
        h->second->Fill((*TTL_NumCSVMbtagJets)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumCSVMbtags" << std::endl;
    }
    if ((h = histos.find("NumCSVTbtags")) != histos.end()) {
        h->second->Fill((*TTL_NumCSVTbtagJets)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumCSVTbtags" << std::endl;
    }
    if ((h = histos.find("NumCSVLnonBtags")) != histos.end()) {
        h->second->Fill((*TTL_NumNonCSVLbtagJets)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumCSVLnonBtags" << std::endl;
    }
    if ((h = histos.find("NumCSVMnonBtags")) != histos.end()) {
        h->second->Fill((*TTL_NumNonCSVMbtagJets)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumCSVMnonBtags" << std::endl;
    }
    if ((h = histos.find("NumCSVTnonBtags")) != histos.end()) {
        h->second->Fill((*TTL_NumNonCSVTbtagJets)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumCSVTnonBtags" << std::endl;
    }
    if ((h = histos.find("NumCSVnonHTauJets")) != histos.end()) {
        h->second->Fill(((*TTL_NumNonCSVMbtagJets)[idx] + (*TTL_NumCSVMbtagJets)[idx]), weight);
    } else {
        std::cout << "Can't find histogram NumCSVnonHTauJets" << std::endl;
    }
    if ((h = histos.find("NumCleanCSVLbtags")) != histos.end()) {
        h->second->Fill((*TTL_NumCleanCSVLbtagJets)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumCleanCSVLbtags" << std::endl;
    }
    if ((h = histos.find("NumCleanCSVMbtags")) != histos.end()) {
        h->second->Fill((*TTL_NumCleanCSVMbtagJets)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumCleanCSVMbtags" << std::endl;
    }
    if ((h = histos.find("NumCleanCSVTbtags")) != histos.end()) {
        h->second->Fill((*TTL_NumCleanCSVTbtagJets)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumCleanCSVTbtags" << std::endl;
    }
    if ((h = histos.find("NumCleanCSVLnonBtags")) != histos.end()) {
        h->second->Fill((*TTL_NumCleanNonCSVLbtagJets)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumCleanCSVLnonBtags" << std::endl;
    }
    if ((h = histos.find("NumCleanCSVMnonBtags")) != histos.end()) {
        h->second->Fill((*TTL_NumCleanNonCSVMbtagJets)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumCleanCSVMnonBtags" << std::endl;
    }
    if ((h = histos.find("NumCleanCSVTnonBtags")) != histos.end()) {
        h->second->Fill((*TTL_NumCleanNonCSVTbtagJets)[idx], weight);
    } else {
        std::cout << "Can't find histogram NumCleanCSVTnonBtags" << std::endl;
    }
    if ((h = histos.find("NumCleanCSVnonHTauJets")) != histos.end()) {
        h->second->Fill(((*TTL_NumCleanNonCSVMbtagJets)[idx] + (*TTL_NumCleanCSVMbtagJets)[idx]), weight);
    } else {
        std::cout << "Can't find histogram NumCleanCSVnonHTauJets" << std::endl;
    }
    if ((h = histos.find("NumInclusiveJets")) != histos.end()) {
        h->second->Fill(((*TTL_NumCleanNonCSVMbtagJets)[idx] + (*TTL_NumCleanCSVMbtagJets)[idx]) + 2, weight);
    } else {
        std::cout << "Can't find histogram NumInclusiveJets" << std::endl;
    }
}

ClassImp(roast::ttl::Branches)
