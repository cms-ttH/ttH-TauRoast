#include "DataFormats/Common/interface/Wrapper.h"
#include "ttH/TauRoast/interface/Fakes.h"
#include "ttH/TauRoast/interface/Fastlane.h"
#include "ttH/TauRoast/interface/SuperSlim.h"

namespace {
   struct dictionary {
      fastlane::Leaf<int> dummy_ileaf;
      fastlane::Leaf<float> dummy_fleaf;
      fastlane::Leaf<std::vector<int>> dummy_vileaf;
      fastlane::Leaf<std::vector<float>> dummy_vfleaf;

      fastlane::Cut dummy_cut;
      fastlane::StaticCut dummy_static_cut;

      superslim::LorentzVector dummy_vector;
      std::map<std::string, superslim::LorentzVector> dummy_vector_map;
      superslim::CutHistogram dummy_hist;
      edm::Wrapper<superslim::CutHistogram> dummy_hist_wrapper;
      superslim::PhysObject dummy_object;
      superslim::Vertex dummy_vertex;
      std::vector<superslim::Vertex> dummy_vertex_vector;
      superslim::Jet dummy_jet;
      std::vector<superslim::Jet> dummy_jet_vector;
      std::map<std::string, std::vector<superslim::Jet>> dummy_jet_map;
      superslim::Lepton dummy_lepton;
      std::vector<superslim::Lepton> dummy_lepton_vector;
      superslim::Tau dummy_tau;
      std::vector<superslim::Tau> dummy_tau_vector;
      superslim::Combination dummy_combination;
      std::vector<superslim::Combination> dummy_combination_vector;
      superslim::Trigger dummy_trigger;
      superslim::Event dummy_event;
      edm::Wrapper<superslim::Event> dummy_event_wrapper;
   };
}
