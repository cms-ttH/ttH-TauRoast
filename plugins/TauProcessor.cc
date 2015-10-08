// -*- C++ -*-
//
// Package:    ttH/TauProcessor
// Class:      TauProcessor
//
/**\class TauProcessor TauProcessor.cc ttH/TauRoast/plugins/TauProcessor.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Matthias Wolf
//         Created:  Mon, 23 Feb 2015 08:51:15 GMT
//
//


// system include files
#include <memory>

// user include files
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Common/interface/TriggerNames.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "HLTrigger/HLTcore/interface/TriggerExpressionData.h"
#include "HLTrigger/HLTcore/interface/TriggerExpressionEvaluator.h"
#include "HLTrigger/HLTcore/interface/TriggerExpressionParser.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "TH1F.h"

#include "MiniAOD/MiniAODHelper/interface/MiniAODHelper.h"
#include "ttH/TauRoast/interface/SuperSlim.h"

template<typename T>
std::vector<std::vector<T>>
build_permutations(const std::vector<T>& things, const unsigned int num)
{
    if (num == 0)
        return {{}};
    if (num > things.size())
        return {};

    std::vector<std::vector<T>> res;
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < num; ++i)
        indices.push_back(i);

    do {
       std::vector<T> perm;
        for (const auto& i: indices)
            perm.push_back(things[i]);
        res.push_back(perm);

        for (int i = indices.size() - 1; i >= 0; --i) {
            // increase indices, starting with the back
            indices[i] += 1;
            if (indices[i] <= things.size() - indices.size() + i) {
                // if an index is smaller than the # of things (minus room
                // for the other indices), we need to reset the
                // remaining ones and stop increasing lower indices.
                for (unsigned int j = 1; i + j < indices.size(); ++j)
                    indices[i + j] = indices[i] + j;
                break;
            }
        }
    } while (indices.front() < things.size() && indices.back() < things.size());

    return res;
}

template<typename T>
edm::Handle<T>
get_collection(const edm::Event& event, const edm::EDGetTokenT<T>& token)
{
   edm::Handle<T> handle;
   event.getByToken(token, handle);
   if (!handle.isValid())
      throw edm::Exception(edm::errors::InvalidReference, "Can't find a collection.");
   return handle;
}

pat::JetCollection
get_non_pileup(const pat::JetCollection& jets)
{
   pat::JetCollection res;
   for (const auto& j: jets) {
      auto mva = j.userFloat("pileupJetId:fullDiscriminant");
      if (std::abs(j.eta()) < 2.5) {
         if (mva <= -0.63)
            continue;
      } else if (std::abs(j.eta()) < 2.7) {
         if ( mva <= -0.60)
            continue;
      } else if (std::abs(j.eta()) < 3.0) {
         if ( mva <= -0.55)
            continue;
      } else if (std::abs(j.eta()) < 5.2) {
         if ( mva <= -0.45)
            continue;
      } else {
         continue;
      }
      res.push_back(j);
   }

   return res;
}

class TauProcessor : public edm::EDAnalyzer {
   public:
      explicit TauProcessor(const edm::ParameterSet&);
      ~TauProcessor();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      template<typename T1, typename T2>
      std::vector<T1>
      removeOverlap(const std::vector<T1>& v1, const std::vector<T2>& v2, double dR = 0.02);

      void passCut(unsigned int cut, const std::string& name, float w = 1.0);
      void passComboCut(unsigned int event_cut, unsigned int combo_cut, int& passed, const std::string& name);

      MiniAODHelper helper_;

      unsigned int min_jets_;
      unsigned int min_tags_;
      int max_tags_;
      unsigned int min_leptons_;
      unsigned int min_loose_leptons_;
      unsigned int min_tight_leptons_;
      int max_loose_leptons_;
      int max_tight_leptons_;
      unsigned int min_taus_;
      unsigned int min_tight_taus_;

      bool subtract_leptons_;
      bool print_preselection_;

      double min_loose_lep_pt_;
      double min_tight_lep_pt_;

      double min_jet_pt_;
      double min_tag_pt_;

      std::string sys_;

      edm::EDGetTokenT<double> rho_token_;
      edm::EDGetTokenT<GenEventInfoProduct> geninfo_token_;
      edm::EDGetTokenT<reco::BeamSpot> bs_token_;
      edm::EDGetTokenT<std::vector<PileupSummaryInfo>> pu_token_;
      edm::EDGetTokenT<reco::VertexCollection> vertices_token_;
      edm::EDGetTokenT<pat::ElectronCollection> electrons_token_;
      edm::EDGetTokenT<pat::MuonCollection> muons_token_;
      edm::EDGetTokenT<pat::TauCollection> taus_token_;
      edm::EDGetTokenT<pat::JetCollection> ak4jets_token_;
      edm::EDGetTokenT<reco::GenParticleCollection> gen_token_;
      edm::EDGetTokenT<pat::METCollection> met_token_;
      edm::EDGetTokenT<edm::TriggerResults> trig_token_;

      TTree *tree_;
      TH1F *cuts_;

      std::vector<std::string> cutnames_;

      superslim::Event *event_;

      // see https://twiki.cern.ch/twiki/bin/view/CMS/TriggerResultsFilter#Use_as_a_Selector_AN1
      triggerExpression::Data m_triggerCache;
      std::unique_ptr<triggerExpression::Evaluator> m_triggerSelector;
};

TauProcessor::TauProcessor(const edm::ParameterSet& config) :
   min_jets_(config.getParameter<unsigned int>("minJets")),
   min_tags_(config.getParameter<unsigned int>("minTags")),
   max_tags_(config.getParameter<int>("maxTags")),
   min_leptons_(config.getParameter<unsigned int>("minPreselectedLeptons")),
   min_loose_leptons_(config.getParameter<unsigned int>("minLooseLeptons")),
   min_tight_leptons_(config.getParameter<unsigned int>("minTightLeptons")),
   max_loose_leptons_(config.getParameter<int>("maxLooseLeptons")),
   max_tight_leptons_(config.getParameter<int>("maxTightLeptons")),
   min_taus_(config.getParameter<unsigned int>("minTaus")),
   min_tight_taus_(config.getParameter<unsigned int>("minTightTaus")),
   subtract_leptons_(config.getParameter<bool>("subtractLeptons")),
   print_preselection_(config.getParameter<bool>("printPreselection")),
   min_loose_lep_pt_(config.getParameter<double>("minLooseLeptonPt")),
   min_tight_lep_pt_(config.getParameter<double>("minTightLeptonPt")),
   min_jet_pt_(config.getParameter<double>("minJetPt")),
   min_tag_pt_(config.getParameter<double>("minTagPt")),
   /* sys_(config.getParameter<std::string>("sys")), */
   event_(0),
   m_triggerCache(
         edm::InputTag("TriggerResults", "", "HLT"),
         edm::InputTag("gtDigis"),
         1, false, false, true,
         edm::ConsumesCollector(consumesCollector())
   ),
   m_triggerSelector(triggerExpression::parse(config.getParameter<std::string>("triggerSelection")))
{
   rho_token_ = consumes<double>(edm::InputTag("fixedGridRhoFastjetAll"));
   geninfo_token_ = consumes<GenEventInfoProduct>(edm::InputTag("generator"));
   bs_token_ = consumes<reco::BeamSpot>(edm::InputTag("offlineBeamSpot"));
   pu_token_ = consumes<std::vector<PileupSummaryInfo>>(edm::InputTag("addPileupInfo"));
   vertices_token_ = consumes<reco::VertexCollection>(edm::InputTag("offlineSlimmedPrimaryVertices"));
   electrons_token_ = consumes<pat::ElectronCollection>(config.getParameter<edm::InputTag>("electrons"));
   muons_token_ = consumes<pat::MuonCollection>(config.getParameter<edm::InputTag>("muons"));
   taus_token_ = consumes<pat::TauCollection>(edm::InputTag("slimmedTaus"));
   ak4jets_token_ = consumes<pat::JetCollection>(edm::InputTag("slimmedJets"));
   gen_token_ = consumes<reco::GenParticleCollection>(edm::InputTag("prunedGenParticles"));
   met_token_ = consumes<pat::METCollection>(edm::InputTag("slimmedMETs"));
   trig_token_ = consumes<edm::TriggerResults>(edm::InputTag("TriggerResults", "", "HLT"));

   helper_.SetUp("2012_53x", 9120, analysisType::TauLJ, false);
   helper_.SetFactorizedJetCorrector();

   edm::Service<TFileService> fs;
   tree_ = fs->make<TTree>("events", "Event data");
   tree_->Branch("Event", "superslim::Event", &event_, 32000, 0);

   cuts_ = fs->make<TH1F>("cuts", "Cut counts", 64, -0.5, 63.5);
}


