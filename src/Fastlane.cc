#include <cctype>
#include <cstdlib>

#include "TFile.h"
#include "TPython.h"
#include "TPyArg.h"

#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "CondFormats/BTauObjects/interface/BTagCalibration.h"

#ifndef OLDCRAP
#include "MiniAOD/MiniAODHelper/interface/LeptonSFHelper.h"
#include "MiniAOD/MiniAODHelper/interface/PUWeightProducer.h"
#include "ttH/TauRoast/interface/FastlaneHelpers.h"
#endif

#include "ttH/TauRoast/interface/Fastlane.h"

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
   edm::FileInPath fn("ttH/TauRoast/data/weights/CSVv2Moriond17_2017_1_26_BtoH.csv");
   BTagCalibration calib("csvv2", fn.fullPath());

   reader_.load(calib, BTagEntry::FLAV_B, "iterativefit");
   reader_.load(calib, BTagEntry::FLAV_C, "iterativefit");
   reader_.load(calib, BTagEntry::FLAV_UDSG, "iterativefit");
}

float
fastlane::CSVHelper::weight(const std::vector<superslim::Jet>& jets, const std::string& sys)
{
   float w = 1.;

   for (const auto& j: jets) {
      float jw = 1.;
      if (std::abs(j.flavor()) == 5) {
         jw = reader_.eval_auto_bounds(sys, BTagEntry::FLAV_B, j.eta(), j.pt(), j.csv());
      } else if (std::abs(j.flavor()) == 4) {
         jw = reader_.eval_auto_bounds(sys, BTagEntry::FLAV_C, j.eta(), j.pt(), j.csv());
      } else {
         jw = reader_.eval_auto_bounds(sys, BTagEntry::FLAV_UDSG, j.eta(), j.pt(), j.csv());
      }
      w *= jw;
   }

   return w;
}

void
graph_to_hists(const TGraphAsymmErrors* g, std::array<std::auto_ptr<TH1F>, 3>& a, const std::string& name)
{
   int n = g->GetN();
   double *xs = g->GetX();
   double *ys = g->GetY();
   double *ex_down = g->GetEXlow();
   double *ey_up = g->GetEYhigh();
   double *ey_down = g->GetEYlow();

   std::vector<double> bins(n + 1);
   for (int i = 0; i < n; ++i)
      bins[i] = xs[i] - ex_down[i];
   bins[n] = 1000000;

   a[0].reset(new TH1F((name + "_central").c_str(), "", n, bins.data()));
   a[1].reset(new TH1F((name + "_up").c_str(), "", n, bins.data()));
   a[2].reset(new TH1F((name + "_down").c_str(), "", n, bins.data()));

   for (int i = 0; i < n; ++i) {
      a[0]->SetBinContent(i + 1, ys[i]);
      a[1]->SetBinContent(i + 1, ys[i] + ey_up[i]);
      a[2]->SetBinContent(i + 1, ys[i] - ey_down[i]);
   }

   a[0]->SetDirectory(0);
   a[1]->SetDirectory(0);
   a[2]->SetDirectory(0);
}

void
get_hist(TFile& f, std::auto_ptr<TH2F>& ptr, const std::string& name)
{
   TH2F *h;
   f.GetObject(name.c_str(), h);
   ptr.reset(h);
   ptr->SetDirectory(0);
}

fastlane::FakeHelper::FakeHelper()
{
   static const std::vector<std::string> dets{
      "jetToTauFakeRate/dR03mvaTight/absEtaLt1_5/",
      "jetToTauFakeRate/dR03mvaTight/absEta1_5to9_9/"
   };
   static const std::vector<std::string> labels{"barrel", "endcap"};
   static const std::string fakerate = "jetToTauFakeRate_mc_hadTaus_pt";
   static const std::string correction = "fitFunction_data_div_mc_hadTaus_pt";

   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/tau_weights.root").fullPath().c_str());
      for (unsigned int i = 0; i < dets.size(); ++i) {
         TGraphAsymmErrors *graph;
         f.GetObject((dets[i] + fakerate).c_str(), graph);
         graph_to_hists(graph, tau[i], labels[i]);
         TF1 *fct;
         f.GetObject((dets[i] + correction).c_str(), fct);
         fct->Copy(ratio[i]);
      }
   }

   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/lepton_weights.root").fullPath().c_str());
      TH2F *hele, *hmu;
      f.GetObject("FR_mva075_el_data_comb", hele);
      f.GetObject("FR_mva075_mu_data_comb", hmu);
      ele_fake.reset(hele);
      ele_fake->SetDirectory(0);
      mu_fake.reset(hmu);
      mu_fake->SetDirectory(0);
   }
}

