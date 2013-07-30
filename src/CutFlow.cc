// vim: ts=4:sw=4:et:sta
#include <limits>

#include "boost/lexical_cast.hpp"

#include "../interface/Helper.h"
#include "../interface/MVABase.h"
#include "../interface/TLLBranches.h"
#include "../interface/TTLBranches.h"
#include "../interface/CutFlow.h"

using namespace std;
using namespace roast;

// Second constructor at the end of file!
CutFlow::Cut::Cut(const string n, CutFlow::Cut::val_f f, const int r, const float mn, const float mx, const double sig, bool bypass):
    name(n), GetVal(f), rank(r), min(mn), max(mx),
    passedSignalEvents(sig), currentSignalResult(false),
    skip(bypass)
{
}

bool
CutFlow::Cut::Check(Branches *b, const int idx, const bool bypass)
{
    float val = GetVal(b, idx);
    if ((rank != 1) || (skip && bypass) || (min <= val && val <= max)) {
        if (!currentSignalResult) {
            ++passedSignalEvents;
            currentSignalResult = true;
        }
        return true;
    } else {
        return false;
    }
}

// Default constructor
CutFlow::CutFlow(){}

CutFlow::CutFlow(const vector<string>& cuts)
{
    using boost::lexical_cast;

    for (const auto& s: cuts) {
        auto first_colon = s.find(":");
        auto last_colon = s.rfind(":");

        string name = s.substr(0, first_colon);

        float max = numeric_limits<float>::infinity();
        float min = -numeric_limits<float>::infinity();
        try {
            max = lexical_cast<float>(s.substr(last_colon + 1));
        } catch (...) {}
        try {
            min = lexical_cast<float>(s.substr(first_colon + 1, last_colon - first_colon - 1));
        } catch (...) {}

        cuts_to_consider[name] = make_pair(min, max);
    }
}

CutFlow::CutFlow(CutFlow const & iCutFlow){
    cuts = iCutFlow.GetCuts();
    name2idx.clear();
    int idx = 0;
    for (const auto& c: cuts)
        name2idx[c.name] = idx++;

	cutsToApply	= iCutFlow.GetCutsToApply();
    cuts_to_consider = iCutFlow.GetCutsToConsider();

	eventPassed	= false;
	comboIs		= false;

	bestCombo	= -1;
}


// Default destructor
CutFlow::~CutFlow(){}

void CutFlow::Reset(){
	eventPassed = false;
	comboIs     = false;

	bestCombo = -1;

    cuts.clear();
    name2idx.clear();
}

void CutFlow::Zero() {
    for (auto& c: cuts) {
        c.passedSignalEvents = 0;
        c.passedSignalCombos = 0;
    }

    eventPassed = false;
    comboIs     = false;

    bestCombo = -1;
}

int const CutFlow::size() const { return cuts.size(); }

void CutFlow::RegisterCut(const CutFlow::Cut& cut)
{
    cuts.push_back(cut);
}

void CutFlow::RegisterCut(string const name, int const rank,  double const iEvents){
    if (!(rank == 0 || 2 == rank)) {
        cerr << "ERROR: Cut named \"" << name << "\" is trying to be registered with rank " << rank << " but rank can only be 0 or 2." << endl;
        exit(1);
    }
    Cut new_cut(name, [](Branches *b, const int& idx) -> float { return 0.; },
            rank, 0, 0, iEvents);
    cuts.push_back(new_cut);
    name2idx[name] = cuts.size() - 1;
}

void
CutFlow::RegisterCut(const string name, const int rank, CutFlow::Cut::val_f f, bool bypass, const double sig)
{
    auto res = cuts_to_consider.find(name);
    if (res == cuts_to_consider.end()) {
        return;
    }

    Cut new_cut(name, f, rank, res->second.first, res->second.second, sig, bypass);
    cuts.push_back(new_cut);
    name2idx[name] = cuts.size() - 1;
}

void
CutFlow::RegisterCutFromLast(string const iName, int const iRank, double const iFactor)
{
    RegisterCut(iName, iRank, iFactor*GetLastCount());
}

void
CutFlow::SetCutCounts(string const iName, double const iEvents)
{
    cuts[name2idx[iName]].passedSignalEvents = iEvents;
}

