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

    std::vector<std::string>
    get_accessor_names()
    {
        std::vector<std::string> res;
        for (const auto& p: accessors)
            res.push_back(p.first);
        return res;
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

        // >>> Begin attr <<<
        accessors["BQuarkCount"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_bQuarkCount;
        };
        accessors["CQuarkCount"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_cQuarkCount;
        };
        accessors["CSVeventWeight"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeight)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeight)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightCErr1down"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightCErr1down)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightCErr1down)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightCErr1up"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightCErr1up)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightCErr1up)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightCErr2down"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightCErr2down)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightCErr2down)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightCErr2up"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightCErr2up)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightCErr2up)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightHFStats1down"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightHFStats1down)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightHFStats1down)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightHFStats1up"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightHFStats1up)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightHFStats1up)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightHFStats2down"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightHFStats2down)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightHFStats2down)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightHFStats2up"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightHFStats2up)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightHFStats2up)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightHFdown"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightHFdown)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightHFdown)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightHFup"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightHFup)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightHFup)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightLFStats1down"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightLFStats1down)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightLFStats1down)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightLFStats1up"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightLFStats1up)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightLFStats1up)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightLFStats2down"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightLFStats2down)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightLFStats2down)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightLFStats2up"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightLFStats2up)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightLFStats2up)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightLFdown"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightLFdown)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightLFdown)[idx];
            }
            throw "";
        };
        accessors["CSVeventWeightLFup"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->CSVeventWeightLFup)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->CSVeventWeightLFup)[idx];
            }
            throw "";
        };
        accessors["DitauCosDeltaPhi"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_DitauCosDeltaPhi)[idx];
        };
        accessors["DitauDeltaR"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_DitauDeltaR)[idx];
        };
        accessors["DitauMETMass"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_DitauMETMass)[idx];
        };
        accessors["DitauVisibleMass"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_DitauVisibleMass)[idx];
        };
        accessors["E_NumElectrons"] = [](Branches *b, const int& idx) -> float {
            return b->E_NumElectrons;
        };
        accessors["ElectronMomentumRank"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_ElectronMomentumRank)[idx];
        };
        accessors["EventNumber"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_eventNumber;
        };
        accessors["GJ_NumGenJets"] = [](Branches *b, const int& idx) -> float {
            return b->GJ_NumGenJets;
        };
        accessors["GT_NumGenTaus"] = [](Branches *b, const int& idx) -> float {
            return b->GT_NumGenTaus;
        };
        accessors["HT"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_HT)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_HT)[idx];
            }
            throw "";
        };
        accessors["HiggsDecayMode"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_higgsDecayMode;
        };
        accessors["IsTauEvent"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_IsTauEvent;
        };
        accessors["LJ_Eta"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_Eta)[i];
        };
        accessors["SubLJ_Eta"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_Eta)[i];
        };
        accessors["SubSubLJ_Eta"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_Eta)[i];
        };
        accessors["SubSubSubLJ_Eta"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_Eta)[i];
        };
        accessors["LJ_MomentumRank"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_MomentumRank)[i];
        };
        accessors["SubLJ_MomentumRank"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_MomentumRank)[i];
        };
        accessors["SubSubLJ_MomentumRank"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_MomentumRank)[i];
        };
        accessors["SubSubSubLJ_MomentumRank"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_MomentumRank)[i];
        };
        accessors["J_NumJets"] = [](Branches *b, const int& idx) -> float {
            return b->J_NumJets;
        };
        accessors["LJ_Phi"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_Phi)[i];
        };
        accessors["SubLJ_Phi"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_Phi)[i];
        };
        accessors["SubSubLJ_Phi"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_Phi)[i];
        };
        accessors["SubSubSubLJ_Phi"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_Phi)[i];
        };
        accessors["LJ_Pt"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_Pt)[i];
        };
        accessors["SubLJ_Pt"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_Pt)[i];
        };
        accessors["SubSubLJ_Pt"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_Pt)[i];
        };
        accessors["SubSubSubLJ_Pt"] = [](Branches *b, const int& idx) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_Pt)[i];
        };
        accessors["L1_Eta"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1Eta)[idx];
        };
        accessors["L1_EventWeight"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1EventWeight)[idx];
        };
        accessors["L1_GenMatchDaughter0Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchDaughter0Id)[idx];
        };
        accessors["L1_GenMatchDaughter0Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchDaughter0Status)[idx];
        };
        accessors["L1_GenMatchDaughter1Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchDaughter1Id)[idx];
        };
        accessors["L1_GenMatchDaughter1Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchDaughter1Status)[idx];
        };
        accessors["L1_GenMatchGrandmother00Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchGrandmother00Id)[idx];
        };
        accessors["L1_GenMatchGrandmother00Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchGrandmother00Status)[idx];
        };
        accessors["L1_GenMatchGrandmother01Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchGrandmother01Id)[idx];
        };
        accessors["L1_GenMatchGrandmother01Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchGrandmother01Status)[idx];
        };
        accessors["L1_GenMatchGrandmother10Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchGrandmother10Id)[idx];
        };
        accessors["L1_GenMatchGrandmother10Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchGrandmother10Status)[idx];
        };
        accessors["L1_GenMatchGrandmother11Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchGrandmother11Id)[idx];
        };
        accessors["L1_GenMatchGrandmother11Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchGrandmother11Status)[idx];
        };
        accessors["L1_GenMatchId"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchId)[idx];
        };
        accessors["L1_GenMatchMother0Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchMother0Id)[idx];
        };
        accessors["L1_GenMatchMother0Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchMother0Status)[idx];
        };
        accessors["L1_GenMatchMother1Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchMother1Id)[idx];
        };
        accessors["L1_GenMatchMother1Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchMother1Status)[idx];
        };
        accessors["L1_GenMatchStatus"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1GenMatchStatus)[idx];
        };
        accessors["L1_IsElectron"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1IsElectron)[idx];
        };
        accessors["L1_IsLoose"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1IsLoose)[idx];
        };
        accessors["L1_IsMuon"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1IsMuon)[idx];
        };
        accessors["L1_IsTight"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1IsTight)[idx];
        };
        accessors["L1_Lepton2CosDeltaPhi"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1Lepton2CosDeltaPhi)[idx];
        };
        accessors["L1_Lepton2DeltaR"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1Lepton2DeltaR)[idx];
        };
        accessors["L1_Lepton2VisibleMass"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1Lepton2VisibleMass)[idx];
        };
        accessors["L1_Phi"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1Phi)[idx];
        };
        accessors["L1_Pt"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1Pt)[idx];
        };
        accessors["L1_RelIso"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1RelIso)[idx];
        };
        accessors["L2_Eta"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2Eta)[idx];
        };
        accessors["L2_EventWeight"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2EventWeight)[idx];
        };
        accessors["L2_GenMatchDaughter0Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchDaughter0Id)[idx];
        };
        accessors["L2_GenMatchDaughter0Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchDaughter0Status)[idx];
        };
        accessors["L2_GenMatchDaughter1Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchDaughter1Id)[idx];
        };
        accessors["L2_GenMatchDaughter1Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchDaughter1Status)[idx];
        };
        accessors["L2_GenMatchGrandmother00Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchGrandmother00Id)[idx];
        };
        accessors["L2_GenMatchGrandmother00Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchGrandmother00Status)[idx];
        };
        accessors["L2_GenMatchGrandmother01Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchGrandmother01Id)[idx];
        };
        accessors["L2_GenMatchGrandmother01Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchGrandmother01Status)[idx];
        };
        accessors["L2_GenMatchGrandmother10Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchGrandmother10Id)[idx];
        };
        accessors["L2_GenMatchGrandmother10Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchGrandmother10Status)[idx];
        };
        accessors["L2_GenMatchGrandmother11Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchGrandmother11Id)[idx];
        };
        accessors["L2_GenMatchGrandmother11Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchGrandmother11Status)[idx];
        };
        accessors["L2_GenMatchId"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchId)[idx];
        };
        accessors["L2_GenMatchMother0Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchMother0Id)[idx];
        };
        accessors["L2_GenMatchMother0Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchMother0Status)[idx];
        };
        accessors["L2_GenMatchMother1Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchMother1Id)[idx];
        };
        accessors["L2_GenMatchMother1Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchMother1Status)[idx];
        };
        accessors["L2_GenMatchStatus"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2GenMatchStatus)[idx];
        };
        accessors["L2_IsElectron"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2IsElectron)[idx];
        };
        accessors["L2_IsLoose"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2IsLoose)[idx];
        };
        accessors["L2_IsMuon"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2IsMuon)[idx];
        };
        accessors["L2_IsTight"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2IsTight)[idx];
        };
        accessors["L2_Phi"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2Phi)[idx];
        };
        accessors["L2_Pt"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2Pt)[idx];
        };
        accessors["L2_RelIso"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2RelIso)[idx];
        };
        accessors["L_Eta"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonEta)[idx];
        };
        accessors["L_EventWeight"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonEventWeight)[idx];
        };
        accessors["L_GenMatchDaughter0Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchDaughter0Id)[idx];
        };
        accessors["L_GenMatchDaughter0Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchDaughter0Status)[idx];
        };
        accessors["L_GenMatchDaughter1Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchDaughter1Id)[idx];
        };
        accessors["L_GenMatchDaughter1Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchDaughter1Status)[idx];
        };
        accessors["L_GenMatchGrandmother00Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchGrandmother00Id)[idx];
        };
        accessors["L_GenMatchGrandmother00Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchGrandmother00Status)[idx];
        };
        accessors["L_GenMatchGrandmother01Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchGrandmother01Id)[idx];
        };
        accessors["L_GenMatchGrandmother01Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchGrandmother01Status)[idx];
        };
        accessors["L_GenMatchGrandmother10Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchGrandmother10Id)[idx];
        };
        accessors["L_GenMatchGrandmother10Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchGrandmother10Status)[idx];
        };
        accessors["L_GenMatchGrandmother11Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchGrandmother11Id)[idx];
        };
        accessors["L_GenMatchGrandmother11Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchGrandmother11Status)[idx];
        };
        accessors["L_GenMatchId"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchId)[idx];
        };
        accessors["L_GenMatchMother0Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchMother0Id)[idx];
        };
        accessors["L_GenMatchMother0Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchMother0Status)[idx];
        };
        accessors["L_GenMatchMother1Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchMother1Id)[idx];
        };
        accessors["L_GenMatchMother1Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchMother1Status)[idx];
        };
        accessors["L_GenMatchStatus"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonGenMatchStatus)[idx];
        };
        accessors["L_IsElectron"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonIsElectron)[idx];
        };
        accessors["L_IsLoose"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonIsLoose)[idx];
        };
        accessors["L_IsMuon"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonIsMuon)[idx];
        };
        accessors["L_IsTight"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonIsTight)[idx];
        };
        accessors["L_MomentumRank"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonMomentumRank)[idx];
        };
        accessors["L_Phi"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonPhi)[idx];
        };
        accessors["L_Pt"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonPt)[idx];
        };
        accessors["L_RelIso"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonRelIso)[idx];
        };
        accessors["LumiBlock"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_lumiBlock;
        };
        accessors["MET"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_MET;
        };
        accessors["METphi"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_METphi;
        };
        accessors["M_NumMuons"] = [](Branches *b, const int& idx) -> float {
            return b->M_NumMuons;
        };
        accessors["MomentumRank"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_MomentumRank)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_MomentumRank)[idx];
            }
            throw "";
        };
        accessors["MuonMomentumRank"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_MuonMomentumRank)[idx];
        };
        accessors["NumCSVLbtagJets"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCSVLbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCSVLbtagJets)[idx];
            }
            throw "";
        };
        accessors["NumCSVMbtagJets"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCSVMbtagJets)[idx];
            }
            throw "";
        };
        accessors["NumCSVTbtagJets"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCSVTbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCSVTbtagJets)[idx];
            }
            throw "";
        };
        accessors["NumCleanCSVLbtagJets"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanCSVLbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanCSVLbtagJets)[idx];
            }
            throw "";
        };
        accessors["NumCleanCSVMbtagJets"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanCSVMbtagJets)[idx];
            }
            throw "";
        };
        accessors["NumCleanCSVTbtagJets"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanCSVTbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanCSVTbtagJets)[idx];
            }
            throw "";
        };
        accessors["NumCleanNonCSVLbtagJets"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanNonCSVLbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanNonCSVLbtagJets)[idx];
            }
            throw "";
        };
        accessors["NumCleanNonCSVMbtagJets"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanNonCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanNonCSVMbtagJets)[idx];
            }
            throw "";
        };
        accessors["NumCleanNonCSVTbtagJets"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanNonCSVTbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanNonCSVTbtagJets)[idx];
            }
            throw "";
        };
        accessors["NumCombos"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return e->TLL_NumCombos;
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return e->TTL_NumCombos;
            }
            throw "";
        };
        accessors["NumElectrons"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->TTL_NumElectrons;
        };
        accessors["NumExLooseElectrons"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumExLooseElectrons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumExLooseElectrons)[idx];
            }
            throw "";
        };
        accessors["NumExLooseMuons"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumExLooseMuons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumExLooseMuons)[idx];
            }
            throw "";
        };
        accessors["NumLeptons"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->TTL_NumLeptons;
        };
        accessors["NumLooseElectrons"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumLooseElectrons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumLooseElectrons)[idx];
            }
            throw "";
        };
        accessors["NumLooseMuons"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumLooseMuons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumLooseMuons)[idx];
            }
            throw "";
        };
        accessors["NumMuons"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->TTL_NumMuons;
        };
        accessors["NumNonCSVLbtagJets"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumNonCSVLbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumNonCSVLbtagJets)[idx];
            }
            throw "";
        };
        accessors["NumNonCSVMbtagJets"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumNonCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumNonCSVMbtagJets)[idx];
            }
            throw "";
        };
        accessors["NumNonCSVTbtagJets"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumNonCSVTbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumNonCSVTbtagJets)[idx];
            }
            throw "";
        };
        accessors["NumTaus"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return e->TLL_NumTaus;
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return e->TTL_NumTaus;
            }
            throw "";
        };
        accessors["NumTightElectrons"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumTightElectrons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumTightElectrons)[idx];
            }
            throw "";
        };
        accessors["NumTightMuons"] = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumTightMuons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumTightMuons)[idx];
            }
            throw "";
        };
        accessors["PuWeight"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_puWeight;
        };
        accessors["PuWeightDown"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_puWeightDown;
        };
        accessors["PuWeightUp"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_puWeightUp;
        };
        accessors["Q2WeightDown"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_q2WeightDown;
        };
        accessors["Q2WeightUp"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_q2WeightUp;
        };
        accessors["RunNumber"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_runNumber;
        };
        accessors["T1_Charge"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1Charge)[idx];
        };
        accessors["T1_DecayMode"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1DecayMode)[idx];
        };
        accessors["T1_EmFraction"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1EmFraction)[idx];
        };
        accessors["T1_Eta"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1Eta)[idx];
        };
        accessors["T1_GenMatchDaughter0Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchDaughter0Id)[idx];
        };
        accessors["T1_GenMatchDaughter0Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchDaughter0Status)[idx];
        };
        accessors["T1_GenMatchDaughter1Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchDaughter1Id)[idx];
        };
        accessors["T1_GenMatchDaughter1Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchDaughter1Status)[idx];
        };
        accessors["T1_GenMatchGrandmother00Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchGrandmother00Id)[idx];
        };
        accessors["T1_GenMatchGrandmother00Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchGrandmother00Status)[idx];
        };
        accessors["T1_GenMatchGrandmother01Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchGrandmother01Id)[idx];
        };
        accessors["T1_GenMatchGrandmother01Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchGrandmother01Status)[idx];
        };
        accessors["T1_GenMatchGrandmother10Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchGrandmother10Id)[idx];
        };
        accessors["T1_GenMatchGrandmother10Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchGrandmother10Status)[idx];
        };
        accessors["T1_GenMatchGrandmother11Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchGrandmother11Id)[idx];
        };
        accessors["T1_GenMatchGrandmother11Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchGrandmother11Status)[idx];
        };
        accessors["T1_GenMatchId"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchId)[idx];
        };
        accessors["T1_GenMatchMother0Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchMother0Id)[idx];
        };
        accessors["T1_GenMatchMother0Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchMother0Status)[idx];
        };
        accessors["T1_GenMatchMother1Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchMother1Id)[idx];
        };
        accessors["T1_GenMatchMother1Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchMother1Status)[idx];
        };
        accessors["T1_GenMatchStatus"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1GenMatchStatus)[idx];
        };
        accessors["T1_HPSagainstElectronDeadECAL"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronDeadECAL)[idx];
        };
        accessors["T1_HPSagainstElectronLoose"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronLoose)[idx];
        };
        accessors["T1_HPSagainstElectronLooseMVA2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronLooseMVA2)[idx];
        };
        accessors["T1_HPSagainstElectronLooseMVA3"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronLooseMVA3)[idx];
        };
        accessors["T1_HPSagainstElectronMVA"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronMVA)[idx];
        };
        accessors["T1_HPSagainstElectronMVA2category"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronMVA2category)[idx];
        };
        accessors["T1_HPSagainstElectronMVA2raw"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronMVA2raw)[idx];
        };
        accessors["T1_HPSagainstElectronMVA3category"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronMVA3category)[idx];
        };
        accessors["T1_HPSagainstElectronMVA3raw"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronMVA3raw)[idx];
        };
        accessors["T1_HPSagainstElectronMedium"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronMedium)[idx];
        };
        accessors["T1_HPSagainstElectronMediumMVA2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronMediumMVA2)[idx];
        };
        accessors["T1_HPSagainstElectronMediumMVA3"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronMediumMVA3)[idx];
        };
        accessors["T1_HPSagainstElectronTight"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronTight)[idx];
        };
        accessors["T1_HPSagainstElectronTightMVA2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronTightMVA2)[idx];
        };
        accessors["T1_HPSagainstElectronTightMVA3"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronTightMVA3)[idx];
        };
        accessors["T1_HPSagainstElectronVLooseMVA2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronVLooseMVA2)[idx];
        };
        accessors["T1_HPSagainstElectronVTightMVA3"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronVTightMVA3)[idx];
        };
        accessors["T1_HPSagainstMuonLoose"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstMuonLoose)[idx];
        };
        accessors["T1_HPSagainstMuonLoose2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstMuonLoose2)[idx];
        };
        accessors["T1_HPSagainstMuonMedium"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstMuonMedium)[idx];
        };
        accessors["T1_HPSagainstMuonMedium2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstMuonMedium2)[idx];
        };
        accessors["T1_HPSagainstMuonTight"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstMuonTight)[idx];
        };
        accessors["T1_HPSagainstMuonTight2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstMuonTight2)[idx];
        };
        accessors["T1_HPSbyCombinedIsolationDeltaBetaCorrRaw"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyCombinedIsolationDeltaBetaCorrRaw)[idx];
        };
        accessors["T1_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx];
        };
        accessors["T1_HPSbyIsolationMVA2raw"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyIsolationMVA2raw)[idx];
        };
        accessors["T1_HPSbyIsolationMVAraw"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyIsolationMVAraw)[idx];
        };
        accessors["T1_HPSbyLooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyLooseCombinedIsolationDeltaBetaCorr)[idx];
        };
        accessors["T1_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[idx];
        };
        accessors["T1_HPSbyLooseIsolationMVA"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyLooseIsolationMVA)[idx];
        };
        accessors["T1_HPSbyLooseIsolationMVA2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyLooseIsolationMVA2)[idx];
        };
        accessors["T1_HPSbyMediumCombinedIsolationDeltaBetaCorr"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyMediumCombinedIsolationDeltaBetaCorr)[idx];
        };
        accessors["T1_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[idx];
        };
        accessors["T1_HPSbyMediumIsolationMVA"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyMediumIsolationMVA)[idx];
        };
        accessors["T1_HPSbyMediumIsolationMVA2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyMediumIsolationMVA2)[idx];
        };
        accessors["T1_HPSbyTightCombinedIsolationDeltaBetaCorr"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyTightCombinedIsolationDeltaBetaCorr)[idx];
        };
        accessors["T1_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[idx];
        };
        accessors["T1_HPSbyTightIsolationMVA"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyTightIsolationMVA)[idx];
        };
        accessors["T1_HPSbyTightIsolationMVA2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyTightIsolationMVA2)[idx];
        };
        accessors["T1_HPSbyVLooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyVLooseCombinedIsolationDeltaBetaCorr)[idx];
        };
        accessors["T1_HPSdecayModeFinding"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSdecayModeFinding)[idx];
        };
        accessors["T1_IsInTheCracks"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1IsInTheCracks)[idx];
        };
        accessors["T1_LTIpVtdxy"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LTIpVtdxy)[idx];
        };
        accessors["T1_LTIpVtdxyError"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LTIpVtdxyError)[idx];
        };
        accessors["T1_LTIpVtdz"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LTIpVtdz)[idx];
        };
        accessors["T1_LTIpVtdzError"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LTIpVtdzError)[idx];
        };
        accessors["T1_LTNormChiSqrd"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LTNormChiSqrd)[idx];
        };
        accessors["T1_LTPt"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LTPt)[idx];
        };
        accessors["T1_LTValidHits"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LTValidHits)[idx];
        };
        accessors["T1_LTvalid"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LTvalid)[idx];
        };
        accessors["T1_LTvx"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LTvx)[idx];
        };
        accessors["T1_LTvy"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LTvy)[idx];
        };
        accessors["T1_LTvz"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LTvz)[idx];
        };
        accessors["T1_LeptonDeltaR"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LeptonDeltaR)[idx];
        };
        accessors["T1_MomentumRank"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1MomentumRank)[idx];
        };
        accessors["T1_NProngs"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1NProngs)[idx];
        };
        accessors["T1_NSignalGammas"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1NSignalGammas)[idx];
        };
        accessors["T1_NSignalNeutrals"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1NSignalNeutrals)[idx];
        };
        accessors["T1_Phi"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1Phi)[idx];
        };
        accessors["T1_Pt"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1Pt)[idx];
        };
        accessors["T2_Charge"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2Charge)[idx];
        };
        accessors["T2_DecayMode"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2DecayMode)[idx];
        };
        accessors["T2_EmFraction"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2EmFraction)[idx];
        };
        accessors["T2_Eta"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2Eta)[idx];
        };
        accessors["T2_GenMatchDaughter0Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchDaughter0Id)[idx];
        };
        accessors["T2_GenMatchDaughter0Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchDaughter0Status)[idx];
        };
        accessors["T2_GenMatchDaughter1Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchDaughter1Id)[idx];
        };
        accessors["T2_GenMatchDaughter1Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchDaughter1Status)[idx];
        };
        accessors["T2_GenMatchGrandmother00Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchGrandmother00Id)[idx];
        };
        accessors["T2_GenMatchGrandmother00Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchGrandmother00Status)[idx];
        };
        accessors["T2_GenMatchGrandmother01Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchGrandmother01Id)[idx];
        };
        accessors["T2_GenMatchGrandmother01Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchGrandmother01Status)[idx];
        };
        accessors["T2_GenMatchGrandmother10Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchGrandmother10Id)[idx];
        };
        accessors["T2_GenMatchGrandmother10Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchGrandmother10Status)[idx];
        };
        accessors["T2_GenMatchGrandmother11Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchGrandmother11Id)[idx];
        };
        accessors["T2_GenMatchGrandmother11Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchGrandmother11Status)[idx];
        };
        accessors["T2_GenMatchId"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchId)[idx];
        };
        accessors["T2_GenMatchMother0Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchMother0Id)[idx];
        };
        accessors["T2_GenMatchMother0Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchMother0Status)[idx];
        };
        accessors["T2_GenMatchMother1Id"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchMother1Id)[idx];
        };
        accessors["T2_GenMatchMother1Status"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchMother1Status)[idx];
        };
        accessors["T2_GenMatchStatus"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2GenMatchStatus)[idx];
        };
        accessors["T2_HPSagainstElectronDeadECAL"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronDeadECAL)[idx];
        };
        accessors["T2_HPSagainstElectronLoose"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronLoose)[idx];
        };
        accessors["T2_HPSagainstElectronLooseMVA2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronLooseMVA2)[idx];
        };
        accessors["T2_HPSagainstElectronLooseMVA3"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronLooseMVA3)[idx];
        };
        accessors["T2_HPSagainstElectronMVA"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronMVA)[idx];
        };
        accessors["T2_HPSagainstElectronMVA2category"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronMVA2category)[idx];
        };
        accessors["T2_HPSagainstElectronMVA2raw"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronMVA2raw)[idx];
        };
        accessors["T2_HPSagainstElectronMVA3category"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronMVA3category)[idx];
        };
        accessors["T2_HPSagainstElectronMVA3raw"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronMVA3raw)[idx];
        };
        accessors["T2_HPSagainstElectronMedium"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronMedium)[idx];
        };
        accessors["T2_HPSagainstElectronMediumMVA2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronMediumMVA2)[idx];
        };
        accessors["T2_HPSagainstElectronMediumMVA3"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronMediumMVA3)[idx];
        };
        accessors["T2_HPSagainstElectronTight"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronTight)[idx];
        };
        accessors["T2_HPSagainstElectronTightMVA2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronTightMVA2)[idx];
        };
        accessors["T2_HPSagainstElectronTightMVA3"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronTightMVA3)[idx];
        };
        accessors["T2_HPSagainstElectronVLooseMVA2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronVLooseMVA2)[idx];
        };
        accessors["T2_HPSagainstElectronVTightMVA3"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronVTightMVA3)[idx];
        };
        accessors["T2_HPSagainstMuonLoose"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstMuonLoose)[idx];
        };
        accessors["T2_HPSagainstMuonLoose2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstMuonLoose2)[idx];
        };
        accessors["T2_HPSagainstMuonMedium"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstMuonMedium)[idx];
        };
        accessors["T2_HPSagainstMuonMedium2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstMuonMedium2)[idx];
        };
        accessors["T2_HPSagainstMuonTight"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstMuonTight)[idx];
        };
        accessors["T2_HPSagainstMuonTight2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstMuonTight2)[idx];
        };
        accessors["T2_HPSbyCombinedIsolationDeltaBetaCorrRaw"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyCombinedIsolationDeltaBetaCorrRaw)[idx];
        };
        accessors["T2_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx];
        };
        accessors["T2_HPSbyIsolationMVA2raw"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyIsolationMVA2raw)[idx];
        };
        accessors["T2_HPSbyIsolationMVAraw"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyIsolationMVAraw)[idx];
        };
        accessors["T2_HPSbyLooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyLooseCombinedIsolationDeltaBetaCorr)[idx];
        };
        accessors["T2_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[idx];
        };
        accessors["T2_HPSbyLooseIsolationMVA"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyLooseIsolationMVA)[idx];
        };
        accessors["T2_HPSbyLooseIsolationMVA2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyLooseIsolationMVA2)[idx];
        };
        accessors["T2_HPSbyMediumCombinedIsolationDeltaBetaCorr"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyMediumCombinedIsolationDeltaBetaCorr)[idx];
        };
        accessors["T2_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[idx];
        };
        accessors["T2_HPSbyMediumIsolationMVA"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyMediumIsolationMVA)[idx];
        };
        accessors["T2_HPSbyMediumIsolationMVA2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyMediumIsolationMVA2)[idx];
        };
        accessors["T2_HPSbyTightCombinedIsolationDeltaBetaCorr"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyTightCombinedIsolationDeltaBetaCorr)[idx];
        };
        accessors["T2_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[idx];
        };
        accessors["T2_HPSbyTightIsolationMVA"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyTightIsolationMVA)[idx];
        };
        accessors["T2_HPSbyTightIsolationMVA2"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyTightIsolationMVA2)[idx];
        };
        accessors["T2_HPSbyVLooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyVLooseCombinedIsolationDeltaBetaCorr)[idx];
        };
        accessors["T2_HPSdecayModeFinding"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSdecayModeFinding)[idx];
        };
        accessors["T2_IsInTheCracks"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2IsInTheCracks)[idx];
        };
        accessors["T2_LTIpVtdxy"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LTIpVtdxy)[idx];
        };
        accessors["T2_LTIpVtdxyError"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LTIpVtdxyError)[idx];
        };
        accessors["T2_LTIpVtdz"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LTIpVtdz)[idx];
        };
        accessors["T2_LTIpVtdzError"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LTIpVtdzError)[idx];
        };
        accessors["T2_LTNormChiSqrd"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LTNormChiSqrd)[idx];
        };
        accessors["T2_LTPt"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LTPt)[idx];
        };
        accessors["T2_LTValidHits"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LTValidHits)[idx];
        };
        accessors["T2_LTvalid"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LTvalid)[idx];
        };
        accessors["T2_LTvx"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LTvx)[idx];
        };
        accessors["T2_LTvy"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LTvy)[idx];
        };
        accessors["T2_LTvz"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LTvz)[idx];
        };
        accessors["T2_LeptonDeltaR"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LeptonDeltaR)[idx];
        };
        accessors["T2_MomentumRank"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2MomentumRank)[idx];
        };
        accessors["T2_NProngs"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2NProngs)[idx];
        };
        accessors["T2_NSignalGammas"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2NSignalGammas)[idx];
        };
        accessors["T2_NSignalNeutrals"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2NSignalNeutrals)[idx];
        };
        accessors["T2_Phi"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2Phi)[idx];
        };
        accessors["T2_Pt"] = [](Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2Pt)[idx];
        };
        accessors["T_Charge"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauCharge)[idx];
        };
        accessors["T_DecayMode"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauDecayMode)[idx];
        };
        accessors["T_EmFraction"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauEmFraction)[idx];
        };
        accessors["T_Eta"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauEta)[idx];
        };
        accessors["T_GenMatchDaughter0Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchDaughter0Id)[idx];
        };
        accessors["T_GenMatchDaughter0Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchDaughter0Status)[idx];
        };
        accessors["T_GenMatchDaughter1Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchDaughter1Id)[idx];
        };
        accessors["T_GenMatchDaughter1Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchDaughter1Status)[idx];
        };
        accessors["T_GenMatchGrandmother00Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchGrandmother00Id)[idx];
        };
        accessors["T_GenMatchGrandmother00Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchGrandmother00Status)[idx];
        };
        accessors["T_GenMatchGrandmother01Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchGrandmother01Id)[idx];
        };
        accessors["T_GenMatchGrandmother01Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchGrandmother01Status)[idx];
        };
        accessors["T_GenMatchGrandmother10Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchGrandmother10Id)[idx];
        };
        accessors["T_GenMatchGrandmother10Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchGrandmother10Status)[idx];
        };
        accessors["T_GenMatchGrandmother11Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchGrandmother11Id)[idx];
        };
        accessors["T_GenMatchGrandmother11Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchGrandmother11Status)[idx];
        };
        accessors["T_GenMatchId"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchId)[idx];
        };
        accessors["T_GenMatchMother0Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchMother0Id)[idx];
        };
        accessors["T_GenMatchMother0Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchMother0Status)[idx];
        };
        accessors["T_GenMatchMother1Id"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchMother1Id)[idx];
        };
        accessors["T_GenMatchMother1Status"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchMother1Status)[idx];
        };
        accessors["T_GenMatchStatus"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauGenMatchStatus)[idx];
        };
        accessors["T_HPSagainstElectronDeadECAL"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronDeadECAL)[idx];
        };
        accessors["T_HPSagainstElectronLoose"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronLoose)[idx];
        };
        accessors["T_HPSagainstElectronLooseMVA2"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronLooseMVA2)[idx];
        };
        accessors["T_HPSagainstElectronLooseMVA3"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronLooseMVA3)[idx];
        };
        accessors["T_HPSagainstElectronMVA"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronMVA)[idx];
        };
        accessors["T_HPSagainstElectronMVA2category"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronMVA2category)[idx];
        };
        accessors["T_HPSagainstElectronMVA2raw"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronMVA2raw)[idx];
        };
        accessors["T_HPSagainstElectronMVA3category"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronMVA3category)[idx];
        };
        accessors["T_HPSagainstElectronMVA3raw"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronMVA3raw)[idx];
        };
        accessors["T_HPSagainstElectronMedium"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronMedium)[idx];
        };
        accessors["T_HPSagainstElectronMediumMVA2"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronMediumMVA2)[idx];
        };
        accessors["T_HPSagainstElectronMediumMVA3"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronMediumMVA3)[idx];
        };
        accessors["T_HPSagainstElectronTight"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronTight)[idx];
        };
        accessors["T_HPSagainstElectronTightMVA2"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronTightMVA2)[idx];
        };
        accessors["T_HPSagainstElectronTightMVA3"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronTightMVA3)[idx];
        };
        accessors["T_HPSagainstElectronVLooseMVA2"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronVLooseMVA2)[idx];
        };
        accessors["T_HPSagainstElectronVTightMVA3"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronVTightMVA3)[idx];
        };
        accessors["T_HPSagainstMuonLoose"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstMuonLoose)[idx];
        };
        accessors["T_HPSagainstMuonLoose2"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstMuonLoose2)[idx];
        };
        accessors["T_HPSagainstMuonMedium"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstMuonMedium)[idx];
        };
        accessors["T_HPSagainstMuonMedium2"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstMuonMedium2)[idx];
        };
        accessors["T_HPSagainstMuonTight"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstMuonTight)[idx];
        };
        accessors["T_HPSagainstMuonTight2"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstMuonTight2)[idx];
        };
        accessors["T_HPSbyCombinedIsolationDeltaBetaCorrRaw"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw)[idx];
        };
        accessors["T_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx];
        };
        accessors["T_HPSbyIsolationMVA2raw"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyIsolationMVA2raw)[idx];
        };
        accessors["T_HPSbyIsolationMVAraw"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyIsolationMVAraw)[idx];
        };
        accessors["T_HPSbyLooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr)[idx];
        };
        accessors["T_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[idx];
        };
        accessors["T_HPSbyLooseIsolationMVA"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyLooseIsolationMVA)[idx];
        };
        accessors["T_HPSbyLooseIsolationMVA2"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyLooseIsolationMVA2)[idx];
        };
        accessors["T_HPSbyMediumCombinedIsolationDeltaBetaCorr"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr)[idx];
        };
        accessors["T_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[idx];
        };
        accessors["T_HPSbyMediumIsolationMVA"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyMediumIsolationMVA)[idx];
        };
        accessors["T_HPSbyMediumIsolationMVA2"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyMediumIsolationMVA2)[idx];
        };
        accessors["T_HPSbyTightCombinedIsolationDeltaBetaCorr"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyTightCombinedIsolationDeltaBetaCorr)[idx];
        };
        accessors["T_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[idx];
        };
        accessors["T_HPSbyTightIsolationMVA"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyTightIsolationMVA)[idx];
        };
        accessors["T_HPSbyTightIsolationMVA2"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyTightIsolationMVA2)[idx];
        };
        accessors["T_HPSbyVLooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyVLooseCombinedIsolationDeltaBetaCorr)[idx];
        };
        accessors["T_HPSdecayModeFinding"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSdecayModeFinding)[idx];
        };
        accessors["T_IsInTheCracks"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauIsInTheCracks)[idx];
        };
        accessors["T_LTIpVtdxy"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLTIpVtdxy)[idx];
        };
        accessors["T_LTIpVtdxyError"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLTIpVtdxyError)[idx];
        };
        accessors["T_LTIpVtdz"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLTIpVtdz)[idx];
        };
        accessors["T_LTIpVtdzError"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLTIpVtdzError)[idx];
        };
        accessors["T_LTNormChiSqrd"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLTNormChiSqrd)[idx];
        };
        accessors["T_LTPt"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLTPt)[idx];
        };
        accessors["T_LTValidHits"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLTValidHits)[idx];
        };
        accessors["T_LTvalid"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLTvalid)[idx];
        };
        accessors["T_LTvx"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLTvx)[idx];
        };
        accessors["T_LTvy"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLTvy)[idx];
        };
        accessors["T_LTvz"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLTvz)[idx];
        };
        accessors["T_Lepton1CosDeltaPhi"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLepton1CosDeltaPhi)[idx];
        };
        accessors["T_Lepton1DeltaR"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLepton1DeltaR)[idx];
        };
        accessors["T_Lepton1METMass"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLepton1METMass)[idx];
        };
        accessors["T_Lepton1VisibleMass"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLepton1VisibleMass)[idx];
        };
        accessors["T_Lepton2CosDeltaPhi"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLepton2CosDeltaPhi)[idx];
        };
        accessors["T_Lepton2DeltaR"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLepton2DeltaR)[idx];
        };
        accessors["T_Lepton2METMass"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLepton2METMass)[idx];
        };
        accessors["T_Lepton2VisibleMass"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLepton2VisibleMass)[idx];
        };
        accessors["T_MomentumRank"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauMomentumRank)[idx];
        };
        accessors["T_NProngs"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauNProngs)[idx];
        };
        accessors["T_NSignalGammas"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauNSignalGammas)[idx];
        };
        accessors["T_NSignalNeutrals"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauNSignalNeutrals)[idx];
        };
        accessors["T_Phi"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauPhi)[idx];
        };
        accessors["T_Pt"] = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauPt)[idx];
        };
        accessors["TopPtWeight"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_topPtWeight;
        };
        accessors["TopPtWeightDown"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_topPtWeightDown;
        };
        accessors["TopPtWeightUp"] = [](Branches *b, const int& idx) -> float {
            return b->Ev_topPtWeightUp;
        };
        accessors["V_NumInteractionsBX0"] = [](Branches *b, const int& idx) -> float {
            return b->V_NumInteractionsBX0;
        };
        accessors["V_NumInteractionsBXm1"] = [](Branches *b, const int& idx) -> float {
            return b->V_NumInteractionsBXm1;
        };
        accessors["V_NumInteractionsBXp1"] = [](Branches *b, const int& idx) -> float {
            return b->V_NumInteractionsBXp1;
        };
        accessors["V_NumVertices"] = [](Branches *b, const int& idx) -> float {
            return b->V_NumVertices;
        };
        // >>> End attr <<<

        setup = true;
    }
}

NamespaceImp(roast)