fastlane::FakeHelper::FakeHelper(const FakeHelper& other)
{
   for (unsigned int i = 0; i < tau.size(); ++i) {
      for (unsigned int j = 0; j < tau[i].size(); ++j) {
         tau[i][j].reset(dynamic_cast<TH1F*>(other.tau[i][j]->Clone()));
         tau[i][j]->SetDirectory(0);
      }
      other.ratio[i].Copy(ratio[i]);
   }
   ele_fake.reset(dynamic_cast<TH2F*>(other.ele_fake->Clone()));
   mu_fake.reset(dynamic_cast<TH2F*>(other.mu_fake->Clone()));
}

float
fastlane::FakeHelper::weight(const std::vector<superslim::Tau>& taus,
                             const std::vector<superslim::Lepton>& leptons)
{
   static const float barrel_cut = 1.479;

   float w = 1.;

   int fake_count = 0;
   for (const auto& t: taus) {
      if (t.isolationMVA03() >= superslim::id::Tight)
         continue;
      int idx = std::abs(t.eta()) > barrel_cut;
      float f = tau[idx][0]->GetBinContent(tau[idx][0]->FindBin(t.pt())) * ratio[idx].Eval(t.pt());
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
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/lepMVAEffSF_e_3l.root").fullPath().c_str());
      get_hist(f, tight_el_, "sf");
   }
   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/lepMVAEffSF_m_3l.root").fullPath().c_str());
      get_hist(f, tight_mu_, "sf");
   }

   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/leptonSF/el_scaleFactors_20160724.root").fullPath().c_str());
      get_hist(f, reco_el1_, "GsfElectronToFOID2D");
      get_hist(f, reco_el2_, "MVAVLooseElectronToMini4");
      get_hist(f, reco_el3_, "MVAVLooseElectronToConvIHit1");
   }
   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/leptonSF/el_scaleFactors_gsf.root").fullPath().c_str());
      get_hist(f, reco_el4_, "EGamma_SF2D");
   }
   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/leptonSF/mu_ttH_presel_barrel.root").fullPath().c_str());
      TGraphAsymmErrors *g;
      f.GetObject("ratio", g);
      reco_mu1b_.reset(g);
   }
   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/leptonSF/mu_ttH_presel_endcap.root").fullPath().c_str());
      TGraphAsymmErrors *g;
      f.GetObject("ratio", g);
      reco_mu1e_.reset(g);
   }
   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/leptonSF/MuonID_Z_RunBCD_prompt80X_7p65_looseID.root").fullPath().c_str());
      get_hist(f, reco_mu2_, "pt_abseta_ratio_MC_NUM_LooseID_DEN_genTracks_PAR_pt_spliteta_bin1");
   }
   {
      TFile f(edm::FileInPath("ttH/TauRoast/data/weights/leptonSF/ratios_HIP_trkEff.root").fullPath().c_str());
      TGraphAsymmErrors *g;
      f.GetObject("ratio_eta", g);
      reco_mu3_.reset(g);
   }
}

fastlane::LeptonHelper::LeptonHelper(const LeptonHelper& other)
{
   tight_el_.reset(dynamic_cast<TH2F*>(tight_el_->Clone()));
   tight_mu_.reset(dynamic_cast<TH2F*>(tight_mu_->Clone()));

   reco_mu1b_.reset(dynamic_cast<TGraphAsymmErrors*>(reco_mu1b_->Clone()));
   reco_mu1e_.reset(dynamic_cast<TGraphAsymmErrors*>(reco_mu1e_->Clone()));
   reco_mu2_.reset(dynamic_cast<TH2F*>(reco_mu2_->Clone()));
   reco_mu3_.reset(dynamic_cast<TGraphAsymmErrors*>(reco_mu3_->Clone()));

   reco_el1_.reset(dynamic_cast<TH2F*>(reco_el1_->Clone()));
   reco_el2_.reset(dynamic_cast<TH2F*>(reco_el2_->Clone()));
   reco_el3_.reset(dynamic_cast<TH2F*>(reco_el3_->Clone()));
   reco_el4_.reset(dynamic_cast<TH2F*>(reco_el4_->Clone()));
}

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
         (std::abs(l.eta()) < 1.2 ? get_factor(reco_mu1b_, l.pt()) : get_factor(reco_mu1e_, l.pt())) *
         get_factor(reco_mu2_, l) *
         get_factor(reco_mu3_, l.eta());
   }
}

