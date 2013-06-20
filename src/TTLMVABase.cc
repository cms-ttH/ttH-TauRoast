/*
    Author:			Nil Valls <nil.valls@cern.ch>, Matthias Wolf <matthias.wolf@cern.ch>
    Date:			11 Jun 2011
    Description:	TMVA evaluating class
    Notes:

*/

#include "../interface/Helper.h"
#include "../interface/TTLMVABase.h"

using namespace std;

namespace roast {
   ttl::MVABase::MVABase(const std::string& dir, const std::vector<std::string>& vars, const int rnk) :
       roast::MVABase(dir, vars, rnk)
   {
   }

   void
   ttl::MVABase::SetupVariables(TMVA::Factory* f)
   {
       SetupVariablesT(f);
   }

   void
   ttl::MVABase::SetupVariables(TMVA::Reader* r)
   {
       SetupVariablesT(r);
   }

   void
   ttl::MVABase::SetupVariables(TTree* t)
   {
       SetupVariablesT(t);
   }

   template<typename T> void
   ttl::MVABase::SetupVariablesT(T* obj)
   {
       AddVariableConditionally(obj, "CSR", 'F', csr);
       AddVariableConditionally(obj, "HT", 'F', HT);
       AddVariableConditionally(obj, "Tau1Pt", 'F', Tau1Pt);
       AddVariableConditionally(obj, "Tau1Eta", 'F', Tau1Eta);
       AddVariableConditionally(obj, "Tau2Pt", 'F', Tau2Pt);
       AddVariableConditionally(obj, "Tau1DecayMode", 'I', Tau1DecayMode);
       AddVariableConditionally(obj, "Tau2DecayMode", 'I', Tau2DecayMode);
       AddVariableConditionally(obj, "Tau1IsolationIndex", 'I', Tau1IsolationIndex);
       AddVariableConditionally(obj, "Tau1IsolationIndexMVA2", 'I', Tau1IsolationIndexMVA2);
       AddVariableConditionally(obj, "Tau1IsolationMVA2Raw", 'F', Tau1IsolationMVA2Raw);
       AddVariableConditionally(obj, "Tau2IsolationIndex", 'I', Tau2IsolationIndex);
       AddVariableConditionally(obj, "Tau2IsolationIndexMVA2", 'I', Tau2IsolationIndexMVA2);
       AddVariableConditionally(obj, "Tau2IsolationMVA2Raw", 'F', Tau2IsolationMVA2Raw);
       AddVariableConditionally(obj, "DeltaRTau1Tau2", 'F', DeltaRTau1Tau2);
       AddVariableConditionally(obj, "DeltaRTau1Lepton", 'F', DeltaRTau1Lepton);
       AddVariableConditionally(obj, "DeltaRTau2Lepton", 'F', DeltaRTau2Lepton);
       AddVariableConditionally(obj, "DeltaRTau1LeadingJet", 'F', DeltaRTau1LeadingJet);
       AddVariableConditionally(obj, "DeltaRTau2LeadingJet", 'F', DeltaRTau2LeadingJet);
       AddVariableConditionally(obj, "DeltaRTau1SubleadingJet", 'F', DeltaRTau1SubleadingJet);
       AddVariableConditionally(obj, "DeltaRTau2SubleadingJet", 'F', DeltaRTau2SubleadingJet);
       AddVariableConditionally(obj, "DeltaRLeptonLeadingJet", 'F', DeltaRLeptonLeadingJet);
       AddVariableConditionally(obj, "DeltaRLeptonSubleadingJet", 'F', DeltaRLeptonSubleadingJet);
       AddVariableConditionally(obj, "LeadingJetSubleadingJetMass", 'F', LeadingJetSubleadingJetMass);
       AddVariableConditionally(obj, "Tau1LTPt", 'F', Tau1LTPt);
       AddVariableConditionally(obj, "Tau2LTPt", 'F', Tau2LTPt);
       AddVariableConditionally(obj, "Tau1NProngs", 'I', Tau1NProngs);
       AddVariableConditionally(obj, "Tau2NProngs", 'I', Tau2NProngs);
       AddVariableConditionally(obj, "LeadingJetPt", 'F', LeadingJetPt);
       AddVariableConditionally(obj, "SubLeadingJetPt", 'F', SubLeadingJetPt);
       AddVariableConditionally(obj, "DitauVisibleMass", 'F', DitauVisibleMass);
   }

