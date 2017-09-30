#include <cctype>
#include <cstdlib>

#include "RooWorkspace.h"
#include "TFile.h"
#include "TPython.h"
#include "TPyArg.h"

#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "CondFormats/BTauObjects/interface/BTagCalibration.h"

#ifndef OLDCRAP
#include "MiniAOD/MiniAODHelper/interface/PUWeightProducer.h"
#include "ttH/TauRoast/interface/FastlaneHelpers.h"
#endif

#include "ttH/TauRoast/interface/Fastlane.h"

float
get_factor(std::auto_ptr<TH2F>& h, const superslim::Lepton& l, bool swapped=false)
{
   int x = std::max(1, std::min(h->GetNbinsX(), h->GetXaxis()->FindBin(swapped ? l.eta() : l.pt())));
   int y = std::max(1, std::min(h->GetNbinsY(), h->GetYaxis()->FindBin(swapped ? l.pt() : std::abs(l.eta()))));
   return h->GetBinContent(x, y);
}

float
get_factor(std::auto_ptr<TGraphAsymmErrors>& g, float v)
{
   float x = std::max(float(g->GetXaxis()->GetXmin() + 1e-5),
         std::min(float(g->GetXaxis()->GetXmax() - 1e-5), v));
   return g->Eval(x);
}

std::string
lower(const std::string& s)
{
   std::string res(s);
   for (unsigned i = 0; i < res.size(); ++i)
      res[i] = std::tolower(res[i]);
   return res;
}

#ifndef OLDCRAP
fastlane::CSVHelper::CSVHelper()
   : reader_(BTagEntry::OP_RESHAPING, "central", {
         "up_jes", "down_jes",
         "up_hf", "down_hf",
         "up_hfstats1", "down_hfstats1",
         "up_hfstats2", "down_hfstats2",
         "up_lf", "down_lf",
         "up_lfstats1", "down_lfstats1",
         "up_lfstats2", "down_lfstats2",
         "up_cferr1", "down_cferr1",
         "up_cferr2", "down_cferr2"
     })
{
   edm::FileInPath fn("ttH/TauRoast/data/weights/CSVv2_Moriond17_B_H.csv");
   BTagCalibration calib("csvv2", fn.fullPath());

   reader_.load(calib, BTagEntry::FLAV_B, "iterativefit");
   reader_.load(calib, BTagEntry::FLAV_C, "iterativefit");
   reader_.load(calib, BTagEntry::FLAV_UDSG, "iterativefit");
}

float
fastlane::CSVHelper::weight(const std::vector<superslim::Jet>& jets, const std::string& sys)
{
   static const std::vector<std::string> bsys{
      "up_jes", "up_lf", "up_hfstats1", "up_hfstats2",
      "down_jes", "down_lf", "down_hfstats1", "down_hfstats2"
   };
   static const std::vector<std::string> lsys{
      "up_jes", "up_hf", "up_lfstats1", "up_lfstats2",
      "down_jes", "down_hf", "down_lfstats1", "down_lfstats2"
   };
   static const std::vector<std::string> csys{
      "up_cferr1", "up_cferr2",
      "down_cferr1", "down_cferr2"
   };

   float w = 1.;

   for (const auto& j: jets) {
      float jw = 1.;
      if (std::abs(j.flavor()) == 5) {
         std::string use = sys;
         if (std::find(bsys.begin(), bsys.end(), sys) == bsys.end())
            use = "central";
         jw = reader_.eval_auto_bounds(use, BTagEntry::FLAV_B, j.eta(), j.pt(), j.csv());
      } else if (std::abs(j.flavor()) == 4) {
         std::string use = sys;
         if (std::find(csys.begin(), csys.end(), sys) == csys.end())
            use = "central";
         jw = reader_.eval_auto_bounds(use, BTagEntry::FLAV_C, j.eta(), j.pt(), j.csv());
      } else {
         std::string use = sys;
         if (std::find(lsys.begin(), lsys.end(), sys) == lsys.end())
            use = "central";
         jw = reader_.eval_auto_bounds(use, BTagEntry::FLAV_UDSG, j.eta(), j.pt(), j.csv());
      }
      w *= jw;
   }

   return w;
}

void
get_hist(TFile& f, std::auto_ptr<TH2F>& ptr, const std::string& name)
{
   TH2F *h;
   f.GetObject(name.c_str(), h);
   ptr.reset(h);
   ptr->SetDirectory(0);
}

