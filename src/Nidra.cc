// vim: ts=4:sw=4:et:sta
#include "../interface/Nidra.h"
#include "../interface/TLLBranches.h"
#include "../interface/TTLBranches.h"

#include "boost/python.hpp"

namespace roast {
    template<typename T>
    long
    analyze(roast::Process& proc, const std::vector<roast::CutFlow::Cut>& cuts, const int& limit, PyObject *log)
    {
        CutFlow cflow;
        cflow.Reset();
        cflow.Zero();

        std::vector<roast::Process::Event> good_events;

        T *event = new T(proc.GetTreeName(), proc.GetNtuplePaths());

        bool checkReality = proc.CheckReality();

        cflow.RegisterCut("Read from DS", 0);
        cflow.RegisterCut("skimming + PAT", 0);
        cflow.RegisterCut("nTuple making", 0);
        if (limit >= 0)
            cflow.RegisterCut("User event limit", 0);
        cflow.RegisterCut("AtLeastOneCombo", 0);

        cflow.SetCutCounts("Read from DS", proc.GetNOEinDS());
        // This is not very meaningful at the moment:
        // customarily set to nentries in the topology specification
        // cflow.SetCutCounts("skimming + PAT", proc.GetNoEreadByNUTter());
        for (const auto& cut: cuts)
            cflow.RegisterCut(cut);

        Long64_t nentries = event->GetEntries();
        cflow.SetCutCounts("nTuple making", nentries);

        double NOEanalyzed = 0;
        double NOEwithAtLeastOneCombo = 0;
        for (Long64_t jentry=0; jentry < nentries && (jentry < limit || limit <= 0); jentry++) {
            if (log) {
                PyGILState_STATE state = PyGILState_Ensure();
                boost::python::call<void>(log, jentry);
                PyGILState_Release(state);
            }

            event->GetEntry(jentry);

            if (event->GetNumCombos() > 0)
                NOEwithAtLeastOneCombo++;

            // Inform cflow that a new event is starting
            cflow.StartOfEvent();

            vector<int> combos;
            for (unsigned int i = 0; i < event->GetNumCombos(); ++i) {
                if (cflow.CheckCuts(event, i, !checkReality))
                    combos.push_back(i);
            }

            // Fill good event vectors for signal analysis
            if (combos.size() > 0) {
                good_events.push_back(Process::Event(jentry, combos));
            }

            NOEanalyzed++;
        }

        if (limit >= 0)
            cflow.SetCutCounts("User event limit", NOEanalyzed);

        cflow.SetCutCounts("AtLeastOneCombo", NOEwithAtLeastOneCombo);

        proc.SetGoodEvents(good_events);
        proc.SetNOEinNtuple(nentries);
        proc.SetNOEanalyzed(NOEanalyzed);
        proc.SetCutFlow(cflow);

        delete event;

        return long(NOEanalyzed);
    }

    namespace tll {
        long
        analyze(roast::Process& p, const std::vector<roast::CutFlow::Cut>& cuts, const int& limit, PyObject *log)
        {
            return roast::analyze<roast::tll::Branches>(p, cuts, limit, log);
        }
    }

    namespace ttl {
        long
        analyze(roast::Process& p, const std::vector<roast::CutFlow::Cut>& cuts, const int& limit, PyObject *log)
        {
            return roast::analyze<roast::ttl::Branches>(p, cuts, limit, log);
        }
    }
}

NamespaceImp(roast)
