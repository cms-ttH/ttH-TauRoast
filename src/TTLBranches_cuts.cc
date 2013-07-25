// vim: ts=4:sw=4:et:sta
#include "../interface/GenHelper.h"
#include "../interface/MVABase.h"
#include "../interface/TTLBranches.h"

namespace roast {
    namespace ttl {
        void
        Branches::RegisterCuts(CutFlow& c)
        {
            // if(ApplyThisCut("RunNumber"    )){ CutOn_RunNumber     =  true; c.RegisterCut("RunNumber", 1);     }
            // if(ApplyThisCut("LumiSection"    )){ CutOn_LumiSection    =  true; c.RegisterCut("LumiSection", 1);    }
            // if(ApplyThisCut("EventNumber"    )){ CutOn_EventNumber    =  true; c.RegisterCut("EventNumber", 1);    }
            // if(ApplyThisCut("GenMatching"    )){ CutOn_GenMatching    =  true; c.RegisterCut("GenMatching", 1);    }
            // if(ApplyThisCut("NumHadronicGenTaus"  )){ CutOn_NumHadronicGenTaus  =  true; c.RegisterCut("NumHadronicGenTaus", 1);   }
            //   if(selectedEvents.size() > 0){   c.RegisterCut("Run:LS:Event", 1);  }
            // if(ApplyThisCut("InvariantMass"   )){ CutOn_InvariantMass    =  true; c.RegisterCut("InvariantMass", 1);    }
            // if(ApplyThisCut("VisibleMass"    )){ CutOn_VisibleMass    =  true; c.RegisterCut("VisibleMass", 1);    }

            // if(ApplyThisCut("G_NumGenHadTausFromH"  )){ CutOn_G_NumGenHadTausFromH  =  true; c.RegisterCut("G_NumGenHadTausFromH", 1);  }

            c.RegisterCut("T1_MatchAbsId", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return abs((*e->TTL_Tau1GenMatchId)[idx]); }, true);
            c.RegisterCut("T2_MatchAbsId", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return abs((*e->TTL_Tau2GenMatchId)[idx]); }, true);
            c.RegisterCut("T1_ParentAbsId", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return abs((*e->TTL_Tau1GenMatchMother0Id)[idx]); }, true);
            c.RegisterCut("T2_ParentAbsId", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return abs((*e->TTL_Tau2GenMatchMother0Id)[idx]); }, true);

            c.RegisterCut("J_NumCSVL", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumCSVLbtagJets)[idx]; });
            c.RegisterCut("J_NumCSVM", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumCSVMbtagJets)[idx]; });
            c.RegisterCut("J_NumCSVT", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumCSVTbtagJets)[idx]; });
            c.RegisterCut("J_NumNonCSVL", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumNonCSVLbtagJets)[idx]; });
            c.RegisterCut("J_NumNonCSVM", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumNonCSVMbtagJets)[idx]; });
            c.RegisterCut("J_NumNonCSVT", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumNonCSVTbtagJets)[idx]; });
            c.RegisterCut("J_NumInclusive", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumCSVMbtagJets)[idx] + (*e->TTL_NumNonCSVMbtagJets)[idx]; });
            c.RegisterCut("J_NumCleanCSVL", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumCleanCSVLbtagJets)[idx]; });
            c.RegisterCut("J_NumCleanCSVM", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumCleanCSVMbtagJets)[idx]; });
            c.RegisterCut("J_NumCleanCSVT", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumCleanCSVTbtagJets)[idx]; });
            c.RegisterCut("J_NumCleanNonCSVL", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumCleanNonCSVLbtagJets)[idx]; });
            c.RegisterCut("J_NumCleanNonCSVM", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumCleanNonCSVMbtagJets)[idx]; });
            c.RegisterCut("J_NumCleanNonCSVT", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumCleanNonCSVTbtagJets)[idx]; });
            c.RegisterCut("J_NumCleanInclusive", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumCleanCSVMbtagJets)[idx] + (*e->TTL_NumCleanNonCSVMbtagJets)[idx]; });
            c.RegisterCut("NumTightLeptons", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumTightMuons)[idx] + (*e->TTL_NumTightElectrons)[idx]; });
            c.RegisterCut("NumTightMuons", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumTightMuons)[idx]; });
            c.RegisterCut("NumTightElectrons", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumTightElectrons)[idx]; });
            c.RegisterCut("NumLooseLeptons", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumLooseMuons)[idx] + (*e->TTL_NumLooseElectrons)[idx]; });
            c.RegisterCut("NumLooseMuons", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumLooseMuons)[idx]; });
            c.RegisterCut("NumLooseElectrons", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumLooseElectrons)[idx]; });
            c.RegisterCut("NumExLooseLeptons", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumExLooseMuons)[idx] + (*e->TTL_NumExLooseElectrons)[idx]; });
            c.RegisterCut("NumExLooseMuons", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumExLooseMuons)[idx]; });
            c.RegisterCut("NumExLooseElectrons", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_NumExLooseElectrons)[idx]; });

            c.RegisterCut("L_pT", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_LeptonPt)[idx]; });
            c.RegisterCut("L_Eta", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_LeptonEta)[idx]; });
            c.RegisterCut("L_IsLoose", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_LeptonIsLoose)[idx]; });
            c.RegisterCut("L_IsTight", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_LeptonIsTight)[idx]; });
            c.RegisterCut("L_RelIso", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_LeptonRelIso)[idx]; });

            c.RegisterCut("T1_pT", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau1Pt)[idx]; });
            c.RegisterCut("T2_pT", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau2Pt)[idx]; });
            c.RegisterCut("T1_Eta", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau1Eta)[idx]; });
            c.RegisterCut("T2_Eta", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau2Eta)[idx]; });
            c.RegisterCut("T1_InCracks", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau1IsInTheCracks)[idx]; });
            c.RegisterCut("T2_InCracks", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau2IsInTheCracks)[idx]; });

            c.RegisterCut("TT_DeltaR", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_DitauDeltaR)[idx]; });
            c.RegisterCut("T1L_DeltaR", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return DeltaR((*e->TTL_Tau1Eta)[idx], (*e->TTL_Tau1Phi)[idx],
                        (*e->TTL_LeptonEta)[idx], (*e->TTL_LeptonPhi)[idx]); });
            c.RegisterCut("T2L_DeltaR", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return DeltaR((*e->TTL_Tau2Eta)[idx], (*e->TTL_Tau2Phi)[idx],
                        (*e->TTL_LeptonEta)[idx], (*e->TTL_LeptonPhi)[idx]); });

            c.RegisterCut("T1_DecayModeFinding", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau1HPSdecayModeFinding)[idx]; });
            c.RegisterCut("T2_DecayModeFinding", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau2HPSdecayModeFinding)[idx]; });
            c.RegisterCut("T1_LTvalid", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau1LTvalid)[idx]; });
            c.RegisterCut("T2_LTvalid", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau2LTvalid)[idx]; });
            c.RegisterCut("T1_LTpT", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau1LTPt)[idx]; });
            c.RegisterCut("T2_LTpT", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau2LTPt)[idx]; });
            c.RegisterCut("T1_NumProngs", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau1NProngs)[idx]; });
            c.RegisterCut("T2_NumProngs", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau2NProngs)[idx]; });

            c.RegisterCut("T1_AntiElectronMVA", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau1HPSagainstElectronMVA)[idx]; });
            c.RegisterCut("T2_AntiElectronMVA", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau2HPSagainstElectronMVA)[idx]; });
            c.RegisterCut("T1_AntiElectronIndex", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return e->GetTau1AntiElectronIndex(idx); });
            c.RegisterCut("T2_AntiElectronIndex", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return e->GetTau2AntiElectronIndex(idx); });
            c.RegisterCut("T1_AntiMuonIndex", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return e->GetTau1AntiMuonIndex(idx); });
            c.RegisterCut("T2_AntiMuonIndex", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return e->GetTau2AntiMuonIndex(idx); });

            c.RegisterCut("T1_IsolationIndex", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return e->GetTau1IsolationIndex(idx); });
            c.RegisterCut("T2_IsolationIndex", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return e->GetTau2IsolationIndex(idx); });
            c.RegisterCut("T1_IsolationIndex3Hits", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return e->GetTau1IsolationIndex3Hits(idx); });
            c.RegisterCut("T2_IsolationIndex3Hits", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return e->GetTau2IsolationIndex3Hits(idx); });
            c.RegisterCut("T1_IsolationIndexMVA", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return e->GetTau1IsolationIndexMVA(idx); });
            c.RegisterCut("T2_IsolationIndexMVA", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return e->GetTau2IsolationIndexMVA(idx); });
            c.RegisterCut("T1_IsolationIndexMVA2", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return e->GetTau1IsolationIndexMVA2(idx); });
            c.RegisterCut("T2_IsolationIndexMVA2", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return e->GetTau2IsolationIndexMVA2(idx); });
            c.RegisterCut("T1_IsolationMVA2Raw", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau1HPSbyIsolationMVA2raw)[idx]; });
            c.RegisterCut("T2_IsolationMVA2Raw", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau2HPSbyIsolationMVA2raw)[idx]; });

            c.RegisterCut("TT_ChargeProduct", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau1Charge)[idx] * (*e->TTL_Tau2Charge)[idx]; });

            c.RegisterCut("TT_VisMass", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_DitauVisibleMass)[idx]; });
            c.RegisterCut("TT_DeltaEta", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_Tau1Eta)[idx] - (*e->TTL_Tau2Eta)[idx]; });
            c.RegisterCut("TT_CosDeltaPhi", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return (*e->TTL_DitauCosDeltaPhi)[idx]; });
            c.RegisterCut("MET", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return e->Ev_MET; });
            c.RegisterCut("MVA", 1, [](roast::Branches *b, const int& idx) -> float {
                    ttl::Branches* e = dynamic_cast<ttl::Branches*>(b);
                    return MVABase::gMVA["CFMlpANN"] ? MVABase::gMVA["CFMlpANN"]->Evaluate(e, idx) : 0.; });
        }
    }
}
