#ifdef FOOBARBAD
// vim: ts=4:sw=4:et:sta
#include "TFile.h"

#include "../interface/MVABase.h"
#include "../interface/LLBranches.h"

using namespace std;
using namespace roast;

// Default constructor
ll::Branches::Branches() : roast::Branches() {
    Null();
}

ll::Branches::Branches(const vector<string>& paths) :
    roast::Branches(paths)
{
    Null();
    SetBranchAddresses();
}

ll::Branches::~Branches()
{
    Delete();
    Null();
}

ClassImp(roast::ll::Branches)
#endif
