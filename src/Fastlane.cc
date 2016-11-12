#include <cctype>
#include <cstdlib>

#include "TPython.h"
#include "TPyArg.h"

#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/Handle.h"

#include "MiniAOD/MiniAODHelper/interface/CSVHelper.h"
#include "MiniAOD/MiniAODHelper/interface/PUWeightProducer.h"
#include "ttH/TauRoast/interface/Fastlane.h"

std::string
lower(const std::string& s)
{
   std::string res(s);
   for (unsigned i = 0; i < res.size(); ++i)
      res[i] = std::tolower(res[i]);
   return res;
}

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
            fastlane::update_weights(ws, e, sys);

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
fastlane::BasicLeaf::updateCache(const superslim::Event& e)
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
fastlane::update_weights(std::unordered_map<std::string, double>& ws, const superslim::Event& e, const std::string& sys)
{
   // =====================
   // Constants for weights
   // =====================

   static const float tau_efficiency_scale = 0.83;
   static const float tau_efficiency = 0.06;

   // ===========
   // CSV weights
   // ===========

   static auto csvhelper = CSVHelper(
         "MiniAOD/MiniAODHelper/data/csv_rwt_fit_hf_76x_2016_02_08.root",
         "MiniAOD/MiniAODHelper/data/csv_rwt_fit_lf_76x_2016_02_08.root", 5);

   static std::unordered_map<std::string, int> csvsys = {
      {"NA", 0},
      {"JERUp", 0},
      {"JERDown", 0},
      {"JESUp", 7},
      {"JESDown", 8},
      {"LFContUp", 9},
      {"LFContDown", 10},
      {"HFContUp", 11},
      {"HFContDown", 12},
      {"HFStats1Up", 13},
      {"HFStats1Down", 14},
      {"HFStats2Up", 15},
      {"HFStats2Down", 16},
      {"LFStats1Up", 17},
      {"LFStats1Down", 18},
      {"LFStats2Up", 19},
      {"LFStats2Down", 20},
      {"CharmErr1Up", 21},
      {"CharmErr1Down", 22},
      {"CharmErr2Up", 23},
      {"CharmErr2Down", 24}
   };

   std::vector<double> jetpt, jeteta, jetcsv;
   std::vector<int> jetflv;

   for (const auto& j: e.jets(sys)) {
      jetpt.push_back(j.p4().pt());
      jeteta.push_back(j.p4().eta());
      jetcsv.push_back(j.csv());
      jetflv.push_back(j.flavor());
   }

   double hf, lf, cf;
   double csv = csvhelper.getCSVWeight(jetpt, jeteta, jetcsv, jetflv, csvsys[sys], hf, lf, cf);

   ws[lower("HFWeight")] = hf;
   ws[lower("LFWeight")] = lf;
   ws[lower("CFWeight")] = cf;
   ws[lower("CSVWeight")] = std::min(csv, 1.);

   // =========
   // PU weight
   // =========
   static auto puhelper = PUWeightProducer(
         "MiniAOD/MiniAODHelper/data/puweights/MC/Spring16_25nsV1_NumTruePU.root",
         "hNumTruePUPdf",
         "MiniAOD/MiniAODHelper/data/puweights/Run2016/DataPileupHistogram_Run2016B-PromptReco-271036-275125_MinBias71300.root",
         "pileup");
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
      static const std::vector<int> debug{};

      const auto e = handle.ptr();
      bool failed = false;
      for (auto& cut: cuts) {
         if (not (*cut)(process, *e, sys)) {
            auto it = std::find(debug.begin(), debug.end(), e->event());
            if (it != debug.end()) {
               std::cout << "FAILED: " << *it << " MISSED " << cut->name() << std::endl;
               const std::string labels("₁₂₃₄₅₆");
               int i = 0;
               for (const auto& l: e->leptons()) {
                  std::cout << "\tl" << labels.substr(i * 3, 3) << " pt " << l.pt()
                     << " :: l" << labels.substr(i * 3, 3) << " mva " << l.mvaRaw()
                     << " :: l" << labels.substr(i * 3, 3) << " id " << l.mva()
                     << " :: l" << labels.substr(i * 3, 3) << " csv " << l.nearestJetCSV()
                     << " :: l" << labels.substr(i * 3, 3) << " pdg " << l.pdgId()
                     << std::endl;
                  i += 1;
               }
               std::cout << "\tτ₁ pt " << e->taus()[0].pt()
                  << "\tτ₁ mva " << e->taus()[0].isolationMVA03()
                  << std::endl;
               std::cout << "\tτ₂ pt " << e->taus()[1].pt()
                  << "\tτ₂ mva " << e->taus()[1].isolationMVA03()
                  << std::endl;
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
         fastlane::update_weights(ws, *e, sys);

      double weight = 1.;
      for (auto& w: weights) {
         if (process.compare(0, 10, "collisions"))
            weight *= ws[lower(w->name())];
         (*w)[process] += weight;
      }

      BasicLeaf::updateCache(*e);
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
