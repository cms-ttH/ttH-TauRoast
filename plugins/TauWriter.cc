// -*- C++ -*-
//
// Package:    ttH/TauRoast
// Class:      TauWriter
//
/**\class TauWriter TauWriter.cc ttH/TauRoast/plugins/TauWriter.cc

 Description: Writes out τ events

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Matthias Wolf
//         Created:  Wed, 11 May 2016 14:21:32 GMT
//
//


// system include files
#include <memory>

// user include files
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TH1F.h"
#include "TTree.h"

#include "ttH/TauRoast/interface/SuperSlim.h"

class TauWriter : public edm::one::EDAnalyzer<edm::one::SharedResources> {
   public:
      explicit TauWriter(const edm::ParameterSet&);
      ~TauWriter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      edm::EDGetTokenT<superslim::Event> etoken_;
      edm::EDGetTokenT<TH1F> ctoken_;

      const superslim::Event *event_;

      TTree *tree_;
      TH1F *cuts_;
};

TauWriter::TauWriter(const edm::ParameterSet& config) :
   event_(0)
{
   etoken_ = consumes<superslim::Event>(config.getParameter<edm::InputTag>("input"));
   ctoken_ = consumes<TH1F>(config.getParameter<edm::InputTag>("input"));

   usesResource("TFileService");

   edm::Service<TFileService> fs;
   tree_ = fs->make<TTree>("events", "Event data");
   tree_->Branch("Event", "superslim::Event", &event_, 32000, 0);

   cuts_ = fs->make<TH1F>("cuts", "Cut counts", 64, -0.5, 63.5);
}


TauWriter::~TauWriter()
{
}


void
TauWriter::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
   edm::Handle<superslim::Event> e;
   edm::Handle<TH1F> c;

   event.getByToken(etoken_, e);
   if (e.isValid()) {
      event_ = e.product();
   } else {
      event_ = 0;
   }
   event.getByToken(ctoken_, c);
   if (!c.isValid()) {
      edm::EDConsumerBase::Labels l;
      this->labelsForToken(ctoken_, l);
      edm::LogError("TauWriter") << "Can't find cut histogram " << l.module;
   } else {
      cuts_->Add(c.product());
   }

   tree_->Fill();
}

void
TauWriter::beginJob()
{
}

void
TauWriter::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TauWriter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TauWriter);
