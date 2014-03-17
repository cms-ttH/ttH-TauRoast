#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "TFile.h"

#include "ttH/TauRoast/interface/Process.h"

struct ProcessInfo {
    ProcessInfo() : p(0), analyzed(0), jobs(0) {};
    ProcessInfo(roast::Process *o) : p(o), analyzed(o->GetNOEanalyzed()), jobs(1) {};
    void Add(roast::Process *o) {
        p->Add(o, true);
        analyzed += o->GetNOEanalyzed();
        ++jobs;
    };

    roast::Process* p;
    long analyzed;
    long jobs;
};

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        std::cerr << "usage: " << argv[0] << " outfile files..." << std::endl;
        return 1;
    }

    TFile f(argv[1], "CREATE");
    if (!f.IsOpen()) {
        std::cerr << "can't open file '" << argv[1] << "' for writing!" << std::endl;
        return 1;
    }

    std::map<std::string, ProcessInfo> procs;
    for (int i = 2; i < argc; ++i) {
        std::vector<roast::Process*> *ps;

        TFile f2(argv[i]);
        if (!f2.IsOpen()) {
            std::cerr << "can't open file '" << argv[i] << "'!" << std::endl;
            continue;
        }

        f2.GetObject("Roast", ps);

        for (const auto& p: *ps) {
           auto it = procs.find(p->GetShortName());
           if (it == procs.end()) {
              procs[p->GetShortName()] = ProcessInfo(new roast::Process(*p));
           } else {
              it->second.Add(p);
           }
        }
    }

    std::vector<roast::Process*> ps;
    for (const auto& p: procs) {
       ps.push_back(p.second.p);
       std::cout << p.second.p->GetShortName() << ": " << p.second.jobs
           << " jobs, " << p.second.analyzed << " events analyzed" << std::endl;
    }
    f.WriteObject(&ps, "Roast");
}
