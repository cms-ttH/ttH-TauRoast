// vim: ts=4:sw=4:et:sta
#include "TFile.h"

#include "../interface/MVABase.h"
#include "../interface/TLBranches.h"

using namespace std;
using namespace roast;

// Default constructor
tl::Branches::Branches() : roast::Branches() {
    Null();
}

tl::Branches::Branches(const vector<string>& paths) :
    roast::Branches(paths)
{
    Null();
    SetBranchAddresses();
}

tl::Branches::~Branches()
{
    Delete();
    Null();
}

bool
tl::Branches::IsGoodGenMatch(const int& idx) const {
    return (abs((*TL_TauGenMatchId)[idx]) == 15 &&
            abs((*TL_TauGenMatchMother0Id)[idx]) == 25);
}

unsigned int tl::Branches::GetTauIsolationIndex(const unsigned int iCombo) const {
    if((*TL_TauHPSbyTightCombinedIsolationDeltaBetaCorr)[iCombo]){ return 4; }
    if((*TL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr)[iCombo]){ return 3; }
    if((*TL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr)[iCombo]){ return 2; }
    if((*TL_TauHPSbyVLooseCombinedIsolationDeltaBetaCorr)[iCombo]){ return 1; }
    return 0;
}

unsigned int tl::Branches::GetTauIsolationIndex3Hits(const unsigned int iCombo) const {
    if((*TL_TauHPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){ return 3; }
    if((*TL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){ return 2; }
    if((*TL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[iCombo]){ return 1; }
    return 0;
}

unsigned int tl::Branches::GetTauIsolationIndexMVA(const unsigned int iCombo) const {
    if((*TL_TauHPSbyTightIsolationMVA)[iCombo]){ return 3; }
    if((*TL_TauHPSbyMediumIsolationMVA)[iCombo]){ return 2; }
    if((*TL_TauHPSbyLooseIsolationMVA)[iCombo]){ return 1; }
    return 0;
}

unsigned int tl::Branches::GetTauIsolationIndexMVA2(const unsigned int iCombo) const {
    if((*TL_TauHPSbyTightIsolationMVA2)[iCombo]){ return 3; }
    if((*TL_TauHPSbyMediumIsolationMVA2)[iCombo]){ return 2; }
    if((*TL_TauHPSbyLooseIsolationMVA2)[iCombo]){ return 1; }
    return 0;
}

unsigned int tl::Branches::GetTauAntiElectronIndex(const unsigned int iCombo) const {
    if((*TL_TauHPSagainstElectronTight)[iCombo]){ return 3; }
    if((*TL_TauHPSagainstElectronMedium)[iCombo]){ return 2; }
    if((*TL_TauHPSagainstElectronLoose)[iCombo]){ return 1; }
    return 0;
}

unsigned int tl::Branches::GetTauAntiElectronIndexMVA2(const unsigned int iCombo) const {
    if((*TL_TauHPSagainstElectronTightMVA2)[iCombo]){ return 4; }
    if((*TL_TauHPSagainstElectronMediumMVA2)[iCombo]){ return 3; }
    if((*TL_TauHPSagainstElectronLooseMVA2)[iCombo]){ return 2; }
    if((*TL_TauHPSagainstElectronVLooseMVA2)[iCombo]){ return 1; }
    return 0;
}

unsigned int tl::Branches::GetTauAntiElectronIndexMVA3(const unsigned int iCombo) const {
    if((*TL_TauHPSagainstElectronVTightMVA3)[iCombo]){ return 4; }
    if((*TL_TauHPSagainstElectronTightMVA3)[iCombo]){ return 3; }
    if((*TL_TauHPSagainstElectronMediumMVA3)[iCombo]){ return 2; }
    if((*TL_TauHPSagainstElectronLooseMVA3)[iCombo]){ return 1; }
    return 0;
}

unsigned int tl::Branches::GetTauAntiMuonIndex(const unsigned int iCombo) const {
    if((*TL_TauHPSagainstMuonTight)[iCombo]){ return 3; }
    if((*TL_TauHPSagainstMuonMedium)[iCombo]){ return 2; }
    if((*TL_TauHPSagainstMuonLoose)[iCombo]){ return 1; }
    return 0;
}

unsigned int tl::Branches::GetTauAntiMuonIndex2(const unsigned int iCombo) const {
    if((*TL_TauHPSagainstMuonTight2)[iCombo]){ return 3; }
    if((*TL_TauHPSagainstMuonMedium2)[iCombo]){ return 2; }
    if((*TL_TauHPSagainstMuonLoose2)[iCombo]){ return 1; }
    return 0;
}

ClassImp(roast::tl::Branches)
