// -*- C++ -*-
//
// Package:    ttH/TauRoast
// Class:      GenJetFilter
// 
/**\class TauRoast GenJetFilter.cc ttH/TauRoast/plugins/GenJetFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Matthias Wolf
//         Created:  Thu Sep 15 05:28:21 EDT 2016
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

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

//
// class declaration
//

class GenJetFilter : public edm::EDFilter {
   public:
      explicit GenJetFilter(const edm::ParameterSet&);
      ~GenJetFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<reco::GenJetCollection> gen_token_;

      int pt_;
      int eta_;
      int n_;
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
GenJetFilter::GenJetFilter(const edm::ParameterSet& config) :
   pt_(config.getParameter<double>("minPt")),
   eta_(config.getParameter<double>("maxEta")),
   n_(config.getParameter<int>("nJets"))
{
   gen_token_ = consumes<reco::GenJetCollection>(config.getParameter<edm::InputTag>("genTag"));
}


GenJetFilter::~GenJetFilter()
{
}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
GenJetFilter::filter(edm::Event& event, const edm::EventSetup& setup)
{
   edm::Handle<reco::GenJetCollection> jets;
   event.getByToken(gen_token_, jets);

   return n_ <= std::count_if(std::begin(*jets), std::end(*jets),
         [&](const auto& j) { return j.pt() > pt_ and abs(j.eta()) < eta_; });
}

// ------------ method called once each job just before starting event loop  ------------
void 
GenJetFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GenJetFilter::endJob() {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenJetFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(GenJetFilter);
