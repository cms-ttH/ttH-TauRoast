// vim: ts=4:sw=4:et:sta
#ifndef __TTL_MVABase_h
#define __TTL_MVABase_h

#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

#include "MVABase.h"
#include "TTLBranches.h"

namespace roast {
    namespace ttl {
        class MVABase : public roast::MVABase {
            public:
                MVABase(const std::string&, const std::vector<std::string>&, const int rnk=1);

                virtual void FillVariables(roast::Branches*, const int);

            protected:
                virtual roast::Branches* GetBranches(const Process*);
                virtual void SetupVariables(TMVA::Factory*);
                virtual void SetupVariables(TMVA::Reader*);
                virtual void SetupVariables(TTree*);
                template<typename T> void SetupVariablesT(T*);

                float csr;
                float HT;
                float Tau1Pt;
                float Tau1Eta;
                float Tau2Pt;
                float Tau1DecayMode;
                float Tau2DecayMode;
                float Tau1IsolationIndex;
                float Tau1IsolationIndexMVA2;
                float Tau1IsolationMVA2Raw;
                float Tau2IsolationIndex;
                float Tau2IsolationIndexMVA2;
                float Tau2IsolationMVA2Raw;
                float Tau1LTPt;
                float Tau2LTPt;
                float Tau1NProngs;
                float Tau2NProngs;
                float DitauVisibleMass;
                float DeltaRTau1Tau2;
                float DeltaRTau1Lepton;
                float DeltaRTau2Lepton;
                float DeltaRTau1LeadingJet;
                float DeltaRTau2LeadingJet;
                float DeltaRTau1SubleadingJet;
                float DeltaRTau2SubleadingJet;
                float DeltaRLeptonLeadingJet;
                float DeltaRLeptonSubleadingJet;
                float LeadingJetSubleadingJetMass;
                float LeadingJetPt;
                float SubLeadingJetPt;

                ClassDef(MVABase, 1);
        };
    }
}

#endif
