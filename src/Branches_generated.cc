// vim: ts=4:sw=4:et:sta
#include "../interface/Branches.h"

using namespace roast;

void
Branches::Clear()
{
    // >>> Begin clear <<<
    E_ElectronGenMatchDaughter0Id->clear();
    E_ElectronGenMatchDaughter0Status->clear();
    E_ElectronGenMatchDaughter1Id->clear();
    E_ElectronGenMatchDaughter1Status->clear();
    E_ElectronGenMatchGrandmother00Id->clear();
    E_ElectronGenMatchGrandmother00Status->clear();
    E_ElectronGenMatchGrandmother01Id->clear();
    E_ElectronGenMatchGrandmother01Status->clear();
    E_ElectronGenMatchGrandmother10Id->clear();
    E_ElectronGenMatchGrandmother10Status->clear();
    E_ElectronGenMatchGrandmother11Id->clear();
    E_ElectronGenMatchGrandmother11Status->clear();
    E_ElectronGenMatchId->clear();
    E_ElectronGenMatchMother0Id->clear();
    E_ElectronGenMatchMother0Status->clear();
    E_ElectronGenMatchMother1Id->clear();
    E_ElectronGenMatchMother1Status->clear();
    E_ElectronGenMatchStatus->clear();
    E_Eta->clear();
    E_IsLooseElectron->clear();
    E_IsTightElectron->clear();
    E_MomentumRank->clear();
    E_NumElectrons = 0;
    E_Phi->clear();
    E_Pt->clear();
    Ev_IsTauEvent = 0;
    Ev_MET = 0;
    Ev_METphi = 0;
    Ev_bQuarkCount = 0;
    Ev_cQuarkCount = 0;
    Ev_eventNumber = 0;
    Ev_higgsDecayMode = 0;
    Ev_lumiBlock = 0;
    Ev_puWeight = 0;
    Ev_puWeightDown = 0;
    Ev_puWeightUp = 0;
    Ev_q2WeightDown = 0;
    Ev_q2WeightUp = 0;
    Ev_runNumber = 0;
    Ev_topPtWeight = 0;
    Ev_topPtWeightDown = 0;
    Ev_topPtWeightUp = 0;
    GJ_Eta->clear();
    GJ_IsBjet->clear();
    GJ_MomentumRank->clear();
    GJ_NumGenJets = 0;
    GJ_Phi->clear();
    GJ_Pt->clear();
    GT_Eta->clear();
    GT_MomentumRank->clear();
    GT_NumGenTaus = 0;
    GT_ParentEta->clear();
    GT_ParentId->clear();
    GT_ParentP->clear();
    GT_ParentPhi->clear();
    GT_ParentPt->clear();
    GT_Phi->clear();
    GT_Pt->clear();
    GT_ToElectron->clear();
    GT_ToHadrons->clear();
    GT_ToMuon->clear();
    GT_VisEta->clear();
    GT_VisPhi->clear();
    GT_VisPt->clear();
    J_Eta->clear();
    J_MomentumRank->clear();
    J_NumJets = 0;
    J_Phi->clear();
    J_Pt->clear();
    J_combSecVtxBTag->clear();
    J_combSecVtxLooseBTag->clear();
    J_combSecVtxMediumBTag->clear();
    J_combSecVtxTightBTag->clear();
    M_Eta->clear();
    M_IsLooseMuon->clear();
    M_IsTightMuon->clear();
    M_MomentumRank->clear();
    M_MuonGenMatchDaughter0Id->clear();
    M_MuonGenMatchDaughter0Status->clear();
    M_MuonGenMatchDaughter1Id->clear();
    M_MuonGenMatchDaughter1Status->clear();
    M_MuonGenMatchGrandmother00Id->clear();
    M_MuonGenMatchGrandmother00Status->clear();
    M_MuonGenMatchGrandmother01Id->clear();
    M_MuonGenMatchGrandmother01Status->clear();
    M_MuonGenMatchGrandmother10Id->clear();
    M_MuonGenMatchGrandmother10Status->clear();
    M_MuonGenMatchGrandmother11Id->clear();
    M_MuonGenMatchGrandmother11Status->clear();
    M_MuonGenMatchId->clear();
    M_MuonGenMatchMother0Id->clear();
    M_MuonGenMatchMother0Status->clear();
    M_MuonGenMatchMother1Id->clear();
    M_MuonGenMatchMother1Status->clear();
    M_MuonGenMatchStatus->clear();
    M_NumMuons = 0;
    M_Phi->clear();
    M_Pt->clear();
    M_RelIso->clear();
    T_Charge->clear();
    T_DecayMode->clear();
    T_EmFraction->clear();
    T_Eta->clear();
    T_HPSagainstElectronDeadECAL->clear();
    T_HPSagainstElectronLoose->clear();
    T_HPSagainstElectronLooseMVA2->clear();
    T_HPSagainstElectronLooseMVA3->clear();
    T_HPSagainstElectronMVA->clear();
    T_HPSagainstElectronMVA2category->clear();
    T_HPSagainstElectronMVA2raw->clear();
    T_HPSagainstElectronMVA3category->clear();
    T_HPSagainstElectronMVA3raw->clear();
    T_HPSagainstElectronMedium->clear();
    T_HPSagainstElectronMediumMVA2->clear();
    T_HPSagainstElectronMediumMVA3->clear();
    T_HPSagainstElectronTight->clear();
    T_HPSagainstElectronTightMVA2->clear();
    T_HPSagainstElectronTightMVA3->clear();
    T_HPSagainstElectronVLooseMVA2->clear();
    T_HPSagainstElectronVTightMVA3->clear();
    T_HPSagainstMuonLoose->clear();
    T_HPSagainstMuonLoose2->clear();
    T_HPSagainstMuonMedium->clear();
    T_HPSagainstMuonMedium2->clear();
    T_HPSagainstMuonTight->clear();
    T_HPSagainstMuonTight2->clear();
    T_HPSbyCombinedIsolationDeltaBetaCorrRaw->clear();
    T_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits->clear();
    T_HPSbyIsolationMVA2raw->clear();
    T_HPSbyIsolationMVAraw->clear();
    T_HPSbyLooseCombinedIsolationDeltaBetaCorr->clear();
    T_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits->clear();
    T_HPSbyLooseIsolationMVA->clear();
    T_HPSbyLooseIsolationMVA2->clear();
    T_HPSbyMediumCombinedIsolationDeltaBetaCorr->clear();
    T_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits->clear();
    T_HPSbyMediumIsolationMVA->clear();
    T_HPSbyMediumIsolationMVA2->clear();
    T_HPSbyTightCombinedIsolationDeltaBetaCorr->clear();
    T_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits->clear();
    T_HPSbyTightIsolationMVA->clear();
    T_HPSbyTightIsolationMVA2->clear();
    T_HPSbyVLooseCombinedIsolationDeltaBetaCorr->clear();
    T_HPSdecayModeFinding->clear();
    T_IsInTheCracks->clear();
    T_LTIpVtdxy->clear();
    T_LTIpVtdxyError->clear();
    T_LTIpVtdz->clear();
    T_LTIpVtdzError->clear();
    T_LTNormChiSqrd->clear();
    T_LTPt->clear();
    T_LTValidHits->clear();
    T_LTvalid->clear();
    T_LTvx->clear();
    T_LTvy->clear();
    T_LTvz->clear();
    T_MomentumRank->clear();
    T_NProngs->clear();
    T_NSignalGammas->clear();
    T_NSignalNeutrals->clear();
    T_NumTaus = 0;
    T_Phi->clear();
    T_Pt->clear();
    T_TauGenMatchDaughter0Id->clear();
    T_TauGenMatchDaughter0Status->clear();
    T_TauGenMatchDaughter1Id->clear();
    T_TauGenMatchDaughter1Status->clear();
    T_TauGenMatchGrandmother00Id->clear();
    T_TauGenMatchGrandmother00Status->clear();
    T_TauGenMatchGrandmother01Id->clear();
    T_TauGenMatchGrandmother01Status->clear();
    T_TauGenMatchGrandmother10Id->clear();
    T_TauGenMatchGrandmother10Status->clear();
    T_TauGenMatchGrandmother11Id->clear();
    T_TauGenMatchGrandmother11Status->clear();
    T_TauGenMatchId->clear();
    T_TauGenMatchMother0Id->clear();
    T_TauGenMatchMother0Status->clear();
    T_TauGenMatchMother1Id->clear();
    T_TauGenMatchMother1Status->clear();
    T_TauGenMatchStatus->clear();
    V_NormChiSquared->clear();
    V_NumDOF->clear();
    V_NumInteractionsBX0 = 0;
    V_NumInteractionsBXm1 = 0;
    V_NumInteractionsBXp1 = 0;
    V_NumVertices = 0;
    V_Rho->clear();
    V_Xcoord->clear();
    V_XcoordErr->clear();
    V_Ycoord->clear();
    V_YcoordErr->clear();
    V_Zcoord->clear();
    V_ZcoordErr->clear();
    // >>> End clear <<<
}

