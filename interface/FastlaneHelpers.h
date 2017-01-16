#ifndef __FastlaneHelpers_h
#define __FastlaneHelpers_h

#include "CondTools/BTau/interface/BTagCalibrationReader.h"

#include "TF1.h"
#include "TGraphAsymmErrors.h"
#include "TH2F.h"

#include "SuperSlim.h"

namespace fastlane {
   class CSVHelper {
      public:
         CSVHelper();
         virtual ~CSVHelper() {};

         float weight(const std::vector<superslim::Jet>&, const std::string& sys="central");

      private:
         BTagCalibrationReader reader_;
   };

   class FakeHelper {
      public:
         enum { central = 0, up = 1, down = 2 } sys;
         FakeHelper();
         FakeHelper(const FakeHelper& other);
         virtual ~FakeHelper() {};

         float weight(const std::vector<superslim::Tau>&,
                      const std::vector<superslim::Lepton>&);
      private:
         std::array<std::array<std::auto_ptr<TH1F>, 3>, 2> tau;
         std::array<TF1, 2> ratio;
         std::auto_ptr<TH2F> ele_fake;
         std::auto_ptr<TH2F> mu_fake;
   };

   class LeptonHelper {
      public:
         LeptonHelper();
         LeptonHelper(const LeptonHelper&);
         virtual ~LeptonHelper() {};

         float weight(const superslim::Lepton&);
      private:
         float recoSF(const superslim::Lepton&);
         float tightSF(const superslim::Lepton&);

         std::auto_ptr<TH2F> tight_el_;
         std::auto_ptr<TH2F> tight_mu_;

         std::auto_ptr<TGraphAsymmErrors> reco_mu1b_;
         std::auto_ptr<TGraphAsymmErrors> reco_mu1e_;
         std::auto_ptr<TH2F> reco_mu2_;
         std::auto_ptr<TGraphAsymmErrors> reco_mu3_;

         std::auto_ptr<TH2F> reco_el1_;
         std::auto_ptr<TH2F> reco_el2_;
         std::auto_ptr<TH2F> reco_el3_;
         std::auto_ptr<TH2F> reco_el4_;
   };
}

#endif