fastlane::FakeHelper::FakeHelper(const std::string& id)
{
   id_ = superslim::id::None;
   for (const auto& p: superslim::id::values)
      if (p.first == id)
         id_ = p.second;
   if (id_ == superslim::id::None)
      throw edm::Exception(edm::errors::InvalidReference, "Invalid id '" + id + "'");
   static const std::vector<std::string> dets{
      "jetToTauFakeRate/dR03mva" + id + "/absEtaLt1_5/",
      "jetToTauFakeRate/dR03mva" + id + "/absEta1_5to9_9/"
   };
   // for (const auto& s: dets)
   //    std::cout << "using " << s << std::endl;
   static const std::vector<std::string> labels{"barrel", "endcap"};
   static const std::string fakerate = "jetToTauFakeRate_mc_hadTaus_pt";
   static const std::map<std::string, std::string> correction{
      {"central", "fitFunction_data_div_mc_hadTaus_pt"},
      {"normUp", "fitFunction_data_div_mc_hadTaus_pt_par1Up"},
      {"normDown", "fitFunction_data_div_mc_hadTaus_pt_par1Down"},
      {"shapeUp", "fitFunction_data_div_mc_hadTaus_pt_par2Up"},
      {"shapeDown", "fitFunction_data_div_mc_hadTaus_pt_par2Down"}
   };

   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/FR_tau_2016.root").fullPath().c_str());
      for (unsigned int i = 0; i < dets.size(); ++i) {
         TGraphAsymmErrors *graph;
         f.GetObject((dets[i] + fakerate).c_str(), graph);
         tau[i].reset(graph);
         for (const auto& corr: correction) {
            TF1 *fct;
            f.GetObject((dets[i] + corr.second).c_str(), fct);
            fct->Copy(ratio[i][corr.first]);
         }
      }
   }

   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/FR_data_ttH_mva.root").fullPath().c_str());
      TH2F *hele, *hmu;
      f.GetObject("FR_mva075_el_data_comb", hele);
      f.GetObject("FR_mva075_mu_data_comb", hmu);
      ele_fake.reset(hele);
      ele_fake->SetDirectory(0);
      mu_fake.reset(hmu);
      mu_fake->SetDirectory(0);
   }
}

float
fastlane::FakeHelper::weight(const std::vector<superslim::Tau>& taus,
                             const std::vector<superslim::Lepton>& leptons,
                             const std::string& sys)
{
   static const float barrel_cut = 1.479;

   float w = 1.;

   int fake_count = 0;
   for (const auto& t: taus) {
      if (t.isolationMVA03() >= id_)
         continue;
      int idx = std::abs(t.eta()) > barrel_cut;
      float f = get_factor(tau[idx], t.pt()) * ratio[idx][sys].Eval(t.pt());
      w *= f / (1. - f);
      fake_count += 1;
   }

   for (const auto& l: leptons) {
      if (l.mva() >= superslim::id::Loose)
         continue;
      float f = 0.;
      if (l.muon())
         f = mu_fake->GetBinContent(mu_fake->FindBin(std::min({l.conePt(), 99.99f}), std::abs(l.eta())));
      else
         f = ele_fake->GetBinContent(ele_fake->FindBin(std::min({l.conePt(), 99.99f}), std::abs(l.eta())));
      w *= f / (1. - f);
      fake_count += 1;
   }

   if (fake_count > 0)
      w *= pow(-1., fake_count + 1);

   return w;
}

fastlane::LeptonHelper::LeptonHelper()
{
   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/leptonSF/lepMVAEffSF_e_3l.root").fullPath().c_str());
      get_hist(f, tight_el_, "sf");
   }
   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/leptonSF/lepMVAEffSF_m_3l.root").fullPath().c_str());
      get_hist(f, tight_mu_, "sf");
   }

   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/leptonSF/el_scaleFactors_Moriond17.root").fullPath().c_str());
      get_hist(f, reco_el1_, "GsfElectronToMVAVLooseFOIDEmuTightIP2D");
      get_hist(f, reco_el2_, "MVAVLooseElectronToMini4");
      get_hist(f, reco_el3_, "MVAVLooseElectronToConvVetoIHit1");
   }
   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/leptonSF/egammaEffi.txt_EGM2D.root").fullPath().c_str());
      get_hist(f, reco_el4_, "EGamma_SF2D");
   }
   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/leptonSF/TnP_NUM_LooseID_DENOM_generalTracks_VAR_map_pt_eta.root").fullPath().c_str());
      get_hist(f, reco_mu1_, "SF");
   }
   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/leptonSF/TnP_NUM_MiniIsoLoose_DENOM_LooseID_VAR_map_pt_eta.root").fullPath().c_str());
      get_hist(f, reco_mu2_, "SF");
   }
   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/leptonSF/TnP_NUM_TightIP2D_DENOM_MediumID_VAR_map_pt_eta.root").fullPath().c_str());
      get_hist(f, reco_mu3_, "SF");
   }
   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/leptonSF/ratios_HIP_trkEff.root").fullPath().c_str());
      TGraphAsymmErrors *g;
      f.GetObject("ratio_eta", g);
      reco_mu4_.reset(g);
   }
}

fastlane::LeptonHelper::LeptonHelper(const LeptonHelper& other)
{
   tight_el_.reset(dynamic_cast<TH2F*>(tight_el_->Clone()));
   tight_mu_.reset(dynamic_cast<TH2F*>(tight_mu_->Clone()));

   reco_mu1_.reset(dynamic_cast<TH2F*>(reco_mu1_->Clone()));
   reco_mu2_.reset(dynamic_cast<TH2F*>(reco_mu2_->Clone()));
   reco_mu3_.reset(dynamic_cast<TH2F*>(reco_mu3_->Clone()));
   reco_mu4_.reset(dynamic_cast<TGraphAsymmErrors*>(reco_mu4_->Clone()));

   reco_el1_.reset(dynamic_cast<TH2F*>(reco_el1_->Clone()));
   reco_el2_.reset(dynamic_cast<TH2F*>(reco_el2_->Clone()));
   reco_el3_.reset(dynamic_cast<TH2F*>(reco_el3_->Clone()));
   reco_el4_.reset(dynamic_cast<TH2F*>(reco_el4_->Clone()));
}

