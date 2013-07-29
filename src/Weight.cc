// vim: ts=4:sw=4:et:sta
#include <cmath>

#include "../interface/TTLBranches.h"
#include "../interface/Weight.h"

namespace roast {
    std::map<std::string, roast::Weight> Weight::weights;

    float
    Weight::operator()(Branches* b, const int& idx)
    {
        float w = GetVal(b, idx);
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
        Weight::val_f fct = 0;

        if (name == "topPt") {
            label = "Top Pt SF";
            switch (kind) {
                case kNominal:
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->Ev_topPtWeight; };
                    break;
                case kUp:
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->Ev_topPtWeightUp; };
                    break;
                case kDown:
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->Ev_topPtWeightDown; };
                    break;
            }
        } else if (name == "lepton") {
            label = "Lepton SF";
            switch (kind) {
                case kNominal:
                    fct = [](roast::Branches *b, const int& idx) -> float {
                        ttl::Branches *e = dynamic_cast<ttl::Branches*>(b);
                        return (*e->TTL_LeptonEventWeight)[idx];
                    };
                    break;
            }
        } else if (name == "PUcorr") {
            label = "PU reweighing";
            switch (kind) {
                case kNominal:
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->Ev_puWeight; };
                    break;
                case kUp:
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->Ev_puWeightUp; };
                    break;
                case kDown:
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->Ev_puWeightDown; };
                    break;
            }
        } else if (name == "qSquared") {
            label = "Q^2 shift";
            switch (kind) {
                case kUp:
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->Ev_q2WeightUp; };
                    break;
                case kDown:
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->Ev_q2WeightDown; };
                    break;
            }
        } else if (name == "CSVeventWeight") {
            label = "Jet CSV weight";
            switch (kind) {
                case kNominal:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeight)[idx]; };
                    break;
            }
        } else if (name == "CSVeventWeightHF") {
            label = "Jet CSV weight";
            switch (kind) {
                case kUp:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightHFup)[idx]; };
                    break;
                case kDown:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightHFdown)[idx]; };
                    break;
            }
        } else if (name == "CSVeventWeightLF") {
            label = "Jet CSV weight";
            switch (kind) {
                case kUp:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightLFup)[idx]; };
                    break;
                case kDown:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightLFdown)[idx]; };
                    break;
            }
        } else if (name == "CSVeventWeightHFStats1") {
            label = "Jet CSV weight";
            switch (kind) {
                case kUp:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightHFStats1up)[idx]; };
                    break;
                case kDown:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightHFStats1down)[idx]; };
                    break;
            }
        } else if (name == "CSVeventWeightHFStats2") {
            label = "Jet CSV weight";
            switch (kind) {
                case kUp:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightHFStats2up)[idx]; };
                    break;
                case kDown:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightHFStats2down)[idx]; };
                    break;
            }
        } else if (name == "CSVeventWeightLFStats1") {
            label = "Jet CSV weight";
            switch (kind) {
                case kUp:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightLFStats1up)[idx]; };
                    break;
                case kDown:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightLFStats1down)[idx]; };
                    break;
            }
        } else if (name == "CSVeventWeightLFStats2") {
            label = "Jet CSV weight";
            switch (kind) {
                case kUp:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightLFStats2up)[idx]; };
                    break;
                case kDown:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightLFStats2down)[idx]; };
                    break;
            }
        } else if (name == "CSVeventWeightCErr1") {
            label = "Jet CSV weight";
            switch (kind) {
                case kUp:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightCErr1up)[idx]; };
                    break;
                case kDown:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightCErr1down)[idx]; };
                    break;
            }
        } else if (name == "CSVeventWeightCErr2") {
            label = "Jet CSV weight";
            switch (kind) {
                case kUp:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightCErr2up)[idx]; };
                    break;
                case kDown:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->CSVeventWeightCErr2down)[idx]; };
                    break;
            }
        } else if (name == "brSF") {
            label = "BR correction";
            switch (kind) {
                case kNominal:
                    fct = [](roast::Branches *b, const int& idx) -> float {
                        unsigned int matches = 0;
                        for (const auto& id: *dynamic_cast<roast::ttl::Branches*>(b)->GT_ParentId)
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
                    fct = [](roast::Branches *b, const int& idx) -> float {
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
                    fct = [](roast::Branches *b, const int& idx) -> float {
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
                    fct = [](roast::Branches *b, const int& idx) -> float {
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
