#ifdef FOOBARBAD
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

bool
CutFlow::Cut::Check(Branches *b, const int idx)
{
    if (RealCheck(b, idx)) {
        if (!current_passed && !immutable) {
            ++events_passed;
            current_passed = true;
        }
        return true;
    }
    return false;
}

CutFlow::ValueCut::ValueCut(const std::string& n, float mn, float mx, bool neg) :
    CutFlow::Cut(n), negate(neg), min(mn), max(mx)
{
    GetVal = roast::get_accessor(n);
}

bool
CutFlow::ValueCut::RealCheck(Branches *b, int idx)
{
    float val = GetVal(b, idx, -1);
    bool res = (min <= val) && (val <= max);
    return negate ? !res : res;
}

bool
CutFlow::ComposedCut::RealCheck(Branches *b, int idx)
{
    bool v1 = op1->RealCheck(b, idx);
    bool v2 = op2->RealCheck(b, idx);

    bool res = v1 && v2;
    if (kind == kOr)
        res = v1 || v2;

    if (negate)
        res = !res;

    return res;
}

CutFlow::CutFlow(CutFlow const & iCutFlow)
{
    Reset();

    for (const auto& c: iCutFlow.GetCuts())
        cuts.push_back(c->Clone());

    int idx = 0;
    for (const auto& c: cuts)
        name2idx[c->name] = idx++;
}

void CutFlow::Reset(){
    eventPassed = false;

    for (auto& c: cuts)
        delete c;

    cuts.clear();
    name2idx.clear();
}

void CutFlow::Zero() {
    for (auto& c: cuts) {
        c->events_passed = 0;
    }

    eventPassed = false;
}

int const CutFlow::size() const { return cuts.size(); }

void CutFlow::RegisterCut(CutFlow::Cut* cut)
{
    cuts.push_back(cut);
    name2idx[cut->name] = cuts.size() - 1;
}

void
CutFlow::RegisterCut(const std::string& name, double events)
{
    Cut *c = new Cut(name, events);
    cuts.push_back(c);
    name2idx[name] = cuts.size() - 1;
}

void
CutFlow::RegisterCutFromLast(const std::string& name, double factor)
{
    RegisterCut(new RelativeCut(name, factor * GetLastCount()));
}

void
CutFlow::SetCutCounts(string const iName, double const iEvents)
{
    cuts[name2idx[iName]]->events_passed = iEvents;
    cuts[name2idx[iName]]->immutable = true;
}

bool
CutFlow::CheckCuts(Branches *b, const int& idx, bool debug)
{
    for (auto& c: cuts) {
        if (debug)
            std::cout << c->name;

        if (!c->Check(b, idx)) {
            if (debug) {
                if (auto vc = dynamic_cast<roast::CutFlow::ValueCut*>(c))
                    std::cout << " failed with value: " << vc->GetVal(b, idx, -1) << std::endl;
                else
                    std::cout << " failed" << std::endl;
            }
            return false;
        }

        if (debug)
            std::cout << " passed" << std::endl;
    }
    return true;
}

// Reset counters relevant to the start of the event
void
CutFlow::StartOfEvent()
{
    eventPassed = false;

    for (auto& c: cuts)
        c->current_passed = false;

    signalComboLocked = false;
}

float const CutFlow::GetRelEff(string const iCut) const {
    const int idx = name2idx.find(iCut)->second;
    if (idx == 0)
        return 1.;
    else if (idx < 0 || !cuts[idx - 1]->events_passed)
        return 0.;
    return cuts[idx]->events_passed / cuts[idx - 1]->events_passed;
}

float const CutFlow::GetCumEff(string const iCut) const {
    const int idx = name2idx.find(iCut)->second;
    return cuts[idx]->events_passed / cuts.front()->events_passed;
}

// Check whether iValue is within range (useful for selection cuts)
bool CutFlow::OutOfRange(float iValue, float iMin, float iMax){
	return ((iValue < iMin) || (iMax < iValue));
}

string const CutFlow::GetLastCut() const{
    return cuts.back()->name;
}

void
CutFlow::RemoveCut(const std::string& name)
{
    for (;;) {
        auto it = find_if(cuts.begin(), cuts.end(), [&](Cut* c) { return c->name == name; });
        if (it != cuts.end()) {
            delete *it;
            cuts.erase(it);
        } else {
            break;
        }
    }
}

void
CutFlow::Add(const CutFlow& iCutFlow, bool same, float iFactor){
    // Check the current cuts	
    if (cuts.size() == 0) {
        // If there are none, add them from the input CutFlow
        for (auto& oc: cuts) {
            auto c = oc->Clone();
            c->events_passed *= iFactor;
            cuts.push_back(c);
        }
    } else {
        // Else, first check that we have the same sizes
        if (size() != iCutFlow.size()) {
            cerr << "ERROR: Current CutFlow has size " << size() << " while input CutFlow has size " << iCutFlow.size() << "." << endl;
            exit(1);
        }
        // Then check that all cuts are the same
        const Cuts other_cuts = iCutFlow.GetCuts();
        for (unsigned int i = 0; i < cuts.size(); ++i) {
            if (cuts[i]->name != other_cuts[i]->name) {
                cerr << "ERROR: Inconsistency in cut names of current and input CutFlows:" << endl; 
                cerr << "\tCurrent\t\tInput" << endl;
                cerr << "\t-------\t\t-----" << endl;
                for (unsigned int j = 0; j < cuts.size(); ++j) {
                    cerr << "\t" << cuts[j]->name << "\t" << other_cuts[j]->name << endl;
                }
                exit(1);
            } else if (!same || i >= 2) {
                cuts[i]->events_passed += other_cuts[i]->events_passed * iFactor;
            } else if (same && i < 2) {
                if (cuts[i]->events_passed != other_cuts[i]->events_passed) {
                    std::cout << "WARNING: event count mismatch in " << cuts[i]->name <<
                        "          " << cuts[i]->events_passed << " vs " <<
                        other_cuts[i]->events_passed << std::endl;
                }
            }
        }
    }
}

void CutFlow::BuildNormalizedCutFlow(const CutFlow* iCutFlow){
    Reset();

    vector<Cut*> other_cuts = iCutFlow->GetCuts();
    double scaleFactor = 1.0;

    for (auto& oc: other_cuts) {
        if (oc->IsRelative())
            scaleFactor *= iCutFlow->GetRelEff(oc->name);
    }

    for (auto& oc: other_cuts) {
        if (oc->IsRelative())
            continue;
        auto c = oc->Clone();
        c->events_passed *= scaleFactor;
        RegisterCut(c);
    }
}

ClassImp(roast::CutFlow)
ClassImp(roast::CutFlow::Cut)
ClassImp(roast::CutFlow::RelativeCut)
ClassImp(roast::CutFlow::ValueCut)
ClassImp(roast::CutFlow::ComposedCut)
#endif
