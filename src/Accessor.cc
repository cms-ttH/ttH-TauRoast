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

        // generated accessors {{{1
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
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauAntiElectronIndex(idx);
        };
        accessors["T_AntiElectronIndexMVA2"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauAntiElectronIndexMVA2(idx);
        };
        accessors["T_AntiElectronIndexMVA3"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauAntiElectronIndexMVA3(idx);
        };
        accessors["T_AntiMuonIndex"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauAntiMuonIndex(idx);
        };
        accessors["T_AntiMuonIndex2"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauAntiMuonIndex2(idx);
        };
        accessors["T_IsolationIndex"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauIsolationIndex(idx);
        };
        accessors["T_IsolationIndex3Hits"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauIsolationIndex3Hits(idx);
        };
        accessors["T_IsolationIndexMVA"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauIsolationIndexMVA(idx);
        };
        accessors["T_IsolationIndexMVA2"] = [](Branches *b, int idx, int n) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauIsolationIndexMVA2(idx);
        };
        accessors["T1_Mt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return sqrt(2 * (*e->T1_P)[idx].pt() * (*e->MET_P)[idx].pt() * (1 - cos((*e->T1_P)[idx].phi() - (*e->MET_P)[idx].phi())));
            }
            throw;
        };
        accessors["T2_Mt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return sqrt(2 * (*e->T2_P)[idx].pt() * (*e->MET_P)[idx].pt() * (1 - cos((*e->T2_P)[idx].phi() - (*e->MET_P)[idx].phi())));
            }
            throw;
        };
        accessors["T_Mt"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return sqrt(2 * (*e->T_P)[idx].pt() * (*e->MET_P)[idx].pt() * (1 - cos((*e->T_P)[idx].phi() - (*e->MET_P)[idx].phi())));
            }
            throw;
        };
        accessors["L1_Mt"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return sqrt(2 * (*e->L1_P)[idx].pt() * (*e->MET_P)[idx].pt() * (1 - cos((*e->L1_P)[idx].phi() - (*e->MET_P)[idx].phi())));
            }
            throw;
        };
        accessors["L2_Mt"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return sqrt(2 * (*e->L2_P)[idx].pt() * (*e->MET_P)[idx].pt() * (1 - cos((*e->L2_P)[idx].phi() - (*e->MET_P)[idx].phi())));
            }
            throw;
        };
        accessors["L_Mt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return sqrt(2 * (*e->L_P)[idx].pt() * (*e->MET_P)[idx].pt() * (1 - cos((*e->L_P)[idx].phi() - (*e->MET_P)[idx].phi())));
            }
            throw;
        };
        accessors["TT_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->T2_P)[idx].phi());
            }
            throw;
        };
        accessors["TT_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->T2_P)[idx]);
            }
            throw;
        };
        accessors["TT_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->T2_P)[idx]).M();
            }
            throw;
        };
        accessors["TT_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->T2_P)[idx]).Pt();
            }
            throw;
        };
        accessors["T1L_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->L_P)[idx].phi());
            }
            throw;
        };
        accessors["T1L_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->L_P)[idx]);
            }
            throw;
        };
        accessors["T1L_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->L_P)[idx]).M();
            }
            throw;
        };
        accessors["T1L_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->L_P)[idx]).Pt();
            }
            throw;
        };
        accessors["T1MET_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->MET_P)[idx].phi());
            }
            throw;
        };
        accessors["T1J_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->J_P)[idx].at(n).phi());
            }
            throw;
        };
        accessors["T1LJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->J_P)[idx].at(0).phi());
            }
            throw;
        };
        accessors["T1SubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->J_P)[idx].at(1).phi());
            }
            throw;
        };
        accessors["T1SubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->J_P)[idx].at(2).phi());
            }
            throw;
        };
        accessors["T1SubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->J_P)[idx].at(3).phi());
            }
            throw;
        };
        accessors["T1TaggedJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T1TaggedLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T1TaggedSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T1TaggedSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T1TaggedSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T1UntaggedJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T1UntaggedLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T1UntaggedSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T1UntaggedSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T1UntaggedSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T1J_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->J_P)[idx].at(n));
            }
            throw;
        };
        accessors["T1LJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->J_P)[idx].at(0));
            }
            throw;
        };
        accessors["T1SubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->J_P)[idx].at(1));
            }
            throw;
        };
        accessors["T1SubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->J_P)[idx].at(2));
            }
            throw;
        };
        accessors["T1SubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->J_P)[idx].at(3));
            }
            throw;
        };
        accessors["T1TaggedJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T1TaggedLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T1TaggedSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T1TaggedSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T1TaggedSubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T1UntaggedJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T1UntaggedLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T1UntaggedSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T1UntaggedSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T1UntaggedSubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T1J_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->J_P)[idx].at(n)).M();
            }
            throw;
        };
        accessors["T1LJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->J_P)[idx].at(0)).M();
            }
            throw;
        };
        accessors["T1SubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->J_P)[idx].at(1)).M();
            }
            throw;
        };
        accessors["T1SubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->J_P)[idx].at(2)).M();
            }
            throw;
        };
        accessors["T1SubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->J_P)[idx].at(3)).M();
            }
            throw;
        };
        accessors["T1TaggedJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T1TaggedLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T1TaggedSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T1TaggedSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T1TaggedSubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T1UntaggedJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T1UntaggedLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T1UntaggedSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T1UntaggedSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T1UntaggedSubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T2L_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->L_P)[idx].phi());
            }
            throw;
        };
        accessors["T2L_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->L_P)[idx]);
            }
            throw;
        };
        accessors["T2L_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->L_P)[idx]).M();
            }
            throw;
        };
        accessors["T2L_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->L_P)[idx]).Pt();
            }
            throw;
        };
        accessors["T2MET_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->MET_P)[idx].phi());
            }
            throw;
        };
        accessors["T2J_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->J_P)[idx].at(n).phi());
            }
            throw;
        };
        accessors["T2LJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->J_P)[idx].at(0).phi());
            }
            throw;
        };
        accessors["T2SubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->J_P)[idx].at(1).phi());
            }
            throw;
        };
        accessors["T2SubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->J_P)[idx].at(2).phi());
            }
            throw;
        };
        accessors["T2SubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->J_P)[idx].at(3).phi());
            }
            throw;
        };
        accessors["T2TaggedJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T2TaggedLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T2TaggedSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T2TaggedSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T2TaggedSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T2UntaggedJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T2UntaggedLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T2UntaggedSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T2UntaggedSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T2UntaggedSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->T2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["T2J_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->J_P)[idx].at(n));
            }
            throw;
        };
        accessors["T2LJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->J_P)[idx].at(0));
            }
            throw;
        };
        accessors["T2SubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->J_P)[idx].at(1));
            }
            throw;
        };
        accessors["T2SubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->J_P)[idx].at(2));
            }
            throw;
        };
        accessors["T2SubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->J_P)[idx].at(3));
            }
            throw;
        };
        accessors["T2TaggedJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T2TaggedLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T2TaggedSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T2TaggedSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T2TaggedSubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T2UntaggedJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T2UntaggedLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T2UntaggedSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T2UntaggedSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T2UntaggedSubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->T2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["T2J_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->J_P)[idx].at(n)).M();
            }
            throw;
        };
        accessors["T2LJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->J_P)[idx].at(0)).M();
            }
            throw;
        };
        accessors["T2SubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->J_P)[idx].at(1)).M();
            }
            throw;
        };
        accessors["T2SubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->J_P)[idx].at(2)).M();
            }
            throw;
        };
        accessors["T2SubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->J_P)[idx].at(3)).M();
            }
            throw;
        };
        accessors["T2TaggedJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T2TaggedLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T2TaggedSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T2TaggedSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T2TaggedSubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T2UntaggedJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T2UntaggedLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T2UntaggedSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T2UntaggedSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["T2UntaggedSubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->T2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["TL1_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->L1_P)[idx].phi());
            }
            throw;
        };
        accessors["TL1_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->L1_P)[idx]);
            }
            throw;
        };
        accessors["TL1_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->L1_P)[idx]).M();
            }
            throw;
        };
        accessors["TL1_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->L1_P)[idx]).Pt();
            }
            throw;
        };
        accessors["TL2_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->L2_P)[idx].phi());
            }
            throw;
        };
        accessors["TL2_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->L2_P)[idx]);
            }
            throw;
        };
        accessors["TL2_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->L2_P)[idx]).M();
            }
            throw;
        };
        accessors["TL2_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->L2_P)[idx]).Pt();
            }
            throw;
        };
        accessors["TMET_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->MET_P)[idx].phi());
            }
            throw;
        };
        accessors["TJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->J_P)[idx].at(n).phi());
            }
            throw;
        };
        accessors["TLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->J_P)[idx].at(0).phi());
            }
            throw;
        };
        accessors["TSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->J_P)[idx].at(1).phi());
            }
            throw;
        };
        accessors["TSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->J_P)[idx].at(2).phi());
            }
            throw;
        };
        accessors["TSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->J_P)[idx].at(3).phi());
            }
            throw;
        };
        accessors["TTaggedJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["TTaggedLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["TTaggedSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["TTaggedSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["TTaggedSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["TUntaggedJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["TUntaggedLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["TUntaggedSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["TUntaggedSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["TUntaggedSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->T_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["TJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->J_P)[idx].at(n));
            }
            throw;
        };
        accessors["TLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->J_P)[idx].at(0));
            }
            throw;
        };
        accessors["TSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->J_P)[idx].at(1));
            }
            throw;
        };
        accessors["TSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->J_P)[idx].at(2));
            }
            throw;
        };
        accessors["TSubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->J_P)[idx].at(3));
            }
            throw;
        };
        accessors["TTaggedJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["TTaggedLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["TTaggedSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["TTaggedSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["TTaggedSubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["TUntaggedJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["TUntaggedLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["TUntaggedSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["TUntaggedSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["TUntaggedSubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->T_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["TJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->J_P)[idx].at(n)).M();
            }
            throw;
        };
        accessors["TLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->J_P)[idx].at(0)).M();
            }
            throw;
        };
        accessors["TSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->J_P)[idx].at(1)).M();
            }
            throw;
        };
        accessors["TSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->J_P)[idx].at(2)).M();
            }
            throw;
        };
        accessors["TSubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->J_P)[idx].at(3)).M();
            }
            throw;
        };
        accessors["TTaggedJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["TTaggedLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["TTaggedSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["TTaggedSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["TTaggedSubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["TUntaggedJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["TUntaggedLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["TUntaggedSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["TUntaggedSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["TUntaggedSubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->T_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["LL_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->L2_P)[idx].phi());
            }
            throw;
        };
        accessors["LL_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->L2_P)[idx]);
            }
            throw;
        };
        accessors["LL_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->L2_P)[idx]).M();
            }
            throw;
        };
        accessors["LL_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->L2_P)[idx]).Pt();
            }
            throw;
        };
        accessors["L1MET_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->MET_P)[idx].phi());
            }
            throw;
        };
        accessors["L1J_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->J_P)[idx].at(n).phi());
            }
            throw;
        };
        accessors["L1LJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->J_P)[idx].at(0).phi());
            }
            throw;
        };
        accessors["L1SubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->J_P)[idx].at(1).phi());
            }
            throw;
        };
        accessors["L1SubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->J_P)[idx].at(2).phi());
            }
            throw;
        };
        accessors["L1SubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->J_P)[idx].at(3).phi());
            }
            throw;
        };
        accessors["L1TaggedJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L1TaggedLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L1TaggedSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L1TaggedSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L1TaggedSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L1UntaggedJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L1UntaggedLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L1UntaggedSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L1UntaggedSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L1UntaggedSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L1_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L1J_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->J_P)[idx].at(n));
            }
            throw;
        };
        accessors["L1LJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->J_P)[idx].at(0));
            }
            throw;
        };
        accessors["L1SubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->J_P)[idx].at(1));
            }
            throw;
        };
        accessors["L1SubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->J_P)[idx].at(2));
            }
            throw;
        };
        accessors["L1SubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->J_P)[idx].at(3));
            }
            throw;
        };
        accessors["L1TaggedJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L1TaggedLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L1TaggedSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L1TaggedSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L1TaggedSubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L1UntaggedJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L1UntaggedLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L1UntaggedSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L1UntaggedSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L1UntaggedSubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L1_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L1J_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->J_P)[idx].at(n)).M();
            }
            throw;
        };
        accessors["L1LJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->J_P)[idx].at(0)).M();
            }
            throw;
        };
        accessors["L1SubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->J_P)[idx].at(1)).M();
            }
            throw;
        };
        accessors["L1SubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->J_P)[idx].at(2)).M();
            }
            throw;
        };
        accessors["L1SubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->J_P)[idx].at(3)).M();
            }
            throw;
        };
        accessors["L1TaggedJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L1TaggedLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L1TaggedSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L1TaggedSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L1TaggedSubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L1UntaggedJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L1UntaggedLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L1UntaggedSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L1UntaggedSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L1UntaggedSubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L1_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L2MET_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->MET_P)[idx].phi());
            }
            throw;
        };
        accessors["L2J_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->J_P)[idx].at(n).phi());
            }
            throw;
        };
        accessors["L2LJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->J_P)[idx].at(0).phi());
            }
            throw;
        };
        accessors["L2SubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->J_P)[idx].at(1).phi());
            }
            throw;
        };
        accessors["L2SubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->J_P)[idx].at(2).phi());
            }
            throw;
        };
        accessors["L2SubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->J_P)[idx].at(3).phi());
            }
            throw;
        };
        accessors["L2TaggedJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L2TaggedLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L2TaggedSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L2TaggedSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L2TaggedSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L2UntaggedJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L2UntaggedLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L2UntaggedSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L2UntaggedSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L2UntaggedSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return cos((*e->L2_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["L2J_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L2_P)[idx], (*e->J_P)[idx].at(n));
            }
            throw;
        };
        accessors["L2LJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L2_P)[idx], (*e->J_P)[idx].at(0));
            }
            throw;
        };
        accessors["L2SubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L2_P)[idx], (*e->J_P)[idx].at(1));
            }
            throw;
        };
        accessors["L2SubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L2_P)[idx], (*e->J_P)[idx].at(2));
            }
            throw;
        };
        accessors["L2SubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L2_P)[idx], (*e->J_P)[idx].at(3));
            }
            throw;
        };
        accessors["L2TaggedJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L2TaggedLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L2TaggedSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L2TaggedSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L2TaggedSubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L2UntaggedJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L2UntaggedLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L2UntaggedSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L2UntaggedSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L2UntaggedSubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return reco::deltaR((*e->L2_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["L2J_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L2_P)[idx] + (*e->J_P)[idx].at(n)).M();
            }
            throw;
        };
        accessors["L2LJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L2_P)[idx] + (*e->J_P)[idx].at(0)).M();
            }
            throw;
        };
        accessors["L2SubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L2_P)[idx] + (*e->J_P)[idx].at(1)).M();
            }
            throw;
        };
        accessors["L2SubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L2_P)[idx] + (*e->J_P)[idx].at(2)).M();
            }
            throw;
        };
        accessors["L2SubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L2_P)[idx] + (*e->J_P)[idx].at(3)).M();
            }
            throw;
        };
        accessors["L2TaggedJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L2TaggedLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L2TaggedSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L2TaggedSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L2TaggedSubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L2UntaggedJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L2UntaggedLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L2UntaggedSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L2UntaggedSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["L2UntaggedSubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return ((*e->L2_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["LMET_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->MET_P)[idx].phi());
            }
            throw;
        };
        accessors["LJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->J_P)[idx].at(n).phi());
            }
            throw;
        };
        accessors["LLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->J_P)[idx].at(0).phi());
            }
            throw;
        };
        accessors["LSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->J_P)[idx].at(1).phi());
            }
            throw;
        };
        accessors["LSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->J_P)[idx].at(2).phi());
            }
            throw;
        };
        accessors["LSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->J_P)[idx].at(3).phi());
            }
            throw;
        };
        accessors["LTaggedJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["LTaggedLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["LTaggedSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["LTaggedSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["LTaggedSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["LUntaggedJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["LUntaggedLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["LUntaggedSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["LUntaggedSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["LUntaggedSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return cos((*e->L_P)[idx].phi() - (*e->J_P)[idx].at(i).phi());
            }
            throw;
        };
        accessors["LJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->L_P)[idx], (*e->J_P)[idx].at(n));
            }
            throw;
        };
        accessors["LLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->L_P)[idx], (*e->J_P)[idx].at(0));
            }
            throw;
        };
        accessors["LSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->L_P)[idx], (*e->J_P)[idx].at(1));
            }
            throw;
        };
        accessors["LSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->L_P)[idx], (*e->J_P)[idx].at(2));
            }
            throw;
        };
        accessors["LSubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->L_P)[idx], (*e->J_P)[idx].at(3));
            }
            throw;
        };
        accessors["LTaggedJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->L_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["LTaggedLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->L_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["LTaggedSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->L_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["LTaggedSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->L_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["LTaggedSubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->L_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["LUntaggedJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->L_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["LUntaggedLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->L_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["LUntaggedSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->L_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["LUntaggedSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->L_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["LUntaggedSubSubSubLJ_DeltaR"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return reco::deltaR((*e->L_P)[idx], (*e->J_P)[idx].at(i));
            }
            throw;
        };
        accessors["LJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->L_P)[idx] + (*e->J_P)[idx].at(n)).M();
            }
            throw;
        };
        accessors["LLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->L_P)[idx] + (*e->J_P)[idx].at(0)).M();
            }
            throw;
        };
        accessors["LSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->L_P)[idx] + (*e->J_P)[idx].at(1)).M();
            }
            throw;
        };
        accessors["LSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->L_P)[idx] + (*e->J_P)[idx].at(2)).M();
            }
            throw;
        };
        accessors["LSubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->L_P)[idx] + (*e->J_P)[idx].at(3)).M();
            }
            throw;
        };
        accessors["LTaggedJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->L_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["LTaggedLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->L_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["LTaggedSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->L_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["LTaggedSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->L_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["LTaggedSubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->L_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["LUntaggedJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->L_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["LUntaggedLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->L_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["LUntaggedSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->L_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["LUntaggedSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->L_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["LUntaggedSubSubSubLJ_VisibleMass"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return ((*e->L_P)[idx] + (*e->J_P)[idx].at(i)).M();
            }
            throw;
        };
        accessors["METJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            return cos((*b->MET_P)[idx].phi() - (*b->J_P)[idx].at(n).phi());
        };
        accessors["METLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            return cos((*b->MET_P)[idx].phi() - (*b->J_P)[idx].at(0).phi());
        };
        accessors["METSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            return cos((*b->MET_P)[idx].phi() - (*b->J_P)[idx].at(1).phi());
        };
        accessors["METSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            return cos((*b->MET_P)[idx].phi() - (*b->J_P)[idx].at(2).phi());
        };
        accessors["METSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            return cos((*b->MET_P)[idx].phi() - (*b->J_P)[idx].at(3).phi());
        };
        accessors["METTaggedJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return cos((*b->MET_P)[idx].phi() - (*b->J_P)[idx].at(i).phi());
        };
        accessors["METTaggedLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return cos((*b->MET_P)[idx].phi() - (*b->J_P)[idx].at(i).phi());
        };
        accessors["METTaggedSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return cos((*b->MET_P)[idx].phi() - (*b->J_P)[idx].at(i).phi());
        };
        accessors["METTaggedSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return cos((*b->MET_P)[idx].phi() - (*b->J_P)[idx].at(i).phi());
        };
        accessors["METTaggedSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return cos((*b->MET_P)[idx].phi() - (*b->J_P)[idx].at(i).phi());
        };
        accessors["METUntaggedJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return cos((*b->MET_P)[idx].phi() - (*b->J_P)[idx].at(i).phi());
        };
        accessors["METUntaggedLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return cos((*b->MET_P)[idx].phi() - (*b->J_P)[idx].at(i).phi());
        };
        accessors["METUntaggedSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return cos((*b->MET_P)[idx].phi() - (*b->J_P)[idx].at(i).phi());
        };
        accessors["METUntaggedSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return cos((*b->MET_P)[idx].phi() - (*b->J_P)[idx].at(i).phi());
        };
        accessors["METUntaggedSubSubSubLJ_CosDeltaPhi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return cos((*b->MET_P)[idx].phi() - (*b->J_P)[idx].at(i).phi());
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
            return (*b->GJ_Eta).at(n);
        };
        accessors["GJ_IsBjet"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GJ_IsBjet).at(n);
        };
        accessors["GJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GJ_MomentumRank).at(n);
        };
        accessors["GJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GJ_Phi).at(n);
        };
        accessors["GJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GJ_Pt).at(n);
        };
        accessors["GT_Eta"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_Eta).at(n);
        };
        accessors["GT_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_MomentumRank).at(n);
        };
        accessors["GT_ParentEta"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ParentEta).at(n);
        };
        accessors["GT_ParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ParentId).at(n);
        };
        accessors["GT_ParentP"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ParentP).at(n);
        };
        accessors["GT_ParentPhi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ParentPhi).at(n);
        };
        accessors["GT_ParentPt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ParentPt).at(n);
        };
        accessors["GT_Phi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_Phi).at(n);
        };
        accessors["GT_Pt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_Pt).at(n);
        };
        accessors["GT_ToElectron"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ToElectron).at(n);
        };
        accessors["GT_ToHadrons"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ToHadrons).at(n);
        };
        accessors["GT_ToMuon"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_ToMuon).at(n);
        };
        accessors["GT_VisEta"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_VisEta).at(n);
        };
        accessors["GT_VisPhi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_VisPhi).at(n);
        };
        accessors["GT_VisPt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->GT_VisPt).at(n);
        };
        accessors["HT"] = [](Branches *b, int idx, int n) -> float {
            return (*b->HT)[idx];
        };
        accessors["IsTauEvent"] = [](Branches *b, int idx, int n) -> float {
            return b->IsTauEvent;
        };
        accessors["J_Charge"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_Charge)[idx].at(n);
        };
        accessors["LJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_Charge)[idx].at(0);
        };
        accessors["SubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_Charge)[idx].at(1);
        };
        accessors["SubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_Charge)[idx].at(2);
        };
        accessors["SubSubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_Charge)[idx].at(3);
        };
        accessors["TaggedJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_Charge)[idx].at(i);
        };
        accessors["TaggedLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_Charge)[idx].at(i);
        };
        accessors["TaggedSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_Charge)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_Charge)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_Charge)[idx].at(i);
        };
        accessors["UntaggedJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_Charge)[idx].at(i);
        };
        accessors["UntaggedLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_Charge)[idx].at(i);
        };
        accessors["UntaggedSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_Charge)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_Charge)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_Charge"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_Charge)[idx].at(i);
        };
        accessors["J_Charge"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_Charge)[idx][n];
        };
        accessors["J_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_MomentumRank)[idx].at(n);
        };
        accessors["LJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_MomentumRank)[idx].at(0);
        };
        accessors["SubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_MomentumRank)[idx].at(1);
        };
        accessors["SubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_MomentumRank)[idx].at(2);
        };
        accessors["SubSubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_MomentumRank)[idx].at(3);
        };
        accessors["TaggedJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_MomentumRank)[idx].at(i);
        };
        accessors["TaggedLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_MomentumRank)[idx].at(i);
        };
        accessors["TaggedSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_MomentumRank)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_MomentumRank)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_MomentumRank)[idx].at(i);
        };
        accessors["UntaggedJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_MomentumRank)[idx].at(i);
        };
        accessors["UntaggedLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_MomentumRank)[idx].at(i);
        };
        accessors["UntaggedSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_MomentumRank)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_MomentumRank)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_MomentumRank)[idx].at(i);
        };
        accessors["J_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_MomentumRank)[idx][n];
        };
        accessors["J_NumJets"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_NumJets)[idx];
        };
        accessors["J_Pt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx].at(n).pt();
        };
        accessors["LJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx].at(0).pt();
        };
        accessors["SubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx].at(1).pt();
        };
        accessors["SubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx].at(2).pt();
        };
        accessors["SubSubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx].at(3).pt();
        };
        accessors["TaggedJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_P)[idx].at(i).pt();
        };
        accessors["TaggedLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_P)[idx].at(i).pt();
        };
        accessors["TaggedSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_P)[idx].at(i).pt();
        };
        accessors["TaggedSubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_P)[idx].at(i).pt();
        };
        accessors["TaggedSubSubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_P)[idx].at(i).pt();
        };
        accessors["UntaggedJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_P)[idx].at(i).pt();
        };
        accessors["UntaggedLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_P)[idx].at(i).pt();
        };
        accessors["UntaggedSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_P)[idx].at(i).pt();
        };
        accessors["UntaggedSubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_P)[idx].at(i).pt();
        };
        accessors["UntaggedSubSubSubLJ_Pt"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_P)[idx].at(i).pt();
        };
        accessors["J_Eta"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx].at(n).eta();
        };
        accessors["LJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx].at(0).eta();
        };
        accessors["SubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx].at(1).eta();
        };
        accessors["SubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx].at(2).eta();
        };
        accessors["SubSubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx].at(3).eta();
        };
        accessors["TaggedJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_P)[idx].at(i).eta();
        };
        accessors["TaggedLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_P)[idx].at(i).eta();
        };
        accessors["TaggedSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_P)[idx].at(i).eta();
        };
        accessors["TaggedSubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_P)[idx].at(i).eta();
        };
        accessors["TaggedSubSubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_P)[idx].at(i).eta();
        };
        accessors["UntaggedJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_P)[idx].at(i).eta();
        };
        accessors["UntaggedLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_P)[idx].at(i).eta();
        };
        accessors["UntaggedSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_P)[idx].at(i).eta();
        };
        accessors["UntaggedSubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_P)[idx].at(i).eta();
        };
        accessors["UntaggedSubSubSubLJ_Eta"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_P)[idx].at(i).eta();
        };
        accessors["J_Phi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx].at(n).phi();
        };
        accessors["LJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx].at(0).phi();
        };
        accessors["SubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx].at(1).phi();
        };
        accessors["SubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx].at(2).phi();
        };
        accessors["SubSubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_P)[idx].at(3).phi();
        };
        accessors["TaggedJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_P)[idx].at(i).phi();
        };
        accessors["TaggedLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_P)[idx].at(i).phi();
        };
        accessors["TaggedSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_P)[idx].at(i).phi();
        };
        accessors["TaggedSubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_P)[idx].at(i).phi();
        };
        accessors["TaggedSubSubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_P)[idx].at(i).phi();
        };
        accessors["UntaggedJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_P)[idx].at(i).phi();
        };
        accessors["UntaggedLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_P)[idx].at(i).phi();
        };
        accessors["UntaggedSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_P)[idx].at(i).phi();
        };
        accessors["UntaggedSubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_P)[idx].at(i).phi();
        };
        accessors["UntaggedSubSubSubLJ_Phi"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_P)[idx].at(i).phi();
        };
        accessors["J_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandParentId)[idx].at(n);
        };
        accessors["LJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandParentId)[idx].at(0);
        };
        accessors["SubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandParentId)[idx].at(1);
        };
        accessors["SubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandParentId)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandParentId)[idx].at(3);
        };
        accessors["TaggedJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandParentId)[idx].at(i);
        };
        accessors["TaggedLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandParentId)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandParentId)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandParentId)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandParentId)[idx].at(i);
        };
        accessors["UntaggedJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandParentId)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandParentId)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandParentId)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandParentId)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandParentId)[idx].at(i);
        };
        accessors["J_PartonGrandParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandParentId)[idx][n];
        };
        accessors["J_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Id)[idx].at(n);
        };
        accessors["LJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Id)[idx].at(0);
        };
        accessors["SubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Id)[idx].at(1);
        };
        accessors["SubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Id)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Id)[idx].at(3);
        };
        accessors["TaggedJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother00Id)[idx].at(i);
        };
        accessors["TaggedLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother00Id)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother00Id)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother00Id)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother00Id)[idx].at(i);
        };
        accessors["UntaggedJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother00Id)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother00Id)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother00Id)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother00Id)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother00Id)[idx].at(i);
        };
        accessors["J_PartonGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Id)[idx][n];
        };
        accessors["J_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Status)[idx].at(n);
        };
        accessors["LJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Status)[idx].at(0);
        };
        accessors["SubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Status)[idx].at(1);
        };
        accessors["SubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Status)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Status)[idx].at(3);
        };
        accessors["TaggedJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother00Status)[idx].at(i);
        };
        accessors["TaggedLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother00Status)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother00Status)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother00Status)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother00Status)[idx].at(i);
        };
        accessors["UntaggedJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother00Status)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother00Status)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother00Status)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother00Status)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother00Status)[idx].at(i);
        };
        accessors["J_PartonGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother00Status)[idx][n];
        };
        accessors["J_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Id)[idx].at(n);
        };
        accessors["LJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Id)[idx].at(0);
        };
        accessors["SubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Id)[idx].at(1);
        };
        accessors["SubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Id)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Id)[idx].at(3);
        };
        accessors["TaggedJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother01Id)[idx].at(i);
        };
        accessors["TaggedLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother01Id)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother01Id)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother01Id)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother01Id)[idx].at(i);
        };
        accessors["UntaggedJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother01Id)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother01Id)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother01Id)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother01Id)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother01Id)[idx].at(i);
        };
        accessors["J_PartonGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Id)[idx][n];
        };
        accessors["J_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Status)[idx].at(n);
        };
        accessors["LJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Status)[idx].at(0);
        };
        accessors["SubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Status)[idx].at(1);
        };
        accessors["SubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Status)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Status)[idx].at(3);
        };
        accessors["TaggedJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother01Status)[idx].at(i);
        };
        accessors["TaggedLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother01Status)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother01Status)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother01Status)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother01Status)[idx].at(i);
        };
        accessors["UntaggedJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother01Status)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother01Status)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother01Status)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother01Status)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother01Status)[idx].at(i);
        };
        accessors["J_PartonGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother01Status)[idx][n];
        };
        accessors["J_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Id)[idx].at(n);
        };
        accessors["LJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Id)[idx].at(0);
        };
        accessors["SubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Id)[idx].at(1);
        };
        accessors["SubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Id)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Id)[idx].at(3);
        };
        accessors["TaggedJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother10Id)[idx].at(i);
        };
        accessors["TaggedLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother10Id)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother10Id)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother10Id)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother10Id)[idx].at(i);
        };
        accessors["UntaggedJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother10Id)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother10Id)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother10Id)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother10Id)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother10Id)[idx].at(i);
        };
        accessors["J_PartonGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Id)[idx][n];
        };
        accessors["J_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Status)[idx].at(n);
        };
        accessors["LJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Status)[idx].at(0);
        };
        accessors["SubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Status)[idx].at(1);
        };
        accessors["SubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Status)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Status)[idx].at(3);
        };
        accessors["TaggedJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother10Status)[idx].at(i);
        };
        accessors["TaggedLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother10Status)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother10Status)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother10Status)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother10Status)[idx].at(i);
        };
        accessors["UntaggedJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother10Status)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother10Status)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother10Status)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother10Status)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother10Status)[idx].at(i);
        };
        accessors["J_PartonGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother10Status)[idx][n];
        };
        accessors["J_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Id)[idx].at(n);
        };
        accessors["LJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Id)[idx].at(0);
        };
        accessors["SubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Id)[idx].at(1);
        };
        accessors["SubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Id)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Id)[idx].at(3);
        };
        accessors["TaggedJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother11Id)[idx].at(i);
        };
        accessors["TaggedLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother11Id)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother11Id)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother11Id)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother11Id)[idx].at(i);
        };
        accessors["UntaggedJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother11Id)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother11Id)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother11Id)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother11Id)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother11Id)[idx].at(i);
        };
        accessors["J_PartonGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Id)[idx][n];
        };
        accessors["J_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Status)[idx].at(n);
        };
        accessors["LJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Status)[idx].at(0);
        };
        accessors["SubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Status)[idx].at(1);
        };
        accessors["SubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Status)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Status)[idx].at(3);
        };
        accessors["TaggedJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother11Status)[idx].at(i);
        };
        accessors["TaggedLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother11Status)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother11Status)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother11Status)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother11Status)[idx].at(i);
        };
        accessors["UntaggedJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonGrandmother11Status)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonGrandmother11Status)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonGrandmother11Status)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonGrandmother11Status)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonGrandmother11Status)[idx].at(i);
        };
        accessors["J_PartonGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonGrandmother11Status)[idx][n];
        };
        accessors["J_PartonId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonId)[idx].at(n);
        };
        accessors["LJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonId)[idx].at(0);
        };
        accessors["SubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonId)[idx].at(1);
        };
        accessors["SubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonId)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonId)[idx].at(3);
        };
        accessors["TaggedJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonId)[idx].at(i);
        };
        accessors["TaggedLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonId)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonId)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonId)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonId)[idx].at(i);
        };
        accessors["UntaggedJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonId)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonId)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonId)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonId)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonId)[idx].at(i);
        };
        accessors["J_PartonId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonId)[idx][n];
        };
        accessors["J_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Id)[idx].at(n);
        };
        accessors["LJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Id)[idx].at(0);
        };
        accessors["SubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Id)[idx].at(1);
        };
        accessors["SubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Id)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Id)[idx].at(3);
        };
        accessors["TaggedJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother0Id)[idx].at(i);
        };
        accessors["TaggedLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother0Id)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother0Id)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother0Id)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother0Id)[idx].at(i);
        };
        accessors["UntaggedJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother0Id)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother0Id)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother0Id)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother0Id)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother0Id)[idx].at(i);
        };
        accessors["J_PartonMother0Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Id)[idx][n];
        };
        accessors["J_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Status)[idx].at(n);
        };
        accessors["LJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Status)[idx].at(0);
        };
        accessors["SubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Status)[idx].at(1);
        };
        accessors["SubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Status)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Status)[idx].at(3);
        };
        accessors["TaggedJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother0Status)[idx].at(i);
        };
        accessors["TaggedLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother0Status)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother0Status)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother0Status)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother0Status)[idx].at(i);
        };
        accessors["UntaggedJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother0Status)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother0Status)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother0Status)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother0Status)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother0Status)[idx].at(i);
        };
        accessors["J_PartonMother0Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother0Status)[idx][n];
        };
        accessors["J_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Id)[idx].at(n);
        };
        accessors["LJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Id)[idx].at(0);
        };
        accessors["SubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Id)[idx].at(1);
        };
        accessors["SubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Id)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Id)[idx].at(3);
        };
        accessors["TaggedJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother1Id)[idx].at(i);
        };
        accessors["TaggedLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother1Id)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother1Id)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother1Id)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother1Id)[idx].at(i);
        };
        accessors["UntaggedJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother1Id)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother1Id)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother1Id)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother1Id)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother1Id)[idx].at(i);
        };
        accessors["J_PartonMother1Id"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Id)[idx][n];
        };
        accessors["J_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Status)[idx].at(n);
        };
        accessors["LJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Status)[idx].at(0);
        };
        accessors["SubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Status)[idx].at(1);
        };
        accessors["SubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Status)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Status)[idx].at(3);
        };
        accessors["TaggedJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother1Status)[idx].at(i);
        };
        accessors["TaggedLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother1Status)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother1Status)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother1Status)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother1Status)[idx].at(i);
        };
        accessors["UntaggedJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonMother1Status)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonMother1Status)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonMother1Status)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonMother1Status)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonMother1Status)[idx].at(i);
        };
        accessors["J_PartonMother1Status"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonMother1Status)[idx][n];
        };
        accessors["J_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonParentId)[idx].at(n);
        };
        accessors["LJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonParentId)[idx].at(0);
        };
        accessors["SubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonParentId)[idx].at(1);
        };
        accessors["SubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonParentId)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonParentId)[idx].at(3);
        };
        accessors["TaggedJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonParentId)[idx].at(i);
        };
        accessors["TaggedLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonParentId)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonParentId)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonParentId)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonParentId)[idx].at(i);
        };
        accessors["UntaggedJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonParentId)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonParentId)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonParentId)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonParentId)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonParentId)[idx].at(i);
        };
        accessors["J_PartonParentId"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonParentId)[idx][n];
        };
        accessors["J_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonStatus)[idx].at(n);
        };
        accessors["LJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonStatus)[idx].at(0);
        };
        accessors["SubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonStatus)[idx].at(1);
        };
        accessors["SubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonStatus)[idx].at(2);
        };
        accessors["SubSubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonStatus)[idx].at(3);
        };
        accessors["TaggedJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonStatus)[idx].at(i);
        };
        accessors["TaggedLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonStatus)[idx].at(i);
        };
        accessors["TaggedSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonStatus)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonStatus)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonStatus)[idx].at(i);
        };
        accessors["UntaggedJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_PartonStatus)[idx].at(i);
        };
        accessors["UntaggedLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_PartonStatus)[idx].at(i);
        };
        accessors["UntaggedSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_PartonStatus)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_PartonStatus)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_PartonStatus)[idx].at(i);
        };
        accessors["J_PartonStatus"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_PartonStatus)[idx][n];
        };
        accessors["J_CSV"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_combSecVtxBTag)[idx].at(n);
        };
        accessors["LJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_combSecVtxBTag)[idx].at(0);
        };
        accessors["SubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_combSecVtxBTag)[idx].at(1);
        };
        accessors["SubSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_combSecVtxBTag)[idx].at(2);
        };
        accessors["SubSubSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_combSecVtxBTag)[idx].at(3);
        };
        accessors["TaggedJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_combSecVtxBTag)[idx].at(i);
        };
        accessors["TaggedLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_combSecVtxBTag)[idx].at(i);
        };
        accessors["TaggedSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_combSecVtxBTag)[idx].at(i);
        };
        accessors["TaggedSubSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_combSecVtxBTag)[idx].at(i);
        };
        accessors["TaggedSubSubSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = get_tag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_combSecVtxBTag)[idx].at(i);
        };
        accessors["UntaggedJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], n);
            return (*b->J_combSecVtxBTag)[idx].at(i);
        };
        accessors["UntaggedLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 0);
            return (*b->J_combSecVtxBTag)[idx].at(i);
        };
        accessors["UntaggedSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 1);
            return (*b->J_combSecVtxBTag)[idx].at(i);
        };
        accessors["UntaggedSubSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 2);
            return (*b->J_combSecVtxBTag)[idx].at(i);
        };
        accessors["UntaggedSubSubSubLJ_CSV"] = [](Branches *b, int idx, int n) -> float {
            int i = get_untag_index((*b->J_combSecVtxMediumBTag)[idx], 3);
            return (*b->J_combSecVtxBTag)[idx].at(i);
        };
        accessors["J_CSV"] = [](Branches *b, int idx, int n) -> float {
            return (*b->J_combSecVtxBTag)[idx][n];
        };
        accessors["L1_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_Charge)[idx];
            }
            throw;
        };
        accessors["L1_CorrectedD0"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_CorrectedD0)[idx];
            }
            throw;
        };
        accessors["L1_CorrectedDZ"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_CorrectedDZ)[idx];
            }
            throw;
        };
        accessors["L1_EventWeight"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_EventWeight)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchDaughter0Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchDaughter0Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchDaughter1Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchDaughter1Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother00Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother00Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother01Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother01Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother10Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother10Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother11Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchGrandmother11Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchId)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchMother0Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchMother0Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchMother1Id)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchMother1Status)[idx];
            }
            throw;
        };
        accessors["L1_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_GenMatchStatus)[idx];
            }
            throw;
        };
        accessors["L1_ImpactParameter"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_ImpactParameter)[idx];
            }
            throw;
        };
        accessors["L1_IsElectron"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_IsElectron)[idx];
            }
            throw;
        };
        accessors["L1_IsLoose"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_IsLoose)[idx];
            }
            throw;
        };
        accessors["L1_IsMuon"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_IsMuon)[idx];
            }
            throw;
        };
        accessors["L1_IsTight"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_IsTight)[idx];
            }
            throw;
        };
        accessors["L1_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_P)[idx].pt();
            }
            throw;
        };
        accessors["L1_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_P)[idx].eta();
            }
            throw;
        };
        accessors["L1_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_P)[idx].phi();
            }
            throw;
        };
        accessors["L1_RelIso"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L1_RelIso)[idx];
            }
            throw;
        };
        accessors["L2_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_Charge)[idx];
            }
            throw;
        };
        accessors["L2_CorrectedD0"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_CorrectedD0)[idx];
            }
            throw;
        };
        accessors["L2_CorrectedDZ"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_CorrectedDZ)[idx];
            }
            throw;
        };
        accessors["L2_EventWeight"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_EventWeight)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchDaughter0Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchDaughter0Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchDaughter1Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchDaughter1Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother00Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother00Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother01Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother01Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother10Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother10Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother11Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchGrandmother11Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchId)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchMother0Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchMother0Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchMother1Id)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchMother1Status)[idx];
            }
            throw;
        };
        accessors["L2_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_GenMatchStatus)[idx];
            }
            throw;
        };
        accessors["L2_ImpactParameter"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_ImpactParameter)[idx];
            }
            throw;
        };
        accessors["L2_IsElectron"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_IsElectron)[idx];
            }
            throw;
        };
        accessors["L2_IsLoose"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_IsLoose)[idx];
            }
            throw;
        };
        accessors["L2_IsMuon"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_IsMuon)[idx];
            }
            throw;
        };
        accessors["L2_IsTight"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_IsTight)[idx];
            }
            throw;
        };
        accessors["L2_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_P)[idx].pt();
            }
            throw;
        };
        accessors["L2_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_P)[idx].eta();
            }
            throw;
        };
        accessors["L2_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_P)[idx].phi();
            }
            throw;
        };
        accessors["L2_RelIso"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->L2_RelIso)[idx];
            }
            throw;
        };
        accessors["L_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_Charge)[idx];
            }
            throw;
        };
        accessors["L_CorrectedD0"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_CorrectedD0)[idx];
            }
            throw;
        };
        accessors["L_CorrectedDZ"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_CorrectedDZ)[idx];
            }
            throw;
        };
        accessors["L_EventWeight"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_EventWeight)[idx];
            }
            throw;
        };
        accessors["L_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchDaughter0Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchDaughter0Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchDaughter1Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchDaughter1Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother00Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother00Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother01Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother01Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother10Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother10Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother11Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchGrandmother11Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchId)[idx];
            }
            throw;
        };
        accessors["L_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchMother0Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchMother0Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchMother1Id)[idx];
            }
            throw;
        };
        accessors["L_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchMother1Status)[idx];
            }
            throw;
        };
        accessors["L_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_GenMatchStatus)[idx];
            }
            throw;
        };
        accessors["L_ImpactParameter"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_ImpactParameter)[idx];
            }
            throw;
        };
        accessors["L_IsElectron"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_IsElectron)[idx];
            }
            throw;
        };
        accessors["L_IsLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_IsLoose)[idx];
            }
            throw;
        };
        accessors["L_IsMuon"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_IsMuon)[idx];
            }
            throw;
        };
        accessors["L_IsTight"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_IsTight)[idx];
            }
            throw;
        };
        accessors["L_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_P)[idx].pt();
            }
            throw;
        };
        accessors["L_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_P)[idx].eta();
            }
            throw;
        };
        accessors["L_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->L_P)[idx].phi();
            }
            throw;
        };
        accessors["L_RelIso"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
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
        accessors["NumRecoTaus"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return e->NumRecoTaus;
            }
            throw;
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
        accessors["RT_Charge"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_Charge)[n];
            }
            throw;
        };
        accessors["RT_DecayMode"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_DecayMode)[n];
            }
            throw;
        };
        accessors["RT_EmFraction"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_EmFraction)[n];
            }
            throw;
        };
        accessors["RT_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchDaughter0Id)[n];
            }
            throw;
        };
        accessors["RT_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchDaughter0Status)[n];
            }
            throw;
        };
        accessors["RT_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchDaughter1Id)[n];
            }
            throw;
        };
        accessors["RT_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchDaughter1Status)[n];
            }
            throw;
        };
        accessors["RT_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchGrandmother00Id)[n];
            }
            throw;
        };
        accessors["RT_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchGrandmother00Status)[n];
            }
            throw;
        };
        accessors["RT_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchGrandmother01Id)[n];
            }
            throw;
        };
        accessors["RT_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchGrandmother01Status)[n];
            }
            throw;
        };
        accessors["RT_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchGrandmother10Id)[n];
            }
            throw;
        };
        accessors["RT_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchGrandmother10Status)[n];
            }
            throw;
        };
        accessors["RT_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchGrandmother11Id)[n];
            }
            throw;
        };
        accessors["RT_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchGrandmother11Status)[n];
            }
            throw;
        };
        accessors["RT_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchId)[n];
            }
            throw;
        };
        accessors["RT_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchMother0Id)[n];
            }
            throw;
        };
        accessors["RT_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchMother0Status)[n];
            }
            throw;
        };
        accessors["RT_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchMother1Id)[n];
            }
            throw;
        };
        accessors["RT_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchMother1Status)[n];
            }
            throw;
        };
        accessors["RT_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_GenMatchStatus)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronDeadECAL"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronDeadECAL)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronLoose)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronLooseMVA2)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronLooseMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronLooseMVA3)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronMVA)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronMVA2category"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronMVA2category)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronMVA2raw)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronMVA3category"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronMVA3category)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronMVA3Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronMVA3raw)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronMedium"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronMedium)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronMediumMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronMediumMVA2)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronMediumMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronMediumMVA3)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronTight"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronTight)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronTightMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronTightMVA2)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronTightMVA3)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronVLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronVLooseMVA2)[n];
            }
            throw;
        };
        accessors["RT_AntiElectronVTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstElectronVTightMVA3)[n];
            }
            throw;
        };
        accessors["RT_AntiMuonLoose"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstMuonLoose)[n];
            }
            throw;
        };
        accessors["RT_AntiMuonLoose2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstMuonLoose2)[n];
            }
            throw;
        };
        accessors["RT_AntiMuonMedium"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstMuonMedium)[n];
            }
            throw;
        };
        accessors["RT_AntiMuonMedium2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstMuonMedium2)[n];
            }
            throw;
        };
        accessors["RT_AntiMuonTight"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstMuonTight)[n];
            }
            throw;
        };
        accessors["RT_AntiMuonTight2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSagainstMuonTight2)[n];
            }
            throw;
        };
        accessors["RT_CombinedIsolationDeltaBetaCorrRaw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyCombinedIsolationDeltaBetaCorrRaw)[n];
            }
            throw;
        };
        accessors["RT_CombinedIsolationDeltaBetaCorrRaw3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[n];
            }
            throw;
        };
        accessors["RT_IsolationMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyIsolationMVA2raw)[n];
            }
            throw;
        };
        accessors["RT_IsolationMVARaw"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyIsolationMVAraw)[n];
            }
            throw;
        };
        accessors["RT_LooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyLooseCombinedIsolationDeltaBetaCorr)[n];
            }
            throw;
        };
        accessors["RT_LooseCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[n];
            }
            throw;
        };
        accessors["RT_LooseIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyLooseIsolationMVA)[n];
            }
            throw;
        };
        accessors["RT_LooseIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyLooseIsolationMVA2)[n];
            }
            throw;
        };
        accessors["RT_MediumCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyMediumCombinedIsolationDeltaBetaCorr)[n];
            }
            throw;
        };
        accessors["RT_MediumCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[n];
            }
            throw;
        };
        accessors["RT_MediumIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyMediumIsolationMVA)[n];
            }
            throw;
        };
        accessors["RT_MediumIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyMediumIsolationMVA2)[n];
            }
            throw;
        };
        accessors["RT_TightCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyTightCombinedIsolationDeltaBetaCorr)[n];
            }
            throw;
        };
        accessors["RT_TightCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[n];
            }
            throw;
        };
        accessors["RT_TightIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyTightIsolationMVA)[n];
            }
            throw;
        };
        accessors["RT_TightIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyTightIsolationMVA2)[n];
            }
            throw;
        };
        accessors["RT_VLooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSbyVLooseCombinedIsolationDeltaBetaCorr)[n];
            }
            throw;
        };
        accessors["RT_DecayModeFinding"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_HPSdecayModeFinding)[n];
            }
            throw;
        };
        accessors["RT_IsInTheCracks"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_IsInTheCracks)[n];
            }
            throw;
        };
        accessors["RT_JetCSV"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_JetCSV)[n];
            }
            throw;
        };
        accessors["RT_LTIpVtdxy"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_LTIpVtdxy)[n];
            }
            throw;
        };
        accessors["RT_LTIpVtdxyError"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_LTIpVtdxyError)[n];
            }
            throw;
        };
        accessors["RT_LTIpVtdz"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_LTIpVtdz)[n];
            }
            throw;
        };
        accessors["RT_LTIpVtdzError"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_LTIpVtdzError)[n];
            }
            throw;
        };
        accessors["RT_LTNormChiSqrd"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_LTNormChiSqrd)[n];
            }
            throw;
        };
        accessors["RT_LTPt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_LTPt)[n];
            }
            throw;
        };
        accessors["RT_LTValidHits"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_LTValidHits)[n];
            }
            throw;
        };
        accessors["RT_LTvalid"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_LTvalid)[n];
            }
            throw;
        };
        accessors["RT_LTvx"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_LTvx)[n];
            }
            throw;
        };
        accessors["RT_LTvy"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_LTvy)[n];
            }
            throw;
        };
        accessors["RT_LTvz"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_LTvz)[n];
            }
            throw;
        };
        accessors["RT_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_MomentumRank)[n];
            }
            throw;
        };
        accessors["RT_NumProngs"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_NProngs)[n];
            }
            throw;
        };
        accessors["RT_NumSignalGammas"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_NSignalGammas)[n];
            }
            throw;
        };
        accessors["RT_NumSignalNeutrals"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_NSignalNeutrals)[n];
            }
            throw;
        };
        accessors["RT_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_P)[n].pt();
            }
            throw;
        };
        accessors["RT_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_P)[n].eta();
            }
            throw;
        };
        accessors["RT_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->RT_P)[n].phi();
            }
            throw;
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
        accessors["T1_JetCSV"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T1_JetCSV)[idx];
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
        accessors["T2_JetCSV"] = [](Branches *b, int idx, int n) -> float {
            if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->T2_JetCSV)[idx];
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
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_Charge)[idx];
            }
            throw;
        };
        accessors["T_DecayMode"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_DecayMode)[idx];
            }
            throw;
        };
        accessors["T_EmFraction"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_EmFraction)[idx];
            }
            throw;
        };
        accessors["T_GenMatchDaughter0Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchDaughter0Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchDaughter0Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchDaughter0Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchDaughter1Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchDaughter1Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchDaughter1Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchDaughter1Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother00Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother00Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother00Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother00Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother01Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother01Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother01Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother01Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother10Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother10Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother10Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother10Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother11Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother11Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchGrandmother11Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchGrandmother11Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchId"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchId)[idx];
            }
            throw;
        };
        accessors["T_GenMatchMother0Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchMother0Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchMother0Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchMother0Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchMother1Id"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchMother1Id)[idx];
            }
            throw;
        };
        accessors["T_GenMatchMother1Status"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchMother1Status)[idx];
            }
            throw;
        };
        accessors["T_GenMatchStatus"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_GenMatchStatus)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronDeadECAL"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronDeadECAL)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronLoose"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronLoose)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronLooseMVA2)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronLooseMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronLooseMVA3)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMVA"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMVA)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMVA2category"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMVA2category)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMVA2raw)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMVA3category"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMVA3category)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMVA3Raw"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMVA3raw)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMedium"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMedium)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMediumMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMediumMVA2)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronMediumMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronMediumMVA3)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronTight"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronTight)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronTightMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronTightMVA2)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronTightMVA3)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronVLooseMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronVLooseMVA2)[idx];
            }
            throw;
        };
        accessors["T_AntiElectronVTightMVA3"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstElectronVTightMVA3)[idx];
            }
            throw;
        };
        accessors["T_AntiMuonLoose"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstMuonLoose)[idx];
            }
            throw;
        };
        accessors["T_AntiMuonLoose2"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstMuonLoose2)[idx];
            }
            throw;
        };
        accessors["T_AntiMuonMedium"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstMuonMedium)[idx];
            }
            throw;
        };
        accessors["T_AntiMuonMedium2"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstMuonMedium2)[idx];
            }
            throw;
        };
        accessors["T_AntiMuonTight"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstMuonTight)[idx];
            }
            throw;
        };
        accessors["T_AntiMuonTight2"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSagainstMuonTight2)[idx];
            }
            throw;
        };
        accessors["T_CombinedIsolationDeltaBetaCorrRaw"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyCombinedIsolationDeltaBetaCorrRaw)[idx];
            }
            throw;
        };
        accessors["T_CombinedIsolationDeltaBetaCorrRaw3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyCombinedIsolationDeltaBetaCorrRaw3Hits)[idx];
            }
            throw;
        };
        accessors["T_IsolationMVA2Raw"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyIsolationMVA2raw)[idx];
            }
            throw;
        };
        accessors["T_IsolationMVARaw"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyIsolationMVAraw)[idx];
            }
            throw;
        };
        accessors["T_LooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyLooseCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T_LooseCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyLooseCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T_LooseIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyLooseIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T_LooseIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyLooseIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T_MediumCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyMediumCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T_MediumCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyMediumCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T_MediumIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyMediumIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T_MediumIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyMediumIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T_TightCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyTightCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T_TightCombinedIsolationDeltaBetaCorr3Hits"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyTightCombinedIsolationDeltaBetaCorr3Hits)[idx];
            }
            throw;
        };
        accessors["T_TightIsolationMVA"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyTightIsolationMVA)[idx];
            }
            throw;
        };
        accessors["T_TightIsolationMVA2"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyTightIsolationMVA2)[idx];
            }
            throw;
        };
        accessors["T_VLooseCombinedIsolationDeltaBetaCorr"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSbyVLooseCombinedIsolationDeltaBetaCorr)[idx];
            }
            throw;
        };
        accessors["T_DecayModeFinding"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_HPSdecayModeFinding)[idx];
            }
            throw;
        };
        accessors["T_IsInTheCracks"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_IsInTheCracks)[idx];
            }
            throw;
        };
        accessors["T_LTIpVtdxy"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTIpVtdxy)[idx];
            }
            throw;
        };
        accessors["T_LTIpVtdxyError"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTIpVtdxyError)[idx];
            }
            throw;
        };
        accessors["T_LTIpVtdz"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTIpVtdz)[idx];
            }
            throw;
        };
        accessors["T_LTIpVtdzError"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTIpVtdzError)[idx];
            }
            throw;
        };
        accessors["T_LTNormChiSqrd"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTNormChiSqrd)[idx];
            }
            throw;
        };
        accessors["T_LTPt"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTPt)[idx];
            }
            throw;
        };
        accessors["T_LTValidHits"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTValidHits)[idx];
            }
            throw;
        };
        accessors["T_LTvalid"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTvalid)[idx];
            }
            throw;
        };
        accessors["T_LTvx"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTvx)[idx];
            }
            throw;
        };
        accessors["T_LTvy"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTvy)[idx];
            }
            throw;
        };
        accessors["T_LTvz"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_LTvz)[idx];
            }
            throw;
        };
        accessors["T_MomentumRank"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_MomentumRank)[idx];
            }
            throw;
        };
        accessors["T_NumProngs"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_NProngs)[idx];
            }
            throw;
        };
        accessors["T_NumSignalGammas"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_NSignalGammas)[idx];
            }
            throw;
        };
        accessors["T_NumSignalNeutrals"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_NSignalNeutrals)[idx];
            }
            throw;
        };
        accessors["T_Pt"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_P)[idx].pt();
            }
            throw;
        };
        accessors["T_Eta"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->T_P)[idx].eta();
            }
            throw;
        };
        accessors["T_Phi"] = [](Branches *b, int idx, int n) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
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
        // }}}1

        setup = true;
    }
}

NamespaceImp(roast)
