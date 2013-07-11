#include "../interface/Nidra.h"
#include "../interface/TTLBranches.h"

using namespace std;

namespace roast {
    template<typename T>
    void
    analyze(roast::Process& proc, const std::vector<std::string>& cuts, const int& limit)
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

        cout << "\tAnalyzing " << proc.GetShortName() << endl;
        cout << "\t>>> Starting loop... " << " " << nentries << " entries available: ";
        cflow.SetCutCounts("nTuple making", nentries);

        if (limit <= 0 || limit >= nentries) {
            cout << "Processing all of them..." << string(14,'.') << " ";
        } else {
            cout << "Stopping at " << limit << " as per-user request" << string(14,'.') << " ";
        }
        cout.flush();

        //ofstream fout("events.txt");


        // Actual loop
        double NOEanalyzed = 0;
        double NOEwithAtLeastOneCombo = 0;
        for (Long64_t jentry=0; jentry<nentries; jentry++) {
            // Keep user informed of the number of events processed and if there is a termination due to reaching of the limit
            if ( limit > 0 && jentry >= (unsigned int)(limit)){ cout << "\n\t>>> INFO: Reached user-imposed event number limit (" << limit << "), skipping the rest." << endl; break; }

            if (jentry > 0 && (jentry + 1) % 1000 == 0) { 
                if (isatty(fileno(stdout))) {
                    ostringstream o;
                    o << (jentry - 999);
                    cout << string(o.str().length(), '\b') << (jentry + 1);
                } else if ((jentry + 1) % 10000 == 0) {
                    cout << " " << jentry + 1;
                }
                cout.flush();
            }
            event->GetEntry(jentry);

            if (event->TTL_NumCombos > 0)
                NOEwithAtLeastOneCombo++;

            // Inform cflow that a new event is starting
            cflow.StartOfEvent();

            vector<int> combos;
            for (unsigned int i = 0; i < event->TTL_NumCombos; ++i) {
                float comboMass = (*event->TTL_DitauVisibleMass)[i];
                if( comboMass <= 0 ){ cout << "WARNING: ditauMass < 0!" << endl;  continue; }

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

        cout << endl;

        proc.SetGoodEvents(good_events);
        proc.SetNOEinNtuple(nentries);
        proc.SetNOEanalyzed(NOEanalyzed);
        proc.SetCutFlow(cflow);

        delete event;
    }

    namespace ttl {
        void
        analyze(roast::Process& p, const std::vector<std::string>& cuts, const int& limit)
        {
            roast::analyze<roast::ttl::Branches>(p, cuts, limit);
        }
    }
}

NamespaceImp(roast)
// NamespaceImp(roast::ttl)
