// vim: ts=4:sw=4:et:sta
#include "../interface/LLBranches.h"

using namespace roast;

void
ll::Branches::Clear()
{
    roast::Branches::Clear();

    // >>> Begin clear <<<
    Ev_METcov->clear();
    Ev_NTruePV = 0;
    J_Lepton1DeltaR->clear();
    J_Lepton2DeltaR->clear();
    LL_Lepton1Charge->clear();
    LL_Lepton1CorrectedD0->clear();
    LL_Lepton1CorrectedDZ->clear();
    LL_Lepton1Eta->clear();
    LL_Lepton1EventWeight->clear();
    LL_Lepton1GenMatchDaughter0Id->clear();
    LL_Lepton1GenMatchDaughter0Status->clear();
    LL_Lepton1GenMatchDaughter1Id->clear();
    LL_Lepton1GenMatchDaughter1Status->clear();
    LL_Lepton1GenMatchGrandmother00Id->clear();
    LL_Lepton1GenMatchGrandmother00Status->clear();
    LL_Lepton1GenMatchGrandmother01Id->clear();
    LL_Lepton1GenMatchGrandmother01Status->clear();
    LL_Lepton1GenMatchGrandmother10Id->clear();
    LL_Lepton1GenMatchGrandmother10Status->clear();
    LL_Lepton1GenMatchGrandmother11Id->clear();
    LL_Lepton1GenMatchGrandmother11Status->clear();
    LL_Lepton1GenMatchId->clear();
    LL_Lepton1GenMatchMother0Id->clear();
    LL_Lepton1GenMatchMother0Status->clear();
    LL_Lepton1GenMatchMother1Id->clear();
    LL_Lepton1GenMatchMother1Status->clear();
    LL_Lepton1GenMatchStatus->clear();
    LL_Lepton1ImpactParameter->clear();
    LL_Lepton1IsElectron->clear();
    LL_Lepton1IsLoose->clear();
    LL_Lepton1IsMuon->clear();
    LL_Lepton1IsTight->clear();
    LL_Lepton1Lepton2CosDeltaPhi->clear();
    LL_Lepton1Lepton2DeltaR->clear();
    LL_Lepton1Lepton2VisibleMass->clear();
    LL_Lepton1Mt->clear();
    LL_Lepton1Mt2->clear();
    LL_Lepton1P->clear();
    LL_Lepton1Phi->clear();
    LL_Lepton1Pt->clear();
    LL_Lepton1RelIso->clear();
    LL_Lepton2Charge->clear();
    LL_Lepton2CorrectedD0->clear();
    LL_Lepton2CorrectedDZ->clear();
    LL_Lepton2Eta->clear();
    LL_Lepton2EventWeight->clear();
    LL_Lepton2GenMatchDaughter0Id->clear();
    LL_Lepton2GenMatchDaughter0Status->clear();
    LL_Lepton2GenMatchDaughter1Id->clear();
    LL_Lepton2GenMatchDaughter1Status->clear();
    LL_Lepton2GenMatchGrandmother00Id->clear();
    LL_Lepton2GenMatchGrandmother00Status->clear();
    LL_Lepton2GenMatchGrandmother01Id->clear();
    LL_Lepton2GenMatchGrandmother01Status->clear();
    LL_Lepton2GenMatchGrandmother10Id->clear();
    LL_Lepton2GenMatchGrandmother10Status->clear();
    LL_Lepton2GenMatchGrandmother11Id->clear();
    LL_Lepton2GenMatchGrandmother11Status->clear();
    LL_Lepton2GenMatchId->clear();
    LL_Lepton2GenMatchMother0Id->clear();
    LL_Lepton2GenMatchMother0Status->clear();
    LL_Lepton2GenMatchMother1Id->clear();
    LL_Lepton2GenMatchMother1Status->clear();
    LL_Lepton2GenMatchStatus->clear();
    LL_Lepton2ImpactParameter->clear();
    LL_Lepton2IsElectron->clear();
    LL_Lepton2IsLoose->clear();
    LL_Lepton2IsMuon->clear();
    LL_Lepton2IsTight->clear();
    LL_Lepton2Mt->clear();
    LL_Lepton2Mt2->clear();
    LL_Lepton2P->clear();
    LL_Lepton2Phi->clear();
    LL_Lepton2Pt->clear();
    LL_Lepton2RelIso->clear();
    LL_PassZMask->clear();
    LL_PassZMask2->clear();
    LL_TauCharge->clear();
    LL_TauDecayMode->clear();
    LL_TauEmFraction->clear();
    LL_TauEta->clear();
    LL_TauGenMatchDaughter0Id->clear();
    LL_TauGenMatchDaughter0Status->clear();
    LL_TauGenMatchDaughter1Id->clear();
    LL_TauGenMatchDaughter1Status->clear();
    LL_TauGenMatchGrandmother00Id->clear();
    LL_TauGenMatchGrandmother00Status->clear();
    LL_TauGenMatchGrandmother01Id->clear();
    LL_TauGenMatchGrandmother01Status->clear();
    LL_TauGenMatchGrandmother10Id->clear();
    LL_TauGenMatchGrandmother10Status->clear();
    LL_TauGenMatchGrandmother11Id->clear();
    LL_TauGenMatchGrandmother11Status->clear();
    LL_TauGenMatchId->clear();
    LL_TauGenMatchMother0Id->clear();
    LL_TauGenMatchMother0Status->clear();
    LL_TauGenMatchMother1Id->clear();
    LL_TauGenMatchMother1Status->clear();
    LL_TauGenMatchStatus->clear();
    LL_TauHPSagainstElectronDeadECAL->clear();
    LL_TauHPSagainstElectronLoose->clear();
    LL_TauHPSagainstElectronLooseMVA2->clear();
    LL_TauHPSagainstElectronLooseMVA3->clear();
    LL_TauHPSagainstElectronMVA->clear();
    LL_TauHPSagainstElectronMVA2category->clear();
    LL_TauHPSagainstElectronMVA2raw->clear();
    LL_TauHPSagainstElectronMVA3category->clear();
    LL_TauHPSagainstElectronMVA3raw->clear();
    LL_TauHPSagainstElectronMedium->clear();
    LL_TauHPSagainstElectronMediumMVA2->clear();
    LL_TauHPSagainstElectronMediumMVA3->clear();
    LL_TauHPSagainstElectronTight->clear();
    LL_TauHPSagainstElectronTightMVA2->clear();
    LL_TauHPSagainstElectronTightMVA3->clear();
    LL_TauHPSagainstElectronVLooseMVA2->clear();
    LL_TauHPSagainstElectronVTightMVA3->clear();
    LL_TauHPSagainstMuonLoose->clear();
    LL_TauHPSagainstMuonLoose2->clear();
    LL_TauHPSagainstMuonMedium->clear();
    LL_TauHPSagainstMuonMedium2->clear();
    LL_TauHPSagainstMuonTight->clear();
    LL_TauHPSagainstMuonTight2->clear();
    LL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw->clear();
    LL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw3Hits->clear();
    LL_TauHPSbyIsolationMVA2raw->clear();
    LL_TauHPSbyIsolationMVAraw->clear();
    LL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr->clear();
    LL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr3Hits->clear();
    LL_TauHPSbyLooseIsolationMVA->clear();
    LL_TauHPSbyLooseIsolationMVA2->clear();
    LL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr->clear();
    LL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr3Hits->clear();
    LL_TauHPSbyMediumIsolationMVA->clear();
    LL_TauHPSbyMediumIsolationMVA2->clear();
    LL_TauHPSbyTightCombinedIsolationDeltaBetaCorr->clear();
    LL_TauHPSbyTightCombinedIsolationDeltaBetaCorr3Hits->clear();
    LL_TauHPSbyTightIsolationMVA->clear();
    LL_TauHPSbyTightIsolationMVA2->clear();
    LL_TauHPSbyVLooseCombinedIsolationDeltaBetaCorr->clear();
    LL_TauHPSdecayModeFinding->clear();
    LL_TauIsInTheCracks->clear();
    LL_TauLTIpVtdxy->clear();
    LL_TauLTIpVtdxyError->clear();
    LL_TauLTIpVtdz->clear();
    LL_TauLTIpVtdzError->clear();
    LL_TauLTNormChiSqrd->clear();
    LL_TauLTPt->clear();
    LL_TauLTValidHits->clear();
    LL_TauLTvalid->clear();
    LL_TauLTvx->clear();
    LL_TauLTvy->clear();
    LL_TauLTvz->clear();
    LL_TauLepton1CosDeltaPhi->clear();
    LL_TauLepton1DeltaR->clear();
    LL_TauLepton1METMass->clear();
    LL_TauLepton1VisibleMass->clear();
    LL_TauLepton2CosDeltaPhi->clear();
    LL_TauLepton2DeltaR->clear();
    LL_TauLepton2METMass->clear();
    LL_TauLepton2VisibleMass->clear();
    LL_TauMomentumRank->clear();
    LL_TauNProngs->clear();
    LL_TauNSignalGammas->clear();
    LL_TauNSignalNeutrals->clear();
    LL_TauP->clear();
    LL_TauPhi->clear();
    LL_TauPt->clear();
    LL_TriggerEventWeight->clear();
    // >>> End clear <<<
}

