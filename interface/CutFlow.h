// vim: ts=4:sw=4:et:sta
#ifndef CutFlow_h
#define CutFlow_h

#include <climits>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <TObject.h>

namespace roast {
    class Branches;

    class CutFlow {
       private:
          bool eventPassed;
          bool comboIs;
          bool signalComboLocked;
          int bestCombo;

          bool OutOfRange(float, float, float);

    //*/
       public :
            struct Cut {
                typedef float (*val_f)(Branches*, const int&);

                std::string name;
                val_f GetVal;
                int rank;
                float min;
                float max;
                float passedSignalCombos;
                float passedSignalEvents;
                bool currentSignalResult;
                bool skip;

                bool Check(Branches*, const int, const bool bypass=false);

                Cut(const std::string n="", val_f=0, const int r=0, const float mn=0., const float mx=0.,
                        const double sig=0., bool bypass=false);
                Cut(const std::string&, const float, const float);

                ClassDef(roast::CutFlow::Cut, 1);
            };

            inline std::vector<roast::CutFlow::Cut> GetCuts() const { return cuts; };

          // Default constructor
          CutFlow();
          CutFlow(roast::CutFlow const &);

          void Reset();
          int const size() const;

            void RegisterCut(const roast::CutFlow::Cut&);
            void RegisterCut(std::string const, int const, double const sig=0.);
          void RegisterCutFromLast(std::string const, int const, double const);
          void SetCutCounts(std::string const, double const);

            bool CheckCuts(Branches*, const int&, const bool bypass=false);
          void StartOfEvent();
          void EndOfEvent();

          int const					GetCutRank(std::string const) const;
          float const					GetPassedEvents(std::string const) const;
          float const					GetRelEff(std::string const) const;
          float const					GetCumEff(std::string const) const;
            std::string const			GetLastCut() const;
          double const				GetLastCount() const;
          void						Add(roast::CutFlow const &, float const iFactor=1.0);
          void						BuildNormalizedCutFlow(roast::CutFlow const *);
            void Zero();

        private:
            std::vector<roast::CutFlow::Cut> cuts;
            std::map<std::string ,int> name2idx;

          ClassDef(CutFlow, 1);
    };
}

#endif

