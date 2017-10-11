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
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "TTree.h"

int
get_iso(const pat::Tau& t, const std::string& s)
{
   if (t.tauID("byTight" + s) > .5)
      return 3;
   else if (t.tauID("byMedium" + s) > .5)
      return 2;
   else if (t.tauID("byLoose" + s) > .5)
      return 1;
   else
      return 0;
}

int
get_mva(const pat::Tau& t, const std::string& s)
{
   if (t.tauID("byVVTight" + s) > .5)
      return 6;
   else if (t.tauID("byVTight" + s) > .5)
      return 5;
   else if (t.tauID("byTight" + s) > .5)
      return 4;
   else if (t.tauID("byMedium" + s) > .5)
      return 3;
   else if (t.tauID("byLoose" + s) > .5)
      return 2;
   else if (t.tauID("byVLoose" + s) > .5)
      return 1;
   else
      return 0;
}

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
   auto id = c->pdgId();

   const reco::Candidate *radstate = 0;
   for (unsigned int i = 0; i < c->numberOfDaughters(); ++i) {
      if (c->daughter(i) and c->daughter(i)->pdgId() == id) {
         radstate = c->daughter(i);
         break;
      }
   }

   if (radstate)
      return get_final(radstate);

   return c;
}

const reco::Candidate *
get_mother(const reco::Candidate * c)
{
   auto id = c->pdgId();

   const reco::Candidate *radstate = 0;
   for (unsigned int i = 0; i < c->numberOfMothers(); ++i) {
      if (c->mother(i) and c->mother(i)->pdgId() == id) {
         radstate = c->mother(i);
         break;
      }
   }

   if (radstate)
      return get_mother(radstate);

   if (c->numberOfMothers() > 0 and c->mother(0))
      return c->mother(0);

   return 0;
}

std::pair<int, const reco::GenParticle*>
get_mc_match(const reco::Candidate& t, const reco::GenParticleCollection& coll)
{
   static const auto veto = {11, 12, 13, 14, 16};
   typedef std::pair<double, std::pair<int, const reco::GenParticle*>> Match;
   std::vector<Match> cands;
   for (auto& p: coll) {
      reco::Candidate::LorentzVector p4;
      if (abs(p.pdgId()) == 15) {
         for (unsigned int i = 0; i < p.numberOfDaughters(); ++i) {
            auto d = get_final(p.daughter(i));
            if (std::find(veto.begin(), veto.end(), abs(d->pdgId())) != veto.end())
               continue;
            p4 += d->p4();
         }
      } else {
         p4 = p.p4();
      }
      auto dR = deltaR(p4, t.p4());
      if (dR > 0.3)
         continue;

      auto cat = 6;
      if (abs(p.pdgId()) == 11 and p.statusFlags().isPrompt())
         cat = 1;
      else if (abs(p.pdgId()) == 13 and p.isPromptFinalState())
         cat = 2;
      else if (abs(p.pdgId()) == 11 and p.statusFlags().isDirectPromptTauDecayProduct())
         cat = 3;
      else if (abs(p.pdgId()) == 13 and p.isDirectPromptTauDecayProductFinalState())
         cat = 4;
      else if (abs(p.pdgId()) == 15)
         cat = 5;
      else
         continue;

      cands.push_back(std::make_pair(dR, std::make_pair(cat, &p)));
   }
   auto min = std::min_element(cands.begin(), cands.end(), [](const Match& a, const Match& b) { return a.first < b.first; });
   if (min != cands.end() and min->first < 0.5)
      return min->second;
   return std::make_pair<int, const reco::GenParticle*>(-99, 0);
}

class TreeDaughter {
   public:
      TreeDaughter(const std::string& prefix, TTree* t);
      void fill(const reco::Candidate *p, const reco::GenParticleCollection& particles, const reco::GenJetCollection& jets, const pat::TauCollection& taus);

   private:
      float id_;
      float pt_;
      float eta_;
      float phi_;
      float decay_;
      float vispt_;
      float viseta_;
      float recopt_;
      float recoeta_;
      float id03iso_;
      float id05iso_;
      float id03mva_;
      float id05mva_;
      float dR_nearest_;
      float dR_hardest_;
      float dR_gen_;
      float dR_manual_;
      float dR_reco_;
      float dR_reco_best_;
      float mum_;
};

class TauGeneratorValidation : public edm::EDAnalyzer {
   public:
      explicit TauGeneratorValidation(const edm::ParameterSet&);
      ~TauGeneratorValidation();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      edm::EDGetTokenT<reco::GenParticleCollection> genparticle_token_;
      edm::EDGetTokenT<reco::GenJetCollection> genjet_token_;
      edm::EDGetTokenT<GenEventInfoProduct> geninfo_token_;
      edm::EDGetTokenT<pat::JetCollection> jet_token_;
      edm::EDGetTokenT<pat::TauCollection> tau_token_;

      TTree *tree_;

      int njets_;
      int njets30_;

      float tt_mass_;
      float tt_pt_;