void
Branches::Delete()
{
    // >>> Begin delete <<<
    delete E_ElectronGenMatchDaughter0Id;
    delete E_ElectronGenMatchDaughter0Status;
    delete E_ElectronGenMatchDaughter1Id;
    delete E_ElectronGenMatchDaughter1Status;
    delete E_ElectronGenMatchGrandmother00Id;
    delete E_ElectronGenMatchGrandmother00Status;
    delete E_ElectronGenMatchGrandmother01Id;
    delete E_ElectronGenMatchGrandmother01Status;
    delete E_ElectronGenMatchGrandmother10Id;
    delete E_ElectronGenMatchGrandmother10Status;
    delete E_ElectronGenMatchGrandmother11Id;
    delete E_ElectronGenMatchGrandmother11Status;
    delete E_ElectronGenMatchId;
    delete E_ElectronGenMatchMother0Id;
    delete E_ElectronGenMatchMother0Status;
    delete E_ElectronGenMatchMother1Id;
    delete E_ElectronGenMatchMother1Status;
    delete E_ElectronGenMatchStatus;
    delete E_Eta;
    delete E_IsLooseElectron;
    delete E_IsTightElectron;
    delete E_MomentumRank;
    delete E_Phi;
    delete E_Pt;
    delete GJ_Eta;
    delete GJ_IsBjet;
    delete GJ_MomentumRank;
    delete GJ_Phi;
    delete GJ_Pt;
    delete GT_Eta;
    delete GT_MomentumRank;
    delete GT_ParentEta;
    delete GT_ParentId;
    delete GT_ParentP;
    delete GT_ParentPhi;
    delete GT_ParentPt;
    delete GT_Phi;
    delete GT_Pt;
    delete GT_ToElectron;
    delete GT_ToHadrons;
    delete GT_ToMuon;
    delete GT_VisEta;
    delete GT_VisPhi;
    delete GT_VisPt;
    delete J_Eta;
    delete J_MomentumRank;
    delete J_Phi;
    delete J_Pt;
    delete J_combSecVtxBTag;
    delete J_combSecVtxLooseBTag;
    delete J_combSecVtxMediumBTag;
    delete J_combSecVtxTightBTag;
    delete M_Eta;
    delete M_IsLooseMuon;
    delete M_IsTightMuon;
    delete M_MomentumRank;
    delete M_MuonGenMatchDaughter0Id;
    delete M_MuonGenMatchDaughter0Status;
    delete M_MuonGenMatchDaughter1Id;
    delete M_MuonGenMatchDaughter1Status;
    delete M_MuonGenMatchGrandmother00Id;
    delete M_MuonGenMatchGrandmother00Status;
    delete M_MuonGenMatchGrandmother01Id;
    delete M_MuonGenMatchGrandmother01Status;
    delete M_MuonGenMatchGrandmother10Id;
    delete M_MuonGenMatchGrandmother10Status;
    delete M_MuonGenMatchGrandmother11Id;
    delete M_MuonGenMatchGrandmother11Status;
    delete M_MuonGenMatchId;
    delete M_MuonGenMatchMother0Id;
    delete M_MuonGenMatchMother0Status;
    delete M_MuonGenMatchMother1Id;
    delete M_MuonGenMatchMother1Status;
    delete M_MuonGenMatchStatus;
    delete M_Phi;
    delete M_Pt;
    delete M_RelIso;
    delete T_Charge;
    delete T_DecayMode;
    delete T_EmFraction;
    delete T_Eta;
    delete T_HPSagainstElectronDeadECAL;
    delete T_HPSagainstElectronLoose;
    delete T_HPSagainstElectronLooseMVA2;
    delete T_HPSagainstElectronLooseMVA3;
    delete T_HPSagainstElectronMVA;
    delete T_HPSagainstElectronMVA2category;
    delete T_HPSagainstElectronMVA2raw;
    delete T_HPSagainstElectronMVA3category;
    delete T_HPSagainstElectronMVA3raw;
    delete T_HPSagainstElectronMedium;
    delete T_HPSagainstElectronMediumMVA2;
    delete T_HPSagainstElectronMediumMVA3;
    delete T_HPSagainstElectronTight;
    delete T_HPSagainstElectronTightMVA2;
    delete T_HPSagainstElectronTightMVA3;
    delete T_HPSagainstElectronVLooseMVA2;
    delete T_HPSagainstElectronVTightMVA3;
    delete T_HPSagainstMuonLoose;
    delete T_HPSagainstMuonLoose2;
    delete T_HPSagainstMuonMedium;
    delete T_HPSagainstMuonMedium2;
    delete T_HPSagainstMuonTight;
    delete T_HPSagainstMuonTight2;
    delete T_HPSbyCombinedIsolationDeltaBetaCorrRaw;
    delete T_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits;
    delete T_HPSbyIsolationMVA2raw;
    delete T_HPSbyIsolationMVAraw;
    delete T_HPSbyLooseCombinedIsolationDeltaBetaCorr;
    delete T_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits;
    delete T_HPSbyLooseIsolationMVA;
    delete T_HPSbyLooseIsolationMVA2;
    delete T_HPSbyMediumCombinedIsolationDeltaBetaCorr;
    delete T_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits;
    delete T_HPSbyMediumIsolationMVA;
    delete T_HPSbyMediumIsolationMVA2;
    delete T_HPSbyTightCombinedIsolationDeltaBetaCorr;
    delete T_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits;
    delete T_HPSbyTightIsolationMVA;
    delete T_HPSbyTightIsolationMVA2;
    delete T_HPSbyVLooseCombinedIsolationDeltaBetaCorr;
    delete T_HPSdecayModeFinding;
    delete T_IsInTheCracks;
    delete T_LTIpVtdxy;
    delete T_LTIpVtdxyError;
    delete T_LTIpVtdz;
    delete T_LTIpVtdzError;
    delete T_LTNormChiSqrd;
    delete T_LTPt;
    delete T_LTValidHits;
    delete T_LTvalid;
    delete T_LTvx;
    delete T_LTvy;
    delete T_LTvz;
    delete T_MomentumRank;
    delete T_NProngs;
    delete T_NSignalGammas;
    delete T_NSignalNeutrals;
    delete T_Phi;
    delete T_Pt;
    delete T_TauGenMatchDaughter0Id;
    delete T_TauGenMatchDaughter0Status;
    delete T_TauGenMatchDaughter1Id;
    delete T_TauGenMatchDaughter1Status;
    delete T_TauGenMatchGrandmother00Id;
    delete T_TauGenMatchGrandmother00Status;
    delete T_TauGenMatchGrandmother01Id;
    delete T_TauGenMatchGrandmother01Status;
    delete T_TauGenMatchGrandmother10Id;
    delete T_TauGenMatchGrandmother10Status;
    delete T_TauGenMatchGrandmother11Id;
    delete T_TauGenMatchGrandmother11Status;
    delete T_TauGenMatchId;
    delete T_TauGenMatchMother0Id;
    delete T_TauGenMatchMother0Status;
    delete T_TauGenMatchMother1Id;
    delete T_TauGenMatchMother1Status;
    delete T_TauGenMatchStatus;
    delete V_NormChiSquared;
    delete V_NumDOF;
    delete V_Rho;
    delete V_Xcoord;
    delete V_XcoordErr;
    delete V_Ycoord;
    delete V_YcoordErr;
    delete V_Zcoord;
    delete V_ZcoordErr;
    // >>> End delete <<<
}

