// vim: ts=4:sw=4:et:sta
#include "TFile.h"

#include "../interface/GenHelper.h"
#include "../interface/MVABase.h"
#include "../interface/TTLBranches.h"

using namespace std;
using namespace roast;

// Default constructor
ttl::Branches::Branches() : roast::Branches()
{
    Null();
}

ttl::Branches::Branches(const std::string& tree_name, const vector<string>& paths) :
        roast::Branches(tree_name, paths)
{
    Null();
    SetBranchAddresses();
}

ttl::Branches::~Branches()
{
    Delete();
    Null();
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

unsigned int
ttl::Branches::GetTau1MatchIndex(unsigned int idx) const
{
    return TranslateMatchIndex((*TTL_Tau1GenMatchId)[idx]);
}

unsigned int
ttl::Branches::GetTau1ParentMatchIndex(unsigned int idx) const
{
    return TranslateMatchIndex((*TTL_Tau1GenMatchMother0Id)[idx]);
}

unsigned int
ttl::Branches::GetTau2MatchIndex(unsigned int idx) const
{
    return TranslateMatchIndex((*TTL_Tau2GenMatchId)[idx]);
}

unsigned int
ttl::Branches::GetTau2ParentMatchIndex(unsigned int idx) const
{
    return TranslateMatchIndex((*TTL_Tau2GenMatchMother0Id)[idx]);
}

ClassImp(roast::ttl::Branches)
