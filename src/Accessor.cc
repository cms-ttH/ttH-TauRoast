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

    inline
    unsigned int
    get_tag_index(const std::vector<bool>& vals, int n)
    {
        unsigned int i = 0;
        for (const auto& val: vals) {
            if (n == 0 and val)
                return i;
            else if (val)
                --n;
            ++i;
        }
        throw std::out_of_range("Not enough tags");
    }

    inline
    unsigned int
    get_untag_index(const std::vector<bool>& vals, int n)
    {
        unsigned int i = 0;
        for (const auto& val: vals) {
            if (n == 0 and not val)
                return i;
            else if (not val)
                --n;
            ++i;
        }
        throw std::out_of_range("Not enough non-tags");
    }

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
                return abs((*e->T_GenMatchId)[idx]);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return abs((*e->T_GenMatchId)[idx]);
            }
            throw "";
        };
        accessors["T1_MatchAbsId"] = [](roast::Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->T1_GenMatchId)[idx]);
        };
        accessors["T2_MatchAbsId"] = [](roast::Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->T2_GenMatchId)[idx]);
        };

        accessors["T_ParentAbsId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return abs((*e->T_GenMatchMother0Id)[idx]);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return abs((*e->T_GenMatchMother0Id)[idx]);
            }
            throw "";
        };
        accessors["T1_ParentAbsId"] = [](roast::Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->T1_GenMatchMother0Id)[idx]);
        };
        accessors["T2_ParentAbsId"] = [](roast::Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->T2_GenMatchMother0Id)[idx]);
        };

        accessors["T_GoodParentId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                int mother = (*e->T_GenMatchMother0Id)[idx];
                if (mother == -99)
                    mother = (*e->T_GenMatchMother1Id)[idx];
                return mother;
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                int mother = (*e->T_GenMatchMother0Id)[idx];
                if (mother == -99)
                    mother = (*e->T_GenMatchMother1Id)[idx];
                return mother;
            }
            throw "";
        };
        accessors["T1_GoodParentId"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            int mother = (*e->T1_GenMatchMother0Id)[idx];
            if (mother == -99)
                mother = (*e->T1_GenMatchMother1Id)[idx];
            return mother;
        };
        accessors["T2_GoodParentId"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            int mother = (*e->T2_GenMatchMother0Id)[idx];
            if (mother == -99)
                mother = (*e->T2_GenMatchMother1Id)[idx];
            return mother;
        };
        accessors["L_GoodParentId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                int mother = (*e->L_GenMatchMother0Id)[idx];
                if (mother == -99)
                    mother = (*e->L_GenMatchMother1Id)[idx];
                return mother;
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                int mother = (*e->L_GenMatchMother0Id)[idx];
                if (mother == -99)
                    mother = (*e->L_GenMatchMother1Id)[idx];
                return mother;
            }
            throw "";
        };
        accessors["L1_GoodParentId"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            int mother = (*e->L1_GenMatchMother0Id)[idx];
            if (mother == -99)
                mother = (*e->L1_GenMatchMother1Id)[idx];
            return mother;
        };
        accessors["L2_GoodParentId"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            int mother = (*e->L2_GenMatchMother0Id)[idx];
            if (mother == -99)
                mother = (*e->L2_GenMatchMother1Id)[idx];
            return mother;
        };

        accessors["T_GoodGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                int grannie = (*e->T_GenMatchGrandmother00Id)[idx];
                if (grannie == -99)
                    grannie = (*e->T_GenMatchGrandmother01Id)[idx];
                if (grannie == -99)
                    grannie = (*e->T_GenMatchGrandmother10Id)[idx];
                if (grannie == -99)
                    grannie = (*e->T_GenMatchGrandmother11Id)[idx];
                return grannie;
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                int grannie = (*e->T_GenMatchGrandmother00Id)[idx];
                if (grannie == -99)
                    grannie = (*e->T_GenMatchGrandmother01Id)[idx];
                if (grannie == -99)
                    grannie = (*e->T_GenMatchGrandmother10Id)[idx];
                if (grannie == -99)
                    grannie = (*e->T_GenMatchGrandmother11Id)[idx];
                return grannie;
            }
            throw "";
        };
        accessors["T1_GoodGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            int grannie = (*e->T1_GenMatchGrandmother00Id)[idx];
            if (grannie == -99)
                grannie = (*e->T1_GenMatchGrandmother01Id)[idx];
            if (grannie == -99)
                grannie = (*e->T1_GenMatchGrandmother10Id)[idx];
            if (grannie == -99)
                grannie = (*e->T1_GenMatchGrandmother11Id)[idx];
            return grannie;
        };
        accessors["T2_GoodGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            int grannie = (*e->T2_GenMatchGrandmother00Id)[idx];
            if (grannie == -99)
                grannie = (*e->T2_GenMatchGrandmother01Id)[idx];
            if (grannie == -99)
                grannie = (*e->T2_GenMatchGrandmother10Id)[idx];
            if (grannie == -99)
                grannie = (*e->T2_GenMatchGrandmother11Id)[idx];
            return grannie;
        };
        accessors["L_GoodGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                int grannie = (*e->L_GenMatchGrandmother00Id)[idx];
                if (grannie == -99)
                    grannie = (*e->L_GenMatchGrandmother01Id)[idx];
                if (grannie == -99)
                    grannie = (*e->L_GenMatchGrandmother10Id)[idx];
                if (grannie == -99)
                    grannie = (*e->L_GenMatchGrandmother11Id)[idx];
                return grannie;
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                int grannie = (*e->L_GenMatchGrandmother00Id)[idx];
                if (grannie == -99)
                    grannie = (*e->L_GenMatchGrandmother01Id)[idx];
                if (grannie == -99)
                    grannie = (*e->L_GenMatchGrandmother10Id)[idx];
                if (grannie == -99)
                    grannie = (*e->L_GenMatchGrandmother11Id)[idx];
                return grannie;
            }
            throw "";
        };
        accessors["L1_GoodGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            int grannie = (*e->L1_GenMatchGrandmother00Id)[idx];
            if (grannie == -99)
                grannie = (*e->L1_GenMatchGrandmother01Id)[idx];
            if (grannie == -99)
                grannie = (*e->L1_GenMatchGrandmother10Id)[idx];
            if (grannie == -99)
                grannie = (*e->L1_GenMatchGrandmother11Id)[idx];
            return grannie;
        };
        accessors["L2_GoodGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            int grannie = (*e->L2_GenMatchGrandmother00Id)[idx];
            if (grannie == -99)
                grannie = (*e->L2_GenMatchGrandmother01Id)[idx];
            if (grannie == -99)
                grannie = (*e->L2_GenMatchGrandmother10Id)[idx];
            if (grannie == -99)
                grannie = (*e->L2_GenMatchGrandmother11Id)[idx];
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
            return reco::deltaR((*e->L1_P)[idx], (*e->L2_P)[idx]);
        };

        accessors["LL_ChargeProduct"] = [](roast::Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_Charge)[idx] * (*e->L2_Charge)[idx];
            } else if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_Charge)[idx] * (*e->L2_Charge)[idx];
            }
            throw;
        };
        // FIXME
        // accessors["LL_ZPeakVeto"] = [](roast::Branches *b, int idx, int n) -> float {
            // tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            // float m = (*e->TLL_Lepton1Lepton2VisibleMass)[idx];
            // return (81. <= m && m <= 101.);
        // };

        // accessors["TL_METMass"] = [](roast::Branches *b, int idx, int n) -> float {
            // tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            // if ((*e->TLL_Lepton1Charge)[idx] * (*e->TLL_TauCharge)[idx] < 0) {
                // return (*e->TLL_TauLepton1METMass)[idx];
            // } else {
                // return (*e->TLL_TauLepton2METMass)[idx];
            // }
        // };
        // accessors["TL_VissMass"] = [](roast::Branches *b, int idx, int n) -> float {
            // tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            // if ((*e->TLL_Lepton1Charge)[idx] * (*e->TLL_TauCharge)[idx] < 0) {
                // return (*e->TLL_TauLepton1VisibleMass)[idx];
            // } else {
                // return (*e->TLL_TauLepton2VisibleMass)[idx];
            // }
        // };

        // accessors["TL_Mass"] = [](roast::Branches *b, int idx, int n) -> float {
            // tl::Branches* e = dynamic_cast<tl::Branches*>(b);

            // svFitStandalone::Vector met(e->Ev_MET * TMath::Sin(e->Ev_METphi), e->Ev_MET * TMath::Cos(e->Ev_METphi), 0.);
            // TMatrixD covMET(2, 2);
            // covMET[0][0] = (*e->Ev_METcov)[0];
            // covMET[0][1] = (*e->Ev_METcov)[1];
            // covMET[1][0] = (*e->Ev_METcov)[2];
            // covMET[1][1] = (*e->Ev_METcov)[3];

            // svFitStandalone::LorentzVector t((*e->TL_TauP)[idx]);
            // svFitStandalone::LorentzVector l((*e->TL_LeptonP)[idx]);

            // std::vector<svFitStandalone::MeasuredTauLepton> prods = {
                // svFitStandalone::MeasuredTauLepton(svFitStandalone::kHadDecay, t),
                // svFitStandalone::MeasuredTauLepton(svFitStandalone::kLepDecay, l)
            // };

            // SVfitStandaloneAlgorithm algo(prods, met, covMET, 2);
            // algo.addLogM(true);
            // algo.integrateVEGAS();
            // double mass = algo.getMass();
            // return algo.isValidSolution() ? mass : -1.0;
        // };

        accessors["TT_ChargeProduct"] = [](roast::Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->T1_Charge)[idx] * (*e->T2_Charge)[idx];
        };
        // accessors["TT_VisMass"] = [](roast::Branches *b, int idx, int n) -> float {
            // ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            // return (*e->TTL_DitauVisibleMass)[idx];
        // };
        // accessors["TT_DeltaEta"] = [](roast::Branches *b, int idx, int n) -> float {
            // ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            // return (*e->T1_Eta)[idx] - (*e->T2_Eta)[idx];
        // };
        // accessors["TT_CosDeltaPhi"] = [](roast::Branches *b, int idx, int n) -> float {
            // ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            // return (*e->TTL_DitauCosDeltaPhi)[idx];
        // };

        accessors["FinalBDTG"] = [](Branches *b, int idx, int n) -> float {
            return MVABase::gMVA["FinalBDTG"] ? MVABase::gMVA["FinalBDTG"]->Evaluate(b, idx) : 0.;
        };

        // accessors["PassQCDVeto"] = [](Branches *b, int idx, int n) -> float {
            // tl::Branches* e = dynamic_cast<tl::Branches*>(b);
            // auto mt = (*e->TL_LeptonMt2)[idx];
            // auto met = e->Ev_MET;
            // return (met > 50) || (mt > 50);
        // };

        // accessors["PassZMask3"] = [](Branches *b, int idx, int n) -> float {
            // tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            // auto dilmass = (*e->TLL_Lepton1Lepton2VisibleMass)[idx];
            // auto met = e->Ev_MET;
            // bool zpeak = dilmass > 82 && dilmass < 100 &&
                    // met - 40 < 30 / 9. * (dilmass - 82) &&
                    // met - 40 < 30 - 30 / 9. * (dilmass - 91);
            // return !zpeak;
        // };

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
        accessors["TT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->T2_P)[idx]);
            }
            throw;
        };
        accessors["T1L_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->L_P)[idx]);
            }
            throw;
        };
        accessors["T2L_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->L_P)[idx]);
            }
            throw;
        };
        accessors["TL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->L1_P)[idx]);
            }
            throw;
        };
        accessors["TL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->L2_P)[idx]);
            }
            throw;
        };
        accessors["TL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->L_P)[idx]);
            }
            throw;
        };
        accessors["LL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->L2_P)[idx]);
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->L2_P)[idx]);
            }
            throw;
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
        accessors["GJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GJ_Eta)[idx];
        };
        accessors["GJ_IsBjet"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GJ_IsBjet)[idx];
        };
        accessors["GJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GJ_MomentumRank)[idx];
        };
        accessors["GJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GJ_Phi)[idx];
        };
        accessors["GJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GJ_Pt)[idx];
        };
        accessors["GT_Eta"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_Eta)[idx];
        };
        accessors["GT_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_MomentumRank)[idx];
        };
        accessors["GT_ParentEta"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ParentEta)[idx];
        };
        accessors["GT_ParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ParentId)[idx];
        };
        accessors["GT_ParentP"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ParentP)[idx];
        };
        accessors["GT_ParentPhi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ParentPhi)[idx];
        };
        accessors["GT_ParentPt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ParentPt)[idx];
        };
        accessors["GT_Phi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_Phi)[idx];
        };
        accessors["GT_Pt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_Pt)[idx];
        };
        accessors["GT_ToElectron"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ToElectron)[idx];
        };
        accessors["GT_ToHadrons"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ToHadrons)[idx];
        };
        accessors["GT_ToMuon"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ToMuon)[idx];
        };
        accessors["GT_VisEta"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_VisEta)[idx];
        };
        accessors["GT_VisPhi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_VisPhi)[idx];
        };
        accessors["GT_VisPt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_VisPt)[idx];
        };
        accessors["HT"] = [](Branches *b, int idx, int n) -> float {
            return 0.;
            return (*b->HT)[idx];
        };
        accessors["IsTauEvent"] = [](Branches *b, int idx, int n) -> float {
            return b->IsTauEvent;
        };
        accessors["TaggedJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_Charge)[idx][i];
        };
        accessors["TaggedLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_Charge)[idx][i];
        };
        accessors["TaggedSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_Charge)[idx][i];
        };
        accessors["TaggedSubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_Charge)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_Charge)[idx][i];
        };
        accessors["UntaggedJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_Charge)[idx][i];
        };
        accessors["UntaggedLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_Charge)[idx][i];
        };
        accessors["UntaggedSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_Charge)[idx][i];
        };
        accessors["UntaggedSubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_Charge)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_Charge)[idx][i];
        };
        accessors["J_Charge"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_Charge)[idx][n];
        };
        accessors["TaggedJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_MomentumRank)[idx][i];
        };
        accessors["TaggedLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_MomentumRank)[idx][i];
        };
        accessors["TaggedSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_MomentumRank)[idx][i];
        };
        accessors["TaggedSubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_MomentumRank)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_MomentumRank)[idx][i];
        };
        accessors["UntaggedJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_MomentumRank)[idx][i];
        };
        accessors["UntaggedLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_MomentumRank)[idx][i];
        };
        accessors["UntaggedSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_MomentumRank)[idx][i];
        };
        accessors["UntaggedSubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_MomentumRank)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_MomentumRank)[idx][i];
        };
        accessors["J_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_MomentumRank)[idx][n];
        };
        accessors["J_NumJets"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_NumJets)[idx];
        };
        accessors["J_Pt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx][n].pt();
        };
        accessors["TaggedJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_P)[idx][i].pt();
        };
        accessors["TaggedLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_P)[idx][i].pt();
        };
        accessors["TaggedSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_P)[idx][i].pt();
        };
        accessors["TaggedSubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_P)[idx][i].pt();
        };
        accessors["TaggedSubSubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_P)[idx][i].pt();
        };
        accessors["UntaggedJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_P)[idx][i].pt();
        };
        accessors["UntaggedLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_P)[idx][i].pt();
        };
        accessors["UntaggedSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_P)[idx][i].pt();
        };
        accessors["UntaggedSubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_P)[idx][i].pt();
        };
        accessors["UntaggedSubSubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_P)[idx][i].pt();
        };
        accessors["J_Eta"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx][n].eta();
        };
        accessors["TaggedJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_P)[idx][i].eta();
        };
        accessors["TaggedLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_P)[idx][i].eta();
        };
        accessors["TaggedSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_P)[idx][i].eta();
        };
        accessors["TaggedSubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_P)[idx][i].eta();
        };
        accessors["TaggedSubSubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_P)[idx][i].eta();
        };
        accessors["UntaggedJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_P)[idx][i].eta();
        };
        accessors["UntaggedLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_P)[idx][i].eta();
        };
        accessors["UntaggedSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_P)[idx][i].eta();
        };
        accessors["UntaggedSubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_P)[idx][i].eta();
        };
        accessors["UntaggedSubSubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_P)[idx][i].eta();
        };
        accessors["J_Phi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx][n].phi();
        };
        accessors["TaggedJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_P)[idx][i].phi();
        };
        accessors["TaggedLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_P)[idx][i].phi();
        };
        accessors["TaggedSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_P)[idx][i].phi();
        };
        accessors["TaggedSubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_P)[idx][i].phi();
        };
        accessors["TaggedSubSubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_P)[idx][i].phi();
        };
        accessors["UntaggedJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_P)[idx][i].phi();
        };
        accessors["UntaggedLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_P)[idx][i].phi();
        };
        accessors["UntaggedSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_P)[idx][i].phi();
        };
        accessors["UntaggedSubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_P)[idx][i].phi();
        };
        accessors["UntaggedSubSubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_P)[idx][i].phi();
        };
        accessors["TaggedJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandParentId)[idx][i];
        };
        accessors["TaggedLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandParentId)[idx][i];
        };
        accessors["TaggedSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandParentId)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandParentId)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandParentId)[idx][i];
        };
        accessors["UntaggedJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandParentId)[idx][i];
        };
        accessors["UntaggedLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandParentId)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandParentId)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandParentId)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandParentId)[idx][i];
        };
        accessors["J_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandParentId)[idx][n];
        };
        accessors["TaggedJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother00Id)[idx][i];
        };
        accessors["TaggedLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother00Id)[idx][i];
        };
        accessors["TaggedSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother00Id)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother00Id)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother00Id)[idx][i];
        };
        accessors["UntaggedJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother00Id)[idx][i];
        };
        accessors["UntaggedLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother00Id)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother00Id)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother00Id)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother00Id)[idx][i];
        };
        accessors["J_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Id)[idx][n];
        };
        accessors["TaggedJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother00Status)[idx][i];
        };
        accessors["TaggedLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother00Status)[idx][i];
        };
        accessors["TaggedSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother00Status)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother00Status)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother00Status)[idx][i];
        };
        accessors["UntaggedJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother00Status)[idx][i];
        };
        accessors["UntaggedLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother00Status)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother00Status)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother00Status)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother00Status)[idx][i];
        };
        accessors["J_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Status)[idx][n];
        };
        accessors["TaggedJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother01Id)[idx][i];
        };
        accessors["TaggedLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother01Id)[idx][i];
        };
        accessors["TaggedSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother01Id)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother01Id)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother01Id)[idx][i];
        };
        accessors["UntaggedJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother01Id)[idx][i];
        };
        accessors["UntaggedLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother01Id)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother01Id)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother01Id)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother01Id)[idx][i];
        };
        accessors["J_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Id)[idx][n];
        };
        accessors["TaggedJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother01Status)[idx][i];
        };
        accessors["TaggedLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother01Status)[idx][i];
        };
        accessors["TaggedSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother01Status)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother01Status)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother01Status)[idx][i];
        };
        accessors["UntaggedJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother01Status)[idx][i];
        };
        accessors["UntaggedLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother01Status)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother01Status)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother01Status)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother01Status)[idx][i];
        };
        accessors["J_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Status)[idx][n];
        };
        accessors["TaggedJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother10Id)[idx][i];
        };
        accessors["TaggedLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother10Id)[idx][i];
        };
        accessors["TaggedSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother10Id)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother10Id)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother10Id)[idx][i];
        };
        accessors["UntaggedJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother10Id)[idx][i];
        };
        accessors["UntaggedLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother10Id)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother10Id)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother10Id)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother10Id)[idx][i];
        };
        accessors["J_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Id)[idx][n];
        };
        accessors["TaggedJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother10Status)[idx][i];
        };
        accessors["TaggedLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother10Status)[idx][i];
        };
        accessors["TaggedSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother10Status)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother10Status)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother10Status)[idx][i];
        };
        accessors["UntaggedJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother10Status)[idx][i];
        };
        accessors["UntaggedLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother10Status)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother10Status)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother10Status)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother10Status)[idx][i];
        };
        accessors["J_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Status)[idx][n];
        };
        accessors["TaggedJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother11Id)[idx][i];
        };
        accessors["TaggedLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother11Id)[idx][i];
        };
        accessors["TaggedSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother11Id)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother11Id)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother11Id)[idx][i];
        };
        accessors["UntaggedJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother11Id)[idx][i];
        };
        accessors["UntaggedLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother11Id)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother11Id)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother11Id)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother11Id)[idx][i];
        };
        accessors["J_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Id)[idx][n];
        };
        accessors["TaggedJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother11Status)[idx][i];
        };
        accessors["TaggedLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother11Status)[idx][i];
        };
        accessors["TaggedSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother11Status)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother11Status)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother11Status)[idx][i];
        };
        accessors["UntaggedJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother11Status)[idx][i];
        };
        accessors["UntaggedLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother11Status)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother11Status)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother11Status)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother11Status)[idx][i];
        };
        accessors["J_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Status)[idx][n];
        };
        accessors["TaggedJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonId)[idx][i];
        };
        accessors["TaggedLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonId)[idx][i];
        };
        accessors["TaggedSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonId)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonId)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonId)[idx][i];
        };
        accessors["UntaggedJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonId)[idx][i];
        };
        accessors["UntaggedLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonId)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonId)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonId)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonId)[idx][i];
        };
        accessors["J_PartonId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonId)[idx][n];
        };
        accessors["TaggedJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother0Id)[idx][i];
        };
        accessors["TaggedLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother0Id)[idx][i];
        };
        accessors["TaggedSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother0Id)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother0Id)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother0Id)[idx][i];
        };
        accessors["UntaggedJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother0Id)[idx][i];
        };
        accessors["UntaggedLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother0Id)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother0Id)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother0Id)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother0Id)[idx][i];
        };
        accessors["J_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Id)[idx][n];
        };
        accessors["TaggedJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother0Status)[idx][i];
        };
        accessors["TaggedLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother0Status)[idx][i];
        };
        accessors["TaggedSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother0Status)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother0Status)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother0Status)[idx][i];
        };
        accessors["UntaggedJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother0Status)[idx][i];
        };
        accessors["UntaggedLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother0Status)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother0Status)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother0Status)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother0Status)[idx][i];
        };
        accessors["J_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Status)[idx][n];
        };
        accessors["TaggedJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother1Id)[idx][i];
        };
        accessors["TaggedLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother1Id)[idx][i];
        };
        accessors["TaggedSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother1Id)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother1Id)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother1Id)[idx][i];
        };
        accessors["UntaggedJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother1Id)[idx][i];
        };
        accessors["UntaggedLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother1Id)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother1Id)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother1Id)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother1Id)[idx][i];
        };
        accessors["J_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Id)[idx][n];
        };
        accessors["TaggedJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother1Status)[idx][i];
        };
        accessors["TaggedLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother1Status)[idx][i];
        };
        accessors["TaggedSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother1Status)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother1Status)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother1Status)[idx][i];
        };
        accessors["UntaggedJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother1Status)[idx][i];
        };
        accessors["UntaggedLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother1Status)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother1Status)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother1Status)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother1Status)[idx][i];
        };
        accessors["J_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Status)[idx][n];
        };
        accessors["TaggedJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonParentId)[idx][i];
        };
        accessors["TaggedLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonParentId)[idx][i];
        };
        accessors["TaggedSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonParentId)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonParentId)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonParentId)[idx][i];
        };
        accessors["UntaggedJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonParentId)[idx][i];
        };
        accessors["UntaggedLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonParentId)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonParentId)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonParentId)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonParentId)[idx][i];
        };
        accessors["J_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonParentId)[idx][n];
        };
        accessors["TaggedJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonStatus)[idx][i];
        };
        accessors["TaggedLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonStatus)[idx][i];
        };
        accessors["TaggedSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonStatus)[idx][i];
        };
        accessors["TaggedSubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonStatus)[idx][i];
        };
        accessors["TaggedSubSubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonStatus)[idx][i];
        };
        accessors["UntaggedJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonStatus)[idx][i];
        };
        accessors["UntaggedLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonStatus)[idx][i];
        };
        accessors["UntaggedSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonStatus)[idx][i];
        };
        accessors["UntaggedSubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonStatus)[idx][i];
        };
        accessors["UntaggedSubSubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonStatus)[idx][i];
        };
        accessors["J_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonStatus)[idx][n];
        };
        accessors["L1_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_Charge)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_Charge)[idx];
            }
            throw;
        };
        accessors["L1_CorrectedD0"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_CorrectedD0)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_CorrectedD0)[idx];
            }
            throw;
        };
        accessors["L1_CorrectedDZ"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_CorrectedDZ)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_CorrectedDZ)[idx];
            }
            throw;
        };
        accessors["L1_EventWeight"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_EventWeight)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_EventWeight)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchDaughter0Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchDaughter0Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchDaughter0Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchDaughter0Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchDaughter1Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchDaughter1Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchDaughter1Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchDaughter1Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother00Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother00Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother00Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother00Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother01Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother01Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother01Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother01Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother10Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother10Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother10Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother10Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother11Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother11Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother11Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother11Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchId)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchId)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchMother0Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchMother0Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchMother0Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchMother0Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchMother1Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchMother1Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchMother1Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchMother1Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_GenMatchStatus)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchStatus)[idx];
            }
            throw;
        };
        accessors["L1_ImpactParameter"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_ImpactParameter)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_ImpactParameter)[idx];
            }
            throw;
        };
        accessors["L1_IsElectron"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_IsElectron)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_IsElectron)[idx];
            }
            throw;
        };
        accessors["L1_IsLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_IsLoose)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_IsLoose)[idx];
            }
            throw;
        };
        accessors["L1_IsMuon"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_IsMuon)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_IsMuon)[idx];
            }
            throw;
        };
        accessors["L1_IsTight"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_IsTight)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_IsTight)[idx];
            }
            throw;
        };
        accessors["L1_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_P)[idx].pt();
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_P)[idx].pt();
            }
            throw;
        };
        accessors["L1_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_P)[idx].eta();
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_P)[idx].eta();
            }
            throw;
        };
        accessors["L1_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_P)[idx].phi();
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_P)[idx].phi();
            }
            throw;
        };
        accessors["L1_RelIso"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L1_RelIso)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_RelIso)[idx];
            }
            throw;
        };
        accessors["L2_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_Charge)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_Charge)[idx];
            }
            throw;
        };
        accessors["L2_CorrectedD0"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_CorrectedD0)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_CorrectedD0)[idx];
            }
            throw;
        };
        accessors["L2_CorrectedDZ"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_CorrectedDZ)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_CorrectedDZ)[idx];
            }
            throw;
        };
        accessors["L2_EventWeight"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_EventWeight)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_EventWeight)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchDaughter0Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchDaughter0Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchDaughter0Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchDaughter0Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchDaughter1Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchDaughter1Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchDaughter1Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchDaughter1Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother00Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother00Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother00Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother00Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother01Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother01Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother01Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother01Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother10Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother10Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother10Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother10Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother11Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother11Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother11Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother11Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchId)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchId)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchMother0Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchMother0Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchMother0Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchMother0Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchMother1Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchMother1Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchMother1Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchMother1Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_GenMatchStatus)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchStatus)[idx];
            }
            throw;
        };
        accessors["L2_ImpactParameter"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_ImpactParameter)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_ImpactParameter)[idx];
            }
            throw;
        };
        accessors["L2_IsElectron"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_IsElectron)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_IsElectron)[idx];
            }
            throw;
        };
        accessors["L2_IsLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_IsLoose)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_IsLoose)[idx];
            }
            throw;
        };
        accessors["L2_IsMuon"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_IsMuon)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_IsMuon)[idx];
            }
            throw;
        };
        accessors["L2_IsTight"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_IsTight)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_IsTight)[idx];
            }
            throw;
        };
        accessors["L2_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_P)[idx].pt();
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_P)[idx].pt();
            }
            throw;
        };
        accessors["L2_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_P)[idx].eta();
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_P)[idx].eta();
            }
            throw;
        };
        accessors["L2_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_P)[idx].phi();
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_P)[idx].phi();
            }
            throw;
        };
        accessors["L2_RelIso"] = [](Branches *b, int idx, int n) -> float {
            if (ll::Branches* e = dynamic_cast<ll::Branches*>(b)) {
                return (*e->L2_RelIso)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_RelIso)[idx];
            }
            throw;
        };
        accessors["L_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_Charge)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_Charge)[idx];
            }
            throw;
        };
        accessors["L_CorrectedD0"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_CorrectedD0)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_CorrectedD0)[idx];
            }
            throw;
        };
        accessors["L_CorrectedDZ"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_CorrectedDZ)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_CorrectedDZ)[idx];
            }
            throw;
        };
        accessors["L_EventWeight"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_EventWeight)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_EventWeight)[idx];
            }
            throw;
        };
        accessors["L_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchDaughter0Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchDaughter0Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchDaughter0Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchDaughter0Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchDaughter1Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchDaughter1Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchDaughter1Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchDaughter1Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother00Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother00Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother00Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother00Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother01Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother01Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother01Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother01Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother10Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother10Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother10Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother10Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother11Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother11Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother11Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother11Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchId)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchId)[idx];
            }
            throw;
        };
        accessors["L_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchMother0Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchMother0Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchMother0Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchMother0Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchMother1Id)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchMother1Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchMother1Status)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchMother1Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_GenMatchStatus)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchStatus)[idx];
            }
            throw;
        };
        accessors["L_ImpactParameter"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_ImpactParameter)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_ImpactParameter)[idx];
            }
            throw;
        };
        accessors["L_IsElectron"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_IsElectron)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_IsElectron)[idx];
            }
            throw;
        };
        accessors["L_IsLoose"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_IsLoose)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_IsLoose)[idx];
            }
            throw;
        };
        accessors["L_IsMuon"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_IsMuon)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_IsMuon)[idx];
            }
            throw;
        };
        accessors["L_IsTight"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_IsTight)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_IsTight)[idx];
            }
            throw;
        };
        accessors["L_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_P)[idx].pt();
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_P)[idx].pt();
            }
            throw;
        };
        accessors["L_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_P)[idx].eta();
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_P)[idx].eta();
            }
            throw;
        };
        accessors["L_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_P)[idx].phi();
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_P)[idx].phi();
            }
            throw;
        };
        accessors["L_RelIso"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->L_RelIso)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_RelIso)[idx];
            }
            throw;
        };
        accessors["MET_Pt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->MET_P)[idx].pt();
        };
        accessors["MET_Eta"] = [](Branches *b, int idx, int n) -> float {
            return (*b->MET_P)[idx].eta();
        };
        accessors["MET_Phi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->MET_P)[idx].phi();
        };
        accessors["MHT"] = [](Branches *b, int idx, int n) -> float {
            return 0.;
            return (*b->MHT)[idx];
        };
        accessors["MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            return (*b->MomentumRank)[idx];
        };
        accessors["NTruePV"] = [](Branches *b, int idx, int n) -> float {
            return b->NTruePV;
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
        accessors["NumExLooseElectrons"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumExLooseElectrons)[idx];
        };
        accessors["NumExLooseMuons"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumExLooseMuons)[idx];
        };
        accessors["NumGenJets"] = [](Branches *b, int idx, int n) -> float {
            return b->NumGenJets;
        };
        accessors["NumGenTaus"] = [](Branches *b, int idx, int n) -> float {
            return b->NumGenTaus;
        };
        accessors["NumInteractionsBX0"] = [](Branches *b, int idx, int n) -> float {
            return b->NumInteractionsBX0;
        };
        accessors["NumInteractionsBXm1"] = [](Branches *b, int idx, int n) -> float {
            return b->NumInteractionsBXm1;
        };
        accessors["NumInteractionsBXp1"] = [](Branches *b, int idx, int n) -> float {
            return b->NumInteractionsBXp1;
        };
        accessors["NumLooseElectrons"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumLooseElectrons)[idx];
        };
        accessors["NumLooseMuons"] = [](Branches *b, int idx, int n) -> float {
            return (*b->NumLooseMuons)[idx];
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
        accessors["NumVertices"] = [](Branches *b, int idx, int n) -> float {
            return b->NumVertices;
        };
        accessors["T1_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_Charge)[idx];
            }
            throw;
        };
        accessors["T1_DecayMode"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_DecayMode)[idx];
            }
            throw;
        };
        accessors["T1_EmFraction"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_EmFraction)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchDaughter0Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchDaughter0Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchDaughter1Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchDaughter1Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchGrandmother00Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchGrandmother00Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchGrandmother01Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchGrandmother01Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchGrandmother10Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchGrandmother10Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchGrandmother11Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchGrandmother11Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchId)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchMother0Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchMother0Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchMother1Id)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchMother1Status)[idx];
            }
            throw;
        };
        accessors["T1_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_GenMatchStatus)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronDeadECAL"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronDeadECAL)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronLoose)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronLooseMVA2)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronLooseMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronLooseMVA3)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronMVA)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMVA2category"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronMVA2category)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronMVA2raw)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMVA3category"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronMVA3category)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMVA3Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronMVA3raw)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMedium"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronMedium)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMediumMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronMediumMVA2)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronMediumMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronMediumMVA3)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronTight"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronTight)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronTightMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronTightMVA2)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronTightMVA3)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronVLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronVLooseMVA2)[idx];
            }
            throw;
        };
        accessors["T1_AntiElectronVTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstElectronVTightMVA3)[idx];
            }
            throw;
        };
        accessors["T1_AntiMuonLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstMuonLoose)[idx];
            }
            throw;
        };
        accessors["T1_AntiMuonLoose2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstMuonLoose2)[idx];
            }
            throw;
        };
        accessors["T1_AntiMuonMedium"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstMuonMedium)[idx];
            }
            throw;
        };
        accessors["T1_AntiMuonMedium2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstMuonMedium2)[idx];
            }
            throw;
        };
        accessors["T1_AntiMuonTight"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstMuonTight)[idx];
            }
            throw;
        };
        accessors["T1_AntiMuonTight2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSagainstMuonTight2)[idx];
            }
            throw;
        };
        accessors["T1_CombinedIsolationDeltaBetaCorrRaw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyCombinedIsolationDeltaBetaCorrRaw)[idx];
            }
            throw;
        };
        accessors["T1_CombinedIsolationDeltaBetaCorrRaw3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx];
            }
            throw;
        };
        accessors["T1_IsolationMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyIsolationMVA2raw)[idx];
            }
            throw;
        };
        accessors["T1_IsolationMVARaw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyIsolationMVAraw)[idx];
            }
            throw;
        };
        accessors["T1_LooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyLooseCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T1_LooseCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T1_LooseIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyLooseIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T1_LooseIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyLooseIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T1_MediumCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyMediumCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T1_MediumCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T1_MediumIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyMediumIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T1_MediumIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyMediumIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T1_TightCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyTightCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T1_TightCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T1_TightIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyTightIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T1_TightIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyTightIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T1_VLooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSbyVLooseCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T1_DecayModeFinding"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_HPSdecayModeFinding)[idx];
            }
            throw;
        };
        accessors["T1_IsInTheCracks"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_IsInTheCracks)[idx];
            }
            throw;
        };
        accessors["T1_LTIpVtdxy"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_LTIpVtdxy)[idx];
            }
            throw;
        };
        accessors["T1_LTIpVtdxyError"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_LTIpVtdxyError)[idx];
            }
            throw;
        };
        accessors["T1_LTIpVtdz"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_LTIpVtdz)[idx];
            }
            throw;
        };
        accessors["T1_LTIpVtdzError"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_LTIpVtdzError)[idx];
            }
            throw;
        };
        accessors["T1_LTNormChiSqrd"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_LTNormChiSqrd)[idx];
            }
            throw;
        };
        accessors["T1_LTPt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_LTPt)[idx];
            }
            throw;
        };
        accessors["T1_LTValidHits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_LTValidHits)[idx];
            }
            throw;
        };
        accessors["T1_LTvalid"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_LTvalid)[idx];
            }
            throw;
        };
        accessors["T1_LTvx"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_LTvx)[idx];
            }
            throw;
        };
        accessors["T1_LTvy"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_LTvy)[idx];
            }
            throw;
        };
        accessors["T1_LTvz"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_LTvz)[idx];
            }
            throw;
        };
        accessors["T1_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_MomentumRank)[idx];
            }
            throw;
        };
        accessors["T1_NumProngs"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_NProngs)[idx];
            }
            throw;
        };
        accessors["T1_NumSignalGammas"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_NSignalGammas)[idx];
            }
            throw;
        };
        accessors["T1_NumSignalNeutrals"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_NSignalNeutrals)[idx];
            }
            throw;
        };
        accessors["T1_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_P)[idx].pt();
            }
            throw;
        };
        accessors["T1_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_P)[idx].eta();
            }
            throw;
        };
        accessors["T1_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_P)[idx].phi();
            }
            throw;
        };
        accessors["T2_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_Charge)[idx];
            }
            throw;
        };
        accessors["T2_DecayMode"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_DecayMode)[idx];
            }
            throw;
        };
        accessors["T2_EmFraction"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_EmFraction)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchDaughter0Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchDaughter0Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchDaughter1Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchDaughter1Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchGrandmother00Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchGrandmother00Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchGrandmother01Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchGrandmother01Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchGrandmother10Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchGrandmother10Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchGrandmother11Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchGrandmother11Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchId)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchMother0Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchMother0Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchMother1Id)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchMother1Status)[idx];
            }
            throw;
        };
        accessors["T2_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_GenMatchStatus)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronDeadECAL"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronDeadECAL)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronLoose)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronLooseMVA2)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronLooseMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronLooseMVA3)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronMVA)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMVA2category"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronMVA2category)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronMVA2raw)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMVA3category"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronMVA3category)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMVA3Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronMVA3raw)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMedium"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronMedium)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMediumMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronMediumMVA2)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronMediumMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronMediumMVA3)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronTight"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronTight)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronTightMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronTightMVA2)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronTightMVA3)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronVLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronVLooseMVA2)[idx];
            }
            throw;
        };
        accessors["T2_AntiElectronVTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstElectronVTightMVA3)[idx];
            }
            throw;
        };
        accessors["T2_AntiMuonLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstMuonLoose)[idx];
            }
            throw;
        };
        accessors["T2_AntiMuonLoose2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstMuonLoose2)[idx];
            }
            throw;
        };
        accessors["T2_AntiMuonMedium"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstMuonMedium)[idx];
            }
            throw;
        };
        accessors["T2_AntiMuonMedium2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstMuonMedium2)[idx];
            }
            throw;
        };
        accessors["T2_AntiMuonTight"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstMuonTight)[idx];
            }
            throw;
        };
        accessors["T2_AntiMuonTight2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSagainstMuonTight2)[idx];
            }
            throw;
        };
        accessors["T2_CombinedIsolationDeltaBetaCorrRaw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyCombinedIsolationDeltaBetaCorrRaw)[idx];
            }
            throw;
        };
        accessors["T2_CombinedIsolationDeltaBetaCorrRaw3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx];
            }
            throw;
        };
        accessors["T2_IsolationMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyIsolationMVA2raw)[idx];
            }
            throw;
        };
        accessors["T2_IsolationMVARaw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyIsolationMVAraw)[idx];
            }
            throw;
        };
        accessors["T2_LooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyLooseCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T2_LooseCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T2_LooseIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyLooseIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T2_LooseIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyLooseIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T2_MediumCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyMediumCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T2_MediumCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T2_MediumIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyMediumIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T2_MediumIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyMediumIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T2_TightCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyTightCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T2_TightCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T2_TightIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyTightIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T2_TightIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyTightIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T2_VLooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSbyVLooseCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T2_DecayModeFinding"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_HPSdecayModeFinding)[idx];
            }
            throw;
        };
        accessors["T2_IsInTheCracks"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_IsInTheCracks)[idx];
            }
            throw;
        };
        accessors["T2_LTIpVtdxy"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_LTIpVtdxy)[idx];
            }
            throw;
        };
        accessors["T2_LTIpVtdxyError"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_LTIpVtdxyError)[idx];
            }
            throw;
        };
        accessors["T2_LTIpVtdz"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_LTIpVtdz)[idx];
            }
            throw;
        };
        accessors["T2_LTIpVtdzError"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_LTIpVtdzError)[idx];
            }
            throw;
        };
        accessors["T2_LTNormChiSqrd"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_LTNormChiSqrd)[idx];
            }
            throw;
        };
        accessors["T2_LTPt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_LTPt)[idx];
            }
            throw;
        };
        accessors["T2_LTValidHits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_LTValidHits)[idx];
            }
            throw;
        };
        accessors["T2_LTvalid"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_LTvalid)[idx];
            }
            throw;
        };
        accessors["T2_LTvx"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_LTvx)[idx];
            }
            throw;
        };
        accessors["T2_LTvy"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_LTvy)[idx];
            }
            throw;
        };
        accessors["T2_LTvz"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_LTvz)[idx];
            }
            throw;
        };
        accessors["T2_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_MomentumRank)[idx];
            }
            throw;
        };
        accessors["T2_NumProngs"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_NProngs)[idx];
            }
            throw;
        };
        accessors["T2_NumSignalGammas"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_NSignalGammas)[idx];
            }
            throw;
        };
        accessors["T2_NumSignalNeutrals"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_NSignalNeutrals)[idx];
            }
            throw;
        };
        accessors["T2_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_P)[idx].pt();
            }
            throw;
        };
        accessors["T2_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_P)[idx].eta();
            }
            throw;
        };
        accessors["T2_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_P)[idx].phi();
            }
            throw;
        };
        accessors["T_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_Charge)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_Charge)[idx];
            }
            throw;
        };
        accessors["T_DecayMode"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_DecayMode)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_DecayMode)[idx];
            }
            throw;
        };
        accessors["T_EmFraction"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_EmFraction)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_EmFraction)[idx];
            }
            throw;
        };
        accessors["T_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchDaughter0Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchDaughter0Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchDaughter0Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchDaughter0Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchDaughter1Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchDaughter1Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchDaughter1Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchDaughter1Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother00Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother00Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother00Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother00Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother01Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother01Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother01Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother01Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother10Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother10Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother10Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother10Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother11Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother11Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother11Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother11Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchId)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchId)[idx];
            }
            throw;
        };
        accessors["T_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchMother0Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchMother0Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchMother0Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchMother0Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchMother1Id)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchMother1Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchMother1Status)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchMother1Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_GenMatchStatus)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchStatus)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronDeadECAL"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronDeadECAL)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronDeadECAL)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronLoose"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronLoose)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronLoose)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronLooseMVA2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronLooseMVA2)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronLooseMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronLooseMVA3)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronLooseMVA3)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMVA"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMVA)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMVA)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMVA2category"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMVA2category)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMVA2category)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMVA2raw)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMVA2raw)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMVA3category"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMVA3category)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMVA3category)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMVA3Raw"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMVA3raw)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMVA3raw)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMedium"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMedium)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMedium)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMediumMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMediumMVA2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMediumMVA2)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMediumMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMediumMVA3)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMediumMVA3)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronTight"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronTight)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronTight)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronTightMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronTightMVA2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronTightMVA2)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronTightMVA3)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronTightMVA3)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronVLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronVLooseMVA2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronVLooseMVA2)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronVTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstElectronVTightMVA3)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronVTightMVA3)[idx];
            }
            throw;
        };
        accessors["T_AntiMuonLoose"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstMuonLoose)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstMuonLoose)[idx];
            }
            throw;
        };
        accessors["T_AntiMuonLoose2"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstMuonLoose2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstMuonLoose2)[idx];
            }
            throw;
        };
        accessors["T_AntiMuonMedium"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstMuonMedium)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstMuonMedium)[idx];
            }
            throw;
        };
        accessors["T_AntiMuonMedium2"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstMuonMedium2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstMuonMedium2)[idx];
            }
            throw;
        };
        accessors["T_AntiMuonTight"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstMuonTight)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstMuonTight)[idx];
            }
            throw;
        };
        accessors["T_AntiMuonTight2"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSagainstMuonTight2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstMuonTight2)[idx];
            }
            throw;
        };
        accessors["T_CombinedIsolationDeltaBetaCorrRaw"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyCombinedIsolationDeltaBetaCorrRaw)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyCombinedIsolationDeltaBetaCorrRaw)[idx];
            }
            throw;
        };
        accessors["T_CombinedIsolationDeltaBetaCorrRaw3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx];
            }
            throw;
        };
        accessors["T_IsolationMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyIsolationMVA2raw)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyIsolationMVA2raw)[idx];
            }
            throw;
        };
        accessors["T_IsolationMVARaw"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyIsolationMVAraw)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyIsolationMVAraw)[idx];
            }
            throw;
        };
        accessors["T_LooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyLooseCombinedIsolationDeltaBetaCorr)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyLooseCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T_LooseCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T_LooseIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyLooseIsolationMVA)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyLooseIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T_LooseIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyLooseIsolationMVA2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyLooseIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T_MediumCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyMediumCombinedIsolationDeltaBetaCorr)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyMediumCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T_MediumCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T_MediumIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyMediumIsolationMVA)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyMediumIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T_MediumIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyMediumIsolationMVA2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyMediumIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T_TightCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyTightCombinedIsolationDeltaBetaCorr)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyTightCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T_TightCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T_TightIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyTightIsolationMVA)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyTightIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T_TightIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyTightIsolationMVA2)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyTightIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T_VLooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSbyVLooseCombinedIsolationDeltaBetaCorr)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyVLooseCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T_DecayModeFinding"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_HPSdecayModeFinding)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSdecayModeFinding)[idx];
            }
            throw;
        };
        accessors["T_IsInTheCracks"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_IsInTheCracks)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_IsInTheCracks)[idx];
            }
            throw;
        };
        accessors["T_LTIpVtdxy"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_LTIpVtdxy)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTIpVtdxy)[idx];
            }
            throw;
        };
        accessors["T_LTIpVtdxyError"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_LTIpVtdxyError)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTIpVtdxyError)[idx];
            }
            throw;
        };
        accessors["T_LTIpVtdz"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_LTIpVtdz)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTIpVtdz)[idx];
            }
            throw;
        };
        accessors["T_LTIpVtdzError"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_LTIpVtdzError)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTIpVtdzError)[idx];
            }
            throw;
        };
        accessors["T_LTNormChiSqrd"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_LTNormChiSqrd)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTNormChiSqrd)[idx];
            }
            throw;
        };
        accessors["T_LTPt"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_LTPt)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTPt)[idx];
            }
            throw;
        };
        accessors["T_LTValidHits"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_LTValidHits)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTValidHits)[idx];
            }
            throw;
        };
        accessors["T_LTvalid"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_LTvalid)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTvalid)[idx];
            }
            throw;
        };
        accessors["T_LTvx"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_LTvx)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTvx)[idx];
            }
            throw;
        };
        accessors["T_LTvy"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_LTvy)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTvy)[idx];
            }
            throw;
        };
        accessors["T_LTvz"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_LTvz)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTvz)[idx];
            }
            throw;
        };
        accessors["T_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_MomentumRank)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_MomentumRank)[idx];
            }
            throw;
        };
        accessors["T_NumProngs"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_NProngs)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_NProngs)[idx];
            }
            throw;
        };
        accessors["T_NumSignalGammas"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_NSignalGammas)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_NSignalGammas)[idx];
            }
            throw;
        };
        accessors["T_NumSignalNeutrals"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_NSignalNeutrals)[idx];
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_NSignalNeutrals)[idx];
            }
            throw;
        };
        accessors["T_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_P)[idx].pt();
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_P)[idx].pt();
            }
            throw;
        };
        accessors["T_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_P)[idx].eta();
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_P)[idx].eta();
            }
            throw;
        };
        accessors["T_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return (*e->T_P)[idx].phi();
            } else if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_P)[idx].phi();
            }
            throw;
        };
        accessors["TriggerEventWeight"] = [](Branches *b, int idx, int n) -> float {
            return (*b->TriggerEventWeight)[idx];
        };
        accessors["V_NormChiSquared"] = [](Branches *b, int idx, int n) -> float {
            return (*b->V_NormChiSquared)[idx];
        };
        accessors["V_NumDOF"] = [](Branches *b, int idx, int n) -> float {
            return (*b->V_NumDOF)[idx];
        };
        accessors["V_Rho"] = [](Branches *b, int idx, int n) -> float {
            return (*b->V_Rho)[idx];
        };
        accessors["V_Xcoord"] = [](Branches *b, int idx, int n) -> float {
            return (*b->V_Xcoord)[idx];
        };
        accessors["V_XcoordErr"] = [](Branches *b, int idx, int n) -> float {
            return (*b->V_XcoordErr)[idx];
        };
        accessors["V_Ycoord"] = [](Branches *b, int idx, int n) -> float {
            return (*b->V_Ycoord)[idx];
        };
        accessors["V_YcoordErr"] = [](Branches *b, int idx, int n) -> float {
            return (*b->V_YcoordErr)[idx];
        };
        accessors["V_Zcoord"] = [](Branches *b, int idx, int n) -> float {
            return (*b->V_Zcoord)[idx];
        };
        accessors["V_ZcoordErr"] = [](Branches *b, int idx, int n) -> float {
            return (*b->V_ZcoordErr)[idx];
        };
        accessors["bQuarkCount"] = [](Branches *b, int idx, int n) -> float {
            return b->bQuarkCount;
        };
        accessors["cQuarkCount"] = [](Branches *b, int idx, int n) -> float {
            return b->cQuarkCount;
        };
        accessors["event"] = [](Branches *b, int idx, int n) -> float {
            return b->event;
        };
        accessors["higgsDecayMode"] = [](Branches *b, int idx, int n) -> float {
            return b->higgsDecayMode;
        };
        accessors["lumi"] = [](Branches *b, int idx, int n) -> float {
            return b->lumi;
        };
        accessors["puWeight"] = [](Branches *b, int idx, int n) -> float {
            return b->puWeight;
        };
        accessors["puWeightDown"] = [](Branches *b, int idx, int n) -> float {
            return b->puWeightDown;
        };
        accessors["puWeightUp"] = [](Branches *b, int idx, int n) -> float {
            return b->puWeightUp;
        };
        accessors["q2WeightDown"] = [](Branches *b, int idx, int n) -> float {
            return b->q2WeightDown;
        };
        accessors["q2WeightUp"] = [](Branches *b, int idx, int n) -> float {
            return b->q2WeightUp;
        };
        accessors["run"] = [](Branches *b, int idx, int n) -> float {
            return b->run;
        };
        accessors["topPtWeight"] = [](Branches *b, int idx, int n) -> float {
            return b->topPtWeight;
        };
        accessors["topPtWeightDown"] = [](Branches *b, int idx, int n) -> float {
            return b->topPtWeightDown;
        };
        accessors["topPtWeightUp"] = [](Branches *b, int idx, int n) -> float {
            return b->topPtWeightUp;
        };
        // >>> End attr <<<

        setup = true;
    }
}

NamespaceImp(roast)
