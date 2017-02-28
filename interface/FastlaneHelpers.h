#ifndef __FastlaneHelpers_h
#define __FastlaneHelpers_h

#include "CondTools/BTau/interface/BTagCalibrationReader.h"

#include "RooFunctor.h"
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
                      const std::vector<superslim::Lepton>&,
                      const std::string& sys="central");
      private:
         std::array<std::array<std::auto_ptr<TH1F>, 3>, 2> tau;
         std::array<std::map<std::string, TF1>, 2> ratio;
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

         std::auto_ptr<TH2F> reco_mu1_;
         std::auto_ptr<TH2F> reco_mu2_;
         std::auto_ptr<TH2F> reco_mu3_;
         std::auto_ptr<TGraphAsymmErrors> reco_mu4_;

         std::auto_ptr<TH2F> reco_el1_;
         std::auto_ptr<TH2F> reco_el2_;
         std::auto_ptr<TH2F> reco_el3_;
         std::auto_ptr<TH2F> reco_el4_;
   };

   class TriggerHelper {
      public:
         TriggerHelper();
         virtual ~TriggerHelper() {};

         float weight(const superslim::Event&);
      private:
         std::array<std::auto_ptr<TGraphAsymmErrors>, 2> eff_et_tau_leg_mc_;
         std::array<std::array<std::auto_ptr<TGraphAsymmErrors>, 2>, 11> eff_et_tau_leg_data_;
         std::array<std::auto_ptr<TGraphAsymmErrors>, 2> eff_mt_tau_leg_mc_;
         std::array<std::auto_ptr<TGraphAsymmErrors>, 2> eff_mt_tau_leg_data_;
         std::array<std::auto_ptr<TGraphAsymmErrors>, 3> eff_et_lep_leg_mc_;
         std::array<std::auto_ptr<TGraphAsymmErrors>, 3> eff_et_lep_leg_data_;
         std::array<std::auto_ptr<TGraphAsymmErrors>, 3> eff_mt_lep_leg_mc_;
         std::array<std::auto_ptr<TGraphAsymmErrors>, 3> eff_mt_lep_leg_data_;
         std::array<std::auto_ptr<TGraphAsymmErrors>, 3> eff_e_mc_;
         std::array<std::auto_ptr<TGraphAsymmErrors>, 3> eff_e_data_;
         std::array<std::auto_ptr<TGraphAsymmErrors>, 3> eff_m_mc_;
         std::array<std::auto_ptr<TGraphAsymmErrors>, 3> eff_m_data_;
   };
}

#endif