float
fastlane::LeptonHelper::weight(const superslim::Lepton& l)
{
   return recoSF(l) * tightSF(l);
}

float
fastlane::LeptonHelper::recoSF(const superslim::Lepton& l)
{
   if (l.electron()) {
      return
         get_factor(reco_el1_, l) *
         get_factor(reco_el2_, l) *
         get_factor(reco_el3_, l) *
         get_factor(reco_el4_, l, true);
   } else {
      return
         get_factor(reco_mu1_, l) *
         get_factor(reco_mu2_, l) *
         get_factor(reco_mu3_, l) *
         get_factor(reco_mu4_, l.eta());
   }
}

float
fastlane::LeptonHelper::tightSF(const superslim::Lepton& l)
{
   auto& h = l.electron() ? tight_el_ : tight_mu_;
   return get_factor(h, l);
}

fastlane::TriggerHelper::TriggerHelper(const std::string& id)
{
   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/Electron_Ele24_eff.root").fullPath().c_str());
      TGraphAsymmErrors *eff_lep_mc_1;
      TGraphAsymmErrors *eff_lep_mc_2;
      TGraphAsymmErrors *eff_lep_mc_3;
      TGraphAsymmErrors *eff_lep_data_1;
      TGraphAsymmErrors *eff_lep_data_2;
      TGraphAsymmErrors *eff_lep_data_3;
      f.GetObject("ZMassEtaLt1p48_MC", eff_lep_mc_1);
      f.GetObject("ZMassEta1p48to2p1_MC", eff_lep_mc_2);
      f.GetObject("ZMassEtaGt2p1_MC", eff_lep_mc_3);
      f.GetObject("ZMassEtaLt1p48_Data", eff_lep_data_1);
      f.GetObject("ZMassEta1p48to2p1_Data", eff_lep_data_2);
      f.GetObject("ZMassEtaGt2p1_Data", eff_lep_data_3);
      eff_et_lep_leg_mc_[0].reset(eff_lep_mc_1);
      eff_et_lep_leg_mc_[1].reset(eff_lep_mc_2);
      eff_et_lep_leg_mc_[2].reset(eff_lep_mc_3);
      eff_et_lep_leg_data_[0].reset(eff_lep_data_1);
      eff_et_lep_leg_data_[1].reset(eff_lep_data_2);
      eff_et_lep_leg_data_[2].reset(eff_lep_data_3);
   }

   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/Electron_Ele25WPTight_eff.root").fullPath().c_str());
      TGraphAsymmErrors *eff_lep_mc_1;
      TGraphAsymmErrors *eff_lep_mc_2;
      TGraphAsymmErrors *eff_lep_mc_3;
      TGraphAsymmErrors *eff_lep_data_1;
      TGraphAsymmErrors *eff_lep_data_2;
      TGraphAsymmErrors *eff_lep_data_3;
      f.GetObject("ZMassEtaLt1p48_MC", eff_lep_mc_1);
      f.GetObject("ZMassEta1p48to2p1_MC", eff_lep_mc_2);
      f.GetObject("ZMassEtaGt2p1_MC", eff_lep_mc_3);
      f.GetObject("ZMassEtaLt1p48_Data", eff_lep_data_1);
      f.GetObject("ZMassEta1p48to2p1_Data", eff_lep_data_2);
      f.GetObject("ZMassEtaGt2p1_Data", eff_lep_data_3);
      eff_e_mc_[0].reset(eff_lep_mc_1);
      eff_e_mc_[1].reset(eff_lep_mc_2);
      eff_e_mc_[2].reset(eff_lep_mc_3);
      eff_e_data_[0].reset(eff_lep_data_1);
      eff_e_data_[1].reset(eff_lep_data_2);
      eff_e_data_[2].reset(eff_lep_data_3);
   }

   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/Muon_Mu19leg_2016BtoH_eff.root").fullPath().c_str());
      TGraphAsymmErrors *eff_lep_mc_1;
      TGraphAsymmErrors *eff_lep_mc_2;
      TGraphAsymmErrors *eff_lep_mc_3;
      TGraphAsymmErrors *eff_lep_data_1;
      TGraphAsymmErrors *eff_lep_data_2;
      TGraphAsymmErrors *eff_lep_data_3;
      f.GetObject("ZMassEtaLt0p9_MC", eff_lep_mc_1);
      f.GetObject("ZMassEta0p9to1p2_MC", eff_lep_mc_2);
      f.GetObject("ZMassEta1p2to2p1_MC", eff_lep_mc_3);
      f.GetObject("ZMassEtaLt0p9_Data", eff_lep_data_1);
      f.GetObject("ZMassEta0p9to1p2_Data", eff_lep_data_2);
      f.GetObject("ZMassEta1p2to2p1_Data", eff_lep_data_3);
      eff_mt_lep_leg_mc_[0].reset(eff_lep_mc_1);
      eff_mt_lep_leg_mc_[1].reset(eff_lep_mc_2);
      eff_mt_lep_leg_mc_[2].reset(eff_lep_mc_3);
      eff_mt_lep_leg_data_[0].reset(eff_lep_data_1);
      eff_mt_lep_leg_data_[1].reset(eff_lep_data_2);
      eff_mt_lep_leg_data_[2].reset(eff_lep_data_3);
   }

   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/Muon_Mu22OR_eta2p1_eff.root").fullPath().c_str());
      TGraphAsymmErrors *eff_lep_mc_1;
      TGraphAsymmErrors *eff_lep_mc_2;
      TGraphAsymmErrors *eff_lep_mc_3;
      TGraphAsymmErrors *eff_lep_data_1;
      TGraphAsymmErrors *eff_lep_data_2;
      TGraphAsymmErrors *eff_lep_data_3;
      f.GetObject("ZMassEtaLt0p9_MC", eff_lep_mc_1);
      f.GetObject("ZMassEta0p9to1p2_MC", eff_lep_mc_2);
      f.GetObject("ZMassEta1p2to2p1_MC", eff_lep_mc_3);
      f.GetObject("ZMassEtaLt0p9_Data", eff_lep_data_1);
      f.GetObject("ZMassEta0p9to1p2_Data", eff_lep_data_2);
      f.GetObject("ZMassEta1p2to2p1_Data", eff_lep_data_3);
      eff_m_mc_[0].reset(eff_lep_mc_1);
      eff_m_mc_[1].reset(eff_lep_mc_2);
      eff_m_mc_[2].reset(eff_lep_mc_3);
      eff_m_data_[0].reset(eff_lep_data_1);
      eff_m_data_[1].reset(eff_lep_data_2);
      eff_m_data_[2].reset(eff_lep_data_3);
   }

   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/trigger_sf_et.root").fullPath().c_str());
      TGraphAsymmErrors *eff_tau_data_b0;
      TGraphAsymmErrors *eff_tau_data_b1;
      TGraphAsymmErrors *eff_tau_data_b10;
      TGraphAsymmErrors *eff_tau_data_e0;
      TGraphAsymmErrors *eff_tau_data_e1;
      TGraphAsymmErrors *eff_tau_data_e10;
      f.GetObject(("data_genuine_barrel_" + id + "Iso_dm0").c_str(), eff_tau_data_b0);
      f.GetObject(("data_genuine_barrel_" + id + "Iso_dm1").c_str(), eff_tau_data_b1);
      f.GetObject(("data_genuine_barrel_" + id + "Iso_dm10").c_str(), eff_tau_data_b10);
      f.GetObject(("data_genuine_endcap_" + id + "Iso_dm0").c_str(), eff_tau_data_e0);
      f.GetObject(("data_genuine_endcap_" + id + "Iso_dm1").c_str(), eff_tau_data_e1);
      f.GetObject(("data_genuine_endcap_" + id + "Iso_dm10").c_str(), eff_tau_data_e10);
      eff_et_tau_leg_data_[0][0].reset(eff_tau_data_b0);
      eff_et_tau_leg_data_[1][0].reset(eff_tau_data_b1);
      eff_et_tau_leg_data_[10][0].reset(eff_tau_data_b10);
      eff_et_tau_leg_data_[0][1].reset(eff_tau_data_e0);
      eff_et_tau_leg_data_[1][1].reset(eff_tau_data_e1);
      eff_et_tau_leg_data_[10][1].reset(eff_tau_data_e10);
   }

   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/tauleg_of_lepton_plus_tau_real_taus_skim_mc_v2.root").fullPath().c_str());
      TGraphAsymmErrors *eff_tau_mc_b0;
      TGraphAsymmErrors *eff_tau_mc_b1;
      TGraphAsymmErrors *eff_tau_mc_b10;
      TGraphAsymmErrors *eff_tau_mc_e0;
      TGraphAsymmErrors *eff_tau_mc_e1;
      TGraphAsymmErrors *eff_tau_mc_e10;
      f.GetObject(("barrel_lowmt_zmass_sub_" + id + "Iso_dm0_HLT_LooseIso30_L1iso26/tau_pt").c_str(), eff_tau_mc_b0);
      f.GetObject(("barrel_lowmt_zmass_sub_" + id + "Iso_dm1_HLT_LooseIso30_L1iso26/tau_pt").c_str(), eff_tau_mc_b1);
      f.GetObject(("barrel_lowmt_zmass_sub_" + id + "Iso_dm10_HLT_LooseIso30_L1iso26/tau_pt").c_str(), eff_tau_mc_b10);
      f.GetObject(("endcap_lowmt_zmass_sub_" + id + "Iso_dm0_HLT_LooseIso30_L1iso26/tau_pt").c_str(), eff_tau_mc_e0);
      f.GetObject(("endcap_lowmt_zmass_sub_" + id + "Iso_dm1_HLT_LooseIso30_L1iso26/tau_pt").c_str(), eff_tau_mc_e1);
      f.GetObject(("endcap_lowmt_zmass_sub_" + id + "Iso_dm10_HLT_LooseIso30_L1iso26/tau_pt").c_str(), eff_tau_mc_e10);
      eff_et_tau_leg_mc_[0][0].reset(eff_tau_mc_b0);
      eff_et_tau_leg_mc_[1][0].reset(eff_tau_mc_b1);
      eff_et_tau_leg_mc_[10][0].reset(eff_tau_mc_b10);
      eff_et_tau_leg_mc_[0][1].reset(eff_tau_mc_e0);
      eff_et_tau_leg_mc_[1][1].reset(eff_tau_mc_e1);
      eff_et_tau_leg_mc_[10][1].reset(eff_tau_mc_e10);
   }

   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/trigger_sf_mt.root").fullPath().c_str());
      TGraphAsymmErrors *eff_tau_mc_b;
      TGraphAsymmErrors *eff_tau_mc_e;
      TGraphAsymmErrors *eff_tau_data_b;
      TGraphAsymmErrors *eff_tau_data_e;
      f.GetObject(("mc_genuine_barrel_" + id + "Iso").c_str(), eff_tau_mc_b);
      f.GetObject(("mc_genuine_endcap_" + id + "Iso").c_str(), eff_tau_mc_e);
      f.GetObject(("data_genuine_barrel_" + id + "Iso").c_str(), eff_tau_data_b);
      f.GetObject(("data_genuine_endcap_" + id + "Iso").c_str(), eff_tau_data_e);
      eff_mt_tau_leg_mc_[0].reset(eff_tau_mc_b);
      eff_mt_tau_leg_mc_[1].reset(eff_tau_mc_e);
      eff_mt_tau_leg_data_[0].reset(eff_tau_data_b);
      eff_mt_tau_leg_data_[1].reset(eff_tau_data_e);
   }
}

