// vim: ts=4:sw=4:et:sta

#include <map>

#include "DataFormats/Math/interface/deltaR.h"
#include "TauAnalysis/SVfitStandalone/interface/SVfitStandaloneAlgorithm.h"

#include "../interface/Accessor.h"
#include "../interface/MVABase.h"
#include "../interface/LLBranches.h"
#include "../interface/TLBranches.h"
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

        auto res = accessors.find(s);

        if (res == accessors.end())
            // FIXME use proper excpetion
            throw "";

        return res->second;
    }

    std::vector<std::string>
    get_accessor_names()
    {
        if (!setup)
            setup_accessors();

        std::vector<std::string> res;
        for (const auto& p: accessors)
            res.push_back(p.first);
        return res;
    }

    void
    setup_accessors()
    {
        accessors["Events"] = [](Branches *b, int idx, int n) -> float {
            return 0;
        };

        accessors["T_MatchAbsId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return abs((*e->TL_TauGenMatchId)[idx]);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return abs((*e->TLL_TauGenMatchId)[idx]);
            }
            throw "";
        };
        accessors["T1_MatchAbsId"] = [](roast::Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->TTL_Tau1GenMatchId)[idx]);
        };
        accessors["T2_MatchAbsId"] = [](roast::Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->TTL_Tau2GenMatchId)[idx]);
        };

        accessors["T_ParentAbsId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return abs((*e->TL_TauGenMatchMother0Id)[idx]);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return abs((*e->TLL_TauGenMatchMother0Id)[idx]);
            }
            throw "";
        };
        accessors["T1_ParentAbsId"] = [](roast::Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->TTL_Tau1GenMatchMother0Id)[idx]);
        };
        accessors["T2_ParentAbsId"] = [](roast::Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->TTL_Tau2GenMatchMother0Id)[idx]);
        };

        accessors["T_GoodParentId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                int mother = (*e->TL_TauGenMatchMother0Id)[idx];
                if (mother == -99)
                    mother = (*e->TL_TauGenMatchMother1Id)[idx];
                return mother;
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                int mother = (*e->TLL_TauGenMatchMother0Id)[idx];
                if (mother == -99)
                    mother = (*e->TLL_TauGenMatchMother1Id)[idx];
                return mother;
            }
            throw "";
        };
        accessors["T1_GoodParentId"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            int mother = (*e->TTL_Tau1GenMatchMother0Id)[idx];
            if (mother == -99)
                mother = (*e->TTL_Tau1GenMatchMother1Id)[idx];
            return mother;
        };
        accessors["T2_GoodParentId"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            int mother = (*e->TTL_Tau2GenMatchMother0Id)[idx];
            if (mother == -99)
                mother = (*e->TTL_Tau2GenMatchMother1Id)[idx];
            return mother;
        };
        accessors["L_GoodParentId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                int mother = (*e->TL_LeptonGenMatchMother0Id)[idx];
                if (mother == -99)
                    mother = (*e->TL_LeptonGenMatchMother1Id)[idx];
                return mother;
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                int mother = (*e->TTL_LeptonGenMatchMother0Id)[idx];
                if (mother == -99)
                    mother = (*e->TTL_LeptonGenMatchMother1Id)[idx];
                return mother;
            }
            throw "";
        };
        accessors["L1_GoodParentId"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            int mother = (*e->TLL_Lepton1GenMatchMother0Id)[idx];
            if (mother == -99)
                mother = (*e->TLL_Lepton1GenMatchMother1Id)[idx];
            return mother;
        };
        accessors["L2_GoodParentId"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            int mother = (*e->TLL_Lepton2GenMatchMother0Id)[idx];
            if (mother == -99)
                mother = (*e->TLL_Lepton2GenMatchMother1Id)[idx];
            return mother;
        };

        accessors["T_GoodGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                int grannie = (*e->TL_TauGenMatchGrandmother00Id)[idx];
                if (grannie == -99)
                    grannie = (*e->TL_TauGenMatchGrandmother01Id)[idx];
                if (grannie == -99)
                    grannie = (*e->TL_TauGenMatchGrandmother10Id)[idx];
                if (grannie == -99)
                    grannie = (*e->TL_TauGenMatchGrandmother11Id)[idx];
                return grannie;
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                int grannie = (*e->TLL_TauGenMatchGrandmother00Id)[idx];
                if (grannie == -99)
                    grannie = (*e->TLL_TauGenMatchGrandmother01Id)[idx];
                if (grannie == -99)
                    grannie = (*e->TLL_TauGenMatchGrandmother10Id)[idx];
                if (grannie == -99)
                    grannie = (*e->TLL_TauGenMatchGrandmother11Id)[idx];
                return grannie;
            }
            throw "";
        };
        accessors["T1_GoodGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            int grannie = (*e->TTL_Tau1GenMatchGrandmother00Id)[idx];
            if (grannie == -99)
                grannie = (*e->TTL_Tau1GenMatchGrandmother01Id)[idx];
            if (grannie == -99)
                grannie = (*e->TTL_Tau1GenMatchGrandmother10Id)[idx];
            if (grannie == -99)
                grannie = (*e->TTL_Tau1GenMatchGrandmother11Id)[idx];
            return grannie;
        };
        accessors["T2_GoodGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            int grannie = (*e->TTL_Tau2GenMatchGrandmother00Id)[idx];
            if (grannie == -99)
                grannie = (*e->TTL_Tau2GenMatchGrandmother01Id)[idx];
            if (grannie == -99)
                grannie = (*e->TTL_Tau2GenMatchGrandmother10Id)[idx];
            if (grannie == -99)
                grannie = (*e->TTL_Tau2GenMatchGrandmother11Id)[idx];
            return grannie;
        };
        accessors["L_GoodGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                int grannie = (*e->TL_LeptonGenMatchGrandmother00Id)[idx];
                if (grannie == -99)
                    grannie = (*e->TL_LeptonGenMatchGrandmother01Id)[idx];
                if (grannie == -99)
                    grannie = (*e->TL_LeptonGenMatchGrandmother10Id)[idx];
                if (grannie == -99)
                    grannie = (*e->TL_LeptonGenMatchGrandmother11Id)[idx];
                return grannie;
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                int grannie = (*e->TTL_LeptonGenMatchGrandmother00Id)[idx];
                if (grannie == -99)
                    grannie = (*e->TTL_LeptonGenMatchGrandmother01Id)[idx];
                if (grannie == -99)
                    grannie = (*e->TTL_LeptonGenMatchGrandmother10Id)[idx];
                if (grannie == -99)
                    grannie = (*e->TTL_LeptonGenMatchGrandmother11Id)[idx];
                return grannie;
            }
            throw "";
        };
        accessors["L1_GoodGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            int grannie = (*e->TLL_Lepton1GenMatchGrandmother00Id)[idx];
            if (grannie == -99)
                grannie = (*e->TLL_Lepton1GenMatchGrandmother01Id)[idx];
            if (grannie == -99)
                grannie = (*e->TLL_Lepton1GenMatchGrandmother10Id)[idx];
            if (grannie == -99)
                grannie = (*e->TLL_Lepton1GenMatchGrandmother11Id)[idx];
            return grannie;
        };
        accessors["L2_GoodGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            int grannie = (*e->TLL_Lepton2GenMatchGrandmother00Id)[idx];
            if (grannie == -99)
                grannie = (*e->TLL_Lepton2GenMatchGrandmother01Id)[idx];
            if (grannie == -99)
                grannie = (*e->TLL_Lepton2GenMatchGrandmother10Id)[idx];
            if (grannie == -99)
                grannie = (*e->TLL_Lepton2GenMatchGrandmother11Id)[idx];
            return grannie;
        };

        accessors["J_NumInclusive"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumCSVMbtagJets)[idx] + (*b->NumNonCSVMbtagJets)[idx];
        };
        accessors["J_NumCleanInclusive"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumCleanCSVMbtagJets)[idx] + (*b->NumCleanNonCSVMbtagJets)[idx];
        };
        accessors["NumTightLeptons"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumTightMuons)[idx] + (*b->NumTightElectrons)[idx];
        };
        accessors["NumLooseLeptons"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumLooseMuons)[idx] + (*b->NumLooseElectrons)[idx];
        };
        accessors["NumExLooseLeptons"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumExLooseMuons)[idx] + (*b->NumExLooseElectrons)[idx];
        };

        accessors["LL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return reco::deltaR((*e->TLL_Lepton1Eta)[idx], (*e->TLL_Lepton1Phi)[idx],
                    (*e->TLL_Lepton2Eta)[idx], (*e->TLL_Lepton2Phi)[idx]);
        };

        accessors["LL_ChargeProduct"] = [](roast::Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1Charge)[idx] * (*e->TLL_Lepton2Charge)[idx];
            } else if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1Charge)[idx] * (*e->LL_Lepton2Charge)[idx];
            }
            throw;
        };
        accessors["LL_ZPeakVeto"] = [](roast::Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            float m = (*e->TLL_Lepton1Lepton2VisibleMass)[idx];
            return (81. <= m && m <= 101.);
        };

        accessors["TL_METMass"] = [](roast::Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            if ((*e->TLL_Lepton1Charge)[idx] * (*e->TLL_TauCharge)[idx] < 0) {
                return (*e->TLL_TauLepton1METMass)[idx];
            } else {
                return (*e->TLL_TauLepton2METMass)[idx];
            }
        };
        accessors["TL_VissMass"] = [](roast::Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            if ((*e->TLL_Lepton1Charge)[idx] * (*e->TLL_TauCharge)[idx] < 0) {
                return (*e->TLL_TauLepton1VisibleMass)[idx];
            } else {
                return (*e->TLL_TauLepton2VisibleMass)[idx];
            }
        };

        accessors["TL_Mass"] = [](roast::Branches *b, int idx, int n) -> float {
            tl::Branches* e = dynamic_cast<tl::Branches*>(b);

            svFitStandalone::Vector met(e->Ev_MET * TMath::Sin(e->Ev_METphi), e->Ev_MET * TMath::Cos(e->Ev_METphi), 0.);
            TMatrixD covMET(2, 2);
            covMET[0][0] = (*e->Ev_METcov)[0];
            covMET[0][1] = (*e->Ev_METcov)[1];
            covMET[1][0] = (*e->Ev_METcov)[2];
            covMET[1][1] = (*e->Ev_METcov)[3];

            svFitStandalone::LorentzVector t((*e->TL_TauP)[idx]);
            svFitStandalone::LorentzVector l((*e->TL_LeptonP)[idx]);

            std::vector<svFitStandalone::MeasuredTauLepton> prods = {
                svFitStandalone::MeasuredTauLepton(svFitStandalone::kHadDecay, t),
                svFitStandalone::MeasuredTauLepton(svFitStandalone::kLepDecay, l)
            };

            SVfitStandaloneAlgorithm algo(prods, met, covMET, 2);
            algo.addLogM(true);
            algo.integrateVEGAS();
            double mass = algo.getMass();
            return algo.isValidSolution() ? mass : -1.0;
        };

        accessors["TT_DeltaR"] = [](roast::Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_DitauDeltaR)[idx];
        };
        accessors["TT_ChargeProduct"] = [](roast::Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1Charge)[idx] * (*e->TTL_Tau2Charge)[idx];
        };
        accessors["TT_VisMass"] = [](roast::Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_DitauVisibleMass)[idx];
        };
        accessors["TT_DeltaEta"] = [](roast::Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1Eta)[idx] - (*e->TTL_Tau2Eta)[idx];
        };
        accessors["TT_CosDeltaPhi"] = [](roast::Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_DitauCosDeltaPhi)[idx];
        };

        accessors["FinalBDTG"] = [](Branches *b, int idx, int n) -> float {
            return MVABase::gMVA["FinalBDTG"] ? MVABase::gMVA["FinalBDTG"]->Evaluate(b, idx) : 0.;
        };

        accessors["PassZMask3"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            auto dilmass = (*e->TLL_Lepton1Lepton2VisibleMass)[idx];
            auto met = e->Ev_MET;
            bool zpeak = dilmass > 82 && dilmass < 100 &&
                    met - 40 < 30 / 9. * (dilmass - 82) &&
                    met - 40 < 30 - 30 / 9. * (dilmass - 91);
            return !zpeak;
        };

        // >>> Begin attr <<<
        accessors["T1_AntiElectronIndex"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1AntiElectronIndex(idx);
        };
        accessors["T1_AntiElectronIndexMVA2"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1AntiElectronIndexMVA2(idx);
        };
        accessors["T1_AntiElectronIndexMVA3"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1AntiElectronIndexMVA3(idx);
        };
        accessors["T1_AntiMuonIndex"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1AntiMuonIndex(idx);
        };
        accessors["T1_AntiMuonIndex2"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1AntiMuonIndex2(idx);
        };
        accessors["T1_IsolationIndex"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1IsolationIndex(idx);
        };
        accessors["T1_IsolationIndex3Hits"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1IsolationIndex3Hits(idx);
        };
        accessors["T1_IsolationIndexMVA"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1IsolationIndexMVA(idx);
        };
        accessors["T1_IsolationIndexMVA2"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1IsolationIndexMVA2(idx);
        };
        accessors["T2_AntiElectronIndex"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2AntiElectronIndex(idx);
        };
        accessors["T2_AntiElectronIndexMVA2"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2AntiElectronIndexMVA2(idx);
        };
        accessors["T2_AntiElectronIndexMVA3"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2AntiElectronIndexMVA3(idx);
        };
        accessors["T2_AntiMuonIndex"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2AntiMuonIndex(idx);
        };
        accessors["T2_AntiMuonIndex2"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2AntiMuonIndex2(idx);
        };
        accessors["T2_IsolationIndex"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2IsolationIndex(idx);
        };
        accessors["T2_IsolationIndex3Hits"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2IsolationIndex3Hits(idx);
        };
        accessors["T2_IsolationIndexMVA"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2IsolationIndexMVA(idx);
        };
        accessors["T2_IsolationIndexMVA2"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2IsolationIndexMVA2(idx);
        };
        accessors["T_AntiElectronIndex"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return e->GetTauAntiElectronIndex(idx);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return e->GetTauAntiElectronIndex(idx);
            }
            throw "";
        };
        accessors["T_AntiElectronIndexMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return e->GetTauAntiElectronIndexMVA2(idx);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return e->GetTauAntiElectronIndexMVA2(idx);
            }
            throw "";
        };
        accessors["T_AntiElectronIndexMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return e->GetTauAntiElectronIndexMVA3(idx);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return e->GetTauAntiElectronIndexMVA3(idx);
            }
            throw "";
        };
        accessors["T_AntiMuonIndex"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return e->GetTauAntiMuonIndex(idx);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return e->GetTauAntiMuonIndex(idx);
            }
            throw "";
        };
        accessors["T_AntiMuonIndex2"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return e->GetTauAntiMuonIndex2(idx);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return e->GetTauAntiMuonIndex2(idx);
            }
            throw "";
        };
        accessors["T_IsolationIndex"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return e->GetTauIsolationIndex(idx);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return e->GetTauIsolationIndex(idx);
            }
            throw "";
        };
        accessors["T_IsolationIndex3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return e->GetTauIsolationIndex3Hits(idx);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return e->GetTauIsolationIndex3Hits(idx);
            }
            throw "";
        };
        accessors["T_IsolationIndexMVA"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return e->GetTauIsolationIndexMVA(idx);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return e->GetTauIsolationIndexMVA(idx);
            }
            throw "";
        };
        accessors["T_IsolationIndexMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return e->GetTauIsolationIndexMVA2(idx);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return e->GetTauIsolationIndexMVA2(idx);
            }
            throw "";
        };
        accessors["BQuarkCount"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_bQuarkCount;
        };
        accessors["CQuarkCount"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_cQuarkCount;
        };
        accessors["CSVeventWeight"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeight)[idx];
        };
        accessors["CSVeventWeightCErr1down"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightCErr1down)[idx];
        };
        accessors["CSVeventWeightCErr1up"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightCErr1up)[idx];
        };
        accessors["CSVeventWeightCErr2down"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightCErr2down)[idx];
        };
        accessors["CSVeventWeightCErr2up"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightCErr2up)[idx];
        };
        accessors["CSVeventWeightHFStats1down"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightHFStats1down)[idx];
        };
        accessors["CSVeventWeightHFStats1up"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightHFStats1up)[idx];
        };
        accessors["CSVeventWeightHFStats2down"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightHFStats2down)[idx];
        };
        accessors["CSVeventWeightHFStats2up"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightHFStats2up)[idx];
        };
        accessors["CSVeventWeightHFdown"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightHFdown)[idx];
        };
        accessors["CSVeventWeightHFup"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightHFup)[idx];
        };
        accessors["CSVeventWeightLFStats1down"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightLFStats1down)[idx];
        };
        accessors["CSVeventWeightLFStats1up"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightLFStats1up)[idx];
        };
        accessors["CSVeventWeightLFStats2down"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightLFStats2down)[idx];
        };
        accessors["CSVeventWeightLFStats2up"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightLFStats2up)[idx];
        };
        accessors["CSVeventWeightLFdown"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightLFdown)[idx];
        };
        accessors["CSVeventWeightLFup"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CSVeventWeightLFup)[idx];
        };
        accessors["CleanJetCSVMIndices"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CleanJetCSVMIndices)[idx][n];
        };
        accessors["CleanJetIndices"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CleanJetIndices)[idx][n];
        };
        accessors["CleanJetNonCSVMIndices"] = [](Branches *b, int idx, int n) -> float {
            return (*b->CleanJetNonCSVMIndices)[idx][n];
        };
        accessors["DitauCosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_DitauCosDeltaPhi)[idx];
            }
            throw;
        };
        accessors["DitauDeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_DitauDeltaR)[idx];
            }
            throw;
        };
        accessors["DitauMETMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_DitauMETMass)[idx];
            }
            throw;
        };
        accessors["DitauVisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_DitauVisibleMass)[idx];
            }
            throw;
        };
        accessors["E_NumElectrons"] = [](Branches *b, int idx, int n) -> float {
            return b->E_NumElectrons;
        };
        accessors["ElectronMomentumRank"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_ElectronMomentumRank)[idx];
            }
            throw;
        };
        accessors["EventNumber"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_eventNumber;
        };
        accessors["GJ_NumGenJets"] = [](Branches *b, int idx, int n) -> float {
            return b->GJ_NumGenJets;
        };
        accessors["GT_NumGenTaus"] = [](Branches *b, int idx, int n) -> float {
            return b->GT_NumGenTaus;
        };
        accessors["HT"] = [](Branches *b, int idx, int n) -> float {
            return (*b->HT)[idx];
        };
        accessors["HiggsDecayMode"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_higgsDecayMode;
        };
        accessors["IsTauEvent"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_IsTauEvent;
        };
        accessors["CleanJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_Charge).at(i);
        };
        accessors["CleanLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_Charge).at(i);
        };
        accessors["CleanSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_Charge).at(i);
        };
        accessors["CleanSubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_Charge).at(i);
        };
        accessors["CleanSubSubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_Charge).at(i);
        };
        accessors["TaggedJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_Charge).at(i);
        };
        accessors["TaggedLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_Charge).at(i);
        };
        accessors["TaggedSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_Charge).at(i);
        };
        accessors["TaggedSubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_Charge).at(i);
        };
        accessors["TaggedSubSubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_Charge).at(i);
        };
        accessors["UntaggedJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_Charge).at(i);
        };
        accessors["UntaggedLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_Charge).at(i);
        };
        accessors["UntaggedSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_Charge).at(i);
        };
        accessors["UntaggedSubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_Charge).at(i);
        };
        accessors["UntaggedSubSubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_Charge).at(i);
        };
        accessors["J_Charge"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_Charge)[idx];
        };
        accessors["CleanJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_combSecVtxBTag).at(i);
        };
        accessors["CleanLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_combSecVtxBTag).at(i);
        };
        accessors["CleanSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_combSecVtxBTag).at(i);
        };
        accessors["CleanSubSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_combSecVtxBTag).at(i);
        };
        accessors["CleanSubSubSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_combSecVtxBTag).at(i);
        };
        accessors["TaggedJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_combSecVtxBTag).at(i);
        };
        accessors["TaggedLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_combSecVtxBTag).at(i);
        };
        accessors["TaggedSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_combSecVtxBTag).at(i);
        };
        accessors["TaggedSubSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_combSecVtxBTag).at(i);
        };
        accessors["TaggedSubSubSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_combSecVtxBTag).at(i);
        };
        accessors["UntaggedJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_combSecVtxBTag).at(i);
        };
        accessors["UntaggedLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_combSecVtxBTag).at(i);
        };
        accessors["UntaggedSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_combSecVtxBTag).at(i);
        };
        accessors["UntaggedSubSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_combSecVtxBTag).at(i);
        };
        accessors["UntaggedSubSubSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_combSecVtxBTag).at(i);
        };
        accessors["J_CombSecVtxBTag"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_combSecVtxBTag)[idx];
        };
        accessors["CleanJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_Eta).at(i);
        };
        accessors["CleanLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_Eta).at(i);
        };
        accessors["CleanSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_Eta).at(i);
        };
        accessors["CleanSubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_Eta).at(i);
        };
        accessors["CleanSubSubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_Eta).at(i);
        };
        accessors["TaggedJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_Eta).at(i);
        };
        accessors["TaggedLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_Eta).at(i);
        };
        accessors["TaggedSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_Eta).at(i);
        };
        accessors["TaggedSubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_Eta).at(i);
        };
        accessors["TaggedSubSubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_Eta).at(i);
        };
        accessors["UntaggedJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_Eta).at(i);
        };
        accessors["UntaggedLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_Eta).at(i);
        };
        accessors["UntaggedSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_Eta).at(i);
        };
        accessors["UntaggedSubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_Eta).at(i);
        };
        accessors["UntaggedSubSubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_Eta).at(i);
        };
        accessors["J_Eta"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_Eta)[idx];
        };
        accessors["CleanJL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["CleanLJL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["CleanSubLJL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["CleanSubSubLJL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["CleanSubSubSubLJL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["TaggedJL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["TaggedLJL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["TaggedSubLJL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["TaggedSubSubLJL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["TaggedSubSubSubLJL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["UntaggedJL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["UntaggedLJL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["UntaggedSubLJL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["UntaggedSubSubLJL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["UntaggedSubSubSubLJL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["JL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton1DeltaR)[idx][n];
            }
            throw "";
        };
        accessors["CleanJL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["CleanLJL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["CleanSubLJL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["CleanSubSubLJL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["CleanSubSubSubLJL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["TaggedJL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["TaggedLJL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["TaggedSubLJL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["TaggedSubSubLJL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["TaggedSubSubSubLJL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["UntaggedJL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["UntaggedLJL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["UntaggedSubLJL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["UntaggedSubSubLJL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["UntaggedSubSubSubLJL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(n);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR).at(i).at(n);
            }
            throw;
        };
        accessors["JL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_Lepton2DeltaR)[idx][n];
            }
            throw "";
        };
        accessors["CleanJL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_LeptonDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["CleanLJL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_LeptonDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["CleanSubLJL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_LeptonDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["CleanSubSubLJL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_LeptonDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["CleanSubSubSubLJL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_LeptonDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["TaggedJL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_LeptonDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["TaggedLJL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_LeptonDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["TaggedSubLJL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_LeptonDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["TaggedSubSubLJL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_LeptonDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["TaggedSubSubSubLJL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_LeptonDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["UntaggedJL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_LeptonDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["UntaggedLJL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_LeptonDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["UntaggedSubLJL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_LeptonDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["UntaggedSubSubLJL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_LeptonDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["UntaggedSubSubSubLJL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_LeptonDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["CleanJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_MomentumRank).at(i);
        };
        accessors["CleanLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_MomentumRank).at(i);
        };
        accessors["CleanSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_MomentumRank).at(i);
        };
        accessors["CleanSubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_MomentumRank).at(i);
        };
        accessors["CleanSubSubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_MomentumRank).at(i);
        };
        accessors["TaggedJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_MomentumRank).at(i);
        };
        accessors["TaggedLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_MomentumRank).at(i);
        };
        accessors["TaggedSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_MomentumRank).at(i);
        };
        accessors["TaggedSubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_MomentumRank).at(i);
        };
        accessors["TaggedSubSubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_MomentumRank).at(i);
        };
        accessors["UntaggedJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_MomentumRank).at(i);
        };
        accessors["UntaggedLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_MomentumRank).at(i);
        };
        accessors["UntaggedSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_MomentumRank).at(i);
        };
        accessors["UntaggedSubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_MomentumRank).at(i);
        };
        accessors["UntaggedSubSubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_MomentumRank).at(i);
        };
        accessors["J_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_MomentumRank)[idx];
        };
        accessors["J_NumJets"] = [](Branches *b, int idx, int n) -> float {
            return b->J_NumJets;
        };
        accessors["CleanJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonGrandParentId).at(i);
        };
        accessors["CleanLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonGrandParentId).at(i);
        };
        accessors["CleanSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonGrandParentId).at(i);
        };
        accessors["CleanSubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonGrandParentId).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonGrandParentId).at(i);
        };
        accessors["TaggedJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonGrandParentId).at(i);
        };
        accessors["TaggedLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonGrandParentId).at(i);
        };
        accessors["TaggedSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonGrandParentId).at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonGrandParentId).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonGrandParentId).at(i);
        };
        accessors["UntaggedJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonGrandParentId).at(i);
        };
        accessors["UntaggedLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonGrandParentId).at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonGrandParentId).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonGrandParentId).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonGrandParentId).at(i);
        };
        accessors["J_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandParentId)[idx];
        };
        accessors["CleanJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonGrandmother00Id).at(i);
        };
        accessors["CleanLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonGrandmother00Id).at(i);
        };
        accessors["CleanSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonGrandmother00Id).at(i);
        };
        accessors["CleanSubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonGrandmother00Id).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonGrandmother00Id).at(i);
        };
        accessors["TaggedJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonGrandmother00Id).at(i);
        };
        accessors["TaggedLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonGrandmother00Id).at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonGrandmother00Id).at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonGrandmother00Id).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonGrandmother00Id).at(i);
        };
        accessors["UntaggedJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonGrandmother00Id).at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonGrandmother00Id).at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonGrandmother00Id).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonGrandmother00Id).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonGrandmother00Id).at(i);
        };
        accessors["J_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Id)[idx];
        };
        accessors["CleanJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonGrandmother00Status).at(i);
        };
        accessors["CleanLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonGrandmother00Status).at(i);
        };
        accessors["CleanSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonGrandmother00Status).at(i);
        };
        accessors["CleanSubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonGrandmother00Status).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonGrandmother00Status).at(i);
        };
        accessors["TaggedJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonGrandmother00Status).at(i);
        };
        accessors["TaggedLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonGrandmother00Status).at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonGrandmother00Status).at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonGrandmother00Status).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonGrandmother00Status).at(i);
        };
        accessors["UntaggedJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonGrandmother00Status).at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonGrandmother00Status).at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonGrandmother00Status).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonGrandmother00Status).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonGrandmother00Status).at(i);
        };
        accessors["J_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Status)[idx];
        };
        accessors["CleanJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonGrandmother01Id).at(i);
        };
        accessors["CleanLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonGrandmother01Id).at(i);
        };
        accessors["CleanSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonGrandmother01Id).at(i);
        };
        accessors["CleanSubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonGrandmother01Id).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonGrandmother01Id).at(i);
        };
        accessors["TaggedJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonGrandmother01Id).at(i);
        };
        accessors["TaggedLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonGrandmother01Id).at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonGrandmother01Id).at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonGrandmother01Id).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonGrandmother01Id).at(i);
        };
        accessors["UntaggedJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonGrandmother01Id).at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonGrandmother01Id).at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonGrandmother01Id).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonGrandmother01Id).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonGrandmother01Id).at(i);
        };
        accessors["J_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Id)[idx];
        };
        accessors["CleanJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonGrandmother01Status).at(i);
        };
        accessors["CleanLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonGrandmother01Status).at(i);
        };
        accessors["CleanSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonGrandmother01Status).at(i);
        };
        accessors["CleanSubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonGrandmother01Status).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonGrandmother01Status).at(i);
        };
        accessors["TaggedJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonGrandmother01Status).at(i);
        };
        accessors["TaggedLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonGrandmother01Status).at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonGrandmother01Status).at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonGrandmother01Status).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonGrandmother01Status).at(i);
        };
        accessors["UntaggedJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonGrandmother01Status).at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonGrandmother01Status).at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonGrandmother01Status).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonGrandmother01Status).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonGrandmother01Status).at(i);
        };
        accessors["J_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Status)[idx];
        };
        accessors["CleanJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonGrandmother10Id).at(i);
        };
        accessors["CleanLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonGrandmother10Id).at(i);
        };
        accessors["CleanSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonGrandmother10Id).at(i);
        };
        accessors["CleanSubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonGrandmother10Id).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonGrandmother10Id).at(i);
        };
        accessors["TaggedJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonGrandmother10Id).at(i);
        };
        accessors["TaggedLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonGrandmother10Id).at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonGrandmother10Id).at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonGrandmother10Id).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonGrandmother10Id).at(i);
        };
        accessors["UntaggedJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonGrandmother10Id).at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonGrandmother10Id).at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonGrandmother10Id).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonGrandmother10Id).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonGrandmother10Id).at(i);
        };
        accessors["J_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Id)[idx];
        };
        accessors["CleanJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonGrandmother10Status).at(i);
        };
        accessors["CleanLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonGrandmother10Status).at(i);
        };
        accessors["CleanSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonGrandmother10Status).at(i);
        };
        accessors["CleanSubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonGrandmother10Status).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonGrandmother10Status).at(i);
        };
        accessors["TaggedJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonGrandmother10Status).at(i);
        };
        accessors["TaggedLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonGrandmother10Status).at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonGrandmother10Status).at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonGrandmother10Status).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonGrandmother10Status).at(i);
        };
        accessors["UntaggedJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonGrandmother10Status).at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonGrandmother10Status).at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonGrandmother10Status).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonGrandmother10Status).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonGrandmother10Status).at(i);
        };
        accessors["J_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Status)[idx];
        };
        accessors["CleanJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonGrandmother11Id).at(i);
        };
        accessors["CleanLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonGrandmother11Id).at(i);
        };
        accessors["CleanSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonGrandmother11Id).at(i);
        };
        accessors["CleanSubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonGrandmother11Id).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonGrandmother11Id).at(i);
        };
        accessors["TaggedJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonGrandmother11Id).at(i);
        };
        accessors["TaggedLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonGrandmother11Id).at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonGrandmother11Id).at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonGrandmother11Id).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonGrandmother11Id).at(i);
        };
        accessors["UntaggedJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonGrandmother11Id).at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonGrandmother11Id).at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonGrandmother11Id).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonGrandmother11Id).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonGrandmother11Id).at(i);
        };
        accessors["J_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Id)[idx];
        };
        accessors["CleanJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonGrandmother11Status).at(i);
        };
        accessors["CleanLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonGrandmother11Status).at(i);
        };
        accessors["CleanSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonGrandmother11Status).at(i);
        };
        accessors["CleanSubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonGrandmother11Status).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonGrandmother11Status).at(i);
        };
        accessors["TaggedJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonGrandmother11Status).at(i);
        };
        accessors["TaggedLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonGrandmother11Status).at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonGrandmother11Status).at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonGrandmother11Status).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonGrandmother11Status).at(i);
        };
        accessors["UntaggedJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonGrandmother11Status).at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonGrandmother11Status).at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonGrandmother11Status).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonGrandmother11Status).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonGrandmother11Status).at(i);
        };
        accessors["J_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Status)[idx];
        };
        accessors["CleanJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonId).at(i);
        };
        accessors["CleanLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonId).at(i);
        };
        accessors["CleanSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonId).at(i);
        };
        accessors["CleanSubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonId).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonId).at(i);
        };
        accessors["TaggedJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonId).at(i);
        };
        accessors["TaggedLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonId).at(i);
        };
        accessors["TaggedSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonId).at(i);
        };
        accessors["TaggedSubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonId).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonId).at(i);
        };
        accessors["UntaggedJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonId).at(i);
        };
        accessors["UntaggedLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonId).at(i);
        };
        accessors["UntaggedSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonId).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonId).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonId).at(i);
        };
        accessors["J_PartonId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonId)[idx];
        };
        accessors["CleanJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonMother0Id).at(i);
        };
        accessors["CleanLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonMother0Id).at(i);
        };
        accessors["CleanSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonMother0Id).at(i);
        };
        accessors["CleanSubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonMother0Id).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonMother0Id).at(i);
        };
        accessors["TaggedJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonMother0Id).at(i);
        };
        accessors["TaggedLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonMother0Id).at(i);
        };
        accessors["TaggedSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonMother0Id).at(i);
        };
        accessors["TaggedSubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonMother0Id).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonMother0Id).at(i);
        };
        accessors["UntaggedJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonMother0Id).at(i);
        };
        accessors["UntaggedLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonMother0Id).at(i);
        };
        accessors["UntaggedSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonMother0Id).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonMother0Id).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonMother0Id).at(i);
        };
        accessors["J_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Id)[idx];
        };
        accessors["CleanJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonMother0Status).at(i);
        };
        accessors["CleanLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonMother0Status).at(i);
        };
        accessors["CleanSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonMother0Status).at(i);
        };
        accessors["CleanSubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonMother0Status).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonMother0Status).at(i);
        };
        accessors["TaggedJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonMother0Status).at(i);
        };
        accessors["TaggedLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonMother0Status).at(i);
        };
        accessors["TaggedSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonMother0Status).at(i);
        };
        accessors["TaggedSubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonMother0Status).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonMother0Status).at(i);
        };
        accessors["UntaggedJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonMother0Status).at(i);
        };
        accessors["UntaggedLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonMother0Status).at(i);
        };
        accessors["UntaggedSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonMother0Status).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonMother0Status).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonMother0Status).at(i);
        };
        accessors["J_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Status)[idx];
        };
        accessors["CleanJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonMother1Id).at(i);
        };
        accessors["CleanLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonMother1Id).at(i);
        };
        accessors["CleanSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonMother1Id).at(i);
        };
        accessors["CleanSubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonMother1Id).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonMother1Id).at(i);
        };
        accessors["TaggedJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonMother1Id).at(i);
        };
        accessors["TaggedLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonMother1Id).at(i);
        };
        accessors["TaggedSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonMother1Id).at(i);
        };
        accessors["TaggedSubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonMother1Id).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonMother1Id).at(i);
        };
        accessors["UntaggedJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonMother1Id).at(i);
        };
        accessors["UntaggedLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonMother1Id).at(i);
        };
        accessors["UntaggedSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonMother1Id).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonMother1Id).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonMother1Id).at(i);
        };
        accessors["J_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Id)[idx];
        };
        accessors["CleanJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonMother1Status).at(i);
        };
        accessors["CleanLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonMother1Status).at(i);
        };
        accessors["CleanSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonMother1Status).at(i);
        };
        accessors["CleanSubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonMother1Status).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonMother1Status).at(i);
        };
        accessors["TaggedJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonMother1Status).at(i);
        };
        accessors["TaggedLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonMother1Status).at(i);
        };
        accessors["TaggedSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonMother1Status).at(i);
        };
        accessors["TaggedSubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonMother1Status).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonMother1Status).at(i);
        };
        accessors["UntaggedJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonMother1Status).at(i);
        };
        accessors["UntaggedLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonMother1Status).at(i);
        };
        accessors["UntaggedSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonMother1Status).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonMother1Status).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonMother1Status).at(i);
        };
        accessors["J_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Status)[idx];
        };
        accessors["CleanJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonParentId).at(i);
        };
        accessors["CleanLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonParentId).at(i);
        };
        accessors["CleanSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonParentId).at(i);
        };
        accessors["CleanSubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonParentId).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonParentId).at(i);
        };
        accessors["TaggedJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonParentId).at(i);
        };
        accessors["TaggedLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonParentId).at(i);
        };
        accessors["TaggedSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonParentId).at(i);
        };
        accessors["TaggedSubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonParentId).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonParentId).at(i);
        };
        accessors["UntaggedJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonParentId).at(i);
        };
        accessors["UntaggedLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonParentId).at(i);
        };
        accessors["UntaggedSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonParentId).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonParentId).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonParentId).at(i);
        };
        accessors["J_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonParentId)[idx];
        };
        accessors["CleanJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_PartonStatus).at(i);
        };
        accessors["CleanLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_PartonStatus).at(i);
        };
        accessors["CleanSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_PartonStatus).at(i);
        };
        accessors["CleanSubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_PartonStatus).at(i);
        };
        accessors["CleanSubSubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_PartonStatus).at(i);
        };
        accessors["TaggedJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_PartonStatus).at(i);
        };
        accessors["TaggedLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_PartonStatus).at(i);
        };
        accessors["TaggedSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_PartonStatus).at(i);
        };
        accessors["TaggedSubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_PartonStatus).at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_PartonStatus).at(i);
        };
        accessors["UntaggedJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_PartonStatus).at(i);
        };
        accessors["UntaggedLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_PartonStatus).at(i);
        };
        accessors["UntaggedSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_PartonStatus).at(i);
        };
        accessors["UntaggedSubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_PartonStatus).at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_PartonStatus).at(i);
        };
        accessors["J_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonStatus)[idx];
        };
        accessors["CleanJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_Phi).at(i);
        };
        accessors["CleanLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_Phi).at(i);
        };
        accessors["CleanSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_Phi).at(i);
        };
        accessors["CleanSubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_Phi).at(i);
        };
        accessors["CleanSubSubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_Phi).at(i);
        };
        accessors["TaggedJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_Phi).at(i);
        };
        accessors["TaggedLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_Phi).at(i);
        };
        accessors["TaggedSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_Phi).at(i);
        };
        accessors["TaggedSubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_Phi).at(i);
        };
        accessors["TaggedSubSubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_Phi).at(i);
        };
        accessors["UntaggedJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_Phi).at(i);
        };
        accessors["UntaggedLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_Phi).at(i);
        };
        accessors["UntaggedSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_Phi).at(i);
        };
        accessors["UntaggedSubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_Phi).at(i);
        };
        accessors["UntaggedSubSubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_Phi).at(i);
        };
        accessors["J_Phi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_Phi)[idx];
        };
        accessors["CleanJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            return (*b->J_Pt).at(i);
        };
        accessors["CleanLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            return (*b->J_Pt).at(i);
        };
        accessors["CleanSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            return (*b->J_Pt).at(i);
        };
        accessors["CleanSubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            return (*b->J_Pt).at(i);
        };
        accessors["CleanSubSubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            return (*b->J_Pt).at(i);
        };
        accessors["TaggedJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            return (*b->J_Pt).at(i);
        };
        accessors["TaggedLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            return (*b->J_Pt).at(i);
        };
        accessors["TaggedSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            return (*b->J_Pt).at(i);
        };
        accessors["TaggedSubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            return (*b->J_Pt).at(i);
        };
        accessors["TaggedSubSubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            return (*b->J_Pt).at(i);
        };
        accessors["UntaggedJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            return (*b->J_Pt).at(i);
        };
        accessors["UntaggedLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            return (*b->J_Pt).at(i);
        };
        accessors["UntaggedSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            return (*b->J_Pt).at(i);
        };
        accessors["UntaggedSubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            return (*b->J_Pt).at(i);
        };
        accessors["UntaggedSubSubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            return (*b->J_Pt).at(i);
        };
        accessors["J_Pt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_Pt)[idx];
        };
        accessors["CleanJT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][n];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["CleanLJT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][0];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["CleanSubLJT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][1];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["CleanSubSubLJT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][2];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["CleanSubSubSubLJT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetIndices)[idx][3];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["TaggedJT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][n];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["TaggedLJT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][0];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["TaggedSubLJT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][1];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["TaggedSubSubLJT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][2];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["TaggedSubSubSubLJT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetCSVMIndices)[idx][3];
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["UntaggedJT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(n);
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["UntaggedLJT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(0);
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["UntaggedSubLJT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(1);
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["UntaggedSubSubLJT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(2);
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["UntaggedSubSubSubLJT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = (*b->CleanJetNonCSVMIndices)[idx].at(3);
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR).at(idx).at(i);
            }
            throw;
        };
        accessors["JT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->J_TauDeltaR)[idx][n];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->J_TauDeltaR)[idx][n];
            }
            throw "";
        };
        accessors["L1L2_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1Lepton2CosDeltaPhi)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1Lepton2CosDeltaPhi)[idx];
            }
            throw "";
        };
        accessors["L1L2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1Lepton2DeltaR)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1Lepton2DeltaR)[idx];
            }
            throw "";
        };
        accessors["L1L2_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1Lepton2VisibleMass)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1Lepton2VisibleMass)[idx];
            }
            throw "";
        };
        accessors["L1_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1Charge)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1Charge)[idx];
            }
            throw "";
        };
        accessors["L1_CorrectedD0"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1CorrectedD0)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1CorrectedD0)[idx];
            }
            throw "";
        };
        accessors["L1_CorrectedDZ"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1CorrectedDZ)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1CorrectedDZ)[idx];
            }
            throw "";
        };
        accessors["L1_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1Eta)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1Eta)[idx];
            }
            throw "";
        };
        accessors["L1_EventWeight"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1EventWeight)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1EventWeight)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchDaughter0Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchDaughter0Id)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchDaughter0Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchDaughter0Status)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchDaughter1Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchDaughter1Id)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchDaughter1Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchDaughter1Status)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchGrandmother00Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchGrandmother00Id)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchGrandmother00Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchGrandmother00Status)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchGrandmother01Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchGrandmother01Id)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchGrandmother01Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchGrandmother01Status)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchGrandmother10Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchGrandmother10Id)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchGrandmother10Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchGrandmother10Status)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchGrandmother11Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchGrandmother11Id)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchGrandmother11Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchGrandmother11Status)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchId)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchId)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchMother0Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchMother0Id)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchMother0Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchMother0Status)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchMother1Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchMother1Id)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchMother1Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchMother1Status)[idx];
            }
            throw "";
        };
        accessors["L1_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1GenMatchStatus)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1GenMatchStatus)[idx];
            }
            throw "";
        };
        accessors["L1_ImpactParameter"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1ImpactParameter)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1ImpactParameter)[idx];
            }
            throw "";
        };
        accessors["L1_IsElectron"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1IsElectron)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1IsElectron)[idx];
            }
            throw "";
        };
        accessors["L1_IsLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1IsLoose)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1IsLoose)[idx];
            }
            throw "";
        };
        accessors["L1_IsMuon"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1IsMuon)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1IsMuon)[idx];
            }
            throw "";
        };
        accessors["L1_IsTight"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1IsTight)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1IsTight)[idx];
            }
            throw "";
        };
        accessors["L1_Mt"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1Mt)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1Mt)[idx];
            }
            throw "";
        };
        accessors["L1_Mt2"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1Mt2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1Mt2)[idx];
            }
            throw "";
        };
        accessors["L1_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1Phi)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1Phi)[idx];
            }
            throw "";
        };
        accessors["L1_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1Pt)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1Pt)[idx];
            }
            throw "";
        };
        accessors["L1_RelIso"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton1RelIso)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton1RelIso)[idx];
            }
            throw "";
        };
        accessors["L2_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2Charge)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2Charge)[idx];
            }
            throw "";
        };
        accessors["L2_CorrectedD0"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2CorrectedD0)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2CorrectedD0)[idx];
            }
            throw "";
        };
        accessors["L2_CorrectedDZ"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2CorrectedDZ)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2CorrectedDZ)[idx];
            }
            throw "";
        };
        accessors["L2_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2Eta)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2Eta)[idx];
            }
            throw "";
        };
        accessors["L2_EventWeight"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2EventWeight)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2EventWeight)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchDaughter0Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchDaughter0Id)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchDaughter0Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchDaughter0Status)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchDaughter1Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchDaughter1Id)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchDaughter1Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchDaughter1Status)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchGrandmother00Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchGrandmother00Id)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchGrandmother00Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchGrandmother00Status)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchGrandmother01Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchGrandmother01Id)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchGrandmother01Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchGrandmother01Status)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchGrandmother10Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchGrandmother10Id)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchGrandmother10Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchGrandmother10Status)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchGrandmother11Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchGrandmother11Id)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchGrandmother11Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchGrandmother11Status)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchId)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchId)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchMother0Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchMother0Id)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchMother0Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchMother0Status)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchMother1Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchMother1Id)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchMother1Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchMother1Status)[idx];
            }
            throw "";
        };
        accessors["L2_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2GenMatchStatus)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2GenMatchStatus)[idx];
            }
            throw "";
        };
        accessors["L2_ImpactParameter"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2ImpactParameter)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2ImpactParameter)[idx];
            }
            throw "";
        };
        accessors["L2_IsElectron"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2IsElectron)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2IsElectron)[idx];
            }
            throw "";
        };
        accessors["L2_IsLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2IsLoose)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2IsLoose)[idx];
            }
            throw "";
        };
        accessors["L2_IsMuon"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2IsMuon)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2IsMuon)[idx];
            }
            throw "";
        };
        accessors["L2_IsTight"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2IsTight)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2IsTight)[idx];
            }
            throw "";
        };
        accessors["L2_Mt"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2Mt)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2Mt)[idx];
            }
            throw "";
        };
        accessors["L2_Mt2"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2Mt2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2Mt2)[idx];
            }
            throw "";
        };
        accessors["L2_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2Phi)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2Phi)[idx];
            }
            throw "";
        };
        accessors["L2_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2Pt)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2Pt)[idx];
            }
            throw "";
        };
        accessors["L2_RelIso"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_Lepton2RelIso)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_Lepton2RelIso)[idx];
            }
            throw "";
        };
        accessors["L_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonCharge)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonCharge)[idx];
            }
            throw "";
        };
        accessors["L_CorrectedD0"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonCorrectedD0)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonCorrectedD0)[idx];
            }
            throw "";
        };
        accessors["L_CorrectedDZ"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonCorrectedDZ)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonCorrectedDZ)[idx];
            }
            throw "";
        };
        accessors["L_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonEta)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonEta)[idx];
            }
            throw "";
        };
        accessors["L_EventWeight"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonEventWeight)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonEventWeight)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchDaughter0Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchDaughter0Id)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchDaughter0Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchDaughter0Status)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchDaughter1Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchDaughter1Id)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchDaughter1Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchDaughter1Status)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchGrandmother00Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchGrandmother00Id)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchGrandmother00Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchGrandmother00Status)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchGrandmother01Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchGrandmother01Id)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchGrandmother01Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchGrandmother01Status)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchGrandmother10Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchGrandmother10Id)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchGrandmother10Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchGrandmother10Status)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchGrandmother11Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchGrandmother11Id)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchGrandmother11Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchGrandmother11Status)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchId)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchId)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchMother0Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchMother0Id)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchMother0Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchMother0Status)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchMother1Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchMother1Id)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchMother1Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchMother1Status)[idx];
            }
            throw "";
        };
        accessors["L_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonGenMatchStatus)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonGenMatchStatus)[idx];
            }
            throw "";
        };
        accessors["L_ImpactParameter"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonImpactParameter)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonImpactParameter)[idx];
            }
            throw "";
        };
        accessors["L_IsElectron"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonIsElectron)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonIsElectron)[idx];
            }
            throw "";
        };
        accessors["L_IsLoose"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonIsLoose)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonIsLoose)[idx];
            }
            throw "";
        };
        accessors["L_IsMuon"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonIsMuon)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonIsMuon)[idx];
            }
            throw "";
        };
        accessors["L_IsTight"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonIsTight)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonIsTight)[idx];
            }
            throw "";
        };
        accessors["L_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonMomentumRank)[idx];
            }
            throw;
        };
        accessors["L_Mt"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonMt)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonMt)[idx];
            }
            throw "";
        };
        accessors["L_Mt2"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonMt2)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonMt2)[idx];
            }
            throw "";
        };
        accessors["L_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonPhi)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonPhi)[idx];
            }
            throw "";
        };
        accessors["L_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonPt)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonPt)[idx];
            }
            throw "";
        };
        accessors["L_RelIso"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_LeptonRelIso)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_LeptonRelIso)[idx];
            }
            throw "";
        };
        accessors["LumiBlock"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_lumiBlock;
        };
        accessors["MET"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_MET;
        };
        accessors["METcov"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->Ev_METcov)[idx];
            } else if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->Ev_METcov)[idx];
            }
            throw "";
        };
        accessors["METphi"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_METphi;
        };
        accessors["MHT"] = [](Branches *b, int idx, int n) -> float {
            return (*b->MHT)[idx];
        };
        accessors["M_NumMuons"] = [](Branches *b, int idx, int n) -> float {
            return b->M_NumMuons;
        };
        accessors["MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            return (*b->MomentumRank)[idx];
        };
        accessors["MuonMomentumRank"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_MuonMomentumRank)[idx];
            }
            throw;
        };
        accessors["NTruePV"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return e->Ev_NTruePV;
            } else if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return e->Ev_NTruePV;
            }
            throw "";
        };
        accessors["J_NumCSVL"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumCSVLbtagJets)[idx];
        };
        accessors["J_NumCSVM"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumCSVMbtagJets)[idx];
        };
        accessors["J_NumCSVT"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumCSVTbtagJets)[idx];
        };
        accessors["J_NumCleanCSVL"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumCleanCSVLbtagJets)[idx];
        };
        accessors["J_NumCleanCSVM"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumCleanCSVMbtagJets)[idx];
        };
        accessors["J_NumCleanCSVT"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumCleanCSVTbtagJets)[idx];
        };
        accessors["J_NumCleanNonCSVL"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumCleanNonCSVLbtagJets)[idx];
        };
        accessors["J_NumCleanNonCSVM"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumCleanNonCSVMbtagJets)[idx];
        };
        accessors["J_NumCleanNonCSVT"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumCleanNonCSVTbtagJets)[idx];
        };
        accessors["NumCombos"] = [](Branches *b, int idx, int n) -> float {
            return b->NumCombos;
        };
        accessors["NumElectrons"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return e->TTL_NumElectrons;
            }
            throw;
        };
        accessors["NumExLooseElectrons"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumExLooseElectrons)[idx];
        };
        accessors["NumExLooseMuons"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumExLooseMuons)[idx];
        };
        accessors["NumLeptons"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return e->TTL_NumLeptons;
            }
            throw;
        };
        accessors["NumLooseElectrons"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumLooseElectrons)[idx];
        };
        accessors["NumLooseMuons"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumLooseMuons)[idx];
        };
        accessors["NumMuons"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return e->TTL_NumMuons;
            }
            throw;
        };
        accessors["J_NumNonCSVL"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumNonCSVLbtagJets)[idx];
        };
        accessors["J_NumNonCSVM"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumNonCSVMbtagJets)[idx];
        };
        accessors["J_NumNonCSVT"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumNonCSVTbtagJets)[idx];
        };
        accessors["NumTaus"] = [](Branches *b, int idx, int n) -> float {
            return b->NumTaus;
        };
        accessors["NumTightElectrons"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumTightElectrons)[idx];
        };
        accessors["NumTightMuons"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumTightMuons)[idx];
        };
        accessors["PassZMask"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_PassZMask)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_PassZMask)[idx];
            }
            throw "";
        };
        accessors["PassZMask2"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_PassZMask2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_PassZMask2)[idx];
            }
            throw "";
        };
        accessors["PuWeight"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_puWeight;
        };
        accessors["PuWeight2012A"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_puWeight2012A;
        };
        accessors["PuWeight2012AB"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_puWeight2012AB;
        };
        accessors["PuWeight2012ABC"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_puWeight2012ABC;
        };
        accessors["PuWeight2012B"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_puWeight2012B;
        };
        accessors["PuWeight2012BC"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_puWeight2012BC;
        };
        accessors["PuWeight2012C"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_puWeight2012C;
        };
        accessors["PuWeight2012CD"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_puWeight2012CD;
        };
        accessors["PuWeight2012D"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_puWeight2012D;
        };
        accessors["PuWeightDown"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_puWeightDown;
        };
        accessors["PuWeightUp"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_puWeightUp;
        };
        accessors["Q2WeightDown"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_q2WeightDown;
        };
        accessors["Q2WeightUp"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_q2WeightUp;
        };
        accessors["RunNumber"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_runNumber;
        };
        accessors["T1L_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1LeptonDeltaR)[idx];
            }
            throw;
        };
        accessors["T1_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1Charge)[idx];
            }
            throw;
        };
        accessors["T1_DecayMode"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1DecayMode)[idx];
            }
            throw;
        };
        accessors["T1_EmFraction"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1EmFraction)[idx];
            }
            throw;
        };
        accessors["T1_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1Eta)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchDaughter0Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchDaughter0Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchDaughter1Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchDaughter1Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchGrandmother00Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchGrandmother00Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchGrandmother01Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchGrandmother01Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchGrandmother10Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchGrandmother10Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchGrandmother11Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchGrandmother11Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchId)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchMother0Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchMother0Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchMother1Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchMother1Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1GenMatchStatus)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronDeadECAL"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronDeadECAL)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronLoose)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronLooseMVA2)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronLooseMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronLooseMVA3)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronMVA)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMVA2category"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronMVA2category)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronMVA2raw)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMVA3category"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronMVA3category)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMVA3Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronMVA3raw)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMedium"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronMedium)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMediumMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronMediumMVA2)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMediumMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronMediumMVA3)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronTight"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronTight)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronTightMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronTightMVA2)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronTightMVA3)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronVLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronVLooseMVA2)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronVTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstElectronVTightMVA3)[idx];
            }
            throw;
        };
        accessors["T1_AntiMuonLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstMuonLoose)[idx];
            }
            throw;
        };
        accessors["T1_AntiMuonLoose2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstMuonLoose2)[idx];
            }
            throw;
        };
        accessors["T1_AntiMuonMedium"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstMuonMedium)[idx];
            }
            throw;
        };
        accessors["T1_AntiMuonMedium2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstMuonMedium2)[idx];
            }
            throw;
        };
        accessors["T1_AntiMuonTight"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstMuonTight)[idx];
            }
            throw;
        };
        accessors["T1_AntiMuonTight2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSagainstMuonTight2)[idx];
            }
            throw;
        };
        accessors["T1_CombinedIsolationDeltaBetaCorrRaw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyCombinedIsolationDeltaBetaCorrRaw)[idx];
            }
            throw;
        };
        accessors["T1_CombinedIsolationDeltaBetaCorrRaw3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx];
            }
            throw;
        };
        accessors["T1_IsolationMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyIsolationMVA2raw)[idx];
            }
            throw;
        };
        accessors["T1_IsolationMVARaw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyIsolationMVAraw)[idx];
            }
            throw;
        };
        accessors["T1_LooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyLooseCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T1_LooseCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T1_LooseIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyLooseIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T1_LooseIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyLooseIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T1_MediumCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyMediumCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T1_MediumCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T1_MediumIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyMediumIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T1_MediumIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyMediumIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T1_TightCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyTightCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T1_TightCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T1_TightIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyTightIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T1_TightIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyTightIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T1_VLooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSbyVLooseCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T1_DecayModeFinding"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1HPSdecayModeFinding)[idx];
            }
            throw;
        };
        accessors["T1_IsInTheCracks"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1IsInTheCracks)[idx];
            }
            throw;
        };
        accessors["T1_LTIpVtdxy"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1LTIpVtdxy)[idx];
            }
            throw;
        };
        accessors["T1_LTIpVtdxyError"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1LTIpVtdxyError)[idx];
            }
            throw;
        };
        accessors["T1_LTIpVtdz"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1LTIpVtdz)[idx];
            }
            throw;
        };
        accessors["T1_LTIpVtdzError"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1LTIpVtdzError)[idx];
            }
            throw;
        };
        accessors["T1_LTNormChiSqrd"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1LTNormChiSqrd)[idx];
            }
            throw;
        };
        accessors["T1_LTPt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1LTPt)[idx];
            }
            throw;
        };
        accessors["T1_LTValidHits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1LTValidHits)[idx];
            }
            throw;
        };
        accessors["T1_LTvalid"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1LTvalid)[idx];
            }
            throw;
        };
        accessors["T1_LTvx"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1LTvx)[idx];
            }
            throw;
        };
        accessors["T1_LTvy"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1LTvy)[idx];
            }
            throw;
        };
        accessors["T1_LTvz"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1LTvz)[idx];
            }
            throw;
        };
        accessors["T1_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1MomentumRank)[idx];
            }
            throw;
        };
        accessors["T1_NumProngs"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1NProngs)[idx];
            }
            throw;
        };
        accessors["T1_NumSignalGammas"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1NSignalGammas)[idx];
            }
            throw;
        };
        accessors["T1_NumSignalNeutrals"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1NSignalNeutrals)[idx];
            }
            throw;
        };
        accessors["T1_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1Phi)[idx];
            }
            throw;
        };
        accessors["T1_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau1Pt)[idx];
            }
            throw;
        };
        accessors["T2L_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2LeptonDeltaR)[idx];
            }
            throw;
        };
        accessors["T2_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2Charge)[idx];
            }
            throw;
        };
        accessors["T2_DecayMode"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2DecayMode)[idx];
            }
            throw;
        };
        accessors["T2_EmFraction"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2EmFraction)[idx];
            }
            throw;
        };
        accessors["T2_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2Eta)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchDaughter0Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchDaughter0Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchDaughter1Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchDaughter1Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchGrandmother00Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchGrandmother00Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchGrandmother01Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchGrandmother01Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchGrandmother10Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchGrandmother10Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchGrandmother11Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchGrandmother11Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchId)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchMother0Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchMother0Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchMother1Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchMother1Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2GenMatchStatus)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronDeadECAL"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronDeadECAL)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronLoose)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronLooseMVA2)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronLooseMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronLooseMVA3)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronMVA)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMVA2category"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronMVA2category)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronMVA2raw)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMVA3category"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronMVA3category)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMVA3Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronMVA3raw)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMedium"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronMedium)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMediumMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronMediumMVA2)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMediumMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronMediumMVA3)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronTight"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronTight)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronTightMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronTightMVA2)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronTightMVA3)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronVLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronVLooseMVA2)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronVTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstElectronVTightMVA3)[idx];
            }
            throw;
        };
        accessors["T2_AntiMuonLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstMuonLoose)[idx];
            }
            throw;
        };
        accessors["T2_AntiMuonLoose2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstMuonLoose2)[idx];
            }
            throw;
        };
        accessors["T2_AntiMuonMedium"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstMuonMedium)[idx];
            }
            throw;
        };
        accessors["T2_AntiMuonMedium2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstMuonMedium2)[idx];
            }
            throw;
        };
        accessors["T2_AntiMuonTight"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstMuonTight)[idx];
            }
            throw;
        };
        accessors["T2_AntiMuonTight2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSagainstMuonTight2)[idx];
            }
            throw;
        };
        accessors["T2_CombinedIsolationDeltaBetaCorrRaw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyCombinedIsolationDeltaBetaCorrRaw)[idx];
            }
            throw;
        };
        accessors["T2_CombinedIsolationDeltaBetaCorrRaw3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx];
            }
            throw;
        };
        accessors["T2_IsolationMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyIsolationMVA2raw)[idx];
            }
            throw;
        };
        accessors["T2_IsolationMVARaw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyIsolationMVAraw)[idx];
            }
            throw;
        };
        accessors["T2_LooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyLooseCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T2_LooseCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T2_LooseIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyLooseIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T2_LooseIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyLooseIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T2_MediumCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyMediumCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T2_MediumCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T2_MediumIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyMediumIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T2_MediumIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyMediumIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T2_TightCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyTightCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T2_TightCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T2_TightIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyTightIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T2_TightIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyTightIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T2_VLooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSbyVLooseCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T2_DecayModeFinding"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2HPSdecayModeFinding)[idx];
            }
            throw;
        };
        accessors["T2_IsInTheCracks"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2IsInTheCracks)[idx];
            }
            throw;
        };
        accessors["T2_LTIpVtdxy"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2LTIpVtdxy)[idx];
            }
            throw;
        };
        accessors["T2_LTIpVtdxyError"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2LTIpVtdxyError)[idx];
            }
            throw;
        };
        accessors["T2_LTIpVtdz"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2LTIpVtdz)[idx];
            }
            throw;
        };
        accessors["T2_LTIpVtdzError"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2LTIpVtdzError)[idx];
            }
            throw;
        };
        accessors["T2_LTNormChiSqrd"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2LTNormChiSqrd)[idx];
            }
            throw;
        };
        accessors["T2_LTPt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2LTPt)[idx];
            }
            throw;
        };
        accessors["T2_LTValidHits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2LTValidHits)[idx];
            }
            throw;
        };
        accessors["T2_LTvalid"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2LTvalid)[idx];
            }
            throw;
        };
        accessors["T2_LTvx"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2LTvx)[idx];
            }
            throw;
        };
        accessors["T2_LTvy"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2LTvy)[idx];
            }
            throw;
        };
        accessors["T2_LTvz"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2LTvz)[idx];
            }
            throw;
        };
        accessors["T2_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2MomentumRank)[idx];
            }
            throw;
        };
        accessors["T2_NumProngs"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2NProngs)[idx];
            }
            throw;
        };
        accessors["T2_NumSignalGammas"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2NSignalGammas)[idx];
            }
            throw;
        };
        accessors["T2_NumSignalNeutrals"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2NSignalNeutrals)[idx];
            }
            throw;
        };
        accessors["T2_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2Phi)[idx];
            }
            throw;
        };
        accessors["T2_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_Tau2Pt)[idx];
            }
            throw;
        };
        accessors["TL1_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLepton1CosDeltaPhi)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLepton1CosDeltaPhi)[idx];
            }
            throw "";
        };
        accessors["TL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLepton1DeltaR)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLepton1DeltaR)[idx];
            }
            throw "";
        };
        accessors["TL1_METMass"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLepton1METMass)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLepton1METMass)[idx];
            }
            throw "";
        };
        accessors["TL1_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLepton1VisibleMass)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLepton1VisibleMass)[idx];
            }
            throw "";
        };
        accessors["TL2_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLepton2CosDeltaPhi)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLepton2CosDeltaPhi)[idx];
            }
            throw "";
        };
        accessors["TL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLepton2DeltaR)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLepton2DeltaR)[idx];
            }
            throw "";
        };
        accessors["TL2_METMass"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLepton2METMass)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLepton2METMass)[idx];
            }
            throw "";
        };
        accessors["TL2_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLepton2VisibleMass)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLepton2VisibleMass)[idx];
            }
            throw "";
        };
        accessors["TL_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauLeptonCosDeltaPhi)[idx];
            }
            throw;
        };
        accessors["TL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauLeptonDeltaR)[idx];
            }
            throw;
        };
        accessors["TL_METMass"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauLeptonMETMass)[idx];
            }
            throw;
        };
        accessors["TL_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauLeptonVisibleMass)[idx];
            }
            throw;
        };
        accessors["T_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauCharge)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauCharge)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauCharge)[idx];
            }
            throw "";
        };
        accessors["T_DecayMode"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauDecayMode)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauDecayMode)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauDecayMode)[idx];
            }
            throw "";
        };
        accessors["T_EmFraction"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauEmFraction)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauEmFraction)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauEmFraction)[idx];
            }
            throw "";
        };
        accessors["T_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauEta)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauEta)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauEta)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchDaughter0Id)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchDaughter0Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchDaughter0Id)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchDaughter0Status)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchDaughter0Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchDaughter0Status)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchDaughter1Id)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchDaughter1Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchDaughter1Id)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchDaughter1Status)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchDaughter1Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchDaughter1Status)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchGrandmother00Id)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchGrandmother00Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchGrandmother00Id)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchGrandmother00Status)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchGrandmother00Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchGrandmother00Status)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchGrandmother01Id)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchGrandmother01Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchGrandmother01Id)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchGrandmother01Status)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchGrandmother01Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchGrandmother01Status)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchGrandmother10Id)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchGrandmother10Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchGrandmother10Id)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchGrandmother10Status)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchGrandmother10Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchGrandmother10Status)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchGrandmother11Id)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchGrandmother11Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchGrandmother11Id)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchGrandmother11Status)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchGrandmother11Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchGrandmother11Status)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchId)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchId)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchId)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchMother0Id)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchMother0Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchMother0Id)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchMother0Status)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchMother0Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchMother0Status)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchMother1Id)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchMother1Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchMother1Id)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchMother1Status)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchMother1Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchMother1Status)[idx];
            }
            throw "";
        };
        accessors["T_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauGenMatchStatus)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauGenMatchStatus)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauGenMatchStatus)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronDeadECAL"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronDeadECAL)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronDeadECAL)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronDeadECAL)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronLoose)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronLoose)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronLoose)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronLooseMVA2)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronLooseMVA2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronLooseMVA2)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronLooseMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronLooseMVA3)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronLooseMVA3)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronLooseMVA3)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronMVA)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronMVA)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronMVA)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronMVA2category"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronMVA2category)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronMVA2category)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronMVA2category)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronMVA2raw)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronMVA2raw)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronMVA2raw)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronMVA3category"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronMVA3category)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronMVA3category)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronMVA3category)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronMVA3Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronMVA3raw)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronMVA3raw)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronMVA3raw)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronMedium"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronMedium)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronMedium)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronMedium)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronMediumMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronMediumMVA2)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronMediumMVA2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronMediumMVA2)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronMediumMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronMediumMVA3)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronMediumMVA3)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronMediumMVA3)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronTight"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronTight)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronTight)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronTight)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronTightMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronTightMVA2)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronTightMVA2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronTightMVA2)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronTightMVA3)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronTightMVA3)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronTightMVA3)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronVLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronVLooseMVA2)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronVLooseMVA2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronVLooseMVA2)[idx];
            }
            throw "";
        };
        accessors["T_AntiElectronVTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstElectronVTightMVA3)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstElectronVTightMVA3)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstElectronVTightMVA3)[idx];
            }
            throw "";
        };
        accessors["T_AntiMuonLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstMuonLoose)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstMuonLoose)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstMuonLoose)[idx];
            }
            throw "";
        };
        accessors["T_AntiMuonLoose2"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstMuonLoose2)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstMuonLoose2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstMuonLoose2)[idx];
            }
            throw "";
        };
        accessors["T_AntiMuonMedium"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstMuonMedium)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstMuonMedium)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstMuonMedium)[idx];
            }
            throw "";
        };
        accessors["T_AntiMuonMedium2"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstMuonMedium2)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstMuonMedium2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstMuonMedium2)[idx];
            }
            throw "";
        };
        accessors["T_AntiMuonTight"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstMuonTight)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstMuonTight)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstMuonTight)[idx];
            }
            throw "";
        };
        accessors["T_AntiMuonTight2"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSagainstMuonTight2)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSagainstMuonTight2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSagainstMuonTight2)[idx];
            }
            throw "";
        };
        accessors["T_CombinedIsolationDeltaBetaCorrRaw"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw)[idx];
            }
            throw "";
        };
        accessors["T_CombinedIsolationDeltaBetaCorrRaw3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx];
            }
            throw "";
        };
        accessors["T_IsolationMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyIsolationMVA2raw)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyIsolationMVA2raw)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyIsolationMVA2raw)[idx];
            }
            throw "";
        };
        accessors["T_IsolationMVARaw"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyIsolationMVAraw)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyIsolationMVAraw)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyIsolationMVAraw)[idx];
            }
            throw "";
        };
        accessors["T_LooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw "";
        };
        accessors["T_LooseCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw "";
        };
        accessors["T_LooseIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyLooseIsolationMVA)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyLooseIsolationMVA)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyLooseIsolationMVA)[idx];
            }
            throw "";
        };
        accessors["T_LooseIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyLooseIsolationMVA2)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyLooseIsolationMVA2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyLooseIsolationMVA2)[idx];
            }
            throw "";
        };
        accessors["T_MediumCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw "";
        };
        accessors["T_MediumCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw "";
        };
        accessors["T_MediumIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyMediumIsolationMVA)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyMediumIsolationMVA)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyMediumIsolationMVA)[idx];
            }
            throw "";
        };
        accessors["T_MediumIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyMediumIsolationMVA2)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyMediumIsolationMVA2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyMediumIsolationMVA2)[idx];
            }
            throw "";
        };
        accessors["T_TightCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyTightCombinedIsolationDeltaBetaCorr)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyTightCombinedIsolationDeltaBetaCorr)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyTightCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw "";
        };
        accessors["T_TightCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw "";
        };
        accessors["T_TightIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyTightIsolationMVA)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyTightIsolationMVA)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyTightIsolationMVA)[idx];
            }
            throw "";
        };
        accessors["T_TightIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyTightIsolationMVA2)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyTightIsolationMVA2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyTightIsolationMVA2)[idx];
            }
            throw "";
        };
        accessors["T_VLooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSbyVLooseCombinedIsolationDeltaBetaCorr)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSbyVLooseCombinedIsolationDeltaBetaCorr)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSbyVLooseCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw "";
        };
        accessors["T_DecayModeFinding"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauHPSdecayModeFinding)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauHPSdecayModeFinding)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauHPSdecayModeFinding)[idx];
            }
            throw "";
        };
        accessors["T_IsInTheCracks"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauIsInTheCracks)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauIsInTheCracks)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauIsInTheCracks)[idx];
            }
            throw "";
        };
        accessors["T_LTIpVtdxy"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLTIpVtdxy)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauLTIpVtdxy)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLTIpVtdxy)[idx];
            }
            throw "";
        };
        accessors["T_LTIpVtdxyError"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLTIpVtdxyError)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauLTIpVtdxyError)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLTIpVtdxyError)[idx];
            }
            throw "";
        };
        accessors["T_LTIpVtdz"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLTIpVtdz)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauLTIpVtdz)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLTIpVtdz)[idx];
            }
            throw "";
        };
        accessors["T_LTIpVtdzError"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLTIpVtdzError)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauLTIpVtdzError)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLTIpVtdzError)[idx];
            }
            throw "";
        };
        accessors["T_LTNormChiSqrd"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLTNormChiSqrd)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauLTNormChiSqrd)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLTNormChiSqrd)[idx];
            }
            throw "";
        };
        accessors["T_LTPt"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLTPt)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauLTPt)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLTPt)[idx];
            }
            throw "";
        };
        accessors["T_LTValidHits"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLTValidHits)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauLTValidHits)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLTValidHits)[idx];
            }
            throw "";
        };
        accessors["T_LTvalid"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLTvalid)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauLTvalid)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLTvalid)[idx];
            }
            throw "";
        };
        accessors["T_LTvx"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLTvx)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauLTvx)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLTvx)[idx];
            }
            throw "";
        };
        accessors["T_LTvy"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLTvy)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauLTvy)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLTvy)[idx];
            }
            throw "";
        };
        accessors["T_LTvz"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauLTvz)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauLTvz)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauLTvz)[idx];
            }
            throw "";
        };
        accessors["T_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauMomentumRank)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauMomentumRank)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauMomentumRank)[idx];
            }
            throw "";
        };
        accessors["T_NumProngs"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauNProngs)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauNProngs)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauNProngs)[idx];
            }
            throw "";
        };
        accessors["T_NumSignalGammas"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauNSignalGammas)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauNSignalGammas)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauNSignalGammas)[idx];
            }
            throw "";
        };
        accessors["T_NumSignalNeutrals"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauNSignalNeutrals)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauNSignalNeutrals)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauNSignalNeutrals)[idx];
            }
            throw "";
        };
        accessors["T_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauPhi)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauPhi)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauPhi)[idx];
            }
            throw "";
        };
        accessors["T_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TauPt)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TauPt)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TauPt)[idx];
            }
            throw "";
        };
        accessors["TopPtWeight"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_topPtWeight;
        };
        accessors["TopPtWeightDown"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_topPtWeightDown;
        };
        accessors["TopPtWeightUp"] = [](Branches *b, int idx, int n) -> float {
            return b->Ev_topPtWeightUp;
        };
        accessors["TriggerEventWeight"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->LL_TriggerEventWeight)[idx];
            } else if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->TL_TriggerEventWeight)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_TriggerEventWeight)[idx];
            }
            throw "";
        };
        accessors["V_NumInteractionsBX0"] = [](Branches *b, int idx, int n) -> float {
            return b->V_NumInteractionsBX0;
        };
        accessors["V_NumInteractionsBXm1"] = [](Branches *b, int idx, int n) -> float {
            return b->V_NumInteractionsBXm1;
        };
        accessors["V_NumInteractionsBXp1"] = [](Branches *b, int idx, int n) -> float {
            return b->V_NumInteractionsBXp1;
        };
        accessors["V_NumVertices"] = [](Branches *b, int idx, int n) -> float {
            return b->V_NumVertices;
        };
        // >>> End attr <<<

        setup = true;
    }
}

NamespaceImp(roast)
