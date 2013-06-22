#ifndef TTLBranches_h
#define TTLBranches_h

#include "Branches.h"

namespace roast {
    namespace ttl {
        class Branches : public roast::Branches {
            private:
                float conesize;
                std::vector<int> jetIndexCache;
                int jetIndexCacheCombo;

                void	Null();
                void	Delete();
                void	Clear();
                void	SetBranchAddresses();

            public:
                Branches();
                Branches(const std::string&, const std::vector<std::string>&);
                ~Branches();

                virtual void GetEntry(double);
                inline virtual unsigned int GetNCombos() { return TTL_NumCombos; };
                virtual void FillHistograms(std::map<std::string, roast::HWrapper*>&, int, float, float pu_weight=1.);
                virtual bool IsGoodGenMatch(const int&) const;
                virtual void RegisterCuts(CutFlow&);

                // Get index (excluding taus) of jet in J_ branches (which include taus)
                unsigned int GetJetIndex(const int, const unsigned int);
                unsigned int	GetTau1IsolationIndex(const unsigned int) const;
                unsigned int	GetTau2IsolationIndex(const unsigned int) const;
                unsigned int	GetTau1IsolationIndexMVA(const unsigned int) const;
                unsigned int	GetTau2IsolationIndexMVA(const unsigned int) const;
                unsigned int	GetTau1IsolationIndexMVA2(const unsigned int) const;
                unsigned int	GetTau2IsolationIndexMVA2(const unsigned int) const;
                unsigned int	GetTau1IsolationIndex3Hits(const unsigned int) const;
                unsigned int	GetTau2IsolationIndex3Hits(const unsigned int) const;
                unsigned int	GetTau1AntiElectronIndex(const unsigned int) const;
                unsigned int	GetTau2AntiElectronIndex(const unsigned int) const;
                unsigned int	GetTau1AntiElectronIndexMVA2(const unsigned int) const;
                unsigned int	GetTau2AntiElectronIndexMVA2(const unsigned int) const;
                unsigned int	GetTau1AntiElectronIndexMVA3(const unsigned int) const;
                unsigned int	GetTau2AntiElectronIndexMVA3(const unsigned int) const;
                unsigned int	GetTau1AntiMuonIndex(const unsigned int) const;
                unsigned int	GetTau2AntiMuonIndex(const unsigned int) const;
                unsigned int	GetTau1AntiMuonIndex2(const unsigned int) const;
                unsigned int	GetTau2AntiMuonIndex2(const unsigned int) const;
                unsigned int    GetTau1MatchIndex(const unsigned int) const;
                unsigned int    GetTau2MatchIndex(const unsigned int) const;
                // float GetMVAResponse(const unsigned int) const;

#include "TTLBranches_declarations.h"

                ClassDef(Branches, 1);
        };
    }
}

#endif
