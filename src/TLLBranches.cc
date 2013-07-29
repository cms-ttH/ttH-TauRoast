// vim: ts=4:sw=4:et:sta
#include "TFile.h"

#include "../interface/TLLBranches.h"
// #include "../interface/GenHelper.h"
#include "../interface/MVABase.h"

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
	if((*TLL_TauHPSbyTightCombinedIsolationDeltaBetaCorr)[iCombo]){	return 4; }
	if((*TLL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr)[iCombo]){	return 3; }
	if((*TLL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr)[iCombo]){	return 2; }
	if((*TLL_TauHPSbyVLooseCombinedIsolationDeltaBetaCorr)[iCombo]){	return 1; }
	return 0;
}

unsigned int tll::Branches::GetTauIsolationIndex3Hits(const unsigned int iCombo) const {
	if((*TLL_TauHPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){	return 3; }
	if((*TLL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){	return 2; }
	if((*TLL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){	return 1; }
	return 0;
}

unsigned int tll::Branches::GetTauIsolationIndexMVA(const unsigned int iCombo) const {
	if((*TLL_TauHPSbyTightIsolationMVA)[iCombo]){	return 3; }
	if((*TLL_TauHPSbyMediumIsolationMVA)[iCombo]){	return 2; }
	if((*TLL_TauHPSbyLooseIsolationMVA)[iCombo]){	return 1; }
	return 0;
}

unsigned int tll::Branches::GetTauIsolationIndexMVA2(const unsigned int iCombo) const {
	if((*TLL_TauHPSbyTightIsolationMVA2)[iCombo]){	return 3; }
	if((*TLL_TauHPSbyMediumIsolationMVA2)[iCombo]){	return 2; }
	if((*TLL_TauHPSbyLooseIsolationMVA2)[iCombo]){	return 1; }
	return 0;
}

unsigned int tll::Branches::GetTauAntiElectronIndex(const unsigned int iCombo) const {
	if((*TLL_TauHPSagainstElectronTight)[iCombo]){	return 3; }
	if((*TLL_TauHPSagainstElectronMedium)[iCombo]){	return 2; }
	if((*TLL_TauHPSagainstElectronLoose)[iCombo]){	return 1; }
	return 0;
}

unsigned int tll::Branches::GetTauAntiElectronIndexMVA2(const unsigned int iCombo) const {
	if((*TLL_TauHPSagainstElectronTightMVA2)[iCombo]){	return 4; }
	if((*TLL_TauHPSagainstElectronMediumMVA2)[iCombo]){	return 3; }
	if((*TLL_TauHPSagainstElectronLooseMVA2)[iCombo]){	return 2; }
	if((*TLL_TauHPSagainstElectronVLooseMVA2)[iCombo]){	return 1; }
	return 0;
}

unsigned int tll::Branches::GetTauAntiElectronIndexMVA3(const unsigned int iCombo) const {
	if((*TLL_TauHPSagainstElectronVTightMVA3)[iCombo]){	return 4; }
	if((*TLL_TauHPSagainstElectronTightMVA3)[iCombo]){	return 3; }
	if((*TLL_TauHPSagainstElectronMediumMVA3)[iCombo]){	return 2; }
	if((*TLL_TauHPSagainstElectronLooseMVA3)[iCombo]){	return 1; }
	return 0;
}

unsigned int tll::Branches::GetTauAntiMuonIndex(const unsigned int iCombo) const {
	if((*TLL_TauHPSagainstMuonTight)[iCombo]){	return 3; }
	if((*TLL_TauHPSagainstMuonMedium)[iCombo]){	return 2; }
	if((*TLL_TauHPSagainstMuonLoose)[iCombo]){	return 1; }
	return 0;
}

unsigned int tll::Branches::GetTauAntiMuonIndex2(const unsigned int iCombo) const {
	if((*TLL_TauHPSagainstMuonTight2)[iCombo]){	return 3; }
	if((*TLL_TauHPSagainstMuonMedium2)[iCombo]){	return 2; }
	if((*TLL_TauHPSagainstMuonLoose2)[iCombo]){	return 1; }
	return 0;
}

unsigned int tll::Branches::GetTauMatchIndex(const unsigned int iCombo) const {
	if((*TLL_TauGenMatchId)[iCombo] == -99){  return 0; } // no match
	if(abs((*TLL_TauGenMatchId)[iCombo]) == 15){	return 2; } // tau
	if(abs((*TLL_TauGenMatchId)[iCombo]) == 13){	return 3; } // muon
	if(abs((*TLL_TauGenMatchId)[iCombo]) == 11){	return 4; } // electron
	if(abs((*TLL_TauGenMatchId)[iCombo]) == 22){	return 5; } // photon
	if(abs((*TLL_TauGenMatchId)[iCombo]) == 23){	return 6; } // Z
	if(abs((*TLL_TauGenMatchId)[iCombo]) == 24){	return 7; } // W
	if(abs((*TLL_TauGenMatchId)[iCombo]) == 25){	return 8; } // Higgs
	return 1; // hadronic
}

unsigned int
tll::Branches::GetJetIndex(const int idx, const unsigned int num)
{
    return 99;
}

void
tll::Branches::FillHistograms(std::map<std::string, roast::HWrapper*>& histos, int idx, float weight)
{
}

ClassImp(roast::tll::Branches)
