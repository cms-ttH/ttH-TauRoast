// -*- C++ -*-
//
// Package:    ttH/TauRoast
// Class:      TauSelector
//
/**\class TauSelector TauSelector.cc ttH/TauRoast/plugins/TauSelector.cc

 Description: Writes out τ events

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Matthias Wolf
//         Created:  Fri, 13 May 2016 11:27:18 GMT
//
//


// system include files
#include <memory>

// user include files
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "ttH/TauRoast/interface/SuperSlim.h"

class TauSelector : public edm::one::EDFilter<> {
   public:
      explicit TauSelector(const edm::ParameterSet&);
      ~TauSelector();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      edm::EDGetTokenT<superslim::Event> etoken_;
};

TauSelector::TauSelector(const edm::ParameterSet& config)
{
   etoken_ = consumes<superslim::Event>(config.getParameter<edm::InputTag>("input"));
}


TauSelector::~TauSelector()
{
}


bool
TauSelector::filter(edm::Event& event, const edm::EventSetup& setup)
{
   edm::Handle<superslim::Event> e;
   event.getByToken(etoken_, e);
   return e.isValid();
}

void
TauSelector::beginJob()
{
}

void
TauSelector::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TauSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TauSelector);