void
ll::Branches::Delete()
{
    roast::Branches::Delete();

    // >>> Begin delete <<<
    delete Ev_METcov;
    delete J_Lepton1DeltaR;
    delete J_Lepton2DeltaR;
    delete LL_Lepton1Charge;
    delete LL_Lepton1CorrectedD0;
    delete LL_Lepton1CorrectedDZ;
    delete LL_Lepton1Eta;
    delete LL_Lepton1EventWeight;
    delete LL_Lepton1GenMatchDaughter0Id;
    delete LL_Lepton1GenMatchDaughter0Status;
    delete LL_Lepton1GenMatchDaughter1Id;
    delete LL_Lepton1GenMatchDaughter1Status;
    delete LL_Lepton1GenMatchGrandmother00Id;
    delete LL_Lepton1GenMatchGrandmother00Status;
    delete LL_Lepton1GenMatchGrandmother01Id;
    delete LL_Lepton1GenMatchGrandmother01Status;
    delete LL_Lepton1GenMatchGrandmother10Id;
    delete LL_Lepton1GenMatchGrandmother10Status;
    delete LL_Lepton1GenMatchGrandmother11Id;
    delete LL_Lepton1GenMatchGrandmother11Status;
    delete LL_Lepton1GenMatchId;
    delete LL_Lepton1GenMatchMother0Id;
    delete LL_Lepton1GenMatchMother0Status;
    delete LL_Lepton1GenMatchMother1Id;
    delete LL_Lepton1GenMatchMother1Status;
    delete LL_Lepton1GenMatchStatus;
    delete LL_Lepton1ImpactParameter;
    delete LL_Lepton1IsElectron;
    delete LL_Lepton1IsLoose;
    delete LL_Lepton1IsMuon;
    delete LL_Lepton1IsTight;
    delete LL_Lepton1Lepton2CosDeltaPhi;
    delete LL_Lepton1Lepton2DeltaR;
    delete LL_Lepton1Lepton2VisibleMass;
    delete LL_Lepton1Mt;
    delete LL_Lepton1Mt2;
    delete LL_Lepton1P;
    delete LL_Lepton1Phi;
    delete LL_Lepton1Pt;
    delete LL_Lepton1RelIso;
    delete LL_Lepton2Charge;
    delete LL_Lepton2CorrectedD0;
    delete LL_Lepton2CorrectedDZ;
    delete LL_Lepton2Eta;
    delete LL_Lepton2EventWeight;
    delete LL_Lepton2GenMatchDaughter0Id;
    delete LL_Lepton2GenMatchDaughter0Status;
    delete LL_Lepton2GenMatchDaughter1Id;
    delete LL_Lepton2GenMatchDaughter1Status;
    delete LL_Lepton2GenMatchGrandmother00Id;
    delete LL_Lepton2GenMatchGrandmother00Status;
    delete LL_Lepton2GenMatchGrandmother01Id;
    delete LL_Lepton2GenMatchGrandmother01Status;
    delete LL_Lepton2GenMatchGrandmother10Id;
    delete LL_Lepton2GenMatchGrandmother10Status;
    delete LL_Lepton2GenMatchGrandmother11Id;
    delete LL_Lepton2GenMatchGrandmother11Status;
    delete LL_Lepton2GenMatchId;
    delete LL_Lepton2GenMatchMother0Id;
    delete LL_Lepton2GenMatchMother0Status;
    delete LL_Lepton2GenMatchMother1Id;
    delete LL_Lepton2GenMatchMother1Status;
    delete LL_Lepton2GenMatchStatus;
    delete LL_Lepton2ImpactParameter;
    delete LL_Lepton2IsElectron;
    delete LL_Lepton2IsLoose;
    delete LL_Lepton2IsMuon;
    delete LL_Lepton2IsTight;
    delete LL_Lepton2Mt;
    delete LL_Lepton2Mt2;
    delete LL_Lepton2P;
    delete LL_Lepton2Phi;
    delete LL_Lepton2Pt;
    delete LL_Lepton2RelIso;
    delete LL_PassZMask;
    delete LL_PassZMask2;
    delete LL_TauCharge;
    delete LL_TauDecayMode;
    delete LL_TauEmFraction;
    delete LL_TauEta;
    delete LL_TauGenMatchDaughter0Id;
    delete LL_TauGenMatchDaughter0Status;
    delete LL_TauGenMatchDaughter1Id;
    delete LL_TauGenMatchDaughter1Status;
    delete LL_TauGenMatchGrandmother00Id;
    delete LL_TauGenMatchGrandmother00Status;
    delete LL_TauGenMatchGrandmother01Id;
    delete LL_TauGenMatchGrandmother01Status;
    delete LL_TauGenMatchGrandmother10Id;
    delete LL_TauGenMatchGrandmother10Status;
    delete LL_TauGenMatchGrandmother11Id;
    delete LL_TauGenMatchGrandmother11Status;
    delete LL_TauGenMatchId;
    delete LL_TauGenMatchMother0Id;
    delete LL_TauGenMatchMother0Status;
    delete LL_TauGenMatchMother1Id;
    delete LL_TauGenMatchMother1Status;
    delete LL_TauGenMatchStatus;
    delete LL_TauHPSagainstElectronDeadECAL;
    delete LL_TauHPSagainstElectronLoose;
    delete LL_TauHPSagainstElectronLooseMVA2;
    delete LL_TauHPSagainstElectronLooseMVA3;
    delete LL_TauHPSagainstElectronMVA;
    delete LL_TauHPSagainstElectronMVA2category;
    delete LL_TauHPSagainstElectronMVA2raw;
    delete LL_TauHPSagainstElectronMVA3category;
    delete LL_TauHPSagainstElectronMVA3raw;
    delete LL_TauHPSagainstElectronMedium;
    delete LL_TauHPSagainstElectronMediumMVA2;
    delete LL_TauHPSagainstElectronMediumMVA3;
    delete LL_TauHPSagainstElectronTight;
    delete LL_TauHPSagainstElectronTightMVA2;
    delete LL_TauHPSagainstElectronTightMVA3;
    delete LL_TauHPSagainstElectronVLooseMVA2;
    delete LL_TauHPSagainstElectronVTightMVA3;
    delete LL_TauHPSagainstMuonLoose;
    delete LL_TauHPSagainstMuonLoose2;
    delete LL_TauHPSagainstMuonMedium;
    delete LL_TauHPSagainstMuonMedium2;
    delete LL_TauHPSagainstMuonTight;
    delete LL_TauHPSagainstMuonTight2;
    delete LL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw;
    delete LL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw3Hits;
    delete LL_TauHPSbyIsolationMVA2raw;
    delete LL_TauHPSbyIsolationMVAraw;
    delete LL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr;
    delete LL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr3Hits;
    delete LL_TauHPSbyLooseIsolationMVA;
    delete LL_TauHPSbyLooseIsolationMVA2;
    delete LL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr;
    delete LL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr3Hits;
    delete LL_TauHPSbyMediumIsolationMVA;
    delete LL_TauHPSbyMediumIsolationMVA2;
    delete LL_TauHPSbyTightCombinedIsolationDeltaBetaCorr;
    delete LL_TauHPSbyTightCombinedIsolationDeltaBetaCorr3Hits;
    delete LL_TauHPSbyTightIsolationMVA;
    delete LL_TauHPSbyTightIsolationMVA2;
    delete LL_TauHPSbyVLooseCombinedIsolationDeltaBetaCorr;
    delete LL_TauHPSdecayModeFinding;
    delete LL_TauIsInTheCracks;
    delete LL_TauLTIpVtdxy;
    delete LL_TauLTIpVtdxyError;
    delete LL_TauLTIpVtdz;
    delete LL_TauLTIpVtdzError;
    delete LL_TauLTNormChiSqrd;
    delete LL_TauLTPt;
    delete LL_TauLTValidHits;
    delete LL_TauLTvalid;
    delete LL_TauLTvx;
    delete LL_TauLTvy;
    delete LL_TauLTvz;
    delete LL_TauLepton1CosDeltaPhi;
    delete LL_TauLepton1DeltaR;
    delete LL_TauLepton1METMass;
    delete LL_TauLepton1VisibleMass;
    delete LL_TauLepton2CosDeltaPhi;
    delete LL_TauLepton2DeltaR;
    delete LL_TauLepton2METMass;
    delete LL_TauLepton2VisibleMass;
    delete LL_TauMomentumRank;
    delete LL_TauNProngs;
    delete LL_TauNSignalGammas;
    delete LL_TauNSignalNeutrals;
    delete LL_TauP;
    delete LL_TauPhi;
    delete LL_TauPt;
    delete LL_TriggerEventWeight;
    // >>> End delete <<<
}

