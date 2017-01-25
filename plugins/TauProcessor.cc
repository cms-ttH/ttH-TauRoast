#ifndef OLDCRAP
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
#include "FWCore/Common/interface/TriggerNames.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "HLTrigger/HLTcore/interface/TriggerExpressionData.h"
#include "HLTrigger/HLTcore/interface/TriggerExpressionEvaluator.h"
#include "HLTrigger/HLTcore/interface/TriggerExpressionParser.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

#include "DataFormats/Common/interface/ValueMap.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "TH1F.h"

#include "MiniAOD/MiniAODHelper/interface/MiniAODHelper.h"
#include "ttH/TauRoast/interface/SuperSlim.h"


template<typename T>
edm::Handle<T>
get_collection(const edm::EDConsumerBase& base, const edm::Event& event, const edm::EDGetTokenT<T>& token)
{
   edm::Handle<T> handle;
   event.getByToken(token, handle);
   if (!handle.isValid()) {
      edm::EDConsumerBase::Labels l;
      base.labelsForToken(token, l);
      throw edm::Exception(edm::errors::ProductNotFound, "Can't find collection: " + std::string(l.module));
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

class TauProcessor : public edm::one::EDProducer<edm::BeginRunProducer, edm::EndRunProducer> {
   public:
      explicit TauProcessor(const edm::ParameterSet&);
      ~TauProcessor();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginRunProduce(edm::Run&, const edm::EventSetup&) override;
      virtual void endRunProduce(edm::Run&, const edm::EventSetup&) override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      pat::JetCollection
      scaleJets(const pat::JetCollection& jets, const std::string& sys);
      std::vector<superslim::Tau>
      scaleTaus(const std::vector<superslim::Tau>& taus, const std::string& sys);

      template<typename T1, typename T2>
      std::vector<T1>
      removeOverlap(const std::vector<T1>& v1, const std::vector<T2>& v2, double dR = 0.02);

      void passCut(unsigned int cut, const std::string& name, float w = 1.0);

      MiniAODHelper helper_;
      std::auto_ptr<JetCorrectionUncertainty> junc_;

      bool data_;

      unsigned int min_leptons_;
      unsigned int max_leptons_;
      unsigned int min_taus_;

      unsigned int min_jets_;
      unsigned int min_tags_;
      int max_tags_;

      double min_jet_pt_;
      double min_tag_pt_;
      double max_jet_eta_;

      bool filter_pu_jets_;
      bool take_all_;
      bool save_gen_;
      bool tau_vloose_;

      edm::EDGetTokenT<double> rho_token_;
      edm::EDGetTokenT<GenEventInfoProduct> geninfo_token_;
      edm::EDGetTokenT<reco::BeamSpot> bs_token_;
      edm::EDGetTokenT<reco::ConversionCollection> cc_token_;
      edm::EDGetTokenT<std::vector<PileupSummaryInfo>> pu_token_;
      edm::EDGetTokenT<reco::VertexCollection> vertices_token_;
      edm::EDGetTokenT<pat::ElectronCollection> electrons_token_;
      edm::EDGetTokenT<edm::View<pat::Electron>> mva_electrons_token_;
      edm::EDGetTokenT<pat::MuonCollection> muons_token_;
      edm::EDGetTokenT<pat::TauCollection> taus_token_;
      edm::EDGetTokenT<pat::JetCollection> ak4jets_token_;
      edm::EDGetTokenT<reco::GenParticleCollection> gen_token_;
      edm::EDGetTokenT<reco::GenJetCollection> gen_jets_token_;
      edm::EDGetTokenT<pat::METCollection> met_token_;
      edm::EDGetTokenT<edm::TriggerResults> trig_token_;

      unsigned int evt_;
      std::vector<unsigned int> evt_list_;

      // see https://twiki.cern.ch/twiki/bin/view/CMS/TriggerResultsFilter#Use_as_a_Selector_AN1
      triggerExpression::Data m_triggerCache;

      edm::EDGetTokenT<reco::GenJetCollection> genJetsToken_;
      edm::EDGetTokenT<std::vector<int>> genBHadJetIndexToken_;
      edm::EDGetTokenT<std::vector<int>> genBHadFlavourToken_;
      edm::EDGetTokenT<std::vector<int>> genBHadFromTopWeakDecayToken_;
      edm::EDGetTokenT<std::vector<reco::GenParticle>> genBHadPlusMothersToken_;
      edm::EDGetTokenT<std::vector<std::vector<int>>> genBHadPlusMothersIndicesToken_;
      edm::EDGetTokenT<std::vector<int>> genBHadIndexToken_;
      edm::EDGetTokenT<std::vector<int>> genBHadLeptonHadronIndexToken_;
      edm::EDGetTokenT<std::vector<int>> genBHadLeptonViaTauToken_;

      edm::EDGetTokenT<std::vector<int>> genCHadJetIndexToken_;
      edm::EDGetTokenT<std::vector<int>> genCHadFlavourToken_;
      edm::EDGetTokenT<std::vector<int>> genCHadFromTopWeakDecayToken_;
      edm::EDGetTokenT<std::vector<int>> genCHadBHadronIdToken_;

      std::vector<std::string> cutnames_;
      std::vector<std::string> systematics_;

      std::auto_ptr<superslim::CutHistogram> cuts_;
};

TauProcessor::TauProcessor(const edm::ParameterSet& config) :
   data_(config.getParameter<bool>("data")),
   min_leptons_(config.getParameter<unsigned int>("minLeptons")),
   max_leptons_(config.getParameter<unsigned int>("maxLeptons")),
   min_taus_(config.getParameter<unsigned int>("minTaus")),
   min_jets_(config.getParameter<unsigned int>("minJets")),
   min_tags_(config.getParameter<unsigned int>("minTags")),
   max_tags_(config.getParameter<int>("maxTags")),
   min_jet_pt_(config.getParameter<double>("minJetPt")),
   min_tag_pt_(config.getParameter<double>("minTagPt")),
   max_jet_eta_(config.getParameter<double>("maxJetEta")),
   filter_pu_jets_(config.getParameter<bool>("filterPUJets")),
   take_all_(config.getParameter<bool>("takeAll")),
   save_gen_(config.getParameter<bool>("saveGenInfo")),
   tau_vloose_(config.getParameter<bool>("tauVLoose")),
   evt_list_(config.getParameter<std::vector<unsigned int>>("debugEvents")),
   m_triggerCache(
         edm::InputTag("TriggerResults", "", config.getParameter<bool>("reHLT") ? "HLT2" : "HLT"),
         edm::InputTag(""),
         false, false,
         edm::ConsumesCollector(consumesCollector())
   )
{
   produces<superslim::Event>();
   produces<superslim::CutHistogram, edm::InRun>();

   // Override lepton counts if we're taking everything.
   if (take_all_) {
      min_leptons_ = 0;
      max_leptons_ = 99999;
   }

   rho_token_ = consumes<double>(edm::InputTag("fixedGridRhoFastjetAll"));
   geninfo_token_ = consumes<GenEventInfoProduct>(edm::InputTag("generator"));
   bs_token_ = consumes<reco::BeamSpot>(edm::InputTag("offlineBeamSpot"));
   cc_token_ = consumes<reco::ConversionCollection>(edm::InputTag("reducedEgamma", "reducedConversions", ""));
   pu_token_ = consumes<std::vector<PileupSummaryInfo>>(edm::InputTag("slimmedAddPileupInfo"));
   vertices_token_ = consumes<reco::VertexCollection>(edm::InputTag("offlineSlimmedPrimaryVertices"));
   electrons_token_ = consumes<pat::ElectronCollection>(config.getParameter<edm::InputTag>("electrons"));
   mva_electrons_token_ = consumes<edm::View<pat::Electron>>(config.getParameter<edm::InputTag>("electrons"));
   muons_token_ = consumes<pat::MuonCollection>(config.getParameter<edm::InputTag>("muons"));
   taus_token_ = consumes<pat::TauCollection>(config.getParameter<edm::InputTag>("taus"));
   ak4jets_token_ = consumes<pat::JetCollection>(config.getParameter<edm::InputTag>("jets"));
   gen_token_ = consumes<reco::GenParticleCollection>(edm::InputTag("prunedGenParticles"));
   gen_jets_token_ = consumes<reco::GenJetCollection>(config.getParameter<edm::InputTag>("genJets"));
   met_token_ = consumes<pat::METCollection>(edm::InputTag("slimmedMETs"));
   trig_token_ = consumes<edm::TriggerResults>(edm::InputTag("TriggerResults", "", config.getParameter<bool>("reHLT") ? "HLT2" : "HLT"));

   genJetsToken_ = consumes<reco::GenJetCollection>(edm::InputTag("ak4GenJetsCustom"));
   genBHadJetIndexToken_ = consumes<std::vector<int>>(edm::InputTag("matchGenBHadron", "genBHadJetIndex"));
   genBHadFlavourToken_ = consumes<std::vector<int>>(edm::InputTag("matchGenBHadron", "genBHadFlavour"));
   genBHadFromTopWeakDecayToken_ = consumes<std::vector<int>>(edm::InputTag("matchGenBHadron", "genBHadFromTopWeakDecay"));
   genBHadPlusMothersToken_ = consumes<std::vector<reco::GenParticle>>(edm::InputTag("matchGenBHadron", "genBHadPlusMothers"));
   genBHadPlusMothersIndicesToken_ = consumes<std::vector<std::vector<int>> >(edm::InputTag("matchGenBHadron", "genBHadPlusMothersIndices"));
   genBHadIndexToken_ = consumes<std::vector<int>>(edm::InputTag("matchGenBHadron", "genBHadIndex"));
   genBHadLeptonHadronIndexToken_ = consumes<std::vector<int>>(edm::InputTag("matchGenBHadron", "genBHadLeptonHadronIndex"));
   genBHadLeptonViaTauToken_ = consumes<std::vector<int>>(edm::InputTag("matchGenBHadron", "genBHadLeptonViaTau"));
   genCHadJetIndexToken_ = consumes<std::vector<int>>(edm::InputTag("matchGenCHadron", "genCHadJetIndex"));
   genCHadFlavourToken_ = consumes<std::vector<int>>(edm::InputTag("matchGenCHadron", "genCHadFlavour"));
   genCHadFromTopWeakDecayToken_ = consumes<std::vector<int>>(edm::InputTag("matchGenCHadron", "genCHadFromTopWeakDecay"));
   genCHadBHadronIdToken_ = consumes<std::vector<int>>(edm::InputTag("matchGenCHadron", "genCHadBHadronId"));

   if (data_) {
      systematics_ = {"NA"};
   } else {
      systematics_ = {
         "NA",
         "CMS_ttHl_JESUp",
         "CMS_ttHl_JESDown",
         "CMS_ttHl_tauESUp",
         "CMS_ttHl_tauESDown"
      };
   }

   helper_.SetUp("2015_74x", 2500, analysisType::LJ, data_);
   helper_.UseCorrectedJets();
   // helper_.SetJetCorrectorUncertainty();
   // helper_.SetFactorizedJetCorrector();
}


TauProcessor::~TauProcessor()
{
}


std::vector<superslim::Tau>
TauProcessor::scaleTaus(const std::vector<superslim::Tau>& taus, const std::string& sys)
{
   const auto unc = .03;
   auto dir = 0.;
   if (sys == "CMS_ttHl_tauESUp")
      dir = 1.;
   else if (sys == "CMS_ttHl_tauESDown")
      dir = -1.;

   std::vector<superslim::Tau> result;
   for (auto t: taus) {
      t.p4() *= (1 + unc * dir);
      if (t.pt() >= 20.)
         result.push_back(t);
   }

   return helper_.GetSortedByPt(result);
}


pat::JetCollection
TauProcessor::scaleJets(const pat::JetCollection& jets, const std::string& sys)
{
   auto dir = 0.;
   if (sys == "CMS_ttHl_JESUp")
      dir = 1.;
   else if (sys == "CMS_ttHl_JESDown")
      dir = -1.;
   else
      return jets;

   pat::JetCollection result;
   for (auto j: jets) {
      junc_->setJetEta(j.eta());
      junc_->setJetPt(j.pt());
      auto unc = junc_->getUncertainty(true);
      j.scaleEnergy(1 + unc * dir);
      result.push_back(j);
   }

   return helper_.GetSortedByPt(result);
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
   (*cuts_)->Fill(cut, w);

   if (std::find(begin(evt_list_), end(evt_list_), evt_) != end(evt_list_))
      std::cout << evt_ << " passes " << name << std::endl;

   if (cutnames_.size() == cut) {
      cutnames_.push_back(name);
      (*cuts_)->GetXaxis()->SetBinLabel(cut + 1, name.c_str());
   }
}


void
TauProcessor::produce(edm::Event& event, const edm::EventSetup& setup)
{
   using namespace edm;

   evt_ = event.id().event();

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

   if (npv == 0) {
      return;
   }

   // passed valid pv cut
   passCut(event_cut++, "Valid primary vertex");

   auto bs = get_collection(*this, event, bs_token_);
   auto cc = get_collection(*this, event, cc_token_);

   auto rho = get_collection(*this, event, rho_token_);
   helper_.SetRho(*rho);

   edm::ESHandle<JetCorrectorParametersCollection> jetcorr;
   setup.get<JetCorrectionsRecord>().get("AK4PF", jetcorr);

   const JetCorrectorParameters& params = (*jetcorr)["Uncertainty"];
   helper_.SetJetCorrectorUncertainty(params);

   // ===============
   // Basic selection
   // ===============

   auto electrons = get_collection(*this, event, electrons_token_);
   auto muons = get_collection(*this, event, muons_token_);
   auto taus = get_collection(*this, event, taus_token_);
   auto ak4jets = get_collection(*this, event, ak4jets_token_);

   // ================
   // Jet preselection
   // ================
   edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
   setup.get<JetCorrectionsRecord>().get("AK5PF", JetCorParColl);
   const JetCorrectorParameters& JetCorPar = (*JetCorParColl)["Uncertainty"];
   junc_.reset(new JetCorrectionUncertainty(JetCorPar));

   auto raw_jets = helper_.GetSelectedJets(*ak4jets, 0., 666., jetID::jetLoose, '-');
   // auto uncorrected_jets = helper_.GetUncorrectedJets(raw_jets);

   auto mets = get_collection(*this, event, met_token_);
   auto old_jets = helper_.GetSelectedJets(*ak4jets, 0., 666., jetID::jetMETcorrection, '-');
   // auto old_jets_uncorrected = helper_.GetUncorrectedJets(old_jets);

   reco::GenParticleCollection particles;
   reco::GenJetCollection genjets;
   if (!data_) {
         particles = *get_collection(*this, event, gen_token_);
         genjets = *get_collection(*this, event, gen_jets_token_);
   }

   // ================
   // Lepton selection
   // ================

   auto lepton_id = superslim::Lepton::MVA;

   std::vector<superslim::Lepton> all_leptons;
   std::vector<superslim::Lepton> chosen_leptons;

   for (const auto& lep: *electrons) {
      auto l = superslim::Lepton(lep, rpv, *bs, particles, -1);
      if (l.preselected(lepton_id))
         all_leptons.push_back(l);
   }

   for (const auto& lep: *muons) {
      auto l = superslim::Lepton(lep, rpv, *bs, particles, -1);
      if (l.preselected(lepton_id))
         all_leptons.push_back(l);
   }
   std::sort(all_leptons.begin(), all_leptons.end());
   for (unsigned int i = 0; i < all_leptons.size(); ++i)
      all_leptons[i].rank(i);

   if (lepton_id == superslim::Lepton::MVA) {
      std::copy_if(all_leptons.begin(), all_leptons.end(), std::back_inserter(chosen_leptons),
            [&](const auto& l) { return l.loose(superslim::Lepton::Fakeable); });
   } else {
      chosen_leptons = all_leptons;
   }

   if (not take_all_ and chosen_leptons.size() < min_leptons_)
      return;

   passCut(event_cut++, "Leptons");

   auto tau_id = superslim::Tau::IsoMVA03;

   int r = 0;
   std::vector<superslim::Tau> raw_taus;
   for (const auto& tau: *taus) {
      auto t = superslim::Tau(tau, rpv, particles, genjets, r++);
      if (t.selected(tau_id, tau_vloose_ ? superslim::id::VLoose : superslim::id::Loose))
         raw_taus.push_back(t);
   }
   auto all_taus = removeOverlap(raw_taus, all_leptons, .3);

   bool pass_taus = false;
   bool pass_jets = false;
   bool pass_tags = false;

   std::map<std::string, std::vector<superslim::Tau>> staus;
   std::map<std::string, std::vector<superslim::Tau>> all_staus;

   std::map<std::string, std::vector<superslim::Jet>> sjets;
   std::map<std::string, superslim::LorentzVector> smets;
   for (auto& sys: systematics_) {
      // =============
      // Tau Selection
      // =============

      auto taus_updown = scaleTaus(all_taus, sys);

      if (taus_updown.size() >= min_taus_)
         pass_taus = true;

      // Select tight τ.  If not enough tight τ are available, select the
      // first n τ — for the fake background.
      std::vector<superslim::Tau> chosen_taus;
      if (take_all_) {
         chosen_taus = taus_updown;
      } else {
         std::copy_if(taus_updown.begin(), taus_updown.end(), std::back_inserter(chosen_taus),
               [&](const superslim::Tau& t) -> bool { return t.selected(tau_id, superslim::id::Tight); });
         if (chosen_taus.size() < min_taus_) {
            std::vector<superslim::Tau>().swap(chosen_taus);
            std::copy(taus_updown.begin(), taus_updown.begin() + min_taus_, std::back_inserter(chosen_taus));
         }
      }

      staus[sys] = chosen_taus;
      all_staus[sys] = taus_updown;

      // =============
      // Jet Selection
      // =============

      auto jets_updown = scaleJets(raw_jets, sys);
      auto jets_wo_lep = removeOverlap(jets_updown, chosen_leptons, .4);
      auto jets_no_taus = removeOverlap(jets_wo_lep, all_taus, .4);
      auto selected_jets = helper_.GetSelectedJets(jets_no_taus, min_jet_pt_, max_jet_eta_, jetID::none, '-');
      auto selected_tags = helper_.GetSelectedJets(jets_no_taus, min_tag_pt_, max_jet_eta_, jetID::none, 'M');
      auto selected_loose_tags = helper_.GetSelectedJets(jets_no_taus, min_tag_pt_, max_jet_eta_, jetID::none, 'L');

      if (filter_pu_jets_) {
         selected_jets = get_non_pileup(selected_jets);
         selected_tags = get_non_pileup(selected_tags);
         selected_loose_tags = get_non_pileup(selected_loose_tags);
      };

      if (selected_jets.size() >= min_jets_ and selected_tags.size() >= min_tags_) {
         pass_jets = true;
      } else if (selected_jets.size() >= min_jets_ and selected_loose_tags.size() >= std::max(min_tags_, 2u)) {
         pass_jets = true;
      }

      if (max_tags_ < 0 or selected_tags.size() <= (unsigned int) max_tags_)
         pass_tags = true;

      // auto new_jets = helper_.GetCorrectedJets(old_jets_uncorrected, event, setup, sys.second);
      // auto corrected_mets = helper_.CorrectMET(old_jets, new_jets, *mets);

      sjets[sys] = {};
      for (const auto& jet: selected_jets)
         sjets[sys].push_back(superslim::Jet(jet, particles));

      // smets[sys.first] = corrected_mets[0].p4();
      smets[sys] = mets->at(0).p4();
   }

   if (not (pass_taus and pass_jets and pass_tags) and not take_all_)
      return;

   // Set leptons *after* jet-cleaning!
   if (take_all_)
      chosen_leptons = all_leptons;

   passCut(event_cut++, "Jet requirements");
   passCut(event_cut++, "Ntuple");

   int category = 0;
   int ntv = -1;

   if (!data_) {
      // HF categorization
      auto genJets = get_collection(*this, event, genJetsToken_);
      auto genBHadFlavour = get_collection(*this, event, genBHadFlavourToken_);
      auto genBHadJetIndex = get_collection(*this, event, genBHadJetIndexToken_);
      auto genBHadFromTopWeakDecay = get_collection(*this, event, genBHadFromTopWeakDecayToken_);
      auto genBHadPlusMothers = get_collection(*this, event, genBHadPlusMothersToken_);
      auto genBHadPlusMothersIndices = get_collection(*this, event, genBHadPlusMothersIndicesToken_);
      auto genBHadIndex = get_collection(*this, event, genBHadIndexToken_);
      auto genBHadLeptonHadronIndex = get_collection(*this, event, genBHadLeptonHadronIndexToken_);
      auto genBHadLeptonViaTau = get_collection(*this, event, genBHadLeptonViaTauToken_);
      auto genCHadFlavour = get_collection(*this, event, genCHadFlavourToken_);
      auto genCHadJetIndex = get_collection(*this, event, genCHadJetIndexToken_);
      auto genCHadFromTopWeakDecay = get_collection(*this, event, genCHadFromTopWeakDecayToken_);
      auto genCHadBHadronId = get_collection(*this, event, genCHadBHadronIdToken_);

      category = helper_.ttHFCategorization(*genJets,
         *genBHadIndex, *genBHadJetIndex, *genBHadFlavour, *genBHadFromTopWeakDecay,
         *genBHadPlusMothers, *genBHadPlusMothersIndices, *genBHadLeptonHadronIndex, *genBHadLeptonViaTau,
         *genCHadFlavour, *genCHadJetIndex, *genCHadFromTopWeakDecay, *genCHadBHadronId, 20., 2.4);

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
            staus, all_staus, chosen_leptons, all_leptons, sjets, smets,
            tau_id, lepton_id,
            event.id().run(), event.id().luminosityBlock(), event.id().event(),
            npv, ntv, pv,
            category,
            superslim::Trigger(*trigger_results, trigger_names),
            particles));

   ptr->setWeight("Generator", genweight);

   if (save_gen_) {
      std::vector<superslim::GenJet> gjets;
      for (const auto& j: genjets) {
         gjets.push_back(j);
         gjets.back().findClosestGenJet(genjets);
         gjets.back().findClosestGenParticle(particles);
      }

      std::vector<superslim::GenObject> gparticles;
      std::array<int, 3> ids{{11, 13, 15}};
      for (const auto& p: particles) {
         if (std::find(ids.begin(), ids.end(), abs(p.pdgId())) != ids.end() and
               (p.statusFlags().isPrompt() or p.isPromptFinalState() or p.statusFlags().isDirectPromptTauDecayProduct() or p.isDirectPromptTauDecayProductFinalState()))
            gparticles.push_back(p);
      }

      ptr->setGenJets(gjets);
      ptr->setGenParticles(gparticles);
   }
   event.put(std::move(ptr));
}

void
TauProcessor::beginRunProduce(edm::Run& run, const edm::EventSetup& s)
{
   cuts_.reset(new superslim::CutHistogram());
}

void
TauProcessor::endRunProduce(edm::Run& run, const edm::EventSetup& s)
{
   run.put(std::move(cuts_));
}

void
TauProcessor::endJob()
{
   if (cuts_.get() != 0) {
      edm::LogError("TauProcessor") << "Cut histogram still set!";
   }
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
#endif