      TTree *trecojet_;

      float recojpt_;

      TTree *trecotau_;

      float recotpt_;
      int recotid05iso_;
      int recotid05mva_;
      int recotid03iso_;
      int recotid03mva_;
      int recotmatch_;

      TTree *tjet_;

      float jpt_;
      float jeta_;
      float jphi_;

      float w_;

      TTree *tdecays_;

      std::auto_ptr<TreeDaughter> d1_;
      std::auto_ptr<TreeDaughter> d2_;

      float m_id_;
      float m_pt_;
      float m_eta_;
      float m_phi_;

      std::vector<int> potential_mothers_ = {6, 23, 24, 25};

      TTree *taus_;

      std::auto_ptr<TreeDaughter> tau_;
};

TreeDaughter::TreeDaughter(const std::string& prefix, TTree* t)
{
   t->Branch((prefix + "_id").c_str(), &id_);
   t->Branch((prefix + "_pt").c_str(), &pt_);
   t->Branch((prefix + "_eta").c_str(), &eta_);
   t->Branch((prefix + "_phi").c_str(), &phi_);
   t->Branch((prefix + "_decay").c_str(), &decay_);
   t->Branch((prefix + "_visible_pt").c_str(), &vispt_);
   t->Branch((prefix + "_visible_eta").c_str(), &viseta_);
   t->Branch((prefix + "_reco_pt").c_str(), &recopt_);
   t->Branch((prefix + "_reco_eta").c_str(), &recoeta_);
   t->Branch((prefix + "_iso03").c_str(), &id03iso_);
   t->Branch((prefix + "_iso05").c_str(), &id05iso_);
   t->Branch((prefix + "_mva03").c_str(), &id03mva_);
   t->Branch((prefix + "_mva05").c_str(), &id05mva_);
   t->Branch((prefix + "_dR_nearest").c_str(), &dR_nearest_);
   t->Branch((prefix + "_dR_hardest").c_str(), &dR_hardest_);
   t->Branch((prefix + "_dR_gen").c_str(), &dR_gen_);
   t->Branch((prefix + "_dR_reco").c_str(), &dR_reco_);
   t->Branch((prefix + "_dR_reco_best").c_str(), &dR_reco_best_);
   t->Branch((prefix + "_mum").c_str(), &mum_);
}

void
TreeDaughter::fill(const reco::Candidate* p, const reco::GenParticleCollection& particles, const reco::GenJetCollection& jets, const pat::TauCollection& taus)
{
   id_ = p->pdgId();
   pt_ = p->pt();
   eta_ = p->eta();
   phi_ = p->phi();

   decay_ = -99;
   vispt_ = -99;
   viseta_ = -99;
   recopt_ = -99;
   recoeta_ = -99;

   id03iso_ = -99;
   id05iso_ = -99;
   id03mva_ = -99;
   id05mva_ = -99;

   dR_hardest_ = -99;
   dR_nearest_ = -99;
   dR_gen_ = -99;
   dR_reco_ = -99;
   dR_reco_best_ = -99;
   mum_ = -99;

   auto m = get_mother(p);
   if (m)
      mum_ = m->pdgId();

   if (abs(id_) == 15) {
      reco::Candidate::LorentzVector p4;
      std::vector<const reco::Candidate*> components;
      for (unsigned int i = 0; i < p->numberOfDaughters(); ++i) {
         auto gd = get_final(p->daughter(i));
         if (abs(gd->pdgId()) % 2 == 0 and abs(gd->pdgId()) > 10 and abs(gd->pdgId()) < 20)
            continue;

         components.push_back(gd);
         p4 += gd->p4();
      }

      vispt_ = p4.pt();
      viseta_ = p4.eta();
      decay_ = components[0]->pdgId();

      if (abs(decay_) == 11 or abs(decay_) == 13)
         return;

      for (const auto& j: jets) {
         auto dR = deltaR(p4, j.p4());

         if (dR_hardest_ < 0.)
            dR_hardest_ = dR;
         if ((dR_nearest_ < 0. or dR < dR_nearest_) and dR > .1)
            dR_nearest_ = dR;
      }

      for (const auto& t: taus) {
         auto match = get_mc_match(t, particles);
         if (match.second == p and match.first == 5) {
            if (t.tauID("decayModeFinding") < .5)
               continue;

            recopt_ = t.pt();
            recoeta_ = t.eta();
            dR_gen_ = deltaR(p->p4(), t.p4());
            dR_reco_ = deltaR(p4, t.p4());

            for (unsigned int i = 0; i < p->numberOfDaughters(); ++i) {
               auto gd = get_final(p->daughter(i));
               if (abs(gd->pdgId()) % 2 == 0 and abs(gd->pdgId()) > 10 and abs(gd->pdgId()) < 20)
                  continue;
               auto dR = deltaR(gd->p4(), t.p4());
               if (dR_reco_best_ < 0 or dR < dR_reco_best_)
                  dR_reco_best_ = dR;
            }

            // id03iso_ = get_iso(t, "CombinedIsolationDeltaBetaCorr3HitsdR03");
            id05iso_ = get_iso(t, "CombinedIsolationDeltaBetaCorr3Hits");
            id03mva_ = get_mva(t, "IsolationMVArun2v1DBdR03oldDMwLT");
            id05mva_ = get_mva(t, "IsolationMVArun2v1DBoldDMwLT");

            break;
         }
      }
   }
}

