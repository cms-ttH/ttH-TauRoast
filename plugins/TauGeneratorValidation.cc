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

      TTree *tree_;

      int njets_;
      int njets30_;

      float t1_pt_;
      float t1_eta_;
      float t2_pt_;
      float t2_eta_;

      int t1_decay_;
      int t2_decay_;
      float tt_mass_;
      float tt_pt_;

      float top1_pt_;
      float top1_eta_;
      float top2_pt_;
      float top2_eta_;

      TTree *tjet_;

      float jpt_;
      float jeta_;
      float jphi_;

      float w_;
};

TauGeneratorValidation::TauGeneratorValidation(const edm::ParameterSet& config)
{
   gen_token_ = consumes<reco::GenParticleCollection>(edm::InputTag("genParticles"));
   jet_token_ = consumes<reco::GenJetCollection>(edm::InputTag("ak5GenJets"));
   geninfo_token_ = consumes<GenEventInfoProduct>(edm::InputTag("generator"));

   edm::Service<TFileService> fs;
   tree_ = fs->make<TTree>("events", "Event data");
   tree_->Branch("njets", &njets_);
   tree_->Branch("njets30", &njets30_);

   tree_->Branch("tau1pt", &t1_pt_);
   tree_->Branch("tau1eta", &t1_eta_);
   tree_->Branch("tau2pt", &t2_pt_);
   tree_->Branch("tau2eta", &t2_eta_);

   tree_->Branch("tau1decay", &t1_decay_);
   tree_->Branch("tau2decay", &t2_decay_);
   tree_->Branch("ditaumass", &tt_mass_);
   tree_->Branch("ditaupt", &tt_pt_);

   tree_->Branch("top1pt", &top1_pt_);
   tree_->Branch("top1eta", &top1_eta_);
   tree_->Branch("top2pt", &top2_pt_);
   tree_->Branch("top2eta", &top2_eta_);

   tree_->Branch("w", &w_);

   tjet_ = fs->make<TTree>("jets", "Jet data");
   tjet_->Branch("pt", &jpt_);
   tjet_->Branch("eta", &jeta_);
   tjet_->Branch("phi", &jphi_);

   tjet_->Branch("w", &w_);
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

   t1_pt_  = -99;
   t1_eta_ = -99;
   t2_pt_  = -99;
   t2_eta_ = -99;

   t1_decay_ = -99;
   t2_decay_ = -99;
   tt_mass_  = -99;
   tt_pt_    = -99;

   top1_pt_  = -99;
   top1_eta_ = -99;
   top2_pt_  = -99;
   top2_eta_ = -99;

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

   auto gen_particles = get_collection(event, gen_token_);

   std::vector<reco::GenParticle> tops;
   for (const auto& particle: *gen_particles) {
      if (abs(particle.pdgId()) == 6 && particle.status() == 62) {
         tops.push_back(particle);
      }
   }

   switch (tops.size()) {
      case 2:
         top2_pt_  = tops[1].pt();
         top2_eta_ = tops[1].eta();
      case 1:
         top1_pt_  = tops[0].pt();
         top1_eta_ = tops[0].eta();
      default:
         break;
   }

   for (const auto& particle: *gen_particles) {
      if (particle.pdgId() != 25)
         continue;

      if (particle.numberOfDaughters() != 2)
         continue;

      auto p1 = particle.daughter(0);
      auto p2 = particle.daughter(1);

      if (abs(p1->pdgId()) != 15 or abs(p2->pdgId()) != 15)
         continue;

      t1_pt_  = p1->pt();
      t1_eta_ = p1->eta();
      t2_pt_  = p2->pt();
      t2_eta_ = p2->eta();

      tt_mass_ = (p1->p4() + p2->p4()).M();
      tt_pt_   = (p1->p4() + p2->p4()).pt();

      for (int n = 0; n < 5; ++n) {
         unsigned int i;
         for (i = 0; i < p1->numberOfDaughters(); ++i) {
            if (abs(p1->daughter(i)->pdgId()) == 11) {
               t1_decay_ = 11;
               break;
            } else if (abs(p1->daughter(i)->pdgId()) == 13) {
               t1_decay_ = 13;
               break;
            } else if (abs(p1->daughter(i)->pdgId()) == 15) {
               t1_decay_ = -15;
               break;
            }
         }

         if (t1_decay_ == -15) {
            t1_decay_ = -99;
            p1 = p1->daughter(i);
            if (!p1) {
               t1_decay_ = 100;
               break;
            }
         } else {
            break;
         }
      }
      if (t1_decay_ < 0)
         t1_decay_ = 0;

      for (int n = 0; n < 5; ++n) {
         unsigned int i;
         for (i = 0; i < p2->numberOfDaughters(); ++i) {
            if (abs(p2->daughter(i)->pdgId()) == 11) {
               t2_decay_ = 11;
               break;
            } else if (abs(p2->daughter(i)->pdgId()) == 13) {
               t2_decay_ = 13;
               break;
            } else if (abs(p2->daughter(i)->pdgId()) == 15) {
               t2_decay_ = -15;
               break;
            }
         }

         if (t2_decay_ == -15) {
            t2_decay_ = -99;
            p2 = p2->daughter(i);
            if (!p2) {
               t2_decay_ = 100;
               break;
            }
         } else {
            break;
         }
      }
      if (t2_decay_ < 0)
         t2_decay_ = 0;

      break;
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