bool
CutFlow::CheckCuts(Branches *b, const int& idx, const bool bypass)
{
    for (auto& c: cuts)
        if (c.rank == 1 && !c.Check(b, idx, bypass))
            return false;
    return true;
}

// Reset counters relevant to the start of the event
void
CutFlow::StartOfEvent()
{
    bestCombo	= -1;
    eventPassed	= false;	

    for (auto& c: cuts) {
        c.passedSignalCombos = 0;
        c.currentSignalResult = false;
    }

    signalComboLocked = false;
}

// At the end of the event, loop over combo counter and check independently which combos have passed which cuts
void CutFlow::EndOfEvent(){
    // Loop over all the cuts
    for (auto& c: cuts) {
        if (c.rank != 1)
            continue; // Worry only about other-than-rank 1 cuts

        // If at least one combo for signal has passed this cut, increase the event count in the "forSignal" cut map FOR THAT CUT only
        if (c.passedSignalCombos > 0)
            c.passedSignalEvents++;

        // Wait for the last registered cut to determine the outcome of the event
        if (c.name == cuts.back().name)
            eventPassed = (c.passedSignalCombos > 0);

        // Reset combo counter
        c.passedSignalCombos = 0;
    }
}

int const CutFlow::GetCutRank(string const iCut) const { 
    return cuts[name2idx.find(iCut)->second].rank;
}

int const CutFlow::GetCutPosition(string const iCut) const { 
    return name2idx.find(iCut)->second;
}

string const				CutFlow::GetCutsToApply() const { return cutsToApply; }


float const CutFlow::GetPassedEvents(string const iCut) const {
    return cuts[name2idx.find(iCut)->second].passedSignalEvents;
}

float const CutFlow::GetRelEff(string const iCut) const {
    const int idx = name2idx.find(iCut)->second;
    if (idx == 0)
        return 1.;
    else if (idx < 0 || !cuts[idx - 1].passedSignalEvents)
        return 0.;
    return cuts[idx].passedSignalEvents / cuts[idx - 1].passedSignalEvents;
}

float const CutFlow::GetCumEff(string const iCut) const {
    const int idx = name2idx.find(iCut)->second;
    return cuts[idx].passedSignalEvents / cuts.front().passedSignalEvents;
}

// Check whether iValue is within range (useful for selection cuts)
bool CutFlow::OutOfRange(float iValue, float iMin, float iMax){
	return ((iValue < iMin) || (iMax < iValue));
}

string const CutFlow::GetLastCut() const{
    return cuts.back().name;
}

double const CutFlow::GetLastCount() const { return  GetPassedEvents(GetLastCut()); }

void CutFlow::Add(CutFlow const & iCutFlow, float const iFactor){
    // Check the current cuts	
    if (cuts.size() == 0) {
        // If there are none, add them from the input CutFlow
        cuts = iCutFlow.GetCuts();
        for (auto& c: cuts) {
            c.passedSignalEvents *= iFactor;
        }
    } else {
        // Else, first check that we have the same sizes
        if (size() != iCutFlow.size()) {
            cerr << "ERROR: Current CutFlow has size " << size() << " while input CutFlow has size " << iCutFlow.size() << "." << endl;
            exit(1);
        }
        // Then check that all cuts are the same
        vector<CutFlow::Cut> other_cuts = iCutFlow.GetCuts();
        for (unsigned int i = 0; i < cuts.size(); ++i) {
            if (cuts[i].name != other_cuts[i].name) {
                cerr << "ERROR: Inconsistency in cut names of current and input CutFlows:" << endl; 
                cerr << "\tCurrent\t\tInput" << endl;
                cerr << "\t-------\t\t-----" << endl;
                for (unsigned int j = 0; j < cuts.size(); ++j) {
                    cerr << "\t" << cuts[j].name << "\t" << other_cuts[j].name << endl;
                }
                exit(1);
            } else {
                cuts[i].passedSignalEvents += other_cuts[i].passedSignalEvents * iFactor;
            }
        }
    }
}

