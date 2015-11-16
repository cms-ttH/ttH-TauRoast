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

#include "JetMETCorrections/Objects/interface/JetCorrector.h"

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
get_collection(const edm::EDConsumerBase& base, const edm::Event& event, const edm::EDGetTokenT<T>& token)
{
   edm::Handle<T> handle;
   event.getByToken(token, handle);
   if (!handle.isValid()) {
      edm::EDConsumerBase::Labels l;
      base.labelsForToken(token, l);
      throw edm::Exception(edm::errors::InvalidReference, "Can't find collection: " + std::string(l.module));
   }
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

      bool data_;

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
      double min_tight_e_pt_;
      double min_tight_mu_pt_;

      double max_loose_lep_eta_;
      double max_tight_lep_eta_;

      double min_jet_pt_;
      double min_tag_pt_;
      double max_jet_eta_;

      bool filter_pu_jets_;

      edm::EDGetTokenT<double> rho_token_;
      edm::EDGetTokenT<GenEventInfoProduct> geninfo_token_;
      edm::EDGetTokenT<reco::BeamSpot> bs_token_;
      edm::EDGetTokenT<reco::ConversionCollection> cc_token_;
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
      std::map<std::string, sysType::sysType> systematics_;

      superslim::Event *event_;

      // see https://twiki.cern.ch/twiki/bin/view/CMS/TriggerResultsFilter#Use_as_a_Selector_AN1
      triggerExpression::Data m_triggerCache;
      std::unique_ptr<triggerExpression::Evaluator> m_triggerSelector;
};