float
fastlane::TriggerHelper::weight(const superslim::Event& e)
{
   static const std::vector<std::string> e_triggers{
       "HLT_Ele25_eta2p1_WPTight_Gsf_v",
   };
   static const std::vector<std::string> e_cross_triggers{
       "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v",
       "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v",
       "HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau30_v"
   };
   static const std::vector<float> e_binning{1.48, 2.1};
   static const std::vector<std::string> mu_triggers{
       "HLT_IsoMu22_v",
       "HLT_IsoTkMu22_v",
       "HLT_IsoMu22_eta2p1_v",
       "HLT_IsoTkMu22_eta2p1_v"
   };
   static const std::vector<std::string> mu_cross_triggers{
       "HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v"
   };
   static const std::vector<float> mu_binning{0.9, 1.2};

   std::vector<std::string> l_triggers;
   std::vector<std::string> l_cross_triggers;
   std::vector<float> l_binning;

   std::array<std::auto_ptr<TGraphAsymmErrors>, 2>* eff_lt_tau1_leg_mc = 0;
   std::array<std::auto_ptr<TGraphAsymmErrors>, 2>* eff_lt_tau2_leg_mc = 0;
   std::array<std::auto_ptr<TGraphAsymmErrors>, 2>* eff_lt_tau1_leg_data = 0;
   std::array<std::auto_ptr<TGraphAsymmErrors>, 2>* eff_lt_tau2_leg_data = 0;
   std::array<std::auto_ptr<TGraphAsymmErrors>, 3>* eff_lt_lep_leg_mc = 0;
   std::array<std::auto_ptr<TGraphAsymmErrors>, 3>* eff_lt_lep_leg_data = 0;
   std::array<std::auto_ptr<TGraphAsymmErrors>, 3>* eff_l_mc = 0;
   std::array<std::auto_ptr<TGraphAsymmErrors>, 3>* eff_l_data = 0;

   if (e.leptons().size() < 1 or e.taus().size() < 2)
      return 0.;

   auto t1_bin = std::abs(e.taus()[0].p4().eta()) > 1.479;
   auto t1_pt = e.taus()[0].p4().pt();
   auto t2_bin = std::abs(e.taus()[1].p4().eta()) > 1.479;
   auto t2_pt = e.taus()[1].p4().pt();

   if (e.leptons()[0].electron()) {
      l_triggers = e_triggers;
      l_cross_triggers = e_cross_triggers;
      l_binning = e_binning;
      eff_lt_tau1_leg_mc = &eff_et_tau_leg_mc_[e.taus()[0].decayMode()];
      eff_lt_tau2_leg_mc = &eff_et_tau_leg_mc_[e.taus()[1].decayMode()];
      eff_lt_tau1_leg_data = &eff_et_tau_leg_data_[e.taus()[0].decayMode()];
      eff_lt_tau2_leg_data = &eff_et_tau_leg_data_[e.taus()[1].decayMode()];
      eff_lt_lep_leg_mc = &eff_et_lep_leg_mc_;
      eff_lt_lep_leg_data = &eff_et_lep_leg_data_;
      eff_l_mc = &eff_e_mc_;
      eff_l_data = &eff_e_data_;
   } else {
      l_triggers = mu_triggers;
      l_cross_triggers = mu_cross_triggers;
      l_binning = mu_binning;
      eff_lt_tau1_leg_mc = &eff_mt_tau_leg_mc_;
      eff_lt_tau2_leg_mc = &eff_mt_tau_leg_mc_;
      eff_lt_tau1_leg_data = &eff_mt_tau_leg_data_;
      eff_lt_tau2_leg_data = &eff_mt_tau_leg_data_;
      eff_lt_lep_leg_mc = &eff_mt_lep_leg_mc_;
      eff_lt_lep_leg_data = &eff_mt_lep_leg_data_;
      eff_l_mc = &eff_m_mc_;
      eff_l_data = &eff_m_data_;
   }

   auto l_find_bin = [&](float eta) -> int {
      int res = 0;
      for (const auto& b: l_binning)
         res += std::abs(eta) > b;
      return res;
   };

   auto l_bin = l_find_bin(e.leptons()[0].p4().eta());
   // auto l_pt = e.leptons()[0].p4().pt();
   auto l_pt = e.leptons()[0].conePt();

   bool l_accepted = e.trigger().accepted(l_triggers);
   bool l_cross_accepted = e.trigger().accepted(l_cross_triggers);

   float p_data = 0;
   float p_mc = 0;

   float tau_leg_data = (1 - (1 - (*eff_lt_tau1_leg_data)[t1_bin]->Eval(t1_pt)) *
                             (1 - (*eff_lt_tau2_leg_data)[t2_bin]->Eval(t2_pt)));
   float tau_leg_mc = (1 - (1 - (*eff_lt_tau1_leg_mc)[t1_bin]->Eval(t1_pt)) *
                           (1 - (*eff_lt_tau2_leg_mc)[t2_bin]->Eval(t2_pt)));

   if (l_accepted and not l_cross_accepted) {
      p_data = (*eff_l_data)[l_bin]->Eval(l_pt) -
               std::min((*eff_l_data)[l_bin]->Eval(l_pt),
                       (*eff_lt_lep_leg_data)[l_bin]->Eval(l_pt)) *
               tau_leg_data;
      p_mc = (*eff_l_mc)[l_bin]->Eval(l_pt) -
             std::min((*eff_l_mc)[l_bin]->Eval(l_pt),
                     (*eff_lt_lep_leg_mc)[l_bin]->Eval(l_pt)) *
             tau_leg_mc;
      p_data = std::max(0.01f, p_data);
      p_mc = std::max(0.01f, p_mc);
   } else if (l_cross_accepted and not l_accepted) {
      p_data = ((*eff_lt_lep_leg_data)[l_bin]->Eval(l_pt) - (*eff_l_data)[l_bin]->Eval(l_pt)) *
               tau_leg_data;
      p_mc = ((*eff_lt_lep_leg_mc)[l_bin]->Eval(l_pt) - (*eff_l_mc)[l_bin]->Eval(l_pt)) *
             tau_leg_mc;
      p_data = std::max(0.01f, p_data);
      p_mc = std::max(0.01f, p_mc);
   } else if (l_accepted and l_cross_accepted) {
      p_data = std::min(
                  (*eff_l_data)[l_bin]->Eval(l_pt),
                  (*eff_lt_lep_leg_data)[l_bin]->Eval(l_pt)
               ) * tau_leg_data;
      p_mc = std::min(
               (*eff_l_mc)[l_bin]->Eval(l_pt),
               (*eff_lt_lep_leg_mc)[l_bin]->Eval(l_pt)
             ) * tau_leg_mc;
   }

   // std::cout << ">>> " << e.event() << " :: " << l_accepted << ", " << l_cross_accepted << std::endl;
   // std::cout << ">> " << e.taus()[0].decayMode() << " --- " << e.taus()[1].decayMode() << std::endl;

   // std::cout << "t1_data " << (*eff_lt_tau1_leg_data)[t1_bin]->Eval(t1_pt) << std::endl;
   // std::cout << "t2_data " << (*eff_lt_tau2_leg_data)[t2_bin]->Eval(t2_pt) << std::endl;
   // std::cout << "t1_mc " << (*eff_lt_tau1_leg_mc)[t1_bin]->Eval(t1_pt) << std::endl;
   // std::cout << "t2_mc " << (*eff_lt_tau2_leg_mc)[t2_bin]->Eval(t2_pt) << std::endl;
   // std::cout << "L_data " << (*eff_l_data)[l_bin]->Eval(l_pt) << std::endl;
   // std::cout << "l_data " << (*eff_lt_lep_leg_data)[l_bin]->Eval(l_pt) << std::endl;
   // std::cout << "L_mc " << (*eff_l_mc)[l_bin]->Eval(l_pt) << std::endl;
   // std::cout << "l_mc " << (*eff_lt_lep_leg_mc)[l_bin]->Eval(l_pt) << std::endl;

   // std::cout << "eff_data " << p_data << std::endl;
   // std::cout << "eff_mc " << p_mc << std::endl;

   return std::min(10.f, p_data / std::max(1e-6f, p_mc));
}
#endif

