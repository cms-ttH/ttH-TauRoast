#include "../interface/Nidra.h"
#include "../interface/TTLBranches.h"

#include "boost/python.hpp"

namespace roast {
    template<typename T>
    long
    analyze(roast::Process& proc, const std::vector<std::string>& cuts, const int& limit, PyObject *log)
    {
        CutFlow cflow(cuts);
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
        cflow.RegisterCut("TTL_AtLeastOneCombo", 0);

        cflow.SetCutCounts("Read from DS", proc.GetNOEinDS());
        cflow.SetCutCounts("skimming + PAT", proc.GetNoEreadByNUTter());

        int n_to_read = proc.GetNoEreadByNUTter();

        event->RegisterCuts(cflow);

        Long64_t nentries = event->GetEntries();
        if (nentries == 0) {
            cerr << "ERROR: process " << proc.GetShortName() << " has zero events to read" << endl;
            exit(1);
        } else if (nentries != n_to_read) {
            cerr << "WARNING: tree size does not match topology specification! "
                << nentries << " events found, " << n_to_read << " expected."  << endl;
        }

        cflow.SetCutCounts("nTuple making", nentries);

        double NOEanalyzed = 0;
        double NOEwithAtLeastOneCombo = 0;
        for (Long64_t jentry=0; jentry < nentries && jentry < limit; jentry++) {
            if (log) {
                PyGILState_STATE state = PyGILState_Ensure();
                boost::python::call<void>(log, jentry);
                PyGILState_Release(state);
            }

            event->GetEntry(jentry);

            if (event->TTL_NumCombos > 0)
                NOEwithAtLeastOneCombo++;

            // Inform cflow that a new event is starting
            cflow.StartOfEvent();

            vector<int> combos;
            for (unsigned int i = 0; i < event->TTL_NumCombos; ++i) {
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

        cflow.SetCutCounts("TTL_AtLeastOneCombo", NOEwithAtLeastOneCombo);

        proc.SetGoodEvents(good_events);
        proc.SetNOEinNtuple(nentries);
        proc.SetNOEanalyzed(NOEanalyzed);
        proc.SetCutFlow(cflow);

        delete event;

        return long(NOEanalyzed);
    }

    namespace ttl {
        long
        analyze(roast::Process& p, const std::vector<std::string>& cuts, const int& limit, PyObject *log)
        {
            return roast::analyze<roast::ttl::Branches>(p, cuts, limit, log);
        }
    }
}

NamespaceImp(roast)
