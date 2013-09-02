// vim: ts=4:sw=4:et:sta
#include <cmath>

#include "../interface/TTLBranches.h"
#include "../interface/Weight.h"

namespace roast {
    std::map<std::string, roast::Weight> Weight::weights;

    float
    Weight::operator()(Branches* b, int idx)
    {
        float w = GetVal(b, idx, -1);
        if (strength > 0)
            w = pow(strength, w);

        n += 1.;
        sum += w;

        return w;
    }

    Weight
    Weight::Create(const std::string& name, direction kind, float strength)
    {
        std::string label;
        GetValue_t fct = 0;

        if (name == "qSquared" || name == "PUcorr" || name.find("lepton") == 0 || name == "topPt") {
            auto varname = name;
            if (name == "qSquared") {
                label = "Q^2 shift";
                varname = "Q2";
            } else if (name == "PUcorr") {
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
                        if (e->GetTau1MatchIndex(idx) == 4)
                            ++matches;
                        if (e->GetTau2MatchIndex(idx) == 4)
                            ++matches;
                        return matches;
                    };
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
                        if (e->GetTau1MatchIndex(idx) == 1)
                            ++matches;
                        if (e->GetTau2MatchIndex(idx) == 1)
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
                        if (e->GetTau1MatchIndex(idx) == 2)
                            ++matches;
                        if (e->GetTau2MatchIndex(idx) == 2)
                            ++matches;
                        return matches;
                    };
                    break;
            }
        }

        if (!fct)
            throw "";

        return Weight(label, fct, strength);
    }
}

ClassImp(roast::Weight)