bool
fastlane::Cut::operator()(const std::string& process, const superslim::Event& e, const std::string& sys)
{
   auto passed = fct_(e, e.taus(), e.allTaus(), e.tauId(), e.leptons(), e.allLeptons(), e.leptonId(), e.jets(sys), e.met(sys));

   if (passed) {
      event_t id = std::make_tuple(e.run(), e.lumi(), e.event());
      if (std::get<0>(id) == std::get<0>(last_) and
            std::get<1>(id) == std::get<1>(last_) and
            std::get<2>(id) == std::get<2>(last_))
         return passed;
      last_ = id;
      counts_[process]++;
      if (callback_) {
         auto event = superslim::Event(e);

         std::unordered_map<std::string, double> ws;
         for (const auto& w: e.weights())
            ws[lower(w.first)] = w.second;
         if (process.compare(0, 10, "collisions"))
            fastlane::update_weights(process, ws, e, sys, "Tight");

         auto py_e = TPython::ObjectProxy_FromVoidPtr(dynamic_cast<void*>(&event), "superslim::Event");
         auto py_w = TPython::ObjectProxy_FromVoidPtr(static_cast<void*>(&ws), "std::unordered_map<std::string,double>");
         std::vector<TPyArg> args = {py_e, py_w};
         TPyArg::CallMethod(callback_, args);
      }
   }
   return passed;
}

