// -*- C++ -*-
//
// Package:    ttH/TauRoast
// Class:      GenLeptonFilter
// 
/**\class TauRoast GenLeptonFilter.cc ttH/TauRoast/plugins/GenLeptonFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Matthias Wolf
//         Created:  Thu Sep 15 05:37:55 EDT 2016
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

class GenLeptonFilter : public edm::EDFilter {
   public:
      explicit GenLeptonFilter(const edm::ParameterSet&);
      ~GenLeptonFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<reco::GenParticleCollection> gen_token_;
      double pt_;
      double eta_;
      int id_;
      int parent_id_;
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
GenLeptonFilter::GenLeptonFilter(const edm::ParameterSet& config) :
   pt_(config.getParameter<double>("minPt")),
   eta_(config.getParameter<double>("maxEta")),
   id_(config.getParameter<int>("pdgId")),
   parent_id_(config.getParameter<int>("parentPdgId"))
{
   gen_token_ = consumes<reco::GenParticleCollection>(config.getParameter<edm::InputTag>("genTag"));
}


GenLeptonFilter::~GenLeptonFilter()
{
}


//
// member functions
//

template<typename T> const T*
mother(const T& p)
{
   for (unsigned int i = 0; i < p.numberOfMothers(); ++i) {
      auto mother = dynamic_cast<const T*>(p.mother(i));

      while (mother and mother->pdgId() == p.pdgId()) {
         if (mother->numberOfMothers() > 0) {
            mother = dynamic_cast<const T*>(mother->mother(0));
         } else {
            mother = 0;
            break;
         }
      }

      if (mother)
         return mother;
   }
   return 0;
}

// ------------ method called on each new Event  ------------
bool
GenLeptonFilter::filter(edm::Event& event, const edm::EventSetup& setup)
{
   edm::Handle<reco::GenParticleCollection> particles;
   event.getByToken(gen_token_, particles);

   for (const auto& p: *particles) {
      if (abs(p.pdgId()) != id_)
         continue;

      if (not (p.pt() > pt_ and abs(p.eta()) < eta_))
         continue;

      auto m = mother(p);

      if (!m)
         continue;

      if (std::abs(m->pdgId()) == parent_id_)
         return true;
   }

   return false;
}

// ------------ method called once each job just before starting event loop  ------------
void 
GenLeptonFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GenLeptonFilter::endJob() {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenLeptonFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(GenLeptonFilter);