   void
   ttl::MVABase::FillVariables(Branches *branches, const int combo)
   {
      roast::ttl::Branches *event = dynamic_cast<roast::ttl::Branches*>(branches);

       if (rank > 0 && MVABase::gComboMVA["BDT"]) {
           csr = MVABase::gComboMVA["BDT"]->Evaluate(event, combo);
       } else {
           csr = 0.;
       }

       HT = (*event->TTL_HT)[combo];
       Tau1Pt = (*event->TTL_Tau1Pt)[combo];
       Tau1Eta = abs((*event->TTL_Tau1Eta)[combo]);
       Tau2Pt = (*event->TTL_Tau2Pt)[combo];
       Tau1DecayMode = (*event->TTL_Tau1DecayMode)[combo];
       Tau2DecayMode = (*event->TTL_Tau2DecayMode)[combo];
       Tau1IsolationIndex = event->GetTau1IsolationIndex(combo);
       Tau1IsolationIndexMVA2 = event->GetTau1IsolationIndexMVA2(combo);
       Tau1IsolationMVA2Raw = (*event->TTL_Tau1HPSbyIsolationMVA2raw)[combo];
       Tau2IsolationIndex = event->GetTau2IsolationIndex(combo);
       Tau2IsolationIndexMVA2 = event->GetTau2IsolationIndexMVA2(combo);
       Tau2IsolationMVA2Raw = (*event->TTL_Tau2HPSbyIsolationMVA2raw)[combo];
       Tau1LTPt = (*event->TTL_Tau1LTPt)[combo];
       Tau2LTPt = (*event->TTL_Tau2LTPt)[combo];
       Tau1NProngs = (*event->TTL_Tau1NProngs)[combo];
       Tau2NProngs = (*event->TTL_Tau2NProngs)[combo];
       DitauVisibleMass = (*event->TTL_DitauVisibleMass)[combo];

       // Leading and subleading jets are not always present!
       LeadingJetPt = 0.;
       SubLeadingJetPt = 0.;
       try {
           LeadingJetPt = event->J_Pt->at(event->GetJetIndex(combo, 0));
           SubLeadingJetPt = event->J_Pt->at(event->GetJetIndex(combo, 1));
       } catch (...) {
           // std::cerr << combo << "\t" << event->J_Pt->size() << endl;
       }

       float tau1eta	= (*event->TTL_Tau1Eta)[combo];
       float tau1phi	= (*event->TTL_Tau1Phi)[combo];
       float tau2eta	= (*event->TTL_Tau2Eta)[combo];
       float tau2phi	= (*event->TTL_Tau2Phi)[combo];
       float lepEta	= (*event->TTL_LeptonEta)[combo];
       float lepPhi	= (*event->TTL_LeptonPhi)[combo];

       DeltaRTau1Tau2 = DeltaR(tau1eta, tau1phi, tau2eta, tau2phi);
       DeltaRTau1Lepton = DeltaR(tau1eta, tau1phi, lepEta, lepPhi);
       DeltaRTau2Lepton = DeltaR(tau2eta, tau2phi, lepEta, lepPhi);

       DeltaRTau1LeadingJet		= -99;
       DeltaRTau2LeadingJet		= -99;
       DeltaRTau1SubleadingJet		= -99;
       DeltaRTau2SubleadingJet		= -99;
       DeltaRLeptonLeadingJet		= -99;
       DeltaRLeptonSubleadingJet	= -99;
       LeadingJetSubleadingJetMass	= -99;

       bool foundLeadingJet = false;
       for(unsigned int j = 0; j < event->J_Pt->size(); j++){
           float pt	= (*event->J_Pt)[j];
           float eta	= (*event->J_Eta)[j];
           float phi	= (*event->J_Phi)[j];
           TLorentzVector leadingJet(0,0,0,0); TLorentzVector subleadingJet(0,0,0,0);
           if(DeltaR(eta, phi, tau1eta, tau1phi) < 0.25 || DeltaR(eta, phi, tau2eta, tau2phi) < 0.25 || DeltaR(eta, phi, lepEta, lepPhi) < 0.25){ continue; }
           if(!foundLeadingJet){ // Fill leading jet
               DeltaRTau1LeadingJet		= DeltaR(eta, phi, tau1eta, tau1phi);
               DeltaRTau2LeadingJet		= DeltaR(eta, phi, tau2eta, tau2phi);
               DeltaRLeptonLeadingJet		= DeltaR(eta, phi, lepEta, lepPhi);
               leadingJet.SetPtEtaPhiM(pt, eta, phi, 0);
               foundLeadingJet = true;
           }else{ // Fill subleading jet
               DeltaRTau1SubleadingJet		= DeltaR(eta, phi, tau1eta, tau1phi);
               DeltaRTau2SubleadingJet		= DeltaR(eta, phi, tau2eta, tau2phi);
               DeltaRLeptonSubleadingJet	= DeltaR(eta, phi, lepEta, lepPhi);
               subleadingJet.SetPtEtaPhiM(pt, eta, phi, 0);
               LeadingJetSubleadingJetMass = (leadingJet + subleadingJet).M();
               break;
           }
       }
   }

   roast::Branches*
   ttl::MVABase::GetBranches(const Process *proc)
   {
       return new roast::ttl::Branches(proc->GetTreeName(), proc->GetNtuplePaths());
   }
}

ClassImp(roast::ttl::MVABase)