std::vector<std::string>
fastlane::Cut::processes() const
{
   std::vector<std::string> res;
   for (const auto& p: counts_)
      res.push_back(p.first);
   return res;
}

std::vector<std::string>
fastlane::StaticCut::processes() const
{
   std::vector<std::string> res;
   for (const auto& p: counts_)
      res.push_back(p.first);
   return res;
}

std::vector<fastlane::BasicLeaf*> fastlane::BasicLeaf::leaves_;
std::vector<superslim::Lepton> fastlane::BasicLeaf::cached_electrons_;
std::vector<superslim::Lepton> fastlane::BasicLeaf::cached_muons_;

void
fastlane::BasicLeaf::update_cache(const superslim::Event& e)
{
   static int run = -1;
   static int lumi = -1;
   static int event = -1;

   if (e.run() == run and e.lumi() == lumi and e.event() == event)
      return;

   run = e.run();
   lumi = e.lumi();
   event = e.event();

   cached_electrons_.clear();
   cached_muons_.clear();

   std::copy_if(e.leptons().begin(), e.leptons().end(), std::back_inserter(cached_electrons_),
         [](const superslim::Lepton& l) -> bool { return l.electron(); });
   std::copy_if(e.leptons().begin(), e.leptons().end(), std::back_inserter(cached_muons_),
         [](const superslim::Lepton& l) -> bool { return l.muon(); });
}

