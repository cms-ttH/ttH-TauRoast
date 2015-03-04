#ifdef FOOBARBAD
// vim: ts=4:sw=4:et:sta:foldmethod=marker

#include <map>

#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Functor.h"

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

    class HiggsMass {
        public:
            HiggsMass(const LorentzVector& j1, const LorentzVector& j2) : j1_(j1), j2_(j2) {};
            double operator()(const double *x) const { return Eval(x); };
            double Eval(const double *x) const {
                auto nu1 = j1_ / j1_.P() * x[0];
                nu1.SetE(fabs(x[0]));
                auto nu2 = j2_ / j2_.P() * x[1];
                nu2.SetE(fabs(x[1]));
                double tmp = 125.6 - (j1_ + j2_ + nu1 + nu2).M();
                // double tmp = j1_.M2() + j2_.M2() - 125.6 * 125.6
                    // + 2 * (j1_.Dot(j2_) + j1_.Dot(nu1) + j1_.Dot(nu2)
                            // + j2_.Dot(nu1) + j2_.Dot(nu2) + nu1.Dot(nu2));
                // double tmp = nu1 * (j1_.E() + j2_.E()) + nu2 * (j1_.E() + j2_.E())
                             // + nu1 * nu2 - 2 * (j1_.P() * nu1 + j2_.P() * nu2)
                             // - 2 * angle * (
                                     // j1_.P() * j2_.P() + j1_.P() * nu2 + nu1 * j2_.P() + nu1 * nu2)
                             // - 125.6 * 125.6 + j1_.M2() + j2_.M2();
                return tmp * tmp;
            };
        private:
            const LorentzVector j1_;
            const LorentzVector j2_;
    };

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

        accessors["L_MatchAbsId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return abs((*e->L_GenMatchId)[idx]);
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return abs((*e->L_GenMatchId)[idx]);
            }
            throw "";
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

        accessors["L_ParentAbsId"] = [](Branches *b, int idx, int n) -> float {
            if (tl::Branches* e = dynamic_cast<tl::Branches*>(b)) {
                return abs((*e->L_GenMatchMother0Id)[idx]);
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return abs((*e->L_GenMatchMother0Id)[idx]);
            }
            throw "";
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

        accessors["FinalBDTG"] = [](Branches *b, int idx, int n) -> float {
            return MVABase::gMVA["FinalBDTG"] ? MVABase::gMVA["FinalBDTG"]->Evaluate(b, idx) : 0.;
        };

        // accessors["PassQCDVeto"] = [](Branches *b, int idx, int n) -> float {
            // tl::Branches* e = dynamic_cast<tl::Branches*>(b);
            // auto mt = (*e->TL_LeptonMt2)[idx];
            // auto met = e->Ev_MET;
            // return (met > 50) || (mt > 50);
        // };

        accessors["PassZMask3"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            auto dilmass = ((*e->L1_P)[idx] + (*e->L2_P)[idx]).M();
            auto met = (*e->MET_P)[idx].pt();
            bool zpeak = dilmass > 82 && dilmass < 100 &&
                    met - 40 < 30 / 9. * (dilmass - 82) &&
                    met - 40 < 30 - 30 / 9. * (dilmass - 91);
            return !zpeak;
        };

        accessors["H_Pt"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            auto t1 = (*e->T1_P)[idx];
            auto t2 = (*e->T2_P)[idx];

            float p_nu1 = (1.777 * 1.777 - t1.M2()) / (t1.E() - 2 * t1.P());
            float p_nu2 = (1.777 * 1.777 - t2.M2()) / (t2.E() - 2 * t2.P());

            auto nu1 = t1 / t1.P() * p_nu1;
            nu1.SetE(p_nu1);

            auto nu2 = t2 / t2.P() * p_nu2;
            nu1.SetE(p_nu2);

            auto h = t1 + t2 + nu1 + nu2;
            return h.Pt();
        };

        accessors["H_M"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            auto t1 = (*e->T1_P)[idx];
            auto t2 = (*e->T2_P)[idx];

            float p_nu1 = (1.777 * 1.777 - t1.M2()) / (t1.E() - 2 * t1.P());
            float p_nu2 = (1.777 * 1.777 - t2.M2()) / (t2.E() - 2 * t2.P());

            auto nu1 = t1 / t1.P() * p_nu1;
            nu1.SetE(p_nu1);

            auto nu2 = t2 / t2.P() * p_nu2;
            nu1.SetE(p_nu2);

            auto h = t1 + t2 + nu1 + nu2;

            return h.M();
        };

        accessors["H_MET"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            auto t1 = (*e->T1_P)[idx];
            auto t2 = (*e->T2_P)[idx];

            if (e->IsCached("H_MET")) {
                return e->GetCached("H_MET");
            } else {
                const float p_nu1 = (1.777 * 1.777 - t1.M2()) / (t1.E() - 2 * t1.P());
                const float p_nu2 = (1.777 * 1.777 - t2.M2()) / (t2.E() - 2 * t2.P());

                double step[2] = {.5, .5};
                double variable[2] = {p_nu1, p_nu2};

                HiggsMass h(t1, t2);
                ROOT::Math::Functor fct(h, 2);

                ROOT::Minuit2::Minuit2Minimizer min(ROOT::Minuit2::kCombined);
                min.SetFunction(fct);
                min.SetVariable(0, "nu1", variable[0], step[0]);
                min.SetVariable(1, "nu2", variable[1], step[1]);
                min.Minimize();
                min.PrintResults();

                const double *ps = min.X();

                auto nu1 = t1 / t1.P() * ps[0];
                nu1.SetE(fabs(ps[0]));
                auto nu2 = t2 / t2.P() * ps[1];
                nu2.SetE(fabs(ps[1]));

                std::cout << "m_H " << (nu1 + nu2 + t1 + t2).M() << std::endl;
                std::cout << "p_T " << (nu1 + nu2 + t1 + t2).Pt() << std::endl;
                std::cout << (nu1 + nu2).Pt() << std::endl;
                std::cout << "------" << std::endl;

                return (nu1 + nu2).Pt();
            }
        };

        accessors["DeltaPzeta"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            auto t1 = (*e->T1_P)[idx];
            auto t2 = (*e->T2_P)[idx];
            auto met = (*e->MET_P)[idx];

            auto dir = .5 * (t1.Phi() + t2.Phi());

            return ((1 + 1.78) * t1.Pt() * cos(t1.Phi() - dir) + (1 + 1.78) * t2.Pt() * cos(t2.Phi() - dir) + met.Pt() * cos(met.Phi() - dir));
        };

        accessors["Pzeta"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            auto t1 = (*e->T1_P)[idx];
            auto t2 = (*e->T2_P)[idx];
            auto met = (*e->MET_P)[idx];

            auto dir = .5 * (t1.Phi() + t2.Phi());

            return (t1.Pt() * cos(t1.Phi() - dir) + t2.Pt() * cos(t2.Phi() - dir) + met.Pt() * cos(met.Phi() - dir));
        };

        accessors["PzetaVis"] = [](Branches *b, int idx, int n) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            auto t1 = (*e->T1_P)[idx];
            auto t2 = (*e->T2_P)[idx];

            auto dir = .5 * (t1.Phi() + t2.Phi());

            return (t1.Pt() * cos(t1.Phi() - dir) + t2.Pt() * cos(t2.Phi() - dir));
        };

        setup = true;
    }
}

NamespaceImp(roast)
#endif