void
ll::Branches::Null()
{
    roast::Branches::Null();

    // >>> Begin null <<<
    Ev_METcov = 0;
    Ev_NTruePV = 0;
    J_Lepton1DeltaR = 0;
    J_Lepton2DeltaR = 0;
    LL_Lepton1Charge = 0;
    LL_Lepton1CorrectedD0 = 0;
    LL_Lepton1CorrectedDZ = 0;
    LL_Lepton1Eta = 0;
    LL_Lepton1EventWeight = 0;
    LL_Lepton1GenMatchDaughter0Id = 0;
    LL_Lepton1GenMatchDaughter0Status = 0;
    LL_Lepton1GenMatchDaughter1Id = 0;
    LL_Lepton1GenMatchDaughter1Status = 0;
    LL_Lepton1GenMatchGrandmother00Id = 0;
    LL_Lepton1GenMatchGrandmother00Status = 0;
    LL_Lepton1GenMatchGrandmother01Id = 0;
    LL_Lepton1GenMatchGrandmother01Status = 0;
    LL_Lepton1GenMatchGrandmother10Id = 0;
    LL_Lepton1GenMatchGrandmother10Status = 0;
    LL_Lepton1GenMatchGrandmother11Id = 0;
    LL_Lepton1GenMatchGrandmother11Status = 0;
    LL_Lepton1GenMatchId = 0;
    LL_Lepton1GenMatchMother0Id = 0;
    LL_Lepton1GenMatchMother0Status = 0;
    LL_Lepton1GenMatchMother1Id = 0;
    LL_Lepton1GenMatchMother1Status = 0;
    LL_Lepton1GenMatchStatus = 0;
    LL_Lepton1ImpactParameter = 0;
    LL_Lepton1IsElectron = 0;
    LL_Lepton1IsLoose = 0;
    LL_Lepton1IsMuon = 0;
    LL_Lepton1IsTight = 0;
    LL_Lepton1Lepton2CosDeltaPhi = 0;
    LL_Lepton1Lepton2DeltaR = 0;
    LL_Lepton1Lepton2VisibleMass = 0;
    LL_Lepton1Mt = 0;
    LL_Lepton1Mt2 = 0;
    LL_Lepton1P = 0;
    LL_Lepton1Phi = 0;
    LL_Lepton1Pt = 0;
    LL_Lepton1RelIso = 0;
    LL_Lepton2Charge = 0;
    LL_Lepton2CorrectedD0 = 0;
    LL_Lepton2CorrectedDZ = 0;
    LL_Lepton2Eta = 0;
    LL_Lepton2EventWeight = 0;
    LL_Lepton2GenMatchDaughter0Id = 0;
    LL_Lepton2GenMatchDaughter0Status = 0;
    LL_Lepton2GenMatchDaughter1Id = 0;
    LL_Lepton2GenMatchDaughter1Status = 0;
    LL_Lepton2GenMatchGrandmother00Id = 0;
    LL_Lepton2GenMatchGrandmother00Status = 0;
    LL_Lepton2GenMatchGrandmother01Id = 0;
    LL_Lepton2GenMatchGrandmother01Status = 0;
    LL_Lepton2GenMatchGrandmother10Id = 0;
    LL_Lepton2GenMatchGrandmother10Status = 0;
    LL_Lepton2GenMatchGrandmother11Id = 0;
    LL_Lepton2GenMatchGrandmother11Status = 0;
    LL_Lepton2GenMatchId = 0;
    LL_Lepton2GenMatchMother0Id = 0;
    LL_Lepton2GenMatchMother0Status = 0;
    LL_Lepton2GenMatchMother1Id = 0;
    LL_Lepton2GenMatchMother1Status = 0;
    LL_Lepton2GenMatchStatus = 0;
    LL_Lepton2ImpactParameter = 0;
    LL_Lepton2IsElectron = 0;
    LL_Lepton2IsLoose = 0;
    LL_Lepton2IsMuon = 0;
    LL_Lepton2IsTight = 0;
    LL_Lepton2Mt = 0;
    LL_Lepton2Mt2 = 0;
    LL_Lepton2P = 0;
    LL_Lepton2Phi = 0;
    LL_Lepton2Pt = 0;
    LL_Lepton2RelIso = 0;
    LL_PassZMask = 0;
    LL_PassZMask2 = 0;
    LL_TauCharge = 0;
    LL_TauDecayMode = 0;
    LL_TauEmFraction = 0;
    LL_TauEta = 0;
    LL_TauGenMatchDaughter0Id = 0;
    LL_TauGenMatchDaughter0Status = 0;
    LL_TauGenMatchDaughter1Id = 0;
    LL_TauGenMatchDaughter1Status = 0;
    LL_TauGenMatchGrandmother00Id = 0;
    LL_TauGenMatchGrandmother00Status = 0;
    LL_TauGenMatchGrandmother01Id = 0;
    LL_TauGenMatchGrandmother01Status = 0;
    LL_TauGenMatchGrandmother10Id = 0;
    LL_TauGenMatchGrandmother10Status = 0;
    LL_TauGenMatchGrandmother11Id = 0;
    LL_TauGenMatchGrandmother11Status = 0;
    LL_TauGenMatchId = 0;
    LL_TauGenMatchMother0Id = 0;
    LL_TauGenMatchMother0Status = 0;
    LL_TauGenMatchMother1Id = 0;
    LL_TauGenMatchMother1Status = 0;
    LL_TauGenMatchStatus = 0;
    LL_TauHPSagainstElectronDeadECAL = 0;
    LL_TauHPSagainstElectronLoose = 0;
    LL_TauHPSagainstElectronLooseMVA2 = 0;
    LL_TauHPSagainstElectronLooseMVA3 = 0;
    LL_TauHPSagainstElectronMVA = 0;
    LL_TauHPSagainstElectronMVA2category = 0;
    LL_TauHPSagainstElectronMVA2raw = 0;
    LL_TauHPSagainstElectronMVA3category = 0;
    LL_TauHPSagainstElectronMVA3raw = 0;
    LL_TauHPSagainstElectronMedium = 0;
    LL_TauHPSagainstElectronMediumMVA2 = 0;
    LL_TauHPSagainstElectronMediumMVA3 = 0;
    LL_TauHPSagainstElectronTight = 0;
    LL_TauHPSagainstElectronTightMVA2 = 0;
    LL_TauHPSagainstElectronTightMVA3 = 0;
    LL_TauHPSagainstElectronVLooseMVA2 = 0;
    LL_TauHPSagainstElectronVTightMVA3 = 0;
    LL_TauHPSagainstMuonLoose = 0;
    LL_TauHPSagainstMuonLoose2 = 0;
    LL_TauHPSagainstMuonMedium = 0;
    LL_TauHPSagainstMuonMedium2 = 0;
    LL_TauHPSagainstMuonTight = 0;
    LL_TauHPSagainstMuonTight2 = 0;
    LL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw = 0;
    LL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw3Hits = 0;
    LL_TauHPSbyIsolationMVA2raw = 0;
    LL_TauHPSbyIsolationMVAraw = 0;
    LL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr = 0;
    LL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr3Hits = 0;
    LL_TauHPSbyLooseIsolationMVA = 0;
    LL_TauHPSbyLooseIsolationMVA2 = 0;
    LL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr = 0;
    LL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr3Hits = 0;
    LL_TauHPSbyMediumIsolationMVA = 0;
    LL_TauHPSbyMediumIsolationMVA2 = 0;
    LL_TauHPSbyTightCombinedIsolationDeltaBetaCorr = 0;
    LL_TauHPSbyTightCombinedIsolationDeltaBetaCorr3Hits = 0;
    LL_TauHPSbyTightIsolationMVA = 0;
    LL_TauHPSbyTightIsolationMVA2 = 0;
    LL_TauHPSbyVLooseCombinedIsolationDeltaBetaCorr = 0;
    LL_TauHPSdecayModeFinding = 0;
    LL_TauIsInTheCracks = 0;
    LL_TauLTIpVtdxy = 0;
    LL_TauLTIpVtdxyError = 0;
    LL_TauLTIpVtdz = 0;
    LL_TauLTIpVtdzError = 0;
    LL_TauLTNormChiSqrd = 0;
    LL_TauLTPt = 0;
    LL_TauLTValidHits = 0;
    LL_TauLTvalid = 0;
    LL_TauLTvx = 0;
    LL_TauLTvy = 0;
    LL_TauLTvz = 0;
    LL_TauLepton1CosDeltaPhi = 0;
    LL_TauLepton1DeltaR = 0;
    LL_TauLepton1METMass = 0;
    LL_TauLepton1VisibleMass = 0;
    LL_TauLepton2CosDeltaPhi = 0;
    LL_TauLepton2DeltaR = 0;
    LL_TauLepton2METMass = 0;
    LL_TauLepton2VisibleMass = 0;
    LL_TauMomentumRank = 0;
    LL_TauNProngs = 0;
    LL_TauNSignalGammas = 0;
    LL_TauNSignalNeutrals = 0;
    LL_TauP = 0;
    LL_TauPhi = 0;
    LL_TauPt = 0;
    LL_TriggerEventWeight = 0;
    // >>> End null <<<
}

