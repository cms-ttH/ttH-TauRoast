#include <cmath>

#include "../interface/TTLBranches.h"
#include "../interface/Weight.h"

namespace roast {
    std::map<std::string, roast::Weight> Weight::weights;

    float
    Weight::operator()(Branches* b, const int& idx)
    {
        float w = GetVal(b, idx);
        if (strength != 1.)
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
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->topPtWeight; };
                    break;
                case kUp:
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->topPtWeightUp; };
                    break;
                case kDown:
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->topPtWeightDown; };
                    break;
            }
        } else if (name == "lepton") {
            label = "Lepton SF";
            switch (kind) {
                case kNominal:
                    fct = [](roast::Branches *b, const int& idx) -> float { return (*b->TTL_LeptonEventWeight)[idx]; };
                    break;
            }
        } else if (name == "PUcorr") {
            label = "PU reweighing";
            switch (kind) {
                case kNominal:
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->puWeight; };
                    break;
                case kUp:
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->puWeightUp; };
                    break;
                case kDown:
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->puWeightDown; };
                    break;
            }
        } else if (name == "qSquared") {
            label = "Q^2 shift";
            switch (kind) {
                case kUp:
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->q2WeightUp; };
                    break;
                case kDown:
                    fct = [](roast::Branches *b, const int& idx) -> float { return b->q2WeightDown; };
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
        }

        if (!fct)
            throw "";

        return Weight(label, fct, strength);
    }
}

ClassImp(roast::Weight)