TauProcessor::TauProcessor(const edm::ParameterSet& config) :
   data_(config.getParameter<bool>("data")),
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
   min_tight_e_pt_(config.getParameter<double>("minTightElectronPt")),
   min_tight_mu_pt_(config.getParameter<double>("minTightMuonPt")),
   max_loose_lep_eta_(config.getParameter<double>("maxLooseLeptonEta")),
   max_tight_lep_eta_(config.getParameter<double>("maxTightLeptonEta")),
   min_jet_pt_(config.getParameter<double>("minJetPt")),
   min_tag_pt_(config.getParameter<double>("minTagPt")),
   max_jet_eta_(config.getParameter<double>("maxJetEta")),
   filter_pu_jets_(config.getParameter<bool>("filterPUJets")),
   event_(0),
   m_triggerCache(
         edm::InputTag("TriggerResults", "", "HLT"),
         edm::InputTag("gtDigis"),
         1, false, false, true,
         edm::ConsumesCollector(consumesCollector())
   )
{
   rho_token_ = consumes<double>(edm::InputTag("fixedGridRhoFastjetAll"));
   geninfo_token_ = consumes<GenEventInfoProduct>(edm::InputTag("generator"));
   bs_token_ = consumes<reco::BeamSpot>(edm::InputTag("offlineBeamSpot"));
   cc_token_ = consumes<reco::ConversionCollection>(edm::InputTag("reducedEgamma", "reducedConversions", ""));
   pu_token_ = consumes<std::vector<PileupSummaryInfo>>(edm::InputTag("slimmedAddPileupInfo"));
   vertices_token_ = consumes<reco::VertexCollection>(edm::InputTag("offlineSlimmedPrimaryVertices"));
   electrons_token_ = consumes<pat::ElectronCollection>(config.getParameter<edm::InputTag>("electrons"));
   muons_token_ = consumes<pat::MuonCollection>(config.getParameter<edm::InputTag>("muons"));
   taus_token_ = consumes<pat::TauCollection>(edm::InputTag("slimmedTaus"));
   ak4jets_token_ = consumes<pat::JetCollection>(edm::InputTag("slimmedJets"));
   gen_token_ = consumes<reco::GenParticleCollection>(edm::InputTag("prunedGenParticles"));
   met_token_ = consumes<pat::METCollection>(edm::InputTag("slimmedMETs"));
   trig_token_ = consumes<edm::TriggerResults>(edm::InputTag("TriggerResults", "", "HLT"));

   if (data_) {
      systematics_ = {{"NA", sysType::NA}};
   } else {
      systematics_ = {
         {"NA", sysType::NA},
         {"JERUp", sysType::JERup},
         {"JERDown", sysType::JERdown},
         {"JESUp", sysType::JESup},
         {"JESDown", sysType::JESdown}
      };
   }

   superslim::Trigger::set_single_e_triggers(config.getParameter<std::vector<std::string>>("triggerSingleE"));
   superslim::Trigger::set_single_mu_triggers(config.getParameter<std::vector<std::string>>("triggerSingleMu"));
   m_triggerSelector = std::unique_ptr<triggerExpression::Evaluator>(triggerExpression::parse(superslim::Trigger::get_selection()));

   helper_.SetUp("2012_53x", 2500, analysisType::LJ, data_);
   helper_.SetJetCorrectorUncertainty();
   // helper_.SetFactorizedJetCorrector();

   if (min_leptons_ == 1) {
      helper_.SetUpElectronMVA(
            "MiniAOD/MiniAODHelper/data/ElectronMVA/EIDmva_EB1_10_oldTrigSpring15_25ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml",
            "MiniAOD/MiniAODHelper/data/ElectronMVA/EIDmva_EB2_10_oldTrigSpring15_25ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml",
            "MiniAOD/MiniAODHelper/data/ElectronMVA/EIDmva_EE_10_oldTrigSpring15_25ns_data_1_VarD_TMVA412_Sig6BkgAll_MG_noSpec_BDT.weights.xml"
      );
   }

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

   int event_cut = 0;

   // events run over count
   passCut(event_cut++, "Dataset processed");

   double genweight = 1.;
   if (not data_) {
      auto geninfo = get_collection(*this, event, geninfo_token_);
      genweight = geninfo->weight();
   }

   // dataset sum of the event weights
   passCut(event_cut++, "Dataset event weights", genweight);

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

   auto vertices = get_collection(*this, event, vertices_token_);

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

   auto bs = get_collection(*this, event, bs_token_);
   auto cc = get_collection(*this, event, cc_token_);

   auto rho = get_collection(*this, event, rho_token_);
   helper_.SetRho(*rho);

   auto corr = JetCorrector::getJetCorrector("ak4PFchsL1L2L3", setup);
   helper_.SetJetCorrector(corr);

   // ===============
   // Setup lepton ID
   // ===============

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

      e_id_pre = electronID::electronEndOf15MVAmedium;
      e_id_loose = electronID::electronEndOf15MVAmedium;
      e_id_tight = electronID::electronEndOf15MVAmedium;

      helper_.SetElectronMVAinfo(cc, bs);
   }

   // ===============
   // Basic selection
   // ===============

   auto electrons = get_collection(*this, event, electrons_token_);
   auto muons = get_collection(*this, event, muons_token_);
   auto taus = get_collection(*this, event, taus_token_);
   auto ak4jets = get_collection(*this, event, ak4jets_token_);

   auto raw_mu = helper_.GetSelectedMuons(*muons, 5., mu_id_pre);
   auto all_preselected_e = helper_.GetSelectedElectrons(*electrons, 7., e_id_pre);
   auto raw_e = removeOverlap(all_preselected_e, raw_mu, 0.05);
   auto raw_tau = helper_.GetSelectedTaus(*taus, 20., tau::loose);
   auto raw_tight_tau = helper_.GetSelectedTaus(raw_tau, 20., tau::tight);

   if (raw_tau.size() < min_taus_)
      return;

   if (raw_tight_tau.size() < min_tight_taus_)
      return;

   // ================
   // Jet preselection
   // ================
   auto raw_jets = helper_.GetSelectedJets(*ak4jets, 0., 666., jetID::jetLoose, '-');
   auto uncorrected_jets = helper_.GetUncorrectedJets(raw_jets);

   auto mets = get_collection(*this, event, met_token_);
   auto old_jets = helper_.GetSelectedJets(*ak4jets, 0., 666., jetID::jetMETcorrection, '-');
   auto old_jets_uncorrected = helper_.GetUncorrectedJets(old_jets);

   reco::GenParticleCollection particles;
   if (!data_)
         particles = *get_collection(*this, event, gen_token_);

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
      auto loose_e = helper_.GetSelectedElectrons(preselected_e, min_loose_lep_pt_, e_id_loose, max_loose_lep_eta_);
      auto loose_mu = helper_.GetSelectedMuons(preselected_mu, min_loose_lep_pt_, mu_id_loose, coneSize::R04, corrType::deltaBeta, max_loose_lep_eta_);
      auto tight_e = helper_.GetSelectedElectrons(loose_e, min_tight_e_pt_, e_id_tight, max_tight_lep_eta_);
      auto tight_mu = helper_.GetSelectedMuons(loose_mu, min_tight_mu_pt_, mu_id_tight, coneSize::R04, corrType::deltaBeta, max_tight_lep_eta_);

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

      bool pass_jets = false;
      bool pass_tags = false;
      std::map<std::string, std::vector<superslim::Jet>> sjets;
      std::map<std::string, superslim::LorentzVector> smets;
      for (auto& sys: systematics_) {
         auto corrected_jets = helper_.GetCorrectedJets(uncorrected_jets, event, setup, sys.second);
         corrected_jets = helper_.GetSelectedJets(corrected_jets, std::min(min_jet_pt_, min_tag_pt_), max_jet_eta_, jetID::none, '-');
         corrected_jets = helper_.GetSortedByPt(corrected_jets);
         pat::JetCollection jets_wo_lep;

         if (subtract_leptons_) {
            auto jets_wo_mu = helper_.RemoveOverlaps(loose_mu, corrected_jets);
            jets_wo_lep = helper_.RemoveOverlaps(loose_e, jets_wo_mu);
         } else {
            jets_wo_lep = helper_.GetDeltaRCleanedJets(corrected_jets, loose_mu, loose_e, .4);
         }

         // Jet selection
         auto jets_no_taus = removeOverlap(jets_wo_lep, loose_tau, .25);
         auto selected_jets = helper_.GetSelectedJets(jets_no_taus, min_jet_pt_, max_jet_eta_, jetID::none, '-');
         auto selected_tags = helper_.GetSelectedJets(jets_no_taus, min_tag_pt_, max_jet_eta_, jetID::none, 'M');
         if (filter_pu_jets_) {
            selected_jets = get_non_pileup(selected_jets);
            selected_tags = get_non_pileup(selected_tags);
         };

         if (selected_jets.size() >= min_jets_ and selected_tags.size() >= min_tags_)
            pass_jets = true;

         if (max_tags_ < 0 or selected_tags.size() <= (unsigned int) max_tags_)
            pass_tags = true;

         auto new_jets = helper_.GetCorrectedJets(old_jets_uncorrected, event, setup, sys.second);
         auto corrected_mets = helper_.CorrectMET(old_jets, new_jets, *mets);

         sjets[sys.first] = {};
         for (const auto& jet: selected_jets)
            sjets[sys.first].push_back(superslim::Jet(jet, particles));

         smets[sys.first] = corrected_mets[0].p4();
      }

      if (not (pass_jets and pass_tags))
         continue;

      passComboCut(event_cut, combo_cut++, passed, "Jet requirements");
      passComboCut(event_cut, combo_cut++, passed, "Ntuple");

      // ===============
      // Conversion part
      // ===============
      std::vector<superslim::Tau> staus;
      std::vector<superslim::Lepton> sleptons;

      for (const auto& tau: loose_tau)
         staus.push_back(superslim::Tau(tau, particles));

      for (const auto& lep: loose_e)
         sleptons.push_back(superslim::Lepton(lep, helper_.GetElectronRelIso(lep), rpv, *bs, particles));

      for (const auto& lep: loose_mu)
         sleptons.push_back(superslim::Lepton(lep, helper_.GetMuonRelIso(lep), rpv, *bs, particles));

      std::sort(sleptons.begin(), sleptons.end());

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

      auto c = superslim::Combination(staus, sleptons, sjets, smets);
      combos.push_back(c);
   }

   if (combos.size() > 0) {
      int ntv = -1;

      if (!data_) {
         auto infos = get_collection(*this, event, pu_token_);
         for (const auto& info: *infos) {
            if (info.getBunchCrossing() == 0) {
               ntv = info.getTrueNumInteractions();
               break;
            }
         }
      }

      auto trigger_results = get_collection(*this, event, trig_token_);
      auto trigger_names = event.triggerNames(*trigger_results);

      std::auto_ptr<superslim::Event> ptr(new superslim::Event(
               combos,
               event.id().run(), event.id().luminosityBlock(), event.id().event(),
               npv, ntv, pv,
               superslim::Trigger(*trigger_results, trigger_names),
               particles));

      ptr->setWeight("generator", genweight);

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