void
ll::Branches::SetBranchAddresses()
{
    roast::Branches::SetBranchAddresses();

    // >>> Begin address <<<
    fChain->SetBranchAddress("Ev_METcov", &Ev_METcov);
    fChain->SetBranchAddress("Ev_NTruePV", &Ev_NTruePV);
    fChain->SetBranchAddress("J_Lepton1DeltaR", &J_Lepton1DeltaR);
    fChain->SetBranchAddress("J_Lepton2DeltaR", &J_Lepton2DeltaR);
    fChain->SetBranchAddress("LL_CSVeventWeight", &CSVeventWeight);
    fChain->SetBranchAddress("LL_CSVeventWeightCErr1down", &CSVeventWeightCErr1down);
    fChain->SetBranchAddress("LL_CSVeventWeightCErr1up", &CSVeventWeightCErr1up);
    fChain->SetBranchAddress("LL_CSVeventWeightCErr2down", &CSVeventWeightCErr2down);
    fChain->SetBranchAddress("LL_CSVeventWeightCErr2up", &CSVeventWeightCErr2up);
    fChain->SetBranchAddress("LL_CSVeventWeightHFStats1down", &CSVeventWeightHFStats1down);
    fChain->SetBranchAddress("LL_CSVeventWeightHFStats1up", &CSVeventWeightHFStats1up);
    fChain->SetBranchAddress("LL_CSVeventWeightHFStats2down", &CSVeventWeightHFStats2down);
    fChain->SetBranchAddress("LL_CSVeventWeightHFStats2up", &CSVeventWeightHFStats2up);
    fChain->SetBranchAddress("LL_CSVeventWeightHFdown", &CSVeventWeightHFdown);
    fChain->SetBranchAddress("LL_CSVeventWeightHFup", &CSVeventWeightHFup);
    fChain->SetBranchAddress("LL_CSVeventWeightLFStats1down", &CSVeventWeightLFStats1down);
    fChain->SetBranchAddress("LL_CSVeventWeightLFStats1up", &CSVeventWeightLFStats1up);
    fChain->SetBranchAddress("LL_CSVeventWeightLFStats2down", &CSVeventWeightLFStats2down);
    fChain->SetBranchAddress("LL_CSVeventWeightLFStats2up", &CSVeventWeightLFStats2up);
    fChain->SetBranchAddress("LL_CSVeventWeightLFdown", &CSVeventWeightLFdown);
    fChain->SetBranchAddress("LL_CSVeventWeightLFup", &CSVeventWeightLFup);
    fChain->SetBranchAddress("LL_CleanJetCSVMIndices", &CleanJetCSVMIndices);
    fChain->SetBranchAddress("LL_CleanJetIndices", &CleanJetIndices);
    fChain->SetBranchAddress("LL_CleanJetNonCSVMIndices", &CleanJetNonCSVMIndices);
    fChain->SetBranchAddress("LL_HT", &HT);
    fChain->SetBranchAddress("LL_Lepton1Charge", &LL_Lepton1Charge);
    fChain->SetBranchAddress("LL_Lepton1CorrectedD0", &LL_Lepton1CorrectedD0);
    fChain->SetBranchAddress("LL_Lepton1CorrectedDZ", &LL_Lepton1CorrectedDZ);
    fChain->SetBranchAddress("LL_Lepton1Eta", &LL_Lepton1Eta);
    fChain->SetBranchAddress("LL_Lepton1EventWeight", &LL_Lepton1EventWeight);
    fChain->SetBranchAddress("LL_Lepton1GenMatchDaughter0Id", &LL_Lepton1GenMatchDaughter0Id);
    fChain->SetBranchAddress("LL_Lepton1GenMatchDaughter0Status", &LL_Lepton1GenMatchDaughter0Status);
    fChain->SetBranchAddress("LL_Lepton1GenMatchDaughter1Id", &LL_Lepton1GenMatchDaughter1Id);
    fChain->SetBranchAddress("LL_Lepton1GenMatchDaughter1Status", &LL_Lepton1GenMatchDaughter1Status);
    fChain->SetBranchAddress("LL_Lepton1GenMatchGrandmother00Id", &LL_Lepton1GenMatchGrandmother00Id);
    fChain->SetBranchAddress("LL_Lepton1GenMatchGrandmother00Status", &LL_Lepton1GenMatchGrandmother00Status);
    fChain->SetBranchAddress("LL_Lepton1GenMatchGrandmother01Id", &LL_Lepton1GenMatchGrandmother01Id);
    fChain->SetBranchAddress("LL_Lepton1GenMatchGrandmother01Status", &LL_Lepton1GenMatchGrandmother01Status);
    fChain->SetBranchAddress("LL_Lepton1GenMatchGrandmother10Id", &LL_Lepton1GenMatchGrandmother10Id);
    fChain->SetBranchAddress("LL_Lepton1GenMatchGrandmother10Status", &LL_Lepton1GenMatchGrandmother10Status);
    fChain->SetBranchAddress("LL_Lepton1GenMatchGrandmother11Id", &LL_Lepton1GenMatchGrandmother11Id);
    fChain->SetBranchAddress("LL_Lepton1GenMatchGrandmother11Status", &LL_Lepton1GenMatchGrandmother11Status);
    fChain->SetBranchAddress("LL_Lepton1GenMatchId", &LL_Lepton1GenMatchId);
    fChain->SetBranchAddress("LL_Lepton1GenMatchMother0Id", &LL_Lepton1GenMatchMother0Id);
    fChain->SetBranchAddress("LL_Lepton1GenMatchMother0Status", &LL_Lepton1GenMatchMother0Status);
    fChain->SetBranchAddress("LL_Lepton1GenMatchMother1Id", &LL_Lepton1GenMatchMother1Id);
    fChain->SetBranchAddress("LL_Lepton1GenMatchMother1Status", &LL_Lepton1GenMatchMother1Status);
    fChain->SetBranchAddress("LL_Lepton1GenMatchStatus", &LL_Lepton1GenMatchStatus);
    fChain->SetBranchAddress("LL_Lepton1ImpactParameter", &LL_Lepton1ImpactParameter);
    fChain->SetBranchAddress("LL_Lepton1IsElectron", &LL_Lepton1IsElectron);
    fChain->SetBranchAddress("LL_Lepton1IsLoose", &LL_Lepton1IsLoose);
    fChain->SetBranchAddress("LL_Lepton1IsMuon", &LL_Lepton1IsMuon);
    fChain->SetBranchAddress("LL_Lepton1IsTight", &LL_Lepton1IsTight);
    fChain->SetBranchAddress("LL_Lepton1Lepton2CosDeltaPhi", &LL_Lepton1Lepton2CosDeltaPhi);
    fChain->SetBranchAddress("LL_Lepton1Lepton2DeltaR", &LL_Lepton1Lepton2DeltaR);
    fChain->SetBranchAddress("LL_Lepton1Lepton2VisibleMass", &LL_Lepton1Lepton2VisibleMass);
    fChain->SetBranchAddress("LL_Lepton1Mt", &LL_Lepton1Mt);
    fChain->SetBranchAddress("LL_Lepton1Mt2", &LL_Lepton1Mt2);
    fChain->SetBranchAddress("LL_Lepton1P", &LL_Lepton1P);
    fChain->SetBranchAddress("LL_Lepton1Phi", &LL_Lepton1Phi);
    fChain->SetBranchAddress("LL_Lepton1Pt", &LL_Lepton1Pt);
    fChain->SetBranchAddress("LL_Lepton1RelIso", &LL_Lepton1RelIso);
    fChain->SetBranchAddress("LL_Lepton2Charge", &LL_Lepton2Charge);
    fChain->SetBranchAddress("LL_Lepton2CorrectedD0", &LL_Lepton2CorrectedD0);
    fChain->SetBranchAddress("LL_Lepton2CorrectedDZ", &LL_Lepton2CorrectedDZ);
    fChain->SetBranchAddress("LL_Lepton2Eta", &LL_Lepton2Eta);
    fChain->SetBranchAddress("LL_Lepton2EventWeight", &LL_Lepton2EventWeight);
    fChain->SetBranchAddress("LL_Lepton2GenMatchDaughter0Id", &LL_Lepton2GenMatchDaughter0Id);
    fChain->SetBranchAddress("LL_Lepton2GenMatchDaughter0Status", &LL_Lepton2GenMatchDaughter0Status);
    fChain->SetBranchAddress("LL_Lepton2GenMatchDaughter1Id", &LL_Lepton2GenMatchDaughter1Id);
    fChain->SetBranchAddress("LL_Lepton2GenMatchDaughter1Status", &LL_Lepton2GenMatchDaughter1Status);
    fChain->SetBranchAddress("LL_Lepton2GenMatchGrandmother00Id", &LL_Lepton2GenMatchGrandmother00Id);
    fChain->SetBranchAddress("LL_Lepton2GenMatchGrandmother00Status", &LL_Lepton2GenMatchGrandmother00Status);
    fChain->SetBranchAddress("LL_Lepton2GenMatchGrandmother01Id", &LL_Lepton2GenMatchGrandmother01Id);
    fChain->SetBranchAddress("LL_Lepton2GenMatchGrandmother01Status", &LL_Lepton2GenMatchGrandmother01Status);
    fChain->SetBranchAddress("LL_Lepton2GenMatchGrandmother10Id", &LL_Lepton2GenMatchGrandmother10Id);
    fChain->SetBranchAddress("LL_Lepton2GenMatchGrandmother10Status", &LL_Lepton2GenMatchGrandmother10Status);
    fChain->SetBranchAddress("LL_Lepton2GenMatchGrandmother11Id", &LL_Lepton2GenMatchGrandmother11Id);
    fChain->SetBranchAddress("LL_Lepton2GenMatchGrandmother11Status", &LL_Lepton2GenMatchGrandmother11Status);
    fChain->SetBranchAddress("LL_Lepton2GenMatchId", &LL_Lepton2GenMatchId);
    fChain->SetBranchAddress("LL_Lepton2GenMatchMother0Id", &LL_Lepton2GenMatchMother0Id);
    fChain->SetBranchAddress("LL_Lepton2GenMatchMother0Status", &LL_Lepton2GenMatchMother0Status);
    fChain->SetBranchAddress("LL_Lepton2GenMatchMother1Id", &LL_Lepton2GenMatchMother1Id);
    fChain->SetBranchAddress("LL_Lepton2GenMatchMother1Status", &LL_Lepton2GenMatchMother1Status);
    fChain->SetBranchAddress("LL_Lepton2GenMatchStatus", &LL_Lepton2GenMatchStatus);
    fChain->SetBranchAddress("LL_Lepton2ImpactParameter", &LL_Lepton2ImpactParameter);
    fChain->SetBranchAddress("LL_Lepton2IsElectron", &LL_Lepton2IsElectron);
    fChain->SetBranchAddress("LL_Lepton2IsLoose", &LL_Lepton2IsLoose);
    fChain->SetBranchAddress("LL_Lepton2IsMuon", &LL_Lepton2IsMuon);
    fChain->SetBranchAddress("LL_Lepton2IsTight", &LL_Lepton2IsTight);
    fChain->SetBranchAddress("LL_Lepton2Mt", &LL_Lepton2Mt);
    fChain->SetBranchAddress("LL_Lepton2Mt2", &LL_Lepton2Mt2);
    fChain->SetBranchAddress("LL_Lepton2P", &LL_Lepton2P);
    fChain->SetBranchAddress("LL_Lepton2Phi", &LL_Lepton2Phi);
    fChain->SetBranchAddress("LL_Lepton2Pt", &LL_Lepton2Pt);
    fChain->SetBranchAddress("LL_Lepton2RelIso", &LL_Lepton2RelIso);
    fChain->SetBranchAddress("LL_MHT", &MHT);
    fChain->SetBranchAddress("LL_MomentumRank", &MomentumRank);
    fChain->SetBranchAddress("LL_NumCSVLbtagJets", &NumCSVLbtagJets);
    fChain->SetBranchAddress("LL_NumCSVMbtagJets", &NumCSVMbtagJets);
    fChain->SetBranchAddress("LL_NumCSVTbtagJets", &NumCSVTbtagJets);
    fChain->SetBranchAddress("LL_NumCleanCSVLbtagJets", &NumCleanCSVLbtagJets);
    fChain->SetBranchAddress("LL_NumCleanCSVMbtagJets", &NumCleanCSVMbtagJets);
    fChain->SetBranchAddress("LL_NumCleanCSVTbtagJets", &NumCleanCSVTbtagJets);
    fChain->SetBranchAddress("LL_NumCleanNonCSVLbtagJets", &NumCleanNonCSVLbtagJets);
    fChain->SetBranchAddress("LL_NumCleanNonCSVMbtagJets", &NumCleanNonCSVMbtagJets);
    fChain->SetBranchAddress("LL_NumCleanNonCSVTbtagJets", &NumCleanNonCSVTbtagJets);
    fChain->SetBranchAddress("LL_NumCombos", &NumCombos);
    fChain->SetBranchAddress("LL_NumExLooseElectrons", &NumExLooseElectrons);
    fChain->SetBranchAddress("LL_NumExLooseMuons", &NumExLooseMuons);
    fChain->SetBranchAddress("LL_NumLooseElectrons", &NumLooseElectrons);
    fChain->SetBranchAddress("LL_NumLooseMuons", &NumLooseMuons);
    fChain->SetBranchAddress("LL_NumNonCSVLbtagJets", &NumNonCSVLbtagJets);
    fChain->SetBranchAddress("LL_NumNonCSVMbtagJets", &NumNonCSVMbtagJets);
    fChain->SetBranchAddress("LL_NumNonCSVTbtagJets", &NumNonCSVTbtagJets);
    fChain->SetBranchAddress("LL_NumTaus", &NumTaus);
    fChain->SetBranchAddress("LL_NumTightElectrons", &NumTightElectrons);
    fChain->SetBranchAddress("LL_NumTightMuons", &NumTightMuons);
    fChain->SetBranchAddress("LL_PassZMask", &LL_PassZMask);
    fChain->SetBranchAddress("LL_PassZMask2", &LL_PassZMask2);
    fChain->SetBranchAddress("LL_TauCharge", &LL_TauCharge);
    fChain->SetBranchAddress("LL_TauDecayMode", &LL_TauDecayMode);
    fChain->SetBranchAddress("LL_TauEmFraction", &LL_TauEmFraction);
    fChain->SetBranchAddress("LL_TauEta", &LL_TauEta);
    fChain->SetBranchAddress("LL_TauGenMatchDaughter0Id", &LL_TauGenMatchDaughter0Id);
    fChain->SetBranchAddress("LL_TauGenMatchDaughter0Status", &LL_TauGenMatchDaughter0Status);
    fChain->SetBranchAddress("LL_TauGenMatchDaughter1Id", &LL_TauGenMatchDaughter1Id);
    fChain->SetBranchAddress("LL_TauGenMatchDaughter1Status", &LL_TauGenMatchDaughter1Status);
    fChain->SetBranchAddress("LL_TauGenMatchGrandmother00Id", &LL_TauGenMatchGrandmother00Id);
    fChain->SetBranchAddress("LL_TauGenMatchGrandmother00Status", &LL_TauGenMatchGrandmother00Status);
    fChain->SetBranchAddress("LL_TauGenMatchGrandmother01Id", &LL_TauGenMatchGrandmother01Id);
    fChain->SetBranchAddress("LL_TauGenMatchGrandmother01Status", &LL_TauGenMatchGrandmother01Status);
    fChain->SetBranchAddress("LL_TauGenMatchGrandmother10Id", &LL_TauGenMatchGrandmother10Id);
    fChain->SetBranchAddress("LL_TauGenMatchGrandmother10Status", &LL_TauGenMatchGrandmother10Status);
    fChain->SetBranchAddress("LL_TauGenMatchGrandmother11Id", &LL_TauGenMatchGrandmother11Id);
    fChain->SetBranchAddress("LL_TauGenMatchGrandmother11Status", &LL_TauGenMatchGrandmother11Status);
    fChain->SetBranchAddress("LL_TauGenMatchId", &LL_TauGenMatchId);
    fChain->SetBranchAddress("LL_TauGenMatchMother0Id", &LL_TauGenMatchMother0Id);
    fChain->SetBranchAddress("LL_TauGenMatchMother0Status", &LL_TauGenMatchMother0Status);
    fChain->SetBranchAddress("LL_TauGenMatchMother1Id", &LL_TauGenMatchMother1Id);
    fChain->SetBranchAddress("LL_TauGenMatchMother1Status", &LL_TauGenMatchMother1Status);
    fChain->SetBranchAddress("LL_TauGenMatchStatus", &LL_TauGenMatchStatus);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronDeadECAL", &LL_TauHPSagainstElectronDeadECAL);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronLoose", &LL_TauHPSagainstElectronLoose);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronLooseMVA2", &LL_TauHPSagainstElectronLooseMVA2);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronLooseMVA3", &LL_TauHPSagainstElectronLooseMVA3);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronMVA", &LL_TauHPSagainstElectronMVA);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronMVA2category", &LL_TauHPSagainstElectronMVA2category);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronMVA2raw", &LL_TauHPSagainstElectronMVA2raw);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronMVA3category", &LL_TauHPSagainstElectronMVA3category);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronMVA3raw", &LL_TauHPSagainstElectronMVA3raw);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronMedium", &LL_TauHPSagainstElectronMedium);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronMediumMVA2", &LL_TauHPSagainstElectronMediumMVA2);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronMediumMVA3", &LL_TauHPSagainstElectronMediumMVA3);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronTight", &LL_TauHPSagainstElectronTight);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronTightMVA2", &LL_TauHPSagainstElectronTightMVA2);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronTightMVA3", &LL_TauHPSagainstElectronTightMVA3);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronVLooseMVA2", &LL_TauHPSagainstElectronVLooseMVA2);
    fChain->SetBranchAddress("LL_TauHPSagainstElectronVTightMVA3", &LL_TauHPSagainstElectronVTightMVA3);
    fChain->SetBranchAddress("LL_TauHPSagainstMuonLoose", &LL_TauHPSagainstMuonLoose);
    fChain->SetBranchAddress("LL_TauHPSagainstMuonLoose2", &LL_TauHPSagainstMuonLoose2);
    fChain->SetBranchAddress("LL_TauHPSagainstMuonMedium", &LL_TauHPSagainstMuonMedium);
    fChain->SetBranchAddress("LL_TauHPSagainstMuonMedium2", &LL_TauHPSagainstMuonMedium2);
    fChain->SetBranchAddress("LL_TauHPSagainstMuonTight", &LL_TauHPSagainstMuonTight);
    fChain->SetBranchAddress("LL_TauHPSagainstMuonTight2", &LL_TauHPSagainstMuonTight2);
    fChain->SetBranchAddress("LL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw", &LL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw);
    fChain->SetBranchAddress("LL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw3Hits", &LL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw3Hits);
    fChain->SetBranchAddress("LL_TauHPSbyIsolationMVA2raw", &LL_TauHPSbyIsolationMVA2raw);
    fChain->SetBranchAddress("LL_TauHPSbyIsolationMVAraw", &LL_TauHPSbyIsolationMVAraw);
    fChain->SetBranchAddress("LL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr", &LL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr);
    fChain->SetBranchAddress("LL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr3Hits", &LL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr3Hits);
    fChain->SetBranchAddress("LL_TauHPSbyLooseIsolationMVA", &LL_TauHPSbyLooseIsolationMVA);
    fChain->SetBranchAddress("LL_TauHPSbyLooseIsolationMVA2", &LL_TauHPSbyLooseIsolationMVA2);
    fChain->SetBranchAddress("LL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr", &LL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr);
    fChain->SetBranchAddress("LL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr3Hits", &LL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr3Hits);
    fChain->SetBranchAddress("LL_TauHPSbyMediumIsolationMVA", &LL_TauHPSbyMediumIsolationMVA);
    fChain->SetBranchAddress("LL_TauHPSbyMediumIsolationMVA2", &LL_TauHPSbyMediumIsolationMVA2);
    fChain->SetBranchAddress("LL_TauHPSbyTightCombinedIsolationDeltaBetaCorr", &LL_TauHPSbyTightCombinedIsolationDeltaBetaCorr);
    fChain->SetBranchAddress("LL_TauHPSbyTightCombinedIsolationDeltaBetaCorr3Hits", &LL_TauHPSbyTightCombinedIsolationDeltaBetaCorr3Hits);
    fChain->SetBranchAddress("LL_TauHPSbyTightIsolationMVA", &LL_TauHPSbyTightIsolationMVA);
    fChain->SetBranchAddress("LL_TauHPSbyTightIsolationMVA2", &LL_TauHPSbyTightIsolationMVA2);
    fChain->SetBranchAddress("LL_TauHPSbyVLooseCombinedIsolationDeltaBetaCorr", &LL_TauHPSbyVLooseCombinedIsolationDeltaBetaCorr);
    fChain->SetBranchAddress("LL_TauHPSdecayModeFinding", &LL_TauHPSdecayModeFinding);
    fChain->SetBranchAddress("LL_TauIsInTheCracks", &LL_TauIsInTheCracks);
    fChain->SetBranchAddress("LL_TauLTIpVtdxy", &LL_TauLTIpVtdxy);
    fChain->SetBranchAddress("LL_TauLTIpVtdxyError", &LL_TauLTIpVtdxyError);
    fChain->SetBranchAddress("LL_TauLTIpVtdz", &LL_TauLTIpVtdz);
    fChain->SetBranchAddress("LL_TauLTIpVtdzError", &LL_TauLTIpVtdzError);
    fChain->SetBranchAddress("LL_TauLTNormChiSqrd", &LL_TauLTNormChiSqrd);
    fChain->SetBranchAddress("LL_TauLTPt", &LL_TauLTPt);
    fChain->SetBranchAddress("LL_TauLTValidHits", &LL_TauLTValidHits);
    fChain->SetBranchAddress("LL_TauLTvalid", &LL_TauLTvalid);
    fChain->SetBranchAddress("LL_TauLTvx", &LL_TauLTvx);
    fChain->SetBranchAddress("LL_TauLTvy", &LL_TauLTvy);
    fChain->SetBranchAddress("LL_TauLTvz", &LL_TauLTvz);
    fChain->SetBranchAddress("LL_TauLepton1CosDeltaPhi", &LL_TauLepton1CosDeltaPhi);
    fChain->SetBranchAddress("LL_TauLepton1DeltaR", &LL_TauLepton1DeltaR);
    fChain->SetBranchAddress("LL_TauLepton1METMass", &LL_TauLepton1METMass);
    fChain->SetBranchAddress("LL_TauLepton1VisibleMass", &LL_TauLepton1VisibleMass);
    fChain->SetBranchAddress("LL_TauLepton2CosDeltaPhi", &LL_TauLepton2CosDeltaPhi);
    fChain->SetBranchAddress("LL_TauLepton2DeltaR", &LL_TauLepton2DeltaR);
    fChain->SetBranchAddress("LL_TauLepton2METMass", &LL_TauLepton2METMass);
    fChain->SetBranchAddress("LL_TauLepton2VisibleMass", &LL_TauLepton2VisibleMass);
    fChain->SetBranchAddress("LL_TauMomentumRank", &LL_TauMomentumRank);
    fChain->SetBranchAddress("LL_TauNProngs", &LL_TauNProngs);
    fChain->SetBranchAddress("LL_TauNSignalGammas", &LL_TauNSignalGammas);
    fChain->SetBranchAddress("LL_TauNSignalNeutrals", &LL_TauNSignalNeutrals);
    fChain->SetBranchAddress("LL_TauP", &LL_TauP);
    fChain->SetBranchAddress("LL_TauPhi", &LL_TauPhi);
    fChain->SetBranchAddress("LL_TauPt", &LL_TauPt);
    fChain->SetBranchAddress("LL_TriggerEventWeight", &LL_TriggerEventWeight);
    // >>> End address <<<
}
