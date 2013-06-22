#include "../interface/Branches.h"
#include "../interface/CutFlow.h"
#include "../interface/HWrapper.h"
#include "../interface/Helper.h"
#include "../interface/MVABase.h"
#include "../interface/Nidra.h"
#include "../interface/Process.h"
#include "../interface/TTLBranches.h"
#include "../interface/TTLMVABase.h"
#include "../interface/Weights.h"

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all structs;
#pragma link off all functions;

#pragma link C++ namespace roast;
#pragma link C++ namespace roast::ttl;
#pragma link C++ namespace roast::Helper;
#pragma link C++ nestedclasses;

#pragma link C++ class roast::Branches+;
#pragma link C++ class roast::ttl::Branches+;
#pragma link C++ class roast::HWrapper+;
#pragma link C++ class std::pair<std::string, roast::HWrapper*>+;
#pragma link C++ class std::map<std::string, roast::HWrapper*>+;
#pragma link C++ class roast::CutFlow+;
#pragma link C++ class roast::CutFlow::Cut+;
#pragma link C++ class roast::Process+;
#pragma link C++ class roast::Process::Event+;
#pragma link C++ class std::vector<roast::Process::Event>+;
#pragma link C++ class roast::MVABase+;
#pragma link C++ class roast::ttl::MVABase+;
#pragma link C++ class std::vector<roast::Process*>+;
#pragma link C++ class roast::Weights+;
#pragma link C++ function roast::Helper::SplitString;
#pragma link C++ function roast::ttl::analyze(roast::Process&, const std::vector<std::string>&, const int&);

#endif
