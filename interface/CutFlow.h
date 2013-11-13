// vim: ts=4:sw=4:et:sta
#ifndef CutFlow_h
#define CutFlow_h

#include <climits>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <TObject.h>

#include "Accessor.h"

namespace roast {
    class Branches;

    class CutFlow {
        private:
            bool eventPassed;
            bool comboIs;
            bool signalComboLocked;

            bool OutOfRange(float, float, float);

        public :
            struct Cut {
                std::string name;
                double events_passed;
                bool current_passed;
                bool immutable;

                bool Check(Branches*, const int, const bool bypass=false);

                Cut(const std::string& n="", double ev=0.) : name(n), events_passed(ev), current_passed(false), immutable(false) {};

                virtual Cut* Clone() { return new Cut(*this); };
                virtual bool IsRelative() { return false; };
                virtual bool RealCheck(Branches*, int, bool) { return true; };

                ClassDef(roast::CutFlow::Cut, 2);
            };

            struct RelativeCut : public Cut {
                RelativeCut(const std::string& n="", double ev=0) : Cut(n, ev) {};

                virtual Cut* Clone() { return new RelativeCut(*this); };
                virtual bool IsRelative() { return true; };

                ClassDef(roast::CutFlow::RelativeCut, 1);
            };

            struct ValueCut : public Cut {
                bool negate;
                bool skip;
                float min;
                float max;
                GetValue_t GetVal;

                ValueCut() : Cut(""), negate(false), skip(false), min(0), max(0), GetVal(0) {};
                ValueCut(const std::string&, float, float, bool=false, bool=false);

                virtual Cut* Clone() { return new ValueCut(*this); };
                virtual bool RealCheck(Branches*, int, bool);

                ClassDef(roast::CutFlow::ValueCut, 1);
            };

            struct ComposedCut : public Cut {
                enum kind_t { kAnd, kOr } kind;

                bool negate;
                Cut *op1;
                Cut *op2;

                ComposedCut() : Cut(), kind(kAnd), negate(false), op1(0), op2(0) {};
                ComposedCut(const std::string& n, Cut* c1, Cut* c2, kind_t k, bool neg) : Cut(n), kind(k), negate(neg), op1(c1), op2(c2) {};

                virtual Cut* Clone() { return new ComposedCut(*this); };
                virtual bool RealCheck(Branches*, int, bool);

                ClassDef(roast::CutFlow::ComposedCut, 1);
            };

            typedef std::vector<roast::CutFlow::Cut*> Cuts;

            inline Cuts GetCuts() const { return cuts; };

            CutFlow() {};
            CutFlow(roast::CutFlow const &);

            void Reset();
            int const size() const;

            void RegisterCut(roast::CutFlow::Cut*);
            void RegisterCut(const std::string&, double=0.);
            void RegisterCutFromLast(const std::string&, double);
            void SetCutCounts(std::string const, double const);

            bool CheckCuts(Branches*, const int&, const bool bypass=false);
            void StartOfEvent();

            inline float GetPassedEvents(const std::string& n) const { return cuts[name2idx.find(n)->second]->events_passed; };
            float const GetRelEff(std::string const) const;
            float const GetCumEff(std::string const) const;
            std::string const GetLastCut() const;
            inline double GetLastCount() const { return cuts.back()->events_passed; };

            void Add(roast::CutFlow const &, float const iFactor=1.0);
            void BuildNormalizedCutFlow(roast::CutFlow const *);
            void Zero();

        private:
            Cuts cuts;
            std::map<std::string ,int> name2idx;

            ClassDef(CutFlow, 1);
    };
}

#endif