void
Branches::Null()
{
    // >>> Begin null <<<
    E_ElectronGenMatchDaughter0Id = 0;
    E_ElectronGenMatchDaughter0Status = 0;
    E_ElectronGenMatchDaughter1Id = 0;
    E_ElectronGenMatchDaughter1Status = 0;
    E_ElectronGenMatchGrandmother00Id = 0;
    E_ElectronGenMatchGrandmother00Status = 0;
    E_ElectronGenMatchGrandmother01Id = 0;
    E_ElectronGenMatchGrandmother01Status = 0;
    E_ElectronGenMatchGrandmother10Id = 0;
    E_ElectronGenMatchGrandmother10Status = 0;
    E_ElectronGenMatchGrandmother11Id = 0;
    E_ElectronGenMatchGrandmother11Status = 0;
    E_ElectronGenMatchId = 0;
    E_ElectronGenMatchMother0Id = 0;
    E_ElectronGenMatchMother0Status = 0;
    E_ElectronGenMatchMother1Id = 0;
    E_ElectronGenMatchMother1Status = 0;
    E_ElectronGenMatchStatus = 0;
    E_Eta = 0;
    E_IsLooseElectron = 0;
    E_IsTightElectron = 0;
    E_MomentumRank = 0;
    E_NumElectrons = 0;
    E_Phi = 0;
    E_Pt = 0;
    Ev_IsTauEvent = 0;
    Ev_MET = 0;
    Ev_METphi = 0;
    Ev_bQuarkCount = 0;
    Ev_cQuarkCount = 0;
    Ev_eventNumber = 0;
    Ev_higgsDecayMode = 0;
    Ev_lumiBlock = 0;
    Ev_puWeight = 0;
    Ev_puWeightDown = 0;
    Ev_puWeightUp = 0;
    Ev_q2WeightDown = 0;
    Ev_q2WeightUp = 0;
    Ev_runNumber = 0;
    Ev_topPtWeight = 0;
    Ev_topPtWeightDown = 0;
    Ev_topPtWeightUp = 0;
    GJ_Eta = 0;
    GJ_IsBjet = 0;
    GJ_MomentumRank = 0;
    GJ_NumGenJets = 0;
    GJ_Phi = 0;
    GJ_Pt = 0;
    GT_Eta = 0;
    GT_MomentumRank = 0;
    GT_NumGenTaus = 0;
    GT_ParentEta = 0;
    GT_ParentId = 0;
    GT_ParentP = 0;
    GT_ParentPhi = 0;
    GT_ParentPt = 0;
    GT_Phi = 0;
    GT_Pt = 0;
    GT_ToElectron = 0;
    GT_ToHadrons = 0;
    GT_ToMuon = 0;
    GT_VisEta = 0;
    GT_VisPhi = 0;
    GT_VisPt = 0;
    J_Eta = 0;
    J_MomentumRank = 0;
    J_NumJets = 0;
    J_Phi = 0;
    J_Pt = 0;
    J_combSecVtxBTag = 0;
    J_combSecVtxLooseBTag = 0;
    J_combSecVtxMediumBTag = 0;
    J_combSecVtxTightBTag = 0;
    M_Eta = 0;
    M_IsLooseMuon = 0;
    M_IsTightMuon = 0;
    M_MomentumRank = 0;
    M_MuonGenMatchDaughter0Id = 0;
    M_MuonGenMatchDaughter0Status = 0;
    M_MuonGenMatchDaughter1Id = 0;
    M_MuonGenMatchDaughter1Status = 0;
    M_MuonGenMatchGrandmother00Id = 0;
    M_MuonGenMatchGrandmother00Status = 0;
    M_MuonGenMatchGrandmother01Id = 0;
    M_MuonGenMatchGrandmother01Status = 0;
    M_MuonGenMatchGrandmother10Id = 0;
    M_MuonGenMatchGrandmother10Status = 0;
    M_MuonGenMatchGrandmother11Id = 0;
    M_MuonGenMatchGrandmother11Status = 0;
    M_MuonGenMatchId = 0;
    M_MuonGenMatchMother0Id = 0;
    M_MuonGenMatchMother0Status = 0;
    M_MuonGenMatchMother1Id = 0;
    M_MuonGenMatchMother1Status = 0;
    M_MuonGenMatchStatus = 0;
    M_NumMuons = 0;
    M_Phi = 0;
    M_Pt = 0;
    M_RelIso = 0;
    T_Charge = 0;
    T_DecayMode = 0;
    T_EmFraction = 0;
    T_Eta = 0;
    T_HPSagainstElectronDeadECAL = 0;
    T_HPSagainstElectronLoose = 0;
    T_HPSagainstElectronLooseMVA2 = 0;
    T_HPSagainstElectronLooseMVA3 = 0;
    T_HPSagainstElectronMVA = 0;
    T_HPSagainstElectronMVA2category = 0;
    T_HPSagainstElectronMVA2raw = 0;
    T_HPSagainstElectronMVA3category = 0;
    T_HPSagainstElectronMVA3raw = 0;
    T_HPSagainstElectronMedium = 0;
    T_HPSagainstElectronMediumMVA2 = 0;
    T_HPSagainstElectronMediumMVA3 = 0;
    T_HPSagainstElectronTight = 0;
    T_HPSagainstElectronTightMVA2 = 0;
    T_HPSagainstElectronTightMVA3 = 0;
    T_HPSagainstElectronVLooseMVA2 = 0;
    T_HPSagainstElectronVTightMVA3 = 0;
    T_HPSagainstMuonLoose = 0;
    T_HPSagainstMuonLoose2 = 0;
    T_HPSagainstMuonMedium = 0;
    T_HPSagainstMuonMedium2 = 0;
    T_HPSagainstMuonTight = 0;
    T_HPSagainstMuonTight2 = 0;
    T_HPSbyCombinedIsolationDeltaBetaCorrRaw = 0;
    T_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits = 0;
    T_HPSbyIsolationMVA2raw = 0;
    T_HPSbyIsolationMVAraw = 0;
    T_HPSbyLooseCombinedIsolationDeltaBetaCorr = 0;
    T_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits = 0;
    T_HPSbyLooseIsolationMVA = 0;
    T_HPSbyLooseIsolationMVA2 = 0;
    T_HPSbyMediumCombinedIsolationDeltaBetaCorr = 0;
    T_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits = 0;
    T_HPSbyMediumIsolationMVA = 0;
    T_HPSbyMediumIsolationMVA2 = 0;
    T_HPSbyTightCombinedIsolationDeltaBetaCorr = 0;
    T_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits = 0;
    T_HPSbyTightIsolationMVA = 0;
    T_HPSbyTightIsolationMVA2 = 0;
    T_HPSbyVLooseCombinedIsolationDeltaBetaCorr = 0;
    T_HPSdecayModeFinding = 0;
    T_IsInTheCracks = 0;
    T_LTIpVtdxy = 0;
    T_LTIpVtdxyError = 0;
    T_LTIpVtdz = 0;
    T_LTIpVtdzError = 0;
    T_LTNormChiSqrd = 0;
    T_LTPt = 0;
    T_LTValidHits = 0;
    T_LTvalid = 0;
    T_LTvx = 0;
    T_LTvy = 0;
    T_LTvz = 0;
    T_MomentumRank = 0;
    T_NProngs = 0;
    T_NSignalGammas = 0;
    T_NSignalNeutrals = 0;
    T_NumTaus = 0;
    T_Phi = 0;
    T_Pt = 0;
    T_TauGenMatchDaughter0Id = 0;
    T_TauGenMatchDaughter0Status = 0;
    T_TauGenMatchDaughter1Id = 0;
    T_TauGenMatchDaughter1Status = 0;
    T_TauGenMatchGrandmother00Id = 0;
    T_TauGenMatchGrandmother00Status = 0;
    T_TauGenMatchGrandmother01Id = 0;
    T_TauGenMatchGrandmother01Status = 0;
    T_TauGenMatchGrandmother10Id = 0;
    T_TauGenMatchGrandmother10Status = 0;
    T_TauGenMatchGrandmother11Id = 0;
    T_TauGenMatchGrandmother11Status = 0;
    T_TauGenMatchId = 0;
    T_TauGenMatchMother0Id = 0;
    T_TauGenMatchMother0Status = 0;
    T_TauGenMatchMother1Id = 0;
    T_TauGenMatchMother1Status = 0;
    T_TauGenMatchStatus = 0;
    V_NormChiSquared = 0;
    V_NumDOF = 0;
    V_NumInteractionsBX0 = 0;
    V_NumInteractionsBXm1 = 0;
    V_NumInteractionsBXp1 = 0;
    V_NumVertices = 0;
    V_Rho = 0;
    V_Xcoord = 0;
    V_XcoordErr = 0;
    V_Ycoord = 0;
    V_YcoordErr = 0;
    V_Zcoord = 0;
    V_ZcoordErr = 0;
    // >>> End null <<<
}

