// vim: ts=4:sw=4:et:sta
#include <limits>

#include "boost/lexical_cast.hpp"

#include "../interface/Accessor.h"
#include "../interface/MVABase.h"
#include "../interface/TLLBranches.h"
#include "../interface/TTLBranches.h"
#include "../interface/CutFlow.h"

using namespace std;
using namespace roast;

// Second constructor at the end of file!
CutFlow::Cut::Cut(const string n, GetValue_t f, const int r, const float mn, const float mx, const double sig, bool bypass):
    name(n), GetVal(f), rank(r), min(mn), max(mx),
    passedSignalEvents(sig), currentSignalResult(false),
    skip(bypass)
{
}

bool
CutFlow::Cut::Check(Branches *b, const int idx, const bool bypass)
{
    float val = GetVal(b, idx, -1);
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

CutFlow::CutFlow(CutFlow const & iCutFlow){
    cuts = iCutFlow.GetCuts();
    name2idx.clear();
    int idx = 0;
    for (const auto& c: cuts)
        name2idx[c.name] = idx++;

	eventPassed	= false;
	comboIs		= false;

	bestCombo	= -1;
}

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
    Cut new_cut(name, [](Branches *b, int idx, int n) -> float { return 0.; },
            rank, 0, 0, iEvents);
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

    vector<Cut> other_cuts = iCutFlow->GetCuts();
    double scaleFactor = 1.0;

    for (auto& oc: other_cuts) {
        if (oc.rank != 2)
            continue;
        scaleFactor *= iCutFlow->GetRelEff(oc.name);
    }

    for (auto& oc: other_cuts) {
        if (oc.rank == 2)
            continue;
        auto c = Cut(oc);
        c.passedSignalEvents *= scaleFactor;
        RegisterCut(c);
    }
}

CutFlow::Cut::Cut(const std::string& n, const float mn, const float mx) :
    name(n), GetVal(0), rank(1), min(mn), max(mx), passedSignalEvents(0), currentSignalResult(false), skip(false)
{
    if (n.find("AbsId") != std::string::npos)
        skip = true;

    GetVal = roast::get_accessor(n);
}

ClassImp(roast::CutFlow)
ClassImp(roast::CutFlow::Cut)
