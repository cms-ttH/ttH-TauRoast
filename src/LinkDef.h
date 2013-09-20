#include "../interface/Accessor.h"
#include "../interface/Branches.h"
#include "../interface/CutFlow.h"
#include "../interface/HWrapper.h"
#include "../interface/MVABase.h"
#include "../interface/Nidra.h"
#include "../interface/Picker.h"
#include "../interface/Process.h"
#include "../interface/Splitter.h"
#include "../interface/TLLBranches.h"
#include "../interface/TTLBranches.h"
#include "../interface/Weight.h"

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all structs;
#pragma link off all functions;

#pragma link C++ namespace roast;
#pragma link C++ namespace roast::tll;
#pragma link C++ namespace roast::ttl;
#pragma link C++ nestedclasses;

#pragma link C++ class std::map<std::string, std::string>+;
#pragma link C++ class std::pair<std::string, std::string>+;
#pragma link C++ class roast::Branches+;
#pragma link C++ class roast::tll::Branches+;
#pragma link C++ class roast::ttl::Branches+;
#pragma link C++ class roast::HWrapper+;
#pragma link C++ class std::pair<std::string, roast::HWrapper*>+;
#pragma link C++ class std::map<std::string, roast::HWrapper*>+;
#pragma link C++ class roast::CutFlow+;
#pragma link C++ class roast::CutFlow::Cut+;
#pragma link C++ class roast::CutFlow::RelativeCut+;
#pragma link C++ class roast::CutFlow::ValueCut+;
#pragma link C++ class roast::CutFlow::ComposedCut+;
#pragma link C++ class std::vector<roast::CutFlow::Cut*>+;
#pragma link C++ class roast::Picker+;
#pragma link C++ class roast::PtPicker+;
#pragma link C++ class roast::tll::IsoPicker+;
#pragma link C++ class roast::ttl::IsoPicker+;
#pragma link C++ class roast::Process+;
#pragma link C++ class roast::Process::Event+;
#pragma link C++ class std::vector<roast::Process::Event>+;
#pragma link C++ class roast::MVABase+;
#pragma link C++ class roast::MVABase::Var+;
#pragma link C++ class roast::MVABase::Get+;
#pragma link C++ class roast::tll::Get+;
#pragma link C++ class roast::ttl::Get+;
#pragma link C++ class std::vector<roast::MVABase::Var>+;
#pragma link C++ class std::vector<roast::Process*>+;
#pragma link C++ class roast::Splitter+;
#pragma link C++ class roast::InclusiveSignalSplitter+;
#pragma link C++ class roast::Weight+;
#pragma link C++ class std::vector<roast::Weight>+;
#pragma link C++ function roast::register_mva;
#pragma link C++ function roast::setup_accessors;
#pragma link C++ function roast::tll::analyze(roast::Process&, const std::vector<std::string>&, const int&, void *);
#pragma link C++ function roast::ttl::analyze(roast::Process&, const std::vector<std::string>&, const int&, void *);
#pragma link C++ function roast::tll::fill(roast::Process&,  const std::vector<roast::Weight>&, void *, roast::Splitter*, roast::Picker*);
#pragma link C++ function roast::ttl::fill(roast::Process&, const std::vector<roast::Weight>&, void *, roast::Splitter*, roast::Picker*);

#endif
