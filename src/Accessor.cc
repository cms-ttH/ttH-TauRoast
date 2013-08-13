// vim: ts=4:sw=4:et:sta

#include <map>

#include "../interface/Accessor.h"
#include "../interface/Helper.h"
#include "../interface/MVABase.h"
#include "../interface/TLLBranches.h"
#include "../interface/TTLBranches.h"

namespace roast {
    std::map<std::string, GetValue_t> accessors;
    bool setup = false;

    GetValue_t
    get_accessor(const std::string& s)
    {
        if (!setup)
            setup_accessors();
        return accessors[s];
    }

    void
    setup_accessors()
    {
        accessors["T_MatchAbsId"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return abs((*e->TLL_TauGenMatchId)[idx]);
        };
        accessors["T_ParentAbsId"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return abs((*e->TLL_TauGenMatchMother0Id)[idx]);
        };
        accessors["T1_MatchAbsId"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->TTL_Tau1GenMatchId)[idx]);
        };
        accessors["T2_MatchAbsId"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->TTL_Tau2GenMatchId)[idx]);
        };
        accessors["T1_ParentAbsId"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->TTL_Tau1GenMatchMother0Id)[idx]);
        };
        accessors["T2_ParentAbsId"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->TTL_Tau2GenMatchMother0Id)[idx]);
        };
        accessors["J_NumCSVL"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCSVLbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCSVLbtagJets)[idx];
            } else {
                throw;
            }
        };
        accessors["J_NumCSVM"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCSVMbtagJets)[idx];
            } else {
                throw;
            }
        };
        accessors["J_NumCSVT"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCSVTbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCSVTbtagJets)[idx];
            } else {
                throw;
            }
        };
        accessors["J_NumNonCSVL"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumNonCSVLbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumNonCSVLbtagJets)[idx];
            } else {
                throw;
            }
        };
        accessors["J_NumNonCSVM"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumNonCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumNonCSVMbtagJets)[idx];
            } else {
                throw;
            }
        };
        accessors["J_NumNonCSVT"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumNonCSVTbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumNonCSVTbtagJets)[idx];
            } else {
                throw;
            }
        };
        accessors["J_NumInclusive"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCSVMbtagJets)[idx] + (*e->TLL_NumNonCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCSVMbtagJets)[idx] + (*e->TTL_NumNonCSVMbtagJets)[idx];
            } else {
                throw;
            }
        };
        accessors["J_NumCleanCSVL"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanCSVLbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanCSVLbtagJets)[idx];
            } else {
                throw;
            }
        };
        accessors["J_NumCleanCSVM"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanCSVMbtagJets)[idx];
            } else {
                throw;
            }
        };
        accessors["J_NumCleanCSVT"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanCSVTbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanCSVTbtagJets)[idx];
            } else {
                throw;
            }
        };
        accessors["J_NumCleanNonCSVL"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanNonCSVLbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanNonCSVLbtagJets)[idx];
            } else {
                throw;
            }
        };
        accessors["J_NumCleanNonCSVM"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanNonCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanNonCSVMbtagJets)[idx];
            } else {
                throw;
            }
        };
        accessors["J_NumCleanNonCSVT"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanNonCSVTbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanNonCSVTbtagJets)[idx];
            } else {
                throw;
            }
        };
        accessors["J_NumCleanInclusive"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanCSVMbtagJets)[idx] + (*e->TLL_NumCleanNonCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanCSVMbtagJets)[idx] + (*e->TTL_NumCleanNonCSVMbtagJets)[idx];
            } else {
                throw;
            }
        };
        accessors["NumTightLeptons"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumTightMuons)[idx] + (*e->TLL_NumTightElectrons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumTightMuons)[idx] + (*e->TTL_NumTightElectrons)[idx];
            } else {
                throw;
            }
        };
        accessors["NumTightMuons"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumTightMuons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumTightMuons)[idx];
            } else {
                throw;
            }
        };
        accessors["NumTightElectrons"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumTightElectrons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumTightElectrons)[idx];
            } else {
                throw;
            }
        };
        accessors["NumLooseLeptons"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumLooseMuons)[idx] + (*e->TLL_NumLooseElectrons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumLooseMuons)[idx] + (*e->TTL_NumLooseElectrons)[idx];
            } else {
                throw;
            }
        };
        accessors["NumLooseMuons"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumLooseMuons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumLooseMuons)[idx];
            } else {
                throw;
            }
        };
        accessors["NumLooseElectrons"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumLooseElectrons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumLooseElectrons)[idx];
            } else {
                throw;
            }
        };
        accessors["NumExLooseLeptons"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumExLooseMuons)[idx] + (*e->TLL_NumExLooseElectrons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumExLooseMuons)[idx] + (*e->TTL_NumExLooseElectrons)[idx];
            } else {
                throw;
            }
        };
        accessors["NumExLooseMuons"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumExLooseMuons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumExLooseMuons)[idx];
            } else {
                throw;
            }
        };
        accessors["NumExLooseElectrons"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumExLooseElectrons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumExLooseElectrons)[idx];
            } else {
                throw;
            }
        };
        accessors["L_pT"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonPt)[idx];
        };
        accessors["L_Eta"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonEta)[idx];
        };
        accessors["L_IsLoose"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonIsLoose)[idx];
        };
        accessors["L_IsTight"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonIsTight)[idx];
        };
        accessors["L_RelIso"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonRelIso)[idx];
        };
        accessors["L1_pT"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1Pt)[idx];
        };
        accessors["L1_Eta"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1Eta)[idx];
        };
        accessors["L1_IsLoose"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1IsLoose)[idx];
        };
        accessors["L1_IsTight"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1IsTight)[idx];
        };
        accessors["L1_RelIso"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1RelIso)[idx];
        };

        accessors["L2_pT"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2Pt)[idx];
        };
        accessors["L2_Eta"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2Eta)[idx];
        };
        accessors["L2_IsLoose"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2IsLoose)[idx];
        };
        accessors["L2_IsTight"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2IsTight)[idx];
        };
        accessors["L2_RelIso"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2RelIso)[idx];
        };

        accessors["T_pT"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauPt)[idx];
        };
        accessors["T_Eta"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauEta)[idx];
        };
        accessors["T_InCracks"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauIsInTheCracks)[idx];
        };
        accessors["LL_DeltaR"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return DeltaR((*e->TLL_Lepton1Eta)[idx], (*e->TLL_Lepton1Phi)[idx],
                    (*e->TLL_Lepton2Eta)[idx], (*e->TLL_Lepton2Phi)[idx]);
        };
        accessors["TL1_DeltaR"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return DeltaR((*e->TLL_TauEta)[idx], (*e->TLL_TauPhi)[idx],
                    (*e->TLL_Lepton1Eta)[idx], (*e->TLL_Lepton1Phi)[idx]);
        };
        accessors["TL2_DeltaR"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return DeltaR((*e->TLL_TauEta)[idx], (*e->TLL_TauPhi)[idx],
                    (*e->TLL_Lepton2Eta)[idx], (*e->TLL_Lepton2Phi)[idx]);
        };
        accessors["T_DecayModeFinding"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSdecayModeFinding)[idx];
        };
        accessors["T_LTvalid"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLTvalid)[idx];
        };
        accessors["T_LTpT"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLTPt)[idx];
        };
        accessors["T_NumProngs"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauNProngs)[idx];
        };

        accessors["T_AntiElectronMVA"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronMVA)[idx];
        };
        accessors["T_AntiElectronIndex"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauAntiElectronIndex(idx);
        };
        accessors["T_AntiMuonIndex"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauAntiMuonIndex(idx);
        };

        accessors["T_IsolationIndex"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauIsolationIndex(idx);
        };
        accessors["T_IsolationIndex3Hits"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauIsolationIndex3Hits(idx);
        };
        accessors["T_IsolationIndexMVA"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauIsolationIndexMVA(idx);
        };
        accessors["T_IsolationIndexMVA2"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauIsolationIndexMVA2(idx);
        };
        accessors["T_IsolationMVA2Raw"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyIsolationMVA2raw)[idx];
        };
        accessors["T1_pT"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1Pt)[idx];
        };
        accessors["T2_pT"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2Pt)[idx];
        };
        accessors["T1_Eta"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1Eta)[idx];
        };
        accessors["T2_Eta"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2Eta)[idx];
        };
        accessors["T1_InCracks"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1IsInTheCracks)[idx];
        };
        accessors["T2_InCracks"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2IsInTheCracks)[idx];
        };

        accessors["TT_DeltaR"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_DitauDeltaR)[idx];
        };
        accessors["T1L_DeltaR"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return DeltaR((*e->TTL_Tau1Eta)[idx], (*e->TTL_Tau1Phi)[idx],
                    (*e->TTL_LeptonEta)[idx], (*e->TTL_LeptonPhi)[idx]);
        };
        accessors["T2L_DeltaR"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return DeltaR((*e->TTL_Tau2Eta)[idx], (*e->TTL_Tau2Phi)[idx],
                    (*e->TTL_LeptonEta)[idx], (*e->TTL_LeptonPhi)[idx]);
        };

        accessors["T1_DecayModeFinding"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSdecayModeFinding)[idx];
        };
        accessors["T2_DecayModeFinding"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSdecayModeFinding)[idx];
        };
        accessors["T1_LTvalid"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LTvalid)[idx];
        };
        accessors["T2_LTvalid"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LTvalid)[idx];
        };
        accessors["T1_LTpT"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LTPt)[idx];
        };
        accessors["T2_LTpT"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LTPt)[idx];
        };
        accessors["T1_NumProngs"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1NProngs)[idx];
        };
        accessors["T2_NumProngs"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2NProngs)[idx];
        };

        accessors["T1_AntiElectronMVA"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronMVA)[idx];
        };
        accessors["T2_AntiElectronMVA"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronMVA)[idx];
        };
        accessors["T1_AntiElectronIndex"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1AntiElectronIndex(idx);
        };
        accessors["T2_AntiElectronIndex"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2AntiElectronIndex(idx);
        };
        accessors["T1_AntiMuonIndex"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1AntiMuonIndex(idx);
        };
        accessors["T2_AntiMuonIndex"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2AntiMuonIndex(idx);
        };

        accessors["T1_IsolationIndex"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1IsolationIndex(idx);
        };
        accessors["T2_IsolationIndex"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2IsolationIndex(idx);
        };
        accessors["T1_IsolationIndex3Hits"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1IsolationIndex3Hits(idx);
        };
        accessors["T2_IsolationIndex3Hits"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2IsolationIndex3Hits(idx);
        };
        accessors["T1_IsolationIndexMVA"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1IsolationIndexMVA(idx);
        };
        accessors["T2_IsolationIndexMVA"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2IsolationIndexMVA(idx);
        };
        accessors["T1_IsolationIndexMVA2"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1IsolationIndexMVA2(idx);
        };
        accessors["T2_IsolationIndexMVA2"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2IsolationIndexMVA2(idx);
        };
        accessors["T1_IsolationMVA2Raw"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyIsolationMVA2raw)[idx];
        };
        accessors["T2_IsolationMVA2Raw"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyIsolationMVA2raw)[idx];
        };

        accessors["TT_ChargeProduct"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1Charge)[idx] * (*e->TTL_Tau2Charge)[idx];
        };

        accessors["TT_VisMass"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_DitauVisibleMass)[idx];
        };
        accessors["TT_DeltaEta"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1Eta)[idx] - (*e->TTL_Tau2Eta)[idx];
        };
        accessors["TT_CosDeltaPhi"] = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_DitauCosDeltaPhi)[idx];
        };
        accessors["MET"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_MET;
        };
        accessors["MVA"] = [](Branches *b, const int& idx) -> float {
            return MVABase::gMVA["CFMlpANN"] ? MVABase::gMVA["CFMlpANN"]->Evaluate(b, idx) : 0.;
        };

        setup = true;
    }
}

NamespaceImp(roast)
