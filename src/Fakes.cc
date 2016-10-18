#include "ttH/TauRoast/interface/Fakes.h"

#include "FakeData.frag"

namespace fakeable {
   double
   value(double val, const std::vector<double>& bins, const std::vector<double>& edges)
   {
      for (int i = edges.size(); i-->0;) {
          if (val >= edges[i])
              return bins[i];
      }
      return -1;
   }

   double
   pjet(int constituents, double pt, int charged_constituents, double charged_pt)
   {
      return value(constituents,
            jet_constituents_bins,
            jet_constituents_edges)
         * value(pt, jet_pt_bins, jet_pt_edges)
         * value(charged_constituents,
            jet_charged_constituents_bins,
            jet_charged_constituents_edges)
         * value(charged_pt, jet_charged_pt_bins, jet_charged_pt_edges);
   }

   double
   pfake(int constituents, double pt, int charged_constituents, double charged_pt)
   {
      return value(constituents,
            fake_constituents_bins,
            fake_constituents_edges)
         * value(pt, fake_pt_bins, fake_pt_edges)
         * value(charged_constituents,
            fake_charged_constituents_bins,
            fake_charged_constituents_edges)
         * value(charged_pt, fake_charged_pt_bins, fake_charged_pt_edges);
   }

   double
   pjet(const superslim::GenJet& j)
   {
      return pjet(
            j.constituents(),
            j.p4().Pt(),
            j.chargedConstituents(),
            j.chargedP4().Pt()
      );
   }

   double
   pfake(const superslim::GenJet& j)
   {
      return pfake(
            j.constituents(),
            j.p4().Pt(),
            j.chargedConstituents(),
            j.chargedP4().Pt()
      );
   }

   double
   pjet(const superslim::Tau& j)
   {
      return pjet(
            j.genJetConstituents(),
            j.genJetP4().Pt(),
            j.genJetChargedConstituents(),
            j.genJetChargedP4().Pt()
      );
   }

   double
   pfake(const superslim::Tau& j)
   {
      return pfake(
            j.genJetConstituents(),
            j.genJetP4().Pt(),
            j.genJetChargedConstituents(),
            j.genJetChargedP4().Pt()
      );
   }
}
