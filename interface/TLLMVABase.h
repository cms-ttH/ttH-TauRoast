// vim: ts=4:sw=4:et:sta
#ifndef __TLL_MVABase_h
#define __TLL_MVABase_h

#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

#include "MVABase.h"
#include "TLLBranches.h"

namespace roast {
    namespace tll {
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
                float TauPt;
                float TauEta;
                float TauDecayMode;
                float TauIsolationIndex;
                float TauIsolationIndexMVA2;
                float TauIsolationMVA2Raw;
                float TauLTPt;
                float TauNProngs;
                float DeltaRTauLepton1;
                float DeltaRTauLepton2;
                float DeltaRLepton1Lepton2;
                float DeltaRTauLeadingJet;
                float DeltaRTauSubleadingJet;
                float DeltaRLepton1LeadingJet;
                float DeltaRLepton1SubleadingJet;
                float DeltaRLepton2LeadingJet;
                float DeltaRLepton2SubleadingJet;
                float LeadingJetSubleadingJetMass;
                float LeadingJetPt;
                float SubLeadingJetPt;

                ClassDef(MVABase, 1);
        };
    }
}

#endif
