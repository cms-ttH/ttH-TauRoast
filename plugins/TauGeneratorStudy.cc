// -*- C++ -*-
//
// Package:    ttH/TauGeneratorStudy
// Class:      TauGeneratorStudy
// 
/**\class TauGeneratorStudy TauGeneratorStudy.cc ttH/TauRoast/plugins/TauGeneratorStudy.cc

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

#include "DataFormats/PatCandidates/interface/GenericParticle.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

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

class TauGeneratorStudy : public edm::EDAnalyzer {
   public:
      explicit TauGeneratorStudy(const edm::ParameterSet&);
      ~TauGeneratorStudy();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      edm::EDGetTokenT<reco::GenParticleCollection> gen_token_;
      edm::EDGetTokenT<pat::TauCollection> taus_token_;

      TTree *tree_;

      float gt1_pt_;
      float gt1_eta_;
      float gt2_pt_;
      float gt2_eta_;

      float rt1_pt_;
      float rt1_eta_;
      float rt2_pt_;
      float rt2_eta_;

      float rgt1_pt_;
      float rgt1_eta_;
      float rgt2_pt_;
      float rgt2_eta_;
};

TauGeneratorStudy::TauGeneratorStudy(const edm::ParameterSet& config)
{
   gen_token_ = consumes<reco::GenParticleCollection>(edm::InputTag("prunedGenParticles"));
   taus_token_ = consumes<pat::TauCollection>(edm::InputTag("slimmedTaus"));

   edm::Service<TFileService> fs;
   tree_ = fs->make<TTree>("events", "Event data");
   tree_->Branch("gentau1pt", &gt1_pt_);
   tree_->Branch("gentau1eta", &gt1_eta_);
   tree_->Branch("gentau2pt", &gt2_pt_);
   tree_->Branch("gentau2eta", &gt2_eta_);

   tree_->Branch("recotau1pt", &rt1_pt_);
   tree_->Branch("recotau1eta", &rt1_eta_);
   tree_->Branch("recotau2pt", &rt2_pt_);
   tree_->Branch("recotau2eta", &rt2_eta_);

   tree_->Branch("recogentau1pt", &rgt1_pt_);
   tree_->Branch("recogentau1eta", &rgt1_eta_);
   tree_->Branch("recogentau2pt", &rgt2_pt_);
   tree_->Branch("recogentau2eta", &rgt2_eta_);
}


TauGeneratorStudy::~TauGeneratorStudy()
{
}


void
TauGeneratorStudy::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
   using namespace edm;

   bool good_event = false;

   auto gen_particles = get_collection(event, gen_token_);
   for (const auto& particle: *gen_particles) {
      if (particle.pdgId() != 25)
         continue;

      if (particle.numberOfDaughters() != 2)
         continue;

      auto p1 = particle.daughter(0);
      auto p2 = particle.daughter(1);

      if (abs(p1->pdgId()) != 15 or abs(p2->pdgId()) != 15)
         continue;

      gt1_pt_  = p1->pt();
      gt1_eta_ = p1->eta();
      gt2_pt_  = p2->pt();
      gt2_eta_ = p2->eta();

      good_event = true;
      break;
   }

   std::vector<pat::Tau> mytaus;
   auto taus = get_collection(event, taus_token_);
   for (const auto& tau: *taus) {
      auto gen = tau.genParticle();

      if (!gen)
         continue;

      if (abs(gen->pdgId()) == 15 and gen->numberOfMothers() == 1 and gen->mother(0)->pdgId() == 25)
         mytaus.push_back(tau);
   }

   rt1_pt_  = -99;
   rt1_eta_ = -99;
   rt2_pt_  = -99;
   rt2_eta_ = -99;

   rgt1_pt_  = -99;
   rgt1_eta_ = -99;
   rgt2_pt_  = -99;
   rgt2_eta_ = -99;

   switch (mytaus.size()) {
      case 2:
         rt2_pt_ = mytaus[1].pt();
         rt2_eta_ = mytaus[1].eta();
         rgt2_pt_ = mytaus[1].genParticle()->pt();
         rgt2_eta_ = mytaus[1].genParticle()->eta();
      case 1:
         rt1_pt_ = mytaus[0].pt();
         rt1_eta_ = mytaus[0].eta();
         rgt1_pt_ = mytaus[0].genParticle()->pt();
         rgt1_eta_ = mytaus[0].genParticle()->eta();
   }

   if (good_event)
      tree_->Fill();
}

void 
TauGeneratorStudy::beginJob()
{
}

void 
TauGeneratorStudy::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TauGeneratorStudy::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TauGeneratorStudy);
