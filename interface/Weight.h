#ifndef __Weights_h
#define __Weights_h

#include <map>
#include <string>

#include "TObject.h"

#include "CutFlow.h"

namespace roast {
    class Branches;

    struct Weight {
        enum direction { kNominal, kUp, kDown };

        typedef float (*val_f)(roast::Branches*, const int&);
        val_f GetVal;
        float strength;

        std::string name;
        double sum;
        double n;

        static std::map<std::string, roast::Weight> weights;

        float operator()(Branches*, const int&);
        inline void RegisterCut(CutFlow* cflow) const { cflow->RegisterCutFromLast(name, 2, n > 0 ? sum / n : (n == 0 && sum == 0 ? 1 : 0)); };
        static Weight Create(const std::string&, direction, float=-1.);

        Weight(const std::string& l="", val_f fct=0, float s=-1.) :
            GetVal(fct), strength(s), name(l), sum(0), n(0) {};

        ClassDef(Weight, 1);
    };
}

#endif
