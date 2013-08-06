// vim: ts=4:sw=4:et:sta
#ifndef Process_h
#define Process_h

#include <map>
#include <vector>
#include <string>

#include <iostream>

#include "CutFlow.h"
#include "HWrapper.h"

namespace roast {
    class Process {
        public:
            struct Event {
                Event(const int& n, const std::vector<int>& cs) : entry(n), combos(cs) {};
                Event(const Event& e) : entry(e.entry), combos(e.combos) {};
                Event() : entry(-1), combos() {};

                int entry;
                std::vector<int> combos;

                ClassDef(roast::Process::Event, 1);
            };

       private:
            std::vector<roast::Process::Event> goodEvents;
            std::map<std::string, roast::HWrapper*> hContainer;
          CutFlow					cutFlow;
          CutFlow					normalizedCutFlow;

          string shortName;
          string niceName;
          string labelForLegend;
          string type;
          string treename;
          bool checkReality;
            std::vector<std::string> ntuplePaths;
          int color;
          bool analyzed;
          bool plot;


          float crossSection;
          float branchingRatio;
          float otherScaleFactor;

          double NOEinDS;
          double NoEreadByNUTter;
          double NOEinNtuple;
          double NOEanalyzed;
          double NOEexpected;

          bool obtainedGoodEvents;
          bool filledHistos;
          bool normalizedHistos;
          
            double relSysUncertainty;

          bool PlotProcess(string const);

       public :
          // Default constructor
          Process();
          Process(const roast::Process&);
          Process(const std::string&, const std::string&, const std::string&, const std::string&,
                  const std::string&, const std::vector<std::string>&, int, int, int,
                  double, double, bool genmatch=false);
          virtual ~Process();
          void Update(roast::Process const *);


          int const			GetNOEinDS() const;
          int const			GetNoEreadByNUTter() const;
          int const			GetNOEinNtuple() const;
          int const			GetNOEanalyzed() const;

          inline std::string GetTreeName() const { return treename; };

          void				SetShortName(std::string const);
          void				SetNiceName(std::string const);
          void				SetLabelForLegend(std::string const);
          void				SetColor(int const);
          void				SetNOEanalyzed(double const);
          void				SetNOEinNtuple(double const);
          void				SetNtuplePaths(std::vector<std::string> const);
            void                SetRelSysUncertainty(double const);

          void				NormalizeToLumi(double const);
          void				BuildNormalizedCutFlow();
          void				SetAnalyzed();
          bool const			Analyzed() const;

            std::vector<Event> const GetGoodEvents() const;
          void				SetCutFlow(CutFlow const &);
          void				SetNormalizedCutFlow(CutFlow const &);
          CutFlow *			GetCutFlow();
          CutFlow const *		GetConstCutFlow() const;
          CutFlow *			GetNormalizedCutFlow();
          CutFlow const *		GetNormalizedCutFlow() const;

          int const			GetColor() const;
          string const		GetShortName() const;
          string const		GetNiceName() const;
          string const		GetLabelForLegend() const;
            std::vector<std::string> const		GetNtuplePaths() const;
          string const		GetType() const;
          bool const			CheckReality() const;
          bool const			IsCollisions() const;
          bool const			IsBackground() const;
          bool const			IsSignal() const;
          bool const			IsMC() const;
          bool const			Plot() const;

          double const		GetCrossSection() const;
          double const		GetBranchingRatio() const;
          double const		GetOtherScaleFactor() const;
          double const		GetNOEexpected() const;
            double const        GetRelSysUncertainty() const;
          bool const			ObtainedGoodEvents() const;
          bool const			FilledHistos() const;
          bool const			NormalizedHistos() const;


          inline void AddHistogram(const std::string& name, const roast::HWrapper& histo) { hContainer[name] = new HWrapper(histo); };
          inline std::map<std::string, roast::HWrapper*>& GetHContainer() {
              return hContainer;
          };
          inline const std::map<std::string, roast::HWrapper*> GetHContainer() const {
              return hContainer;
          };
          void ResetHistograms();
          void ScaleHistograms(double);
          std::vector<std::string> GetHistogramNames() const;

          void 				SetGoodEvents(const std::vector<roast::Process::Event>&);
          void				Add(roast::Process*);

          ClassDef(Process, 1);
    };
}

#endif