TauProcessor::~TauProcessor()
{
}


template<typename T1, typename T2>
std::vector<T1>
TauProcessor::removeOverlap(const std::vector<T1>& v1, const std::vector<T2>& v2, double dR)
{
   std::vector<T1> res;
   for (const auto& o1: v1) {
      bool keeper = true;
      for (const auto& o2: v2)
         if (helper_.DeltaR(&o1, &o2) < dR)
            keeper = false;
      if (keeper)
         res.push_back(o1);
   }
   return res;
}


void
TauProcessor::passCut(unsigned int cut, const std::string& name, float w)
{
   cuts_->Fill(cut, w);

   if (cutnames_.size() == cut) {
      cutnames_.push_back(name);
      cuts_->GetXaxis()->SetBinLabel(cut + 1, name.c_str());
   }
}


void
TauProcessor::passComboCut(unsigned int event_cut, unsigned int combo_cut, int& passed, const std::string& name)
{
   if (not (passed & (1 << combo_cut))) {
      passed |= 1 << combo_cut;
      passCut(event_cut + combo_cut, name);
   }
}


void
TauProcessor::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
   using namespace edm;

   muonID::muonID mu_id_pre = muonID::muonPreselection;
   muonID::muonID mu_id_loose = muonID::muonLooseCutBased;
   muonID::muonID mu_id_tight = muonID::muonTightCutBased;

   electronID::electronID e_id_pre = electronID::electronPreselection;
   electronID::electronID e_id_loose = electronID::electronLooseCutBased;
   electronID::electronID e_id_tight = electronID::electronTightCutBased;

   if (min_leptons_ == 1) {
      mu_id_pre = muonID::muonLoose;
      mu_id_loose = muonID::muonLoose;
      mu_id_tight = muonID::muonTight;

      e_id_pre = electronID::electronPhys14L;
      e_id_loose = electronID::electronPhys14L;
      e_id_tight = electronID::electronPhys14M;
   }

   int event_cut = 0;

   // events run over count
   passCut(event_cut++, "Dataset processed");

   auto geninfo = get_collection(event, geninfo_token_);

   // dataset sum of the event weights
   passCut(event_cut++, "Dataset event weights", geninfo->weight());

   // filter on HLT paths
   if (m_triggerSelector and m_triggerCache.setEvent(event, setup)) {
      if (m_triggerCache.configurationUpdated())
         m_triggerSelector ->init(m_triggerCache);

      if ((*m_triggerSelector)(m_triggerCache)) {
         passCut(event_cut++, "HLT selection");
      } else {
         return;
      }
   } else {
      return;
   }

   auto vertices = get_collection(event, vertices_token_);

   std::vector<superslim::Vertex> pv;
   reco::Vertex rpv;
   int npv = 0;
   bool first = true;
   for (const auto& v: *vertices) {
      if (!v.isFake() && v.ndof() >= 4 && abs(v.z()) <= 24. && abs(v.position().Rho()) <= 2.) {
         if (npv++ == 0) {
            helper_.SetVertex(v);
            rpv = v;
         }
         pv.push_back(v);
      } else if (first) {
         // first pv needs to pass cuts, otherwise RECO step is messed up
         // https://hypernews.cern.ch/HyperNews/CMS/get/csa14/85/1.html
         return;
      }
      first = false;
   }

   if (npv == 0)
      return;

   // passed valid pv cut
   passCut(event_cut++, "Valid primary vertex");

   auto bs = get_collection(event, bs_token_);

   auto rho = get_collection(event, rho_token_);
   helper_.SetRho(*rho);

   // ===============
   // Basic selection
   // ===============

   auto electrons = get_collection(event, electrons_token_);
   auto muons = get_collection(event, muons_token_);
   auto taus = get_collection(event, taus_token_);
   auto ak4jets = get_collection(event, ak4jets_token_);
   auto genstuff = get_collection(event, gen_token_);

   auto raw_mu = helper_.GetSelectedMuons(*muons, 5., mu_id_pre);
   auto all_preselected_e = helper_.GetSelectedElectrons(*electrons, 7., e_id_pre);
   auto raw_e = removeOverlap(all_preselected_e, raw_mu, 0.05);
   auto raw_tau = helper_.GetSelectedTaus(*taus, 20., tau::loose);
   auto raw_tight_tau = helper_.GetSelectedTaus(raw_tau, 20., tau::tight);

   if (raw_tau.size() < min_taus_)
      return;

   if (raw_tight_tau.size() < min_tight_taus_)
      return;

   // =========================
   // Tau combination selection
   // =========================

   std::vector<superslim::Combination> combos;

   // cut index bitmap
   int passed = 0;

   for (const std::vector<pat::Tau>& combo: build_permutations(raw_tau, min_taus_)) {
      int combo_cut = 0;

      auto loose_tau = helper_.GetSelectedTaus(combo, 20., tau::loose);
      auto tight_tau = helper_.GetSelectedTaus(loose_tau, 20., tau::tight);

      if (tight_tau.size() < min_tight_taus_)
         continue;

      passComboCut(event_cut, combo_cut++, passed, "Taus in combo");

      auto preselected_e = removeOverlap(raw_e, loose_tau, 0.15);
      auto preselected_mu = removeOverlap(raw_mu, loose_tau, 0.15);

      // TODO there might be tight electrons that overlap with loose muons,
      // but not tight muons?  Should these be considered when only dealing
      // with tight leptons?
      auto loose_e = helper_.GetSelectedElectrons(preselected_e, min_loose_lep_pt_, e_id_loose);
      auto loose_mu = helper_.GetSelectedMuons(preselected_mu, min_loose_lep_pt_, mu_id_loose);
      auto tight_e = helper_.GetSelectedElectrons(loose_e, min_loose_lep_pt_, e_id_tight);
      auto tight_mu = helper_.GetSelectedMuons(loose_mu, min_loose_lep_pt_, mu_id_tight);

      if (preselected_e.size() + preselected_mu.size() < min_leptons_)
         continue;

      if (loose_e.size() + loose_mu.size() < min_loose_leptons_)
         continue;

      if (tight_e.size() + tight_mu.size() < min_tight_leptons_)
         continue;

      passComboCut(event_cut, combo_cut++, passed, "Leptons in combo (min)");

      if (max_loose_leptons_ >= 0 && loose_e.size() + loose_mu.size() > (unsigned int) max_loose_leptons_)
         continue;

      if (max_tight_leptons_ >= 0 && tight_e.size() + tight_mu.size() > (unsigned int) max_tight_leptons_)
         continue;

      passComboCut(event_cut, combo_cut++, passed, "Leptons in combo (max)");

      // TODO check trigger

      // Get to corrected jets
      auto raw_jets = helper_.GetUncorrectedJets(*ak4jets);
      pat::JetCollection jets_wo_lep;
      pat::JetCollection corrected_jets;

      if (subtract_leptons_) {
         auto jets_wo_mu = helper_.RemoveOverlaps(loose_mu, raw_jets);
         jets_wo_lep = helper_.RemoveOverlaps(loose_e, jets_wo_mu);
         corrected_jets = helper_.GetCorrectedJets(jets_wo_lep);
      } else {
         auto jets_wo_mu = removeOverlap(raw_jets, loose_mu, .4);
         jets_wo_lep = removeOverlap(raw_jets, loose_e, .4);
         corrected_jets = helper_.GetCorrectedJets(jets_wo_lep);
      }

      // Jet selection
      auto jets_no_taus = removeOverlap(corrected_jets, loose_tau, .25);
      auto selected_jets = helper_.GetSelectedJets(jets_no_taus, min_jet_pt_, 2.5, jetID::jetLoose, '-');
      selected_jets = get_non_pileup(selected_jets);
      auto selected_tags = helper_.GetSelectedJets(jets_no_taus, min_tag_pt_, 2.5, jetID::jetLoose, 'M');
      selected_tags = get_non_pileup(selected_tags);

      auto uncorrected_jets = removeOverlap(jets_wo_lep, loose_tau, .25);

      if (selected_jets.size() < min_jets_ or selected_tags.size() < min_tags_)
         continue;

      if (max_tags_ >= 0 && selected_tags.size() > (unsigned int) max_tags_)
         continue;

      passComboCut(event_cut, combo_cut++, passed, "Jet requirements");
      passComboCut(event_cut, combo_cut++, passed, "Ntuple");

      // ===============
      // Conversion part
      // ===============
      std::vector<superslim::Tau> staus;
      std::vector<superslim::Lepton> sleptons;
      std::vector<superslim::Jet> sjets;

      for (const auto& tau: loose_tau)
         staus.push_back(superslim::Tau(tau));

      for (const auto& lep: loose_e)
         sleptons.push_back(superslim::Lepton(lep, helper_.GetElectronRelIso(lep), rpv, *bs));

      for (const auto& lep: loose_mu)
         sleptons.push_back(superslim::Lepton(lep, helper_.GetMuonRelIso(lep), rpv, *bs));

      std::sort(sleptons.begin(), sleptons.end());

      for (const auto& jet: selected_jets)
         sjets.push_back(superslim::Jet(jet));

      if (print_preselection_) {
         for (const auto& l: preselected_e) {
            std::cout << "DEBUG "
               << event.id().event() << ','
               << l.pdgId() << ','
               << l.pt() << ',' << l.eta() << ',' << l.phi() << ','
               << std::abs(l.gsfTrack()->dxy(rpv.position())) << ','
               << std::abs(l.gsfTrack()->dz(rpv.position())) << ','
               << l.userFloat("relIso") << ','
               << std::abs(l.dB(pat::Electron::PV3D) / l.edB(pat::Electron::PV3D)) << ','
               << l.userFloat("leptonMVA") << '\n';
         }
         for (const auto& l: preselected_mu) {
            std::cout << "DEBUG "
               << event.id().event() << ','
               << l.pdgId() << ','
               << l.pt() << ',' << l.eta() << ',' << l.phi() << ','
               << std::abs(l.innerTrack()->dxy(rpv.position())) << ','
               << std::abs(l.innerTrack()->dz(rpv.position())) << ','
               << l.userFloat("relIso") << ','
               << std::abs(l.dB(pat::Muon::PV3D) / l.edB(pat::Muon::PV3D)) << ','
               << l.userFloat("leptonMVA") << '\n';
         }
      }

      combos.push_back(superslim::Combination(staus, sleptons, sjets));
   }

   if (combos.size() > 0) {
      auto infos = get_collection(event, pu_token_);
      int ntv = -1;

      for (const auto& info: *infos) {
         if (info.getBunchCrossing() == 0) {
            ntv = info.getTrueNumInteractions();
            break;
         }
      }

      auto mets = get_collection(event, met_token_);
      auto trigger_results = get_collection(event, trig_token_);
      auto trigger_names = event.triggerNames(*trigger_results);

      std::auto_ptr<superslim::Event> ptr(new superslim::Event(
               combos,
               event.id().run(), event.id().luminosityBlock(), event.id().event(),
               npv, ntv,
               superslim::Trigger(*trigger_results, trigger_names),
               mets->at(0).p4(), pv,
               *genstuff));

      auto geninfo = get_collection(event, geninfo_token_);
      ptr->setWeight("generator", geninfo->weight());

      event_ = ptr.get();
      tree_->Fill();
   }
}

void
TauProcessor::beginJob()
{
}

void
TauProcessor::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TauProcessor::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TauProcessor);