void
Branches::SetBranchAddresses()
{
    // >>> Begin address <<<
    fChain->SetBranchAddress("E_ElectronGenMatchDaughter0Id", &E_ElectronGenMatchDaughter0Id);
    fChain->SetBranchAddress("E_ElectronGenMatchDaughter0Status", &E_ElectronGenMatchDaughter0Status);
    fChain->SetBranchAddress("E_ElectronGenMatchDaughter1Id", &E_ElectronGenMatchDaughter1Id);
    fChain->SetBranchAddress("E_ElectronGenMatchDaughter1Status", &E_ElectronGenMatchDaughter1Status);
    fChain->SetBranchAddress("E_ElectronGenMatchGrandmother00Id", &E_ElectronGenMatchGrandmother00Id);
    fChain->SetBranchAddress("E_ElectronGenMatchGrandmother00Status", &E_ElectronGenMatchGrandmother00Status);
    fChain->SetBranchAddress("E_ElectronGenMatchGrandmother01Id", &E_ElectronGenMatchGrandmother01Id);
    fChain->SetBranchAddress("E_ElectronGenMatchGrandmother01Status", &E_ElectronGenMatchGrandmother01Status);
    fChain->SetBranchAddress("E_ElectronGenMatchGrandmother10Id", &E_ElectronGenMatchGrandmother10Id);
    fChain->SetBranchAddress("E_ElectronGenMatchGrandmother10Status", &E_ElectronGenMatchGrandmother10Status);
    fChain->SetBranchAddress("E_ElectronGenMatchGrandmother11Id", &E_ElectronGenMatchGrandmother11Id);
    fChain->SetBranchAddress("E_ElectronGenMatchGrandmother11Status", &E_ElectronGenMatchGrandmother11Status);
    fChain->SetBranchAddress("E_ElectronGenMatchId", &E_ElectronGenMatchId);
    fChain->SetBranchAddress("E_ElectronGenMatchMother0Id", &E_ElectronGenMatchMother0Id);
    fChain->SetBranchAddress("E_ElectronGenMatchMother0Status", &E_ElectronGenMatchMother0Status);
    fChain->SetBranchAddress("E_ElectronGenMatchMother1Id", &E_ElectronGenMatchMother1Id);
    fChain->SetBranchAddress("E_ElectronGenMatchMother1Status", &E_ElectronGenMatchMother1Status);
    fChain->SetBranchAddress("E_ElectronGenMatchStatus", &E_ElectronGenMatchStatus);
    fChain->SetBranchAddress("E_Eta", &E_Eta);
    fChain->SetBranchAddress("E_IsLooseElectron", &E_IsLooseElectron);
    fChain->SetBranchAddress("E_IsTightElectron", &E_IsTightElectron);
    fChain->SetBranchAddress("E_MomentumRank", &E_MomentumRank);
    fChain->SetBranchAddress("E_NumElectrons", &E_NumElectrons);
    fChain->SetBranchAddress("E_Phi", &E_Phi);
    fChain->SetBranchAddress("E_Pt", &E_Pt);
    fChain->SetBranchAddress("Ev_IsTauEvent", &Ev_IsTauEvent);
    fChain->SetBranchAddress("Ev_MET", &Ev_MET);
    fChain->SetBranchAddress("Ev_METphi", &Ev_METphi);
    fChain->SetBranchAddress("Ev_bQuarkCount", &Ev_bQuarkCount);
    fChain->SetBranchAddress("Ev_cQuarkCount", &Ev_cQuarkCount);
    fChain->SetBranchAddress("Ev_eventNumber", &Ev_eventNumber);
    fChain->SetBranchAddress("Ev_higgsDecayMode", &Ev_higgsDecayMode);
    fChain->SetBranchAddress("Ev_lumiBlock", &Ev_lumiBlock);
    fChain->SetBranchAddress("Ev_puWeight", &Ev_puWeight);
    fChain->SetBranchAddress("Ev_puWeightDown", &Ev_puWeightDown);
    fChain->SetBranchAddress("Ev_puWeightUp", &Ev_puWeightUp);
    fChain->SetBranchAddress("Ev_q2WeightDown", &Ev_q2WeightDown);
    fChain->SetBranchAddress("Ev_q2WeightUp", &Ev_q2WeightUp);
    fChain->SetBranchAddress("Ev_runNumber", &Ev_runNumber);
    fChain->SetBranchAddress("Ev_topPtWeight", &Ev_topPtWeight);
    fChain->SetBranchAddress("Ev_topPtWeightDown", &Ev_topPtWeightDown);
    fChain->SetBranchAddress("Ev_topPtWeightUp", &Ev_topPtWeightUp);
    fChain->SetBranchAddress("GJ_Eta", &GJ_Eta);
    fChain->SetBranchAddress("GJ_IsBjet", &GJ_IsBjet);
    fChain->SetBranchAddress("GJ_MomentumRank", &GJ_MomentumRank);
    fChain->SetBranchAddress("GJ_NumGenJets", &GJ_NumGenJets);
    fChain->SetBranchAddress("GJ_Phi", &GJ_Phi);
    fChain->SetBranchAddress("GJ_Pt", &GJ_Pt);
    fChain->SetBranchAddress("GT_Eta", &GT_Eta);
    fChain->SetBranchAddress("GT_MomentumRank", &GT_MomentumRank);
    fChain->SetBranchAddress("GT_NumGenTaus", &GT_NumGenTaus);
    fChain->SetBranchAddress("GT_ParentEta", &GT_ParentEta);
    fChain->SetBranchAddress("GT_ParentId", &GT_ParentId);
    fChain->SetBranchAddress("GT_ParentP", &GT_ParentP);
    fChain->SetBranchAddress("GT_ParentPhi", &GT_ParentPhi);
    fChain->SetBranchAddress("GT_ParentPt", &GT_ParentPt);
    fChain->SetBranchAddress("GT_Phi", &GT_Phi);
    fChain->SetBranchAddress("GT_Pt", &GT_Pt);
    fChain->SetBranchAddress("GT_ToElectron", &GT_ToElectron);
    fChain->SetBranchAddress("GT_ToHadrons", &GT_ToHadrons);
    fChain->SetBranchAddress("GT_ToMuon", &GT_ToMuon);
    fChain->SetBranchAddress("GT_VisEta", &GT_VisEta);
    fChain->SetBranchAddress("GT_VisPhi", &GT_VisPhi);
    fChain->SetBranchAddress("GT_VisPt", &GT_VisPt);
    fChain->SetBranchAddress("J_Eta", &J_Eta);
    fChain->SetBranchAddress("J_MomentumRank", &J_MomentumRank);
    fChain->SetBranchAddress("J_NumJets", &J_NumJets);
    fChain->SetBranchAddress("J_Phi", &J_Phi);
    fChain->SetBranchAddress("J_Pt", &J_Pt);
    fChain->SetBranchAddress("J_combSecVtxBTag", &J_combSecVtxBTag);
    fChain->SetBranchAddress("J_combSecVtxLooseBTag", &J_combSecVtxLooseBTag);
    fChain->SetBranchAddress("J_combSecVtxMediumBTag", &J_combSecVtxMediumBTag);
    fChain->SetBranchAddress("J_combSecVtxTightBTag", &J_combSecVtxTightBTag);
    fChain->SetBranchAddress("M_Eta", &M_Eta);
    fChain->SetBranchAddress("M_IsLooseMuon", &M_IsLooseMuon);
    fChain->SetBranchAddress("M_IsTightMuon", &M_IsTightMuon);
    fChain->SetBranchAddress("M_MomentumRank", &M_MomentumRank);
    fChain->SetBranchAddress("M_MuonGenMatchDaughter0Id", &M_MuonGenMatchDaughter0Id);
    fChain->SetBranchAddress("M_MuonGenMatchDaughter0Status", &M_MuonGenMatchDaughter0Status);
    fChain->SetBranchAddress("M_MuonGenMatchDaughter1Id", &M_MuonGenMatchDaughter1Id);
    fChain->SetBranchAddress("M_MuonGenMatchDaughter1Status", &M_MuonGenMatchDaughter1Status);
    fChain->SetBranchAddress("M_MuonGenMatchGrandmother00Id", &M_MuonGenMatchGrandmother00Id);
    fChain->SetBranchAddress("M_MuonGenMatchGrandmother00Status", &M_MuonGenMatchGrandmother00Status);
    fChain->SetBranchAddress("M_MuonGenMatchGrandmother01Id", &M_MuonGenMatchGrandmother01Id);
    fChain->SetBranchAddress("M_MuonGenMatchGrandmother01Status", &M_MuonGenMatchGrandmother01Status);
    fChain->SetBranchAddress("M_MuonGenMatchGrandmother10Id", &M_MuonGenMatchGrandmother10Id);
    fChain->SetBranchAddress("M_MuonGenMatchGrandmother10Status", &M_MuonGenMatchGrandmother10Status);
    fChain->SetBranchAddress("M_MuonGenMatchGrandmother11Id", &M_MuonGenMatchGrandmother11Id);
    fChain->SetBranchAddress("M_MuonGenMatchGrandmother11Status", &M_MuonGenMatchGrandmother11Status);
    fChain->SetBranchAddress("M_MuonGenMatchId", &M_MuonGenMatchId);
    fChain->SetBranchAddress("M_MuonGenMatchMother0Id", &M_MuonGenMatchMother0Id);
    fChain->SetBranchAddress("M_MuonGenMatchMother0Status", &M_MuonGenMatchMother0Status);
    fChain->SetBranchAddress("M_MuonGenMatchMother1Id", &M_MuonGenMatchMother1Id);
    fChain->SetBranchAddress("M_MuonGenMatchMother1Status", &M_MuonGenMatchMother1Status);
    fChain->SetBranchAddress("M_MuonGenMatchStatus", &M_MuonGenMatchStatus);
    fChain->SetBranchAddress("M_NumMuons", &M_NumMuons);
    fChain->SetBranchAddress("M_Phi", &M_Phi);
    fChain->SetBranchAddress("M_Pt", &M_Pt);
    fChain->SetBranchAddress("M_RelIso", &M_RelIso);
    fChain->SetBranchAddress("T_Charge", &T_Charge);
    fChain->SetBranchAddress("T_DecayMode", &T_DecayMode);
    fChain->SetBranchAddress("T_EmFraction", &T_EmFraction);
    fChain->SetBranchAddress("T_Eta", &T_Eta);
    fChain->SetBranchAddress("T_HPSagainstElectronDeadECAL", &T_HPSagainstElectronDeadECAL);
    fChain->SetBranchAddress("T_HPSagainstElectronLoose", &T_HPSagainstElectronLoose);
    fChain->SetBranchAddress("T_HPSagainstElectronLooseMVA2", &T_HPSagainstElectronLooseMVA2);
    fChain->SetBranchAddress("T_HPSagainstElectronLooseMVA3", &T_HPSagainstElectronLooseMVA3);
    fChain->SetBranchAddress("T_HPSagainstElectronMVA", &T_HPSagainstElectronMVA);
    fChain->SetBranchAddress("T_HPSagainstElectronMVA2category", &T_HPSagainstElectronMVA2category);
    fChain->SetBranchAddress("T_HPSagainstElectronMVA2raw", &T_HPSagainstElectronMVA2raw);
    fChain->SetBranchAddress("T_HPSagainstElectronMVA3category", &T_HPSagainstElectronMVA3category);
    fChain->SetBranchAddress("T_HPSagainstElectronMVA3raw", &T_HPSagainstElectronMVA3raw);
    fChain->SetBranchAddress("T_HPSagainstElectronMedium", &T_HPSagainstElectronMedium);
    fChain->SetBranchAddress("T_HPSagainstElectronMediumMVA2", &T_HPSagainstElectronMediumMVA2);
    fChain->SetBranchAddress("T_HPSagainstElectronMediumMVA3", &T_HPSagainstElectronMediumMVA3);
    fChain->SetBranchAddress("T_HPSagainstElectronTight", &T_HPSagainstElectronTight);
    fChain->SetBranchAddress("T_HPSagainstElectronTightMVA2", &T_HPSagainstElectronTightMVA2);
    fChain->SetBranchAddress("T_HPSagainstElectronTightMVA3", &T_HPSagainstElectronTightMVA3);
    fChain->SetBranchAddress("T_HPSagainstElectronVLooseMVA2", &T_HPSagainstElectronVLooseMVA2);
    fChain->SetBranchAddress("T_HPSagainstElectronVTightMVA3", &T_HPSagainstElectronVTightMVA3);
    fChain->SetBranchAddress("T_HPSagainstMuonLoose", &T_HPSagainstMuonLoose);
    fChain->SetBranchAddress("T_HPSagainstMuonLoose2", &T_HPSagainstMuonLoose2);
    fChain->SetBranchAddress("T_HPSagainstMuonMedium", &T_HPSagainstMuonMedium);
    fChain->SetBranchAddress("T_HPSagainstMuonMedium2", &T_HPSagainstMuonMedium2);
    fChain->SetBranchAddress("T_HPSagainstMuonTight", &T_HPSagainstMuonTight);
    fChain->SetBranchAddress("T_HPSagainstMuonTight2", &T_HPSagainstMuonTight2);
    fChain->SetBranchAddress("T_HPSbyCombinedIsolationDeltaBetaCorrRaw", &T_HPSbyCombinedIsolationDeltaBetaCorrRaw);
    fChain->SetBranchAddress("T_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits", &T_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits);
    fChain->SetBranchAddress("T_HPSbyIsolationMVA2raw", &T_HPSbyIsolationMVA2raw);
    fChain->SetBranchAddress("T_HPSbyIsolationMVAraw", &T_HPSbyIsolationMVAraw);
    fChain->SetBranchAddress("T_HPSbyLooseCombinedIsolationDeltaBetaCorr", &T_HPSbyLooseCombinedIsolationDeltaBetaCorr);
    fChain->SetBranchAddress("T_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits", &T_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits);
    fChain->SetBranchAddress("T_HPSbyLooseIsolationMVA", &T_HPSbyLooseIsolationMVA);
    fChain->SetBranchAddress("T_HPSbyLooseIsolationMVA2", &T_HPSbyLooseIsolationMVA2);
    fChain->SetBranchAddress("T_HPSbyMediumCombinedIsolationDeltaBetaCorr", &T_HPSbyMediumCombinedIsolationDeltaBetaCorr);
    fChain->SetBranchAddress("T_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits", &T_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits);
    fChain->SetBranchAddress("T_HPSbyMediumIsolationMVA", &T_HPSbyMediumIsolationMVA);
    fChain->SetBranchAddress("T_HPSbyMediumIsolationMVA2", &T_HPSbyMediumIsolationMVA2);
    fChain->SetBranchAddress("T_HPSbyTightCombinedIsolationDeltaBetaCorr", &T_HPSbyTightCombinedIsolationDeltaBetaCorr);
    fChain->SetBranchAddress("T_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits", &T_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits);
    fChain->SetBranchAddress("T_HPSbyTightIsolationMVA", &T_HPSbyTightIsolationMVA);
    fChain->SetBranchAddress("T_HPSbyTightIsolationMVA2", &T_HPSbyTightIsolationMVA2);
    fChain->SetBranchAddress("T_HPSbyVLooseCombinedIsolationDeltaBetaCorr", &T_HPSbyVLooseCombinedIsolationDeltaBetaCorr);
    fChain->SetBranchAddress("T_HPSdecayModeFinding", &T_HPSdecayModeFinding);
    fChain->SetBranchAddress("T_IsInTheCracks", &T_IsInTheCracks);
    fChain->SetBranchAddress("T_LTIpVtdxy", &T_LTIpVtdxy);
    fChain->SetBranchAddress("T_LTIpVtdxyError", &T_LTIpVtdxyError);
    fChain->SetBranchAddress("T_LTIpVtdz", &T_LTIpVtdz);
    fChain->SetBranchAddress("T_LTIpVtdzError", &T_LTIpVtdzError);
    fChain->SetBranchAddress("T_LTNormChiSqrd", &T_LTNormChiSqrd);
    fChain->SetBranchAddress("T_LTPt", &T_LTPt);
    fChain->SetBranchAddress("T_LTValidHits", &T_LTValidHits);
    fChain->SetBranchAddress("T_LTvalid", &T_LTvalid);
    fChain->SetBranchAddress("T_LTvx", &T_LTvx);
    fChain->SetBranchAddress("T_LTvy", &T_LTvy);
    fChain->SetBranchAddress("T_LTvz", &T_LTvz);
    fChain->SetBranchAddress("T_MomentumRank", &T_MomentumRank);
    fChain->SetBranchAddress("T_NProngs", &T_NProngs);
    fChain->SetBranchAddress("T_NSignalGammas", &T_NSignalGammas);
    fChain->SetBranchAddress("T_NSignalNeutrals", &T_NSignalNeutrals);
    fChain->SetBranchAddress("T_NumTaus", &T_NumTaus);
    fChain->SetBranchAddress("T_Phi", &T_Phi);
    fChain->SetBranchAddress("T_Pt", &T_Pt);
    fChain->SetBranchAddress("T_TauGenMatchDaughter0Id", &T_TauGenMatchDaughter0Id);
    fChain->SetBranchAddress("T_TauGenMatchDaughter0Status", &T_TauGenMatchDaughter0Status);
    fChain->SetBranchAddress("T_TauGenMatchDaughter1Id", &T_TauGenMatchDaughter1Id);
    fChain->SetBranchAddress("T_TauGenMatchDaughter1Status", &T_TauGenMatchDaughter1Status);
    fChain->SetBranchAddress("T_TauGenMatchGrandmother00Id", &T_TauGenMatchGrandmother00Id);
    fChain->SetBranchAddress("T_TauGenMatchGrandmother00Status", &T_TauGenMatchGrandmother00Status);
    fChain->SetBranchAddress("T_TauGenMatchGrandmother01Id", &T_TauGenMatchGrandmother01Id);
    fChain->SetBranchAddress("T_TauGenMatchGrandmother01Status", &T_TauGenMatchGrandmother01Status);
    fChain->SetBranchAddress("T_TauGenMatchGrandmother10Id", &T_TauGenMatchGrandmother10Id);
    fChain->SetBranchAddress("T_TauGenMatchGrandmother10Status", &T_TauGenMatchGrandmother10Status);
    fChain->SetBranchAddress("T_TauGenMatchGrandmother11Id", &T_TauGenMatchGrandmother11Id);
    fChain->SetBranchAddress("T_TauGenMatchGrandmother11Status", &T_TauGenMatchGrandmother11Status);
    fChain->SetBranchAddress("T_TauGenMatchId", &T_TauGenMatchId);
    fChain->SetBranchAddress("T_TauGenMatchMother0Id", &T_TauGenMatchMother0Id);
    fChain->SetBranchAddress("T_TauGenMatchMother0Status", &T_TauGenMatchMother0Status);
    fChain->SetBranchAddress("T_TauGenMatchMother1Id", &T_TauGenMatchMother1Id);
    fChain->SetBranchAddress("T_TauGenMatchMother1Status", &T_TauGenMatchMother1Status);
    fChain->SetBranchAddress("T_TauGenMatchStatus", &T_TauGenMatchStatus);
    fChain->SetBranchAddress("V_NormChiSquared", &V_NormChiSquared);
    fChain->SetBranchAddress("V_NumDOF", &V_NumDOF);
    fChain->SetBranchAddress("V_NumInteractionsBX0", &V_NumInteractionsBX0);
    fChain->SetBranchAddress("V_NumInteractionsBXm1", &V_NumInteractionsBXm1);
    fChain->SetBranchAddress("V_NumInteractionsBXp1", &V_NumInteractionsBXp1);
    fChain->SetBranchAddress("V_NumVertices", &V_NumVertices);
    fChain->SetBranchAddress("V_Rho", &V_Rho);
    fChain->SetBranchAddress("V_Xcoord", &V_Xcoord);
    fChain->SetBranchAddress("V_XcoordErr", &V_XcoordErr);
    fChain->SetBranchAddress("V_Ycoord", &V_Ycoord);
    fChain->SetBranchAddress("V_YcoordErr", &V_YcoordErr);
    fChain->SetBranchAddress("V_Zcoord", &V_Zcoord);
    fChain->SetBranchAddress("V_ZcoordErr", &V_ZcoordErr);
    // >>> End address <<<
}
