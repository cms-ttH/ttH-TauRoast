// vim: ts=4:sw=4:et:sta
#include <cmath>

#include "TH1F.h"

#include "../interface/TLBranches.h"
#include "../interface/TTLBranches.h"
#include "../interface/Weight.h"

namespace roast {
    std::map<std::string, roast::Weight> Weight::weights;

    float
    Weight::operator()(Branches* b, int idx)
    {
        value = GetVal(b, idx, -1);
        if (strength > 0)
            value = pow(strength, value);

        n += 1.;
        sum += value;

        return value;
    }

    Weight
    Weight::Create(const std::string& name, direction kind, float strength)
    {
        std::string label;
        GetValue_t fct = 0;

        if (name == "qSquared" || name.find("PUcorr") == 0 || name.find("lepton") == 0 || name == "topPt") {
            auto varname = name;
            if (name == "qSquared") {
                label = "Q^2 shift";
                varname = "Q2";
            } else if (name.find("PUcorr") == 0) {
                label = "PU reweighing";
                varname = "Pu";
            } else if (name == "lepton") {
                label = "Lepton SF";
                varname = "L_Event";
            } else if (name == "lepton1") {
                label = "Lepton 1 SF";
                varname = "L1_Event";
            } else if (name == "lepton2") {
                label = "Lepton 2 SF";
                varname = "L2_Event";
            } else if (name == "topPt") {
                label = "Top Pt SF";
                varname = "TopPt";
            }

            varname += "Weight";

            if (name.find("PUcorr") == 0)
                varname += name.substr(6);

            switch (kind) {
                case kUp:
                    varname += "Up";
                    break;
                case kDown:
                    varname += "Down";
                    break;
                case kNominal:
                    break;
            }

            fct = get_accessor(varname);
        } else if (name.find("CSV") != std::string::npos) {
            label = "Jet CSV weight";
            auto varname = name;

            switch (kind) {
                case kUp:
                    varname += "up";
                    break;
                case kDown:
                    varname += "down";
                    break;
                case kNominal:
                    break;
            }

            fct = get_accessor(varname);
        } else if (name == "brSF") {
            label = "BR correction";
            switch (kind) {
                case kNominal:
                    fct = [](roast::Branches *b, int idx, int n) -> float {
                        unsigned int matches = 0;
                        for (const auto& id: *b->GT_ParentId)
                            matches += (abs(id) == 25);
                        return (matches == 2) ? 0.0632 / 0.0722 : (1 - 0.0632) / (1 - 0.0722);
                    };
                    break;
                case kUp:
                case kDown:
                    break;
            }
        } else if (name == "eTauFake") {
            label = "Tau ID sys";
            switch (kind) {
                case kNominal:
                    break;
                case kUp:
                case kDown:
                    fct = [](roast::Branches *b, int idx, int n) -> float {
                        auto e = dynamic_cast<roast::ttl::Branches*>(b);
                        unsigned int matches = 0;
                        if (abs((*e->TTL_Tau1GenMatchId)[idx]) == 11)
                            ++matches;
                        if (abs((*e->TTL_Tau1GenMatchId)[idx]) == 11)
                            ++matches;
                        return matches;
                    };
                    break;
            }
        } else if (name == "jetTauFakeScale") {
            label = "Tau ID scale";
            switch (kind) {
                case kNominal:
                    fct = [](roast::Branches *b, int idx, int n) -> float {
                        static TH1D * tau_fake_scale = 0;
                        if (tau_fake_scale == 0) {
                            tau_fake_scale = new TH1D("tau_fake_scale", "", 15, 0, 150);
                            double vals[] = {
                                0.0,
                                0.0,
                                0.0,
                                1.3942559957504272,
                                1.2666946649551392,
                                1.1317439079284668,
                                1.1534698009490967,
                                1.0859955549240112,
                                1.2146579027175903,
                                1.0547099113464355,
                                1.0547881126403809,
                                0.85909730195999146,
                                1.0713921785354614,
                                0.96107715368270874,
                                1.3806561231613159,
                                0.98702037334442139,
                                0.81862592697143555
                            };

                            for (int i = 0; i <= 16; ++i)
                                tau_fake_scale->SetBinContent(i, vals[i]);
                        }

                        auto e = dynamic_cast<roast::tl::Branches*>(b);
                        if (e->TranslateMatchIndex((*e->TL_TauGenMatchId)[idx]) == 1) {
                            return tau_fake_scale->GetBinContent(tau_fake_scale->FindBin((*e->TL_TauPt)[idx]));
                        }
                        return 1.;
                    };
                    break;
                case kUp:
                case kDown:
                    break;
            }
        } else if (name == "jetTauFake") {
            label = "Tau ID sys";
            switch (kind) {
                case kNominal:
                    break;
                case kUp:
                case kDown:
                    fct = [](roast::Branches *b, int idx, int n) -> float {
                        auto e = dynamic_cast<roast::ttl::Branches*>(b);
                        unsigned int matches = 0;
                        if (e->TranslateMatchIndex((*e->TTL_Tau1GenMatchId)[idx]) == 1)
                            ++matches;
                        if (e->TranslateMatchIndex((*e->TTL_Tau2GenMatchId)[idx]) == 1)
                            ++matches;
                        return matches;
                    };
                    break;
            }
        } else if (name == "tauIdEff") {
            label = "Tau ID sys";
            switch (kind) {
                case kNominal:
                    break;
                case kUp:
                case kDown:
                    fct = [](roast::Branches *b, int idx, int n) -> float {
                        auto e = dynamic_cast<roast::ttl::Branches*>(b);
                        unsigned int matches = 0;
                        if (abs((*e->TTL_Tau1GenMatchId)[idx]) == 15)
                            ++matches;
                        if (abs((*e->TTL_Tau2GenMatchId)[idx]) == 15)
                            ++matches;
                        return matches;
                    };
                    break;
            }
        } else if (name == "trigger") {
            label = "Trigger SF";
            switch (kind) {
                case kNominal:
                    fct = get_accessor("TriggerEventWeight");
                    break;
                case kUp:
                case kDown:
                    break;
            }
        }

        if (!fct)
            throw "";

        return Weight(name, label, fct, strength);
    }
}

ClassImp(roast::Weight)
