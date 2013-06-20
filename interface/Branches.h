#ifndef Branches_h
#define Branches_h

#include <map>
#include <string>
#include <vector>

#include "TChain.h"

#include "CutFlow.h"

namespace roast {
    class Branches {
        protected:
            void SetUp(std::map<std::string,std::string> const &, std::vector<std::string> const &);
            virtual void Null() = 0;
            virtual void Delete() = 0;
            virtual void Clear() = 0;
            virtual void SetBranchAddresses() = 0;
            virtual void SetChain(TChain*);

            std::map<std::string,std::string>		params;
            TChain* fChain;

        public:
            Branches();
            Branches(const std::string&, const std::vector<std::string>&);
            ~Branches();

            void	Init();
            virtual void	GetEntry(double);
            Long64_t GetEntries();
            virtual unsigned int GetNCombos() = 0;
            virtual bool IsGoodGenMatch(const int&) const = 0;
            virtual void RegisterCuts(CutFlow&) = 0;

            void				SetBestCombo(int);
            unsigned int const	GetBestCombo() const;

        private:
            int bestCombo;

        ClassDef(Branches, 1);
    };
}

#endif
