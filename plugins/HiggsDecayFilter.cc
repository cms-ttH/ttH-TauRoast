// -*- C++ -*-
//
// Package:    ttH/TauRoast
// Class:      HiggsDecayFilter
// 
/**\class TauRoast HiggsDecayFilter.cc ttH/TauRoast/plugins/HiggsDecayFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Matthias Wolf
//         Created:  Mon, 03 Aug 2015 11:24:06 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/GenericParticle.h"

//
// class declaration
//

class HiggsDecayFilter : public edm::EDFilter {
   public:
      explicit HiggsDecayFilter(const edm::ParameterSet&);
      ~HiggsDecayFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<reco::GenParticleCollection> gen_token_;
      std::vector<unsigned int> daughters_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HiggsDecayFilter::HiggsDecayFilter(const edm::ParameterSet& config) :
   daughters_(config.getParameter<std::vector<unsigned int>>("daughterIds"))
{
   gen_token_ = consumes<reco::GenParticleCollection>(edm::InputTag("prunedGenParticles"));
}


HiggsDecayFilter::~HiggsDecayFilter()
{
}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
HiggsDecayFilter::filter(edm::Event& event, const edm::EventSetup& setup)
{
   edm::Handle<reco::GenParticleCollection> particles;
   event.getByToken(gen_token_, particles);

   for (const auto& p: *particles) {
      if (abs(p.pdgId()) == 25) {
         for (unsigned int i = 0; i < p.numberOfDaughters(); ++i) {
            if (std::find(daughters_.begin(), daughters_.end(), abs(p.daughter(i)->pdgId())) != daughters_.end())
               return true;
         }
      }
   }

   return false;
}

// ------------ method called once each job just before starting event loop  ------------
void 
HiggsDecayFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HiggsDecayFilter::endJob() {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HiggsDecayFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(HiggsDecayFilter);
