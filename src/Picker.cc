#ifdef FOOBARBAD
// vim: ts=4:sw=4:et:sta

#include <algorithm>

#include "../interface/TLBranches.h"
#include "../interface/TLLBranches.h"
#include "../interface/TTLBranches.h"
#include "../interface/Picker.h"

inline double
tau_iso(roast::tll::Branches *b, int idx)
{
    return (*b->T_HPSbyIsolationMVA2raw)[idx];
}

inline double
tau_iso2(roast::tl::Branches *b, int idx)
{
    return (*b->T_HPSbyIsolationMVA2raw)[idx];
}

inline double
ditau_iso(roast::ttl::Branches *b, int idx)
{
    return ((*b->T1_HPSbyIsolationMVA2raw)[idx] + 1.) *
        ((*b->T1_HPSbyIsolationMVA2raw)[idx] + 1.) +
        ((*b->T2_HPSbyIsolationMVA2raw)[idx] + 1.) *
        ((*b->T2_HPSbyIsolationMVA2raw)[idx] + 1.);
}

int
roast::ttl::IsoPicker::Pick(roast::Branches *b, const std::vector<int>& idxs)
{
    roast::ttl::Branches *br = dynamic_cast<roast::ttl::Branches*>(b);
    auto idx = std::max_element(idxs.begin(), idxs.end(),
            [&](int a, int b) -> bool { return ditau_iso(br, a) < ditau_iso(br, b); });
    return *idx;
}

int
roast::tl::IsoPicker::Pick(roast::Branches *b, const std::vector<int>& idxs)
{
    roast::tl::Branches *br = dynamic_cast<roast::tl::Branches*>(b);
    auto idx = std::max_element(idxs.begin(), idxs.end(),
            [&](int a, int b) -> bool { return tau_iso2(br, a) < tau_iso2(br, b); });
    return *idx;
}

int
roast::tll::IsoPicker::Pick(roast::Branches *b, const std::vector<int>& idxs)
{
    roast::tll::Branches *br = dynamic_cast<roast::tll::Branches*>(b);
    auto idx = std::max_element(idxs.begin(), idxs.end(),
            [&](int a, int b) -> bool { return tau_iso(br, a) < tau_iso(br, b); });
    return *idx;
}

ClassImp(roast::tl::IsoPicker)
ClassImp(roast::tll::IsoPicker)
ClassImp(roast::ttl::IsoPicker)
#endif
