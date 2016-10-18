#ifndef __Fakes_h
#define __Fakes_h

#include <vector>

#include "ttH/TauRoast/interface/SuperSlim.h"

namespace fakeable {
   double value(double val, const std::vector<double>& bins, const std::vector<double>& edges);
   double pjet(int constituents, double pt, int charged_constituents, double charged_pt);
   double pfake(int constituents, double pt, int charged_constituents, double charged_pt);

   double pjet(const superslim::GenJet& j);
   double pfake(const superslim::GenJet& j);

   double pjet(const superslim::Tau& j);
   double pfake(const superslim::Tau& j);
}

#endif
