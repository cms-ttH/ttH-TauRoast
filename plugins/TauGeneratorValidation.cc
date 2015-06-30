// -*- C++ -*-
//
// Package:    ttH/TauGeneratorValidation
// Class:      TauGeneratorValidation
// 
/**\class TauGeneratorValidation TauGeneratorValidation.cc ttH/TauRoast/plugins/TauGeneratorValidation.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Matthias Wolf
//         Created:  Wed, 18 Mar 2015 10:24:15 GMT
//
//


// system include files
#include <memory>

// user include files
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/PatCandidates/interface/GenericParticle.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "TTree.h"

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

const reco::Candidate *
get_final(const reco::Candidate * c)
{
   while (c->numberOfDaughters() > 0 and c->daughter(0) and c->daughter(0)->pdgId() == c->pdgId())
      c = c->daughter(0);

   return c;
}

class TauGeneratorValidation : public edm::EDAnalyzer {
   public:
      explicit TauGeneratorValidation(const edm::ParameterSet&);
      ~TauGeneratorValidation();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      edm::EDGetTokenT<reco::GenParticleCollection> gen_token_;
      edm::EDGetTokenT<reco::GenJetCollection> jet_token_;
      edm::EDGetTokenT<GenEventInfoProduct> geninfo_token_;
      edm::EDGetTokenT<pat::TauCollection> tau_token_;

      TTree *tree_;

      int njets_;
      int njets30_;

      float tt_mass_;
      float tt_pt_;

      TTree *tjet_;

      float jpt_;
      float jeta_;
      float jphi_;

      float w_;

      TTree *tdecays_;

      float m_id_;
      float m_pt_;
      float m_eta_;
      float m_phi_;

      float d1_id_;
      float d1_pt_;
      float d1_eta_;
      float d1_phi_;
      float d1_decay_;
      float d1_vispt_;
      float d1_recopt_;
      float d1_iso3hits_;
      float d1_mvaid_;
      float d1_dR_nearest_;
      float d1_dR_hardest_;

      float d2_id_;
      float d2_pt_;
      float d2_eta_;
      float d2_phi_;
      float d2_decay_;
      float d2_vispt_;
      float d2_recopt_;
      float d2_iso3hits_;
      float d2_mvaid_;
      float d2_dR_nearest_;
      float d2_dR_hardest_;

      std::vector<int> potential_mothers_ = {6, 23, 24, 25};
};

TauGeneratorValidation::TauGeneratorValidation(const edm::ParameterSet& config)
{
   // gen_token_ = consumes<reco::GenParticleCollection>(edm::InputTag("genParticles"));
   gen_token_ = consumes<reco::GenParticleCollection>(edm::InputTag("prunedGenParticles"));
   // jet_token_ = consumes<reco::GenJetCollection>(edm::InputTag("ak5GenJets"));
   jet_token_ = consumes<reco::GenJetCollection>(edm::InputTag("slimmedGenJets"));
   tau_token_ = consumes<pat::TauCollection>(edm::InputTag("slimmedTaus"));
   geninfo_token_ = consumes<GenEventInfoProduct>(edm::InputTag("generator"));

   edm::Service<TFileService> fs;
   tree_ = fs->make<TTree>("events", "Event data");
   tree_->Branch("njets", &njets_);
   tree_->Branch("njets30", &njets30_);

   tree_->Branch("ditaumass", &tt_mass_);
   tree_->Branch("ditaupt", &tt_pt_);

   tree_->Branch("w", &w_);

   tjet_ = fs->make<TTree>("jets", "Jet data");
   tjet_->Branch("pt", &jpt_);
   tjet_->Branch("eta", &jeta_);
   tjet_->Branch("phi", &jphi_);

   tjet_->Branch("w", &w_);

   tdecays_ = fs->make<TTree>("decays", "Boson decays");
   tdecays_->Branch("mother_id", &m_id_);
   tdecays_->Branch("mother_pt", &m_pt_);
   tdecays_->Branch("mother_eta", &m_eta_);
   tdecays_->Branch("mother_phi", &m_phi_);

   tdecays_->Branch("daughter1_id", &d1_id_);
   tdecays_->Branch("daughter1_pt", &d1_pt_);
   tdecays_->Branch("daughter1_eta", &d1_eta_);
   tdecays_->Branch("daughter1_phi", &d1_phi_);
   tdecays_->Branch("daughter1_decay", &d1_decay_);
   tdecays_->Branch("daughter1_visible_pt", &d1_vispt_);
   tdecays_->Branch("daughter1_reco_pt", &d1_recopt_);
   tdecays_->Branch("daughter1_iso_3hits", &d1_iso3hits_);
   tdecays_->Branch("daughter1_iso_mva", &d1_mvaid_);
   tdecays_->Branch("daughter1_dR_nearest", &d1_dR_nearest_);
   tdecays_->Branch("daughter1_dR_hardest", &d1_dR_hardest_);

   tdecays_->Branch("daughter2_id", &d2_id_);
   tdecays_->Branch("daughter2_pt", &d2_pt_);
   tdecays_->Branch("daughter2_eta", &d2_eta_);
   tdecays_->Branch("daughter2_phi", &d2_phi_);
   tdecays_->Branch("daughter2_decay", &d2_decay_);
   tdecays_->Branch("daughter2_visible_pt", &d2_vispt_);
   tdecays_->Branch("daughter2_reco_pt", &d2_recopt_);
   tdecays_->Branch("daughter2_iso_3hits", &d2_iso3hits_);
   tdecays_->Branch("daughter2_iso_mva", &d2_mvaid_);
   tdecays_->Branch("daughter2_dR_nearest", &d2_dR_nearest_);
   tdecays_->Branch("daughter2_dR_hardest", &d2_dR_hardest_);
}


TauGeneratorValidation::~TauGeneratorValidation()
{
}


void
TauGeneratorValidation::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
   using namespace edm;

   auto geninfo = get_collection(event, geninfo_token_);
   w_ = geninfo->weight();

   tt_mass_  = -99;
   tt_pt_    = -99;

   m_id_  = -99;
   m_pt_  = -99;
   m_eta_ = -99;
   m_phi_ = -99;

   njets30_ = 0;
   auto gen_jets = get_collection(event, jet_token_);
   for (const auto& jet: *gen_jets) {
      if (jet.pt() < 30)
         continue;

      ++njets30_;

      jpt_ = jet.pt();
      jeta_ = jet.eta();
      jphi_ = jet.phi();
      tjet_->Fill();
   }

   njets_ = gen_jets->size();

   auto taus = get_collection(event, tau_token_);
   auto gen_particles = get_collection(event, gen_token_);
   for (const auto& particle: *gen_particles) {
      if (std::find(potential_mothers_.begin(), potential_mothers_.end(), abs(particle.pdgId())) == potential_mothers_.end())
         continue;

      if (particle.numberOfDaughters() != 2 or !particle.daughter(0) or particle.daughter(0)->pdgId() == particle.pdgId())
         continue;

      auto d1 = get_final(particle.daughter(0));
      auto d2 = get_final(particle.daughter(1));

      m_id_ = particle.pdgId();
      m_pt_ = particle.pt();
      m_eta_ = particle.eta();
      m_phi_ = particle.phi();

      d1_id_ = d1->pdgId();
      d1_pt_ = d1->pt();
      d1_eta_ = d1->eta();
      d1_phi_ = d1->phi();

      d2_id_ = d2->pdgId();
      d2_pt_ = d2->pt();
      d2_eta_ = d2->eta();
      d2_phi_ = d2->phi();

      d1_decay_ = -99;
      d1_vispt_ = -99;
      d1_recopt_ = -99;
      d1_iso3hits_ = -99;
      d1_mvaid_ = -99;
      d1_dR_hardest_ = -99;
      d1_dR_nearest_ = -99;

      d2_decay_ = -99;
      d2_vispt_ = -99;
      d2_recopt_ = -99;
      d2_iso3hits_ = -99;
      d2_mvaid_ = -99;
      d2_dR_hardest_ = -99;
      d2_dR_nearest_ = -99;

      for (const auto& j: *gen_jets) {
         if (j.pt() < 30)
            continue;
         auto dR_d1 = deltaR(d1->p4(), j.p4());
         auto dR_d2 = deltaR(d2->p4(), j.p4());

         if (d1_dR_hardest_ < 0.)
            d1_dR_hardest_ = dR_d1;
         if ((d1_dR_nearest_ < 0. or dR_d1 < d1_dR_nearest_) and dR_d1 > .1)
            d1_dR_nearest_ = dR_d1;

         if (d2_dR_hardest_ < 0.)
            d2_dR_hardest_ = dR_d2;
         if ((d2_dR_nearest_ < 0. or dR_d2 < d2_dR_nearest_) and dR_d2 > .1)
            d2_dR_nearest_ = dR_d2;
      }

      if (m_id_ == 25 and abs(d1_id_) == 15) {
         const reco::Candidate* gd = 0;
         for (unsigned int i = 0; i < d1->numberOfDaughters(); ++i) {
            gd = get_final(d1->daughter(i));
            if (abs(gd->pdgId()) == 16)
               continue;
            if (gd->pdgId() % 2 == 0 and abs(gd->pdgId()) > 10 and abs(gd->pdgId()) < 20)
               continue;
            break;
         }

         d1_decay_ = gd->pdgId();
         d1_vispt_ = 0;
         for (unsigned int i = 0; i < d1->numberOfDaughters(); ++i) {
            auto p = get_final(d1->daughter(i));
            if (p->pdgId() % 2 == 0 and abs(p->pdgId()) > 10 and abs(p->pdgId()) < 20)
               continue;
            d1_vispt_ += p->pt();
         }

         for (const auto& t: *taus) {
            if (t.genParticle() == d1) {
               if (t.tauID("decayModeFindingNewDMs") < .5)
                  continue;

               d1_recopt_ = t.pt();

               if (t.tauID("byTightCombinedIsolationDeltaBetaCorr3Hits") > .5)
                  d1_iso3hits_ = 3;
               else if (t.tauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") > .5)
                  d1_iso3hits_ = 2;
               else if (t.tauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") > .5)
                  d1_iso3hits_ = 1;
               else
                  d1_iso3hits_ = 0;

               if (t.tauID("byVVTightIsolationMVA3newDMwLT") > .5)
                  d1_mvaid_ = 6;
               else if (t.tauID("byVTightIsolationMVA3newDMwLT") > .5)
                  d1_mvaid_ = 5;
               else if (t.tauID("byTightIsolationMVA3newDMwLT") > .5)
                  d1_mvaid_ = 4;
               else if (t.tauID("byMediumIsolationMVA3newDMwLT") > .5)
                  d1_mvaid_ = 3;
               else if (t.tauID("byLooseIsolationMVA3newDMwLT") > .5)
                  d1_mvaid_ = 2;
               else if (t.tauID("byVLooseIsolationMVA3newDMwLT") > .5)
                  d1_mvaid_ = 1;
               else
                  d1_mvaid_ = 0;

               break;
            }
         }
      }

      if (m_id_ == 25 and abs(d2_id_) == 15) {
         const reco::Candidate* gd = 0;
         for (unsigned int i = 0; i < d2->numberOfDaughters(); ++i) {
            gd = get_final(d2->daughter(i));
            if (abs(gd->pdgId()) == 16)
               continue;
            if (gd->pdgId() % 2 == 0 and abs(gd->pdgId()) > 10 and abs(gd->pdgId()) < 20)
               continue;
            break;
         }

         d2_decay_ = gd->pdgId();
         d2_vispt_ = 0;
         for (unsigned int i = 0; i < d2->numberOfDaughters(); ++i) {
            auto p = get_final(d2->daughter(i));
            if (p->pdgId() % 2 == 0 and abs(p->pdgId()) > 10 and abs(p->pdgId()) < 20)
               continue;
            d2_vispt_ += p->pt();
         }

         for (const auto& t: *taus) {
            if (t.genParticle() == d2) {
               if (t.tauID("decayModeFindingNewDMs") < .5)
                  continue;

               d2_recopt_ = t.pt();

               if (t.tauID("byTightCombinedIsolationDeltaBetaCorr3Hits") > .5)
                  d2_iso3hits_ = 3;
               else if (t.tauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") > .5)
                  d2_iso3hits_ = 2;
               else if (t.tauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") > .5)
                  d2_iso3hits_ = 1;
               else
                  d2_iso3hits_ = 0;

               if (t.tauID("byVVTightIsolationMVA3newDMwLT") > .5)
                  d2_mvaid_ = 6;
               else if (t.tauID("byVTightIsolationMVA3newDMwLT") > .5)
                  d2_mvaid_ = 5;
               else if (t.tauID("byTightIsolationMVA3newDMwLT") > .5)
                  d2_mvaid_ = 4;
               else if (t.tauID("byMediumIsolationMVA3newDMwLT") > .5)
                  d2_mvaid_ = 3;
               else if (t.tauID("byLooseIsolationMVA3newDMwLT") > .5)
                  d2_mvaid_ = 2;
               else if (t.tauID("byVLooseIsolationMVA3newDMwLT") > .5)
                  d2_mvaid_ = 1;
               else
                  d2_mvaid_ = 0;

               break;
            }
         }
      }

      tdecays_->Fill();
   }

   tree_->Fill();
}

void 
TauGeneratorValidation::beginJob()
{
}

void 
TauGeneratorValidation::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TauGeneratorValidation::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TauGeneratorValidation);