template<> void fastlane::Leaf<std::vector<float>>::pick(const superslim::Event& e, std::unordered_map<std::string, double>& w, const std::string& sys)
{
   val_.clear();
   fct_(e, e.taus(), e.allTaus(), e.leptons(), cached_electrons_, cached_muons_, e.allLeptons(), e.jets(sys), e.met(sys), w, val_);
}

template<> void fastlane::Leaf<std::vector<int>>::pick(const superslim::Event& e, std::unordered_map<std::string, double>& w, const std::string& sys)
{
   val_.clear();
   fct_(e, e.taus(), e.allTaus(), e.leptons(), cached_electrons_, cached_muons_, e.allLeptons(), e.jets(sys), e.met(sys), w, val_);
}

void
fastlane::update_weights(const std::string& process, std::unordered_map<std::string, double>& ws, const superslim::Event& e, const std::string& sys, const std::string& id)
{
#ifndef OLDCRAP
   // =====================
   // Constants for weights
   // =====================

   static const float tau_efficiency_scale = 0.95;
   static const float tau_efficiency = 0.05;

   // =====================
   // Lepton and Trigger SF
   // =====================

   static TriggerHelper triggerhelper(id);
   static LeptonHelper leptonhelper;

   ws[lower("LeptonSF")] = leptonhelper.weight(e.leptons()[0]);
   ws[lower("TriggerSF")] = triggerhelper.weight(e);

   // ===========
   // CSV weights
   // ===========

   static auto csvhelper = CSVHelper();

   std::string csv_sys = "central";
   if (lower(sys) == "cms_tthl_jesup") {
      csv_sys = "up_jes";
   } else if (lower(sys) == "cms_tthl_jesdown") {
      csv_sys = "down_jes";
   }

   ws[lower("CSVWeight")] = csvhelper.weight(e.jets(), csv_sys);
   for (const std::string& dir: {"up", "down"}) {
      ws[lower("CMS_ttHl_btag_HF") + dir] = csvhelper.weight(e.jets(), dir + "_hf");
      ws[lower("CMS_ttHl_btag_HFStats1") + dir] = csvhelper.weight(e.jets(), dir + "_hfstats1");
      ws[lower("CMS_ttHl_btag_HFStats2") + dir] = csvhelper.weight(e.jets(), dir + "_hfstats2");
      ws[lower("CMS_ttHl_btag_LF") + dir] = csvhelper.weight(e.jets(), dir + "_lf");
      ws[lower("CMS_ttHl_btag_LFStats1") + dir] = csvhelper.weight(e.jets(), dir + "_lfstats1");
      ws[lower("CMS_ttHl_btag_LFStats2") + dir] = csvhelper.weight(e.jets(), dir + "_lfstats2");
      ws[lower("CMS_ttHl_btag_cErr1") + dir] = csvhelper.weight(e.jets(), dir + "_cferr1");
      ws[lower("CMS_ttHl_btag_cErr2") + dir] = csvhelper.weight(e.jets(), dir + "_cferr2");
   }

   // =========
   // PU weight
   // =========
   static auto puhelper = PUWeightProducer(
         "MiniAOD/MiniAODHelper/data/puweights/MC/Summer16_NumTruePU.root",
         "hNumTruePUPdf",
         "MiniAOD/MiniAODHelper/data/puweights/Run2016/DataPileupHistogram_Run2016-Complete_MinBias69200.root",
         "pileup");
   if (process.compare(0, 5, "fakes"))
      ws[lower("PUWeight")] = puhelper(e.ntv());

   // =================
   // τ related weights
   // =================

   auto taus = e.taus();
   int real_taus = std::count_if(std::begin(taus), std::end(taus),
         [](const superslim::Tau& t) { return t.match() < 6; });
   // int real_electrons = std::count_if(std::begin(taus), std::end(taus),
   //       [](const superslim::Tau& t) { return t.match() == 1; });
   // int real_jets = std::count_if(std::begin(taus), std::end(taus),
   //       [](const superslim::Tau& t) { return t.match() == 6; });

   ws[lower("tauIdEff")] = std::pow(tau_efficiency_scale, real_taus);
   ws[lower("tauIdEffUp")] = std::pow(tau_efficiency_scale * (1 + tau_efficiency), real_taus);
   ws[lower("tauIdEffDown")] = std::pow(tau_efficiency_scale * (1 - tau_efficiency), real_taus);
   ws[lower("jetTauFakeUp")] = 1;
   ws[lower("jetTauFakeDown")] = 1;
   ws[lower("eTauFakeUp")] = 1;
   ws[lower("eTauFakeDown")] = 1;

   static FakeHelper fakerate(id);

   auto wtaus = e.allTaus();
   wtaus.resize(std::min({wtaus.size(), 2ul}));
   auto wleptons = e.leptons();
   wleptons.resize(std::min({wleptons.size(), 1ul}));

   ws[lower("fake")] = fakerate.weight(wtaus, wleptons);
   ws[lower("CMS_ttHl_FRjt_normUp")] = fakerate.weight(wtaus, wleptons, "normUp");
   ws[lower("CMS_ttHl_FRjt_normDown")] = fakerate.weight(wtaus, wleptons, "normDown");
   ws[lower("CMS_ttHl_FRjt_shapeUp")] = fakerate.weight(wtaus, wleptons, "shapeUp");
   ws[lower("CMS_ttHl_FRjt_shapeDown")] = fakerate.weight(wtaus, wleptons, "shapeDown");
#endif
}