void CutFlow::BuildNormalizedCutFlow(CutFlow const * iCutFlow){
    Reset();

    cutsToApply = iCutFlow->GetCutsToApply();
    cuts_to_consider = iCutFlow->cuts_to_consider;

    vector<Cut> other_cuts = iCutFlow->GetCuts();
    double scaleFactor = 1.0;

    for (auto& c: other_cuts) {
        if (c.rank != 2)
            continue;
        scaleFactor *= iCutFlow->GetRelEff(c.name);
    }

    for (auto& c: other_cuts) {
        if (c.rank == 2)
            continue;
        RegisterCut(c.name, c.rank, c.GetVal, c.skip,
                scaleFactor * c.passedSignalEvents);
    }
}

CutFlow::Cut::Cut(const std::string& n, const float mn, const float mx) :
    name(n), GetVal(0), rank(1), min(mn), max(mx), passedSignalEvents(0), currentSignalResult(false), skip(false)
{
    if (n == "T_MatchAbsId") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return abs((*e->TLL_TauGenMatchId)[idx]);
        };
        skip = true;
    } else if (n == "T_ParentAbsId") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return abs((*e->TLL_TauGenMatchMother0Id)[idx]);
        };
        skip = true;
    } else if (n == "T1_MatchAbsId") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->TTL_Tau1GenMatchId)[idx]);
        };
        skip = true;
    } else if (n == "T2_MatchAbsId") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->TTL_Tau2GenMatchId)[idx]);
        };
        skip = true;
    } else if (n == "T1_ParentAbsId") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->TTL_Tau1GenMatchMother0Id)[idx]);
        };
        skip = true;
    } else if (n == "T2_ParentAbsId") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return abs((*e->TTL_Tau2GenMatchMother0Id)[idx]);
        };
        skip = true;
    } else if (n == "J_NumCSVL") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCSVLbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCSVLbtagJets)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "J_NumCSVM") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCSVMbtagJets)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "J_NumCSVT") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCSVTbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCSVTbtagJets)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "J_NumNonCSVL") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumNonCSVLbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumNonCSVLbtagJets)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "J_NumNonCSVM") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumNonCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumNonCSVMbtagJets)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "J_NumNonCSVT") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumNonCSVTbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumNonCSVTbtagJets)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "J_NumInclusive") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCSVMbtagJets)[idx] + (*e->TLL_NumNonCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCSVMbtagJets)[idx] + (*e->TTL_NumNonCSVMbtagJets)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "J_NumCleanCSVL") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanCSVLbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanCSVLbtagJets)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "J_NumCleanCSVM") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanCSVMbtagJets)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "J_NumCleanCSVT") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanCSVTbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanCSVTbtagJets)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "J_NumCleanNonCSVL") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanNonCSVLbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanNonCSVLbtagJets)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "J_NumCleanNonCSVM") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanNonCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanNonCSVMbtagJets)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "J_NumCleanNonCSVT") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanNonCSVTbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanNonCSVTbtagJets)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "J_NumCleanInclusive") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumCleanCSVMbtagJets)[idx] + (*e->TLL_NumCleanNonCSVMbtagJets)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumCleanCSVMbtagJets)[idx] + (*e->TTL_NumCleanNonCSVMbtagJets)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "NumTightLeptons") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumTightMuons)[idx] + (*e->TLL_NumTightElectrons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumTightMuons)[idx] + (*e->TTL_NumTightElectrons)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "NumTightMuons") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumTightMuons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumTightMuons)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "NumTightElectrons") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumTightElectrons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumTightElectrons)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "NumLooseLeptons") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumLooseMuons)[idx] + (*e->TLL_NumLooseElectrons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumLooseMuons)[idx] + (*e->TTL_NumLooseElectrons)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "NumLooseMuons") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumLooseMuons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumLooseMuons)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "NumLooseElectrons") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumLooseElectrons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumLooseElectrons)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "NumExLooseLeptons") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumExLooseMuons)[idx] + (*e->TLL_NumExLooseElectrons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumExLooseMuons)[idx] + (*e->TTL_NumExLooseElectrons)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "NumExLooseMuons") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumExLooseMuons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumExLooseMuons)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "NumExLooseElectrons") {
        GetVal = [](Branches *b, const int& idx) -> float {
            if (tll::Branches* e = dynamic_cast<tll::Branches*>(b)) {
                return (*e->TLL_NumExLooseElectrons)[idx];
            } else if (ttl::Branches* e = dynamic_cast<ttl::Branches*>(b)) {
                return (*e->TTL_NumExLooseElectrons)[idx];
            } else {
                throw;
            }
        };
    } else if (n == "L_pT") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonPt)[idx];
        };
    } else if (n == "L_Eta") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonEta)[idx];
        };
    } else if (n == "L_IsLoose") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonIsLoose)[idx];
        };
    } else if (n == "L_IsTight") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonIsTight)[idx];
        };
    } else if (n == "L_RelIso") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_LeptonRelIso)[idx];
        };
    } else if (n == "L1_pT") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1Pt)[idx];
        };
    } else if (n == "L1_Eta") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1Eta)[idx];
        };
    } else if (n == "L1_IsLoose") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1IsLoose)[idx];
        };
    } else if (n == "L1_IsTight") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1IsTight)[idx];
        };
    } else if (n == "L1_RelIso") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton1RelIso)[idx];
        };

    } else if (n == "L2_pT") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2Pt)[idx];
        };
    } else if (n == "L2_Eta") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2Eta)[idx];
        };
    } else if (n == "L2_IsLoose") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2IsLoose)[idx];
        };
    } else if (n == "L2_IsTight") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2IsTight)[idx];
        };
    } else if (n == "L2_RelIso") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_Lepton2RelIso)[idx];
        };

    } else if (n == "T_pT") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauPt)[idx];
        };
    } else if (n == "T_Eta") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauEta)[idx];
        };
    } else if (n == "T_InCracks") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauIsInTheCracks)[idx];
        };
    } else if (n == "LL_DeltaR") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return DeltaR((*e->TLL_Lepton1Eta)[idx], (*e->TLL_Lepton1Phi)[idx],
                (*e->TLL_Lepton2Eta)[idx], (*e->TLL_Lepton2Phi)[idx]);
        };
    } else if (n == "TL1_DeltaR") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return DeltaR((*e->TLL_TauEta)[idx], (*e->TLL_TauPhi)[idx],
                (*e->TLL_Lepton1Eta)[idx], (*e->TLL_Lepton1Phi)[idx]);
        };
    } else if (n == "TL2_DeltaR") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return DeltaR((*e->TLL_TauEta)[idx], (*e->TLL_TauPhi)[idx],
                (*e->TLL_Lepton2Eta)[idx], (*e->TLL_Lepton2Phi)[idx]);
        };
    } else if (n == "T_DecayModeFinding") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSdecayModeFinding)[idx];
        };
    } else if (n == "T_LTvalid") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLTvalid)[idx];
        };
    } else if (n == "T_LTpT") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauLTPt)[idx];
        };
    } else if (n == "T_NumProngs") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauNProngs)[idx];
        };

    } else if (n == "T_AntiElectronMVA") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSagainstElectronMVA)[idx];
        };
    } else if (n == "T_AntiElectronIndex") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauAntiElectronIndex(idx);
        };
    } else if (n == "T_AntiMuonIndex") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauAntiMuonIndex(idx);
        };

    } else if (n == "T_IsolationIndex") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauIsolationIndex(idx);
        };
    } else if (n == "T_IsolationIndex3Hits") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauIsolationIndex3Hits(idx);
        };
    } else if (n == "T_IsolationIndexMVA") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauIsolationIndexMVA(idx);
        };
    } else if (n == "T_IsolationIndexMVA2") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return e->GetTauIsolationIndexMVA2(idx);
        };
    } else if (n == "T_IsolationMVA2Raw") {
        GetVal = [](Branches *b, const int& idx) -> float {
            tll::Branches* e = dynamic_cast<tll::Branches*>(b);
            return (*e->TLL_TauHPSbyIsolationMVA2raw)[idx];
        };
    } else if (n == "T1_pT") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1Pt)[idx];
        };
    } else if (n == "T2_pT") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2Pt)[idx];
        };
    } else if (n == "T1_Eta") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1Eta)[idx];
        };
    } else if (n == "T2_Eta") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2Eta)[idx];
        };
    } else if (n == "T1_InCracks") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1IsInTheCracks)[idx];
        };
    } else if (n == "T2_InCracks") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2IsInTheCracks)[idx];
        };

    } else if (n == "TT_DeltaR") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_DitauDeltaR)[idx];
        };
    } else if (n == "T1L_DeltaR") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return DeltaR((*e->TTL_Tau1Eta)[idx], (*e->TTL_Tau1Phi)[idx],
                    (*e->TTL_LeptonEta)[idx], (*e->TTL_LeptonPhi)[idx]);
        };
    } else if (n == "T2L_DeltaR") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return DeltaR((*e->TTL_Tau2Eta)[idx], (*e->TTL_Tau2Phi)[idx],
                    (*e->TTL_LeptonEta)[idx], (*e->TTL_LeptonPhi)[idx]);
        };

    } else if (n == "T1_DecayModeFinding") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSdecayModeFinding)[idx];
        };
    } else if (n == "T2_DecayModeFinding") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSdecayModeFinding)[idx];
        };
    } else if (n == "T1_LTvalid") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LTvalid)[idx];
        };
    } else if (n == "T2_LTvalid") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LTvalid)[idx];
        };
    } else if (n == "T1_LTpT") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1LTPt)[idx];
        };
    } else if (n == "T2_LTpT") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2LTPt)[idx];
        };
    } else if (n == "T1_NumProngs") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1NProngs)[idx];
        };
    } else if (n == "T2_NumProngs") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2NProngs)[idx];
        };

    } else if (n == "T1_AntiElectronMVA") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSagainstElectronMVA)[idx];
        };
    } else if (n == "T2_AntiElectronMVA") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSagainstElectronMVA)[idx];
        };
    } else if (n == "T1_AntiElectronIndex") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1AntiElectronIndex(idx);
        };
    } else if (n == "T2_AntiElectronIndex") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2AntiElectronIndex(idx);
        };
    } else if (n == "T1_AntiMuonIndex") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1AntiMuonIndex(idx);
        };
    } else if (n == "T2_AntiMuonIndex") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2AntiMuonIndex(idx);
        };

    } else if (n == "T1_IsolationIndex") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1IsolationIndex(idx);
        };
    } else if (n == "T2_IsolationIndex") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2IsolationIndex(idx);
        };
    } else if (n == "T1_IsolationIndex3Hits") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1IsolationIndex3Hits(idx);
        };
    } else if (n == "T2_IsolationIndex3Hits") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2IsolationIndex3Hits(idx);
        };
    } else if (n == "T1_IsolationIndexMVA") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1IsolationIndexMVA(idx);
        };
    } else if (n == "T2_IsolationIndexMVA") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2IsolationIndexMVA(idx);
        };
    } else if (n == "T1_IsolationIndexMVA2") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau1IsolationIndexMVA2(idx);
        };
    } else if (n == "T2_IsolationIndexMVA2") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return e->GetTau2IsolationIndexMVA2(idx);
        };
    } else if (n == "T1_IsolationMVA2Raw") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1HPSbyIsolationMVA2raw)[idx];
        };
    } else if (n == "T2_IsolationMVA2Raw") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau2HPSbyIsolationMVA2raw)[idx];
        };

    } else if (n == "TT_ChargeProduct") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1Charge)[idx] * (*e->TTL_Tau2Charge)[idx];
        };

    } else if (n == "TT_VisMass") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_DitauVisibleMass)[idx];
        };
    } else if (n == "TT_DeltaEta") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_Tau1Eta)[idx] - (*e->TTL_Tau2Eta)[idx];
        };
    } else if (n == "TT_CosDeltaPhi") {
        GetVal = [](roast::Branches *b, const int& idx) -> float {
            ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
            return (*e->TTL_DitauCosDeltaPhi)[idx];
        };
    } else if (n == "MET") {
        GetVal = [](Branches *b, const int& idx) -> float {
            return b->Ev_MET;
        };
    } else if (n == "MVA") {
        GetVal = [](Branches *b, const int& idx) -> float {
            return MVABase::gMVA["CFMlpANN"] ? MVABase::gMVA["CFMlpANN"]->Evaluate(b, idx) : 0.;
        };
    } else {
        // FIXME add proper exception
        throw;
    }
}

ClassImp(roast::CutFlow)
ClassImp(roast::CutFlow::Cut)
