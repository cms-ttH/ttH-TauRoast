#ifdef FOOBARBAD
// vim: ts=4:sw=4:et:sta
#include "TFile.h"

#include "../interface/MVABase.h"
#include "../interface/TLLBranches.h"

using namespace std;
using namespace roast;

// Default constructor
tll::Branches::Branches() : roast::Branches() {
    Null();
}

tll::Branches::Branches(const vector<string>& paths) :
    roast::Branches(paths)
{
    Null();
    SetBranchAddresses();
}

tll::Branches::~Branches()
{
    Delete();
    Null();
}

bool
tll::Branches::IsGoodGenMatch(const int& idx) const {
    return (abs((*T_GenMatchId)[idx]) == 15 &&
            abs((*T_GenMatchMother0Id)[idx]) == 25);
}

unsigned int tll::Branches::GetTauIsolationIndex(const unsigned int iCombo) const {
    if((*T_HPSbyTightCombinedIsolationDeltaBetaCorr)[iCombo]){ return 4; }
    if((*T_HPSbyMediumCombinedIsolationDeltaBetaCorr)[iCombo]){ return 3; }
    if((*T_HPSbyLooseCombinedIsolationDeltaBetaCorr)[iCombo]){ return 2; }
    if((*T_HPSbyVLooseCombinedIsolationDeltaBetaCorr)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauIsolationIndex3Hits(const unsigned int iCombo) const {
    if((*T_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){ return 3; }
    if((*T_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){ return 2; }
    if((*T_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauIsolationIndexMVA(const unsigned int iCombo) const {
    if((*T_HPSbyTightIsolationMVA)[iCombo]){ return 3; }
    if((*T_HPSbyMediumIsolationMVA)[iCombo]){ return 2; }
    if((*T_HPSbyLooseIsolationMVA)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauIsolationIndexMVA2(const unsigned int iCombo) const {
    if((*T_HPSbyTightIsolationMVA2)[iCombo]){ return 3; }
    if((*T_HPSbyMediumIsolationMVA2)[iCombo]){ return 2; }
    if((*T_HPSbyLooseIsolationMVA2)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauAntiElectronIndex(const unsigned int iCombo) const {
    if((*T_HPSagainstElectronTight)[iCombo]){ return 3; }
    if((*T_HPSagainstElectronMedium)[iCombo]){ return 2; }
    if((*T_HPSagainstElectronLoose)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauAntiElectronIndexMVA2(const unsigned int iCombo) const {
    if((*T_HPSagainstElectronTightMVA2)[iCombo]){ return 4; }
    if((*T_HPSagainstElectronMediumMVA2)[iCombo]){ return 3; }
    if((*T_HPSagainstElectronLooseMVA2)[iCombo]){ return 2; }
    if((*T_HPSagainstElectronVLooseMVA2)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauAntiElectronIndexMVA3(const unsigned int iCombo) const {
    if((*T_HPSagainstElectronVTightMVA3)[iCombo]){ return 4; }
    if((*T_HPSagainstElectronTightMVA3)[iCombo]){ return 3; }
    if((*T_HPSagainstElectronMediumMVA3)[iCombo]){ return 2; }
    if((*T_HPSagainstElectronLooseMVA3)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauAntiMuonIndex(const unsigned int iCombo) const {
    if((*T_HPSagainstMuonTight)[iCombo]){ return 3; }
    if((*T_HPSagainstMuonMedium)[iCombo]){ return 2; }
    if((*T_HPSagainstMuonLoose)[iCombo]){ return 1; }
    return 0;
}

unsigned int tll::Branches::GetTauAntiMuonIndex2(const unsigned int iCombo) const {
    if((*T_HPSagainstMuonTight2)[iCombo]){ return 3; }
    if((*T_HPSagainstMuonMedium2)[iCombo]){ return 2; }
    if((*T_HPSagainstMuonLoose2)[iCombo]){ return 1; }
    return 0;
}

ClassImp(roast::tll::Branches)
#endif
