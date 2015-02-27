#ifdef FOOBARBAD
// vim: ts=4:sw=4:et:sta
#include "TFile.h"

#include "../interface/MVABase.h"
#include "../interface/TTLBranches.h"

using namespace std;
using namespace roast;

// Default constructor
ttl::Branches::Branches() : roast::Branches()
{
    Null();
}

ttl::Branches::Branches(const vector<string>& paths) :
        roast::Branches(paths)
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
    return (abs((*T1_GenMatchId)[idx]) == 15 &&
            abs((*T2_GenMatchId)[idx]) == 15 &&
            abs((*T1_GenMatchMother0Id)[idx]) == 25 &&
            abs((*T2_GenMatchMother0Id)[idx]) == 25);
}

unsigned int ttl::Branches::GetTau1IsolationIndex(const unsigned int iCombo) const {
	if((*T1_HPSbyTightCombinedIsolationDeltaBetaCorr)[iCombo]){	return 4; }
	if((*T1_HPSbyMediumCombinedIsolationDeltaBetaCorr)[iCombo]){	return 3; }
	if((*T1_HPSbyLooseCombinedIsolationDeltaBetaCorr)[iCombo]){	return 2; }
	if((*T1_HPSbyVLooseCombinedIsolationDeltaBetaCorr)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2IsolationIndex(const unsigned int iCombo) const {
	if((*T2_HPSbyTightCombinedIsolationDeltaBetaCorr)[iCombo]){	return 4; }
	if((*T2_HPSbyMediumCombinedIsolationDeltaBetaCorr)[iCombo]){	return 3; }
	if((*T2_HPSbyLooseCombinedIsolationDeltaBetaCorr)[iCombo]){	return 2; }
	if((*T2_HPSbyVLooseCombinedIsolationDeltaBetaCorr)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau1IsolationIndex3Hits(const unsigned int iCombo) const {
	if((*T1_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){	return 3; }
	if((*T1_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){	return 2; }
	if((*T1_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2IsolationIndex3Hits(const unsigned int iCombo) const {
	if((*T2_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){	return 3; }
	if((*T2_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){	return 2; }
	if((*T2_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau1IsolationIndexMVA(const unsigned int iCombo) const {
	if((*T1_HPSbyTightIsolationMVA)[iCombo]){	return 3; }
	if((*T1_HPSbyMediumIsolationMVA)[iCombo]){	return 2; }
	if((*T1_HPSbyLooseIsolationMVA)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2IsolationIndexMVA(const unsigned int iCombo) const {
	if((*T2_HPSbyTightIsolationMVA)[iCombo]){	return 3; }
	if((*T2_HPSbyMediumIsolationMVA)[iCombo]){	return 2; }
	if((*T2_HPSbyLooseIsolationMVA)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau1IsolationIndexMVA2(const unsigned int iCombo) const {
	if((*T1_HPSbyTightIsolationMVA2)[iCombo]){	return 3; }
	if((*T1_HPSbyMediumIsolationMVA2)[iCombo]){	return 2; }
	if((*T1_HPSbyLooseIsolationMVA2)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2IsolationIndexMVA2(const unsigned int iCombo) const {
	if((*T2_HPSbyTightIsolationMVA2)[iCombo]){	return 3; }
	if((*T2_HPSbyMediumIsolationMVA2)[iCombo]){	return 2; }
	if((*T2_HPSbyLooseIsolationMVA2)[iCombo]){	return 1; }
	return 0;
}


unsigned int ttl::Branches::GetTau1AntiElectronIndex(const unsigned int iCombo) const {
	if((*T1_HPSagainstElectronTight)[iCombo]){	return 3; }
	if((*T1_HPSagainstElectronMedium)[iCombo]){	return 2; }
	if((*T1_HPSagainstElectronLoose)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2AntiElectronIndex(const unsigned int iCombo) const {
	if((*T2_HPSagainstElectronTight)[iCombo]){	return 3; }
	if((*T2_HPSagainstElectronMedium)[iCombo]){	return 2; }
	if((*T2_HPSagainstElectronLoose)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau1AntiElectronIndexMVA2(const unsigned int iCombo) const {
	if((*T1_HPSagainstElectronTightMVA2)[iCombo]){	return 4; }
	if((*T1_HPSagainstElectronMediumMVA2)[iCombo]){	return 3; }
	if((*T1_HPSagainstElectronLooseMVA2)[iCombo]){	return 2; }
	if((*T1_HPSagainstElectronVLooseMVA2)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2AntiElectronIndexMVA2(const unsigned int iCombo) const {
	if((*T2_HPSagainstElectronTightMVA2)[iCombo]){	return 4; }
	if((*T2_HPSagainstElectronMediumMVA2)[iCombo]){	return 3; }
	if((*T2_HPSagainstElectronLooseMVA2)[iCombo]){	return 2; }
	if((*T2_HPSagainstElectronVLooseMVA2)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau1AntiElectronIndexMVA3(const unsigned int iCombo) const {
	if((*T1_HPSagainstElectronVTightMVA3)[iCombo]){	return 4; }
	if((*T1_HPSagainstElectronTightMVA3)[iCombo]){	return 3; }
	if((*T1_HPSagainstElectronMediumMVA3)[iCombo]){	return 2; }
	if((*T1_HPSagainstElectronLooseMVA3)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2AntiElectronIndexMVA3(const unsigned int iCombo) const {
	if((*T2_HPSagainstElectronVTightMVA3)[iCombo]){	return 4; }
	if((*T2_HPSagainstElectronTightMVA3)[iCombo]){	return 3; }
	if((*T2_HPSagainstElectronMediumMVA3)[iCombo]){	return 2; }
	if((*T2_HPSagainstElectronLooseMVA3)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau1AntiMuonIndex(const unsigned int iCombo) const {
	if((*T1_HPSagainstMuonTight)[iCombo]){	return 3; }
	if((*T1_HPSagainstMuonMedium)[iCombo]){	return 2; }
	if((*T1_HPSagainstMuonLoose)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2AntiMuonIndex(const unsigned int iCombo) const {
	if((*T2_HPSagainstMuonTight)[iCombo]){	return 3; }
	if((*T2_HPSagainstMuonMedium)[iCombo]){	return 2; }
	if((*T2_HPSagainstMuonLoose)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau1AntiMuonIndex2(const unsigned int iCombo) const {
	if((*T1_HPSagainstMuonTight2)[iCombo]){	return 3; }
	if((*T1_HPSagainstMuonMedium2)[iCombo]){	return 2; }
	if((*T1_HPSagainstMuonLoose2)[iCombo]){	return 1; }
	return 0;
}

unsigned int ttl::Branches::GetTau2AntiMuonIndex2(const unsigned int iCombo) const {
	if((*T2_HPSagainstMuonTight2)[iCombo]){	return 3; }
	if((*T2_HPSagainstMuonMedium2)[iCombo]){	return 2; }
	if((*T2_HPSagainstMuonLoose2)[iCombo]){	return 1; }
	return 0;
}

ClassImp(roast::ttl::Branches)
#endif