float
fastlane::LeptonHelper::tightSF(const superslim::Lepton& l)
{
   auto& h = l.electron() ? tight_el_ : tight_mu_;
   return get_factor(h, l);
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
            fastlane::update_weights(process, ws, e, sys);

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
fastlane::update_weights(const std::string& process, std::unordered_map<std::string, double>& ws, const superslim::Event& e, const std::string& sys)
{
#ifndef OLDCRAP
   // =====================
   // Constants for weights
   // =====================

   static const float tau_efficiency_scale = 0.83;
   static const float tau_efficiency = 0.06;

   // =====================
   // Lepton and Trigger SF
   // =====================

   static auto triggerhelper = LeptonSFHelper();
   static auto leptonhelper = LeptonHelper();

   for (const auto& l: e.leptons()) {
      float trig_sf = 0.;

      if (l.electron()) {
         trig_sf = triggerhelper.GetElectronSF(l.pt(), l.eta(), 0, "Trigger");
      } else {
         trig_sf = triggerhelper.GetMuonSF(l.pt(), l.eta(), 0, "Trigger");
      }

      ws[lower("LeptonSF")] = leptonhelper.weight(l);
      ws[lower("TriggerSF")] = trig_sf;

      break;
   }

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
         [](const superslim::Tau& t) { return t.match() == 5; });
   // int real_electrons = std::count_if(std::begin(taus), std::end(taus),
   //       [](const superslim::Tau& t) { return t.match() == 1; });
   // int real_jets = std::count_if(std::begin(taus), std::end(taus),
   //       [](const superslim::Tau& t) { return t.match() == 6; });

   ws[lower("tauIdEff")] = tau_efficiency_scale;
   ws[lower("tauIdEffUp")] = std::pow(tau_efficiency_scale * (1 + tau_efficiency), real_taus);
   ws[lower("tauIdEffDown")] = std::pow(tau_efficiency_scale * (1 - tau_efficiency), real_taus);
   ws[lower("jetTauFakeUp")] = 1;
   ws[lower("jetTauFakeDown")] = 1;
   ws[lower("eTauFakeUp")] = 1;
   ws[lower("eTauFakeDown")] = 1;

   static auto fakerate = FakeHelper();

   auto wtaus = e.allTaus();
   wtaus.resize(std::min({wtaus.size(), 2ul}));
   auto wleptons = e.leptons();
   wleptons.resize(std::min({wleptons.size(), 1ul}));

   ws[lower("fake")] = fakerate.weight(wtaus, wleptons);
#endif
}

void
fastlane::process(const std::string& process, const std::string& channel, const std::vector<std::string>& files, TTree& t, std::vector<fastlane::Cut*>& cuts, std::vector<fastlane::StaticCut*>& weights, const std::string& sys, PyObject* log, int max)
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
                     << std::endl;
                  i += 1;
               }
               i = 0;
               for (const auto& t: e->allTaus()) {
                  std::cout << "\tτ" << labels.substr(i * 3, 3) << " pt " << t.pt()
                     << " :: τ" << labels.substr(i * 3, 3) << " mva " << t.isolationMVA03()
                     << " :: τ" << labels.substr(i * 3, 3) << " pdg " << t.pdgId()
                     << std::endl;
                  i += 1;
               }
               std::cout << "\tTriggers:" << std::endl;
               for (const auto& t: e->trigger().triggers())
                  std::cout << "\t\t" << t << std::endl;
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
      if (process.compare(0, 10, "collisions"))
         fastlane::update_weights(process, ws, *e, sys);

      double weight = 1.;
      for (auto& w: weights) {
         if (process.compare(0, 10, "collisions"))
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
