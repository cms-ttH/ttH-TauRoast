// vim: ts=4:sw=4:et:sta
#include <fstream>
#include <unordered_set>

#include "../interface/Nidra.h"
#include "../interface/TLBranches.h"
#include "../interface/TLLBranches.h"
#include "../interface/TTLBranches.h"

#include "boost/python.hpp"

typedef std::tuple<unsigned long, unsigned long, unsigned long> info;

namespace std {
    template <>
        class hash<info>{
            public:
                size_t operator()(const info &i) const {
                    return hash<unsigned long>()(get<0>(i))
                        ^ hash<unsigned long>()(get<1>(i))
                        ^ hash<unsigned long>()(get<2>(i));
                }
        };
}

namespace roast {
    template<typename T>
    long
    analyze(roast::Process& proc, const roast::CutFlow::Cuts& cuts, const int& limit, PyObject *log)
    {
        CutFlow cflow;
        cflow.Reset();
        cflow.Zero();

        std::unordered_set<info> infos;
        std::unordered_set<info> debug_events;
        std::fstream fs;
        fs.open("events.txt", std::fstream::in);
        if (fs.is_open()) {
            unsigned long run, ls, event;
            while (fs >> run >> ls >> event) {
                info i(run, ls, event);
                debug_events.insert(i);
            }
        }

        std::vector<roast::Process::Event> events;

        T *event = new T(proc.GetNtuplePaths());

        bool checkReality = proc.CheckReality();

        cflow.RegisterCut("Read from DS", 0);
        // cflow.RegisterCut("skimming + PAT", 0);
        cflow.RegisterCut("nTuple making", 0);
        if (limit >= 0)
            cflow.RegisterCut("User event limit", 0);
        cflow.RegisterCut("AtLeastOneCombo", 0);

        cflow.SetCutCounts("Read from DS", proc.GetNOEinDS());
        // This is not very meaningful at the moment:
        // customarily set to nentries in the topology specification
        // cflow.SetCutCounts("skimming + PAT", proc.GetNoEreadByNUTter());
        for (const auto& cut: cuts)
            cflow.RegisterCut(cut->Clone());

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

            // std::cout << jentry << std::endl;

            event->GetEntry(jentry);

            info i(event->Ev_runNumber, event->Ev_lumiBlock, event->Ev_eventNumber);
            if (infos.find(i) != infos.end())
                continue;
            infos.insert(i);

            if (event->GetNumCombos() > 0)
                NOEwithAtLeastOneCombo++;

            bool debug = debug_events.find(i) != debug_events.end();
            if (debug)
                std::cout << std::get<0>(i) << "\t" << std::get<1>(i) << "\t" << std::get<2>(i) << std::endl;

            // Inform cflow that a new event is starting
            cflow.StartOfEvent();

            std::vector<int> combos;
            for (unsigned int i = 0; i < event->GetNumCombos(); ++i) {
                if (cflow.CheckCuts(event, i, !checkReality, debug))
                    combos.push_back(i);
            }

            // Fill good event vectors for signal analysis
            if (combos.size() > 0) {
                events.push_back(Process::Event(jentry, combos));
            }

            NOEanalyzed++;
        }

        if (limit >= 0) {
            cflow.SetCutCounts("User event limit", NOEanalyzed);
            proc.SetNOEinNtuple(nentries); // this does not remove duplicated events
        } else {
            proc.SetNOEinNtuple(infos.size());
        }

        cflow.SetCutCounts("AtLeastOneCombo", NOEwithAtLeastOneCombo);

        proc.SetEvents(events);
        proc.SetNOEanalyzed(NOEanalyzed);
        proc.SetCutFlow(cflow);

        delete event;

        return long(NOEanalyzed);
    }

    template<typename T>
    long
    fill(roast::Process& proc, std::vector<roast::Weight>& weights, PyObject* log, roast::Splitter *s, roast::Picker *p)
    {
        T* branches = new T(proc.GetNtuplePaths());

        long count = 0;
        for (const auto& e: proc.GetEvents()) {
            branches->GetEntry(e.entry);

            int idx = p->Pick(branches, e.combos);

            if (s && !s->Use(branches, idx))
                continue;

            if (log && count % 100 == 0) {
                PyGILState_STATE state = PyGILState_Ensure();
                boost::python::call<void>(log, count);
                PyGILState_Release(state);
            }

            float weight = 1.;
            if (proc.IsMC()) {
                for (auto& w: weights) {
                    weight *= w(branches, idx);
                }
            }

            for (auto& h: proc.GetHContainer()) {
                try {
                    // std::cout << h.first << std::endl;
                    h.second->Fill(branches, idx, weight);
                } catch (std::out_of_range e) {}
            }

            ++count;
        }

        delete branches;

        return count;
    }

    namespace tl {
        long
        analyze(roast::Process& p, const roast::CutFlow::Cuts& cuts, const int& limit, PyObject *log)
        {
            return roast::analyze<roast::tl::Branches>(p, cuts, limit, log);
        }

        long
        fill(roast::Process& proc, std::vector<roast::Weight>& weights, PyObject* log, roast::Splitter *s, roast::Picker *p)
        {
            return roast::fill<roast::tl::Branches>(proc, weights, log, s, p);
        }
    }

    namespace tll {
        long
        analyze(roast::Process& p, const roast::CutFlow::Cuts& cuts, const int& limit, PyObject *log)
        {
            return roast::analyze<roast::tll::Branches>(p, cuts, limit, log);
        }

        long
        fill(roast::Process& proc, std::vector<roast::Weight>& weights, PyObject* log, roast::Splitter *s, roast::Picker *p)
        {
            return roast::fill<roast::tll::Branches>(proc, weights, log, s, p);
        }
    }

    namespace ttl {
        long
        analyze(roast::Process& p, const roast::CutFlow::Cuts& cuts, const int& limit, PyObject *log)
        {
            return roast::analyze<roast::ttl::Branches>(p, cuts, limit, log);
        }

        long
        fill(roast::Process& proc, std::vector<roast::Weight>& weights, PyObject* log, roast::Splitter *s, roast::Picker *p)
        {
            return roast::fill<roast::ttl::Branches>(proc, weights, log, s, p);
        }
    }
}

NamespaceImp(roast)
