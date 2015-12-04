#include "ttH/TauRoast/interface/Fastlane.h"
#include "ttH/TauRoast/interface/SuperSlim.h"

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all structs;
#pragma link off all functions;

#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedefs;

#pragma link C++ namespace fastlane;
#pragma link C++ class fastlane::BasicCut+;
#pragma link C++ class fastlane::Cut+;
#pragma link C++ class fastlane::StaticCut+;
#pragma link C++ calss fastlane::BasicLeaf+;
#pragma link C++ calss fastlane::Leaf+;
#pragma link C++ function fastlane::process(const std::string& process, TChain& c, TTree& t, std::vector<fastlane::Cut*>& cuts, std::vector<fastlane::StaticCut*>& weights, const std::string& sys);
#pragma link C++ namespace superslim;
#pragma link C++ typedef superslim::LorentzVector;
#pragma link C++ class superslim::PhysObject+;
#pragma link C++ class std::vector<superslim::PhysObject>+;
#pragma link C++ class superslim::Jet+;
#pragma link C++ class superslim::Lepton+;
#pragma link C++ class superslim::Tau+;
#pragma link C++ class std::vector<superslim::Jet>+;
#pragma link C++ class std::vector<superslim::Lepton>+;
#pragma link C++ class std::vector<superslim::Tau>+;
#pragma link C++ class superslim::Combination+;
#pragma link C++ class superslim::Vertex+;
#pragma link C++ class std::vector<superslim::Vertex>+;
#pragma link C++ class std::vector<superslim::Combination>+;
#pragma link C++ class superslim::Trigger+;
#pragma link C++ class superslim::Event+;

#endif