void
fastlane::process(const std::string& process, const std::string& channel, const std::vector<std::string>& files, TTree& t, std::vector<fastlane::Cut*>& cuts, std::vector<fastlane::StaticCut*>& weights, const std::string& sys, const std::string& id, PyObject* log, int max, bool calculate_weights)
{
   fwlite::Handle<superslim::Event> handle;
   fwlite::ChainEvent events(files);

   auto label = channel + "Taus";

   int i = 0;
   for (events.toBegin(); !events.atEnd() and (max < 0 or i < max); ++events, ++i) {
      if (i % 10000 == 0) {
         std::vector<TPyArg> args = {Int_t(i)};
         TPyArg::CallMethod(log, args);
      }

      handle.getByLabel(events, label.c_str());

      // Event numbers in this vector will trigger debug output, printing
      // when they fail a cut.
      static const std::vector<long> debug{};

      const auto e = handle.ptr();
      bool failed = false;
      for (auto& cut: cuts) {
         if (not (*cut)(process, *e, sys)) {
            auto it = std::find(debug.begin(), debug.end(), e->event());
            if (it != debug.end()) {
               std::cout << "FAILED: " << *it << " MISSED " << cut->name() << std::endl;
               const std::string labels("₁₂₃₄₅₆");
               int i = 0;
               for (const auto& l: e->allLeptons()) {
                  std::cout << "\tl" << labels.substr(i * 3, 3) << " pt " << l.pt()
                     << " :: l" << labels.substr(i * 3, 3) << " mva " << l.mvaRaw()
                     << " :: l" << labels.substr(i * 3, 3) << " id " << l.mva()
                     << " :: l" << labels.substr(i * 3, 3) << " csv " << l.nearestJetCSV()
                     << " :: l" << labels.substr(i * 3, 3) << " pdg " << l.pdgId()
                     << " :: l" << labels.substr(i * 3, 3) << " match " << l.match()
                     << std::endl;
                  i += 1;
               }
               i = 0;
               for (const auto& t: e->allTaus()) {
                  std::cout << "\tτ" << labels.substr(i * 3, 3) << " pt " << t.pt()
                     << " :: τ" << labels.substr(i * 3, 3) << " mva " << t.isolationMVA03()
                     << " :: τ" << labels.substr(i * 3, 3) << " pdg " << t.pdgId()
                     << " :: τ" << labels.substr(i * 3, 3) << " match " << t.match()
                     << std::endl;
                  i += 1;
               }
               // std::cout << "\tTriggers:" << std::endl;
               // for (const auto& t: e->trigger().triggers())
               //    std::cout << "\t\t" << t << std::endl;
            }
            failed = true;
            break;
         }
      }

      if (failed)
         continue;

      std::unordered_map<std::string, double> ws;
      for (const auto& w: e->weights())
         ws[lower(w.first)] = w.second;
      if (calculate_weights)
         fastlane::update_weights(process, ws, *e, sys, id);

      double weight = 1.;
      for (auto& w: weights) {
         if (calculate_weights)
            weight *= ws[lower(w->name())];
         (*w)[process] += weight;
      }

      BasicLeaf::update_cache(*e);
      for (auto& leaf: BasicLeaf::leaves()) {
         // std::cout << leaf->name() << std::endl;
         try {
            leaf->pick(*e, ws, sys);
         } catch (const std::out_of_range& e) {
         }
      }

      t.Fill();
   }
}

const TH1*
fastlane::get_cuts(const std::string& label, const std::vector<std::string>& files)
{
   TH1* result = 0;
   edm::Handle<superslim::CutHistogram> handle;
   edm::InputTag tag(label);
   for (const auto& filename: files) {
      TFile f(filename.c_str());
      fwlite::Run run(&f);
      for (run.toBegin(); !run.atEnd(); ++run) {
         run.getByLabel(tag, handle);
         if (!result) {
            result = static_cast<TH1*>(handle.product()->product()->Clone());
            result->SetDirectory(0);
         } else {
            result->Add(handle.product()->product());
         }
      }
      f.Close();
   }
   return result;
}