TauGeneratorValidation::TauGeneratorValidation(const edm::ParameterSet& config)
{
   // genparticle_token_ = consumes<reco::GenParticleCollection>(edm::InputTag("genParticles"));
   // genjet_token_ = consumes<reco::GenJetCollection>(edm::InputTag("ak5GenJets"));
   geninfo_token_ = consumes<GenEventInfoProduct>(edm::InputTag("generator"));
   genjet_token_ = consumes<reco::GenJetCollection>(edm::InputTag("slimmedGenJets"));
   genparticle_token_ = consumes<reco::GenParticleCollection>(edm::InputTag("prunedGenParticles"));
   jet_token_ = consumes<pat::JetCollection>(edm::InputTag("slimmedJets"));
   tau_token_ = consumes<pat::TauCollection>(edm::InputTag("slimmedTaus"));

   edm::Service<TFileService> fs;
   tree_ = fs->make<TTree>("events", "Event data");
   tree_->Branch("njets", &njets_);
   tree_->Branch("njets30", &njets30_);

   tree_->Branch("ditaumass", &tt_mass_);
   tree_->Branch("ditaupt", &tt_pt_);

   tree_->Branch("w", &w_);

   trecojet_ = fs->make<TTree>("recojets", "RECO jet data");
   trecojet_->Branch("pt", &recojpt_);

   trecotau_ = fs->make<TTree>("recotaus", "RECO tau data");
   trecotau_->Branch("pt", &recotpt_);
   trecotau_->Branch("iso03", &recotid03iso_);
   trecotau_->Branch("iso05", &recotid05iso_);
   trecotau_->Branch("mva03", &recotid03mva_);
   trecotau_->Branch("mva05", &recotid05mva_);
   trecotau_->Branch("match", &recotmatch_);

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
   tdecays_->Branch("w", &w_);

   d1_ = std::auto_ptr<TreeDaughter>(new TreeDaughter("daughter1", tdecays_));
   d2_ = std::auto_ptr<TreeDaughter>(new TreeDaughter("daughter2", tdecays_));

   taus_ = fs->make<TTree>("taus", "Tau decays");
   taus_->Branch("w", &w_);
   tau_ = std::auto_ptr<TreeDaughter>(new TreeDaughter("t", taus_));
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
   auto gen_jets = get_collection(event, genjet_token_);
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

   auto jets = get_collection(event, jet_token_);
   auto taus = get_collection(event, tau_token_);
   auto gen_particles = get_collection(event, genparticle_token_);

   for (const auto& j: *jets) {
      if (j.pt() < 20 or std::abs(j.eta()) > 2.3)
         continue;
      recojpt_ = j.pt();
      trecojet_->Fill();
   }

   for (const auto& t: *taus) {
      if (t.pt() < 20 or std::abs(t.eta()) > 2.3 or t.tauID("decayModeFinding") < 0.5)
         continue;
      recotpt_ = t.pt();
      // recotid03iso_ = get_iso(t, "CombinedIsolationDeltaBetaCorr3HitsdR03");
      recotid05iso_ = get_iso(t, "CombinedIsolationDeltaBetaCorr3Hits");
      recotid03mva_ = get_mva(t, "IsolationMVArun2v1DBdR03oldDMwLT");
      recotid05mva_ = get_mva(t, "IsolationMVArun2v1DBoldDMwLT");
      recotmatch_ = get_mc_match(t, *gen_particles).first;
      trecotau_->Fill();
   }

   for (const auto& particle: *gen_particles) {
      if (std::find(potential_mothers_.begin(), potential_mothers_.end(), abs(particle.pdgId())) == potential_mothers_.end())
         continue;

      if (particle.numberOfDaughters() != 2 or !particle.daughter(0) or particle.daughter(0)->pdgId() == particle.pdgId())
         continue;

      m_id_ = particle.pdgId();
      m_pt_ = particle.pt();
      m_eta_ = particle.eta();
      m_phi_ = particle.phi();

      auto d1 = get_final(particle.daughter(0));
      auto d2 = get_final(particle.daughter(1));

      d1_->fill(d1, *gen_particles, *gen_jets, *taus);
      d2_->fill(d2, *gen_particles, *gen_jets, *taus);

      tdecays_->Fill();
   }

   for (const auto& particle: *gen_particles) {
      if (abs(particle.pdgId()) != 15)
         continue;

      auto fnl = get_final(&particle);

      if (fnl != &particle)
         continue;

      tau_->fill(fnl, *gen_particles, *gen_jets, *taus);
      taus_->Fill();
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
