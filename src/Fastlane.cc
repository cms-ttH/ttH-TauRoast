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
fastlane::Cut::operator()(const std::string& process, const superslim::Event& e, const superslim::Combination& c, const std::string& sys)
{
   auto passed = fct_(e, c.taus(), e.taus(), c.tauId(), c.leptons(), c.electrons(), c.muons(), c.leptonId() ,c.jets(sys), c.met(sys));

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
         auto combo = superslim::Combination(c);

         std::unordered_map<std::string, double> ws;
         for (const auto& w: e.weights())
            ws[lower(w.first)] = w.second;
         for (const auto& w: c.weights())
            ws[lower(w.first)] = w.second;
         if (process.compare(0, 10, "collisions"))
            fastlane::update_weights(ws, e, c, sys);

         auto py_e = TPython::ObjectProxy_FromVoidPtr(dynamic_cast<void*>(&event), "superslim::Event");
         auto py_c = TPython::ObjectProxy_FromVoidPtr(dynamic_cast<void*>(&combo), "superslim::Combination");
         auto py_w = TPython::ObjectProxy_FromVoidPtr(static_cast<void*>(&ws), "std::unordered_map<std::string,double>");
         std::vector<TPyArg> args = {py_e, py_c, py_w};
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

namespace fastlane {
   template<> void Leaf<std::vector<float>>::pick(const superslim::Event& e, const superslim::Combination& c, std::unordered_map<std::string, double>& w, const std::string& sys)
   {
      val_.clear();
      fct_(e, c.taus(), e.taus(), c.leptons(), c.electrons(), c.muons(), c.jets(sys), c.met(sys), w, val_);
   }
   template<> void Leaf<std::vector<int>>::pick(const superslim::Event& e, const superslim::Combination& c, std::unordered_map<std::string, double>& w, const std::string& sys)
   {
      val_.clear();
      fct_(e, c.taus(), e.taus(), c.leptons(), c.electrons(), c.muons(), c.jets(sys), c.met(sys), w, val_);
   }
}

void
fastlane::update_weights(std::unordered_map<std::string, double>& ws, const superslim::Event& e, const superslim::Combination& combo, const std::string& sys)
{
   static auto puhelper = PUWeightProducer(
         "MiniAOD/MiniAODHelper/data/Fall15_NumTruePU.root",
         "h_numTruePVs",
         "MiniAOD/MiniAODHelper/data/DataPileupHistogram_Run2015D-PromptReco-Silver_MinBias69000.root",
         "pileup");
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

   for (const auto& j: combo.jets(sys)) {
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
   ws[lower("PUWeight")] = puhelper(e.ntv());
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

      const auto e = handle.ptr();
      const auto& combos = e->combos();
      std::vector<superslim::Combination> passed;
      for (const auto& combo: combos) {
         bool failed = false;
         for (auto& cut: cuts) {
            if (not (*cut)(process, *e, combo, sys)) {
               failed = true;
               break;
            }
         }
         if (not failed) {
            passed.push_back(combo);
         }
      }

      if (passed.size() == 0)
         continue;

      auto& selected = passed[0];

      std::unordered_map<std::string, double> ws;
      for (const auto& w: e->weights())
         ws[lower(w.first)] = w.second;
      for (const auto& w: selected.weights())
         ws[lower(w.first)] = w.second;
      if (process.compare(0, 10, "collisions"))
         fastlane::update_weights(ws, *e, selected, sys);

      double weight = 1.;
      for (auto& w: weights) {
         if (process.compare(0, 10, "collisions"))
            weight *= ws[lower(w->name())];
         (*w)[process] += weight;
      }

      for (auto& leaf: BasicLeaf::leaves()) {
         // std::cout << leaf->name() << std::endl;
         try {
            leaf->pick(*e, selected, ws, sys);
         } catch (const std::out_of_range& e) {
         }
      }

      t.Fill();
   }
}
