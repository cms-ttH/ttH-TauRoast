#ifndef Branches_h
#define Branches_h

#include <map>
#include <string>
#include <vector>

#include "TChain.h"

#include "CutFlow.h"
#include "HWrapper.h"

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
            virtual void FillHistograms(std::map<std::string, roast::HWrapper*>&, int, float) = 0;
            virtual void RegisterCuts(CutFlow&) = 0;

            void				SetBestCombo(int);
            unsigned int const	GetBestCombo() const;

            // >>> Begin declarations <<<
            Double_t puWeight2012A;
            Double_t puWeight2012AB;
            Double_t puWeight2012ABC;
            Double_t puWeight2012B;
            Double_t puWeight2012BC;
            Double_t puWeight2012C;
            Double_t puWeight2012CD;
            Double_t puWeight2012D;
            Double_t puWeight;
            Double_t puWeightDown;
            Double_t puWeightUp;
            Double_t q2WeightDown;
            Double_t q2WeightUp;
            Double_t topPtWeight;
            Double_t topPtWeightDown;
            Double_t topPtWeightUp;
            std::vector<float>* CSVeventWeight;
            std::vector<float>* CSVeventWeightCErr1down;
            std::vector<float>* CSVeventWeightCErr1up;
            std::vector<float>* CSVeventWeightCErr2down;
            std::vector<float>* CSVeventWeightCErr2up;
            std::vector<float>* CSVeventWeightHFStats1down;
            std::vector<float>* CSVeventWeightHFStats1up;
            std::vector<float>* CSVeventWeightHFStats2down;
            std::vector<float>* CSVeventWeightHFStats2up;
            std::vector<float>* CSVeventWeightHFdown;
            std::vector<float>* CSVeventWeightHFup;
            std::vector<float>* CSVeventWeightLFStats1down;
            std::vector<float>* CSVeventWeightLFStats1up;
            std::vector<float>* CSVeventWeightLFStats2down;
            std::vector<float>* CSVeventWeightLFStats2up;
            std::vector<float>* CSVeventWeightLFdown;
            std::vector<float>* CSVeventWeightLFup;
            std::vector<float>* TLL_Lepton1EventWeight;
            std::vector<float>* TLL_Lepton2EventWeight;
            std::vector<float>* TTL_LeptonEventWeight;
            // >>> End declarations <<<

        private:
            int bestCombo;

        ClassDef(Branches, 1);
    };
}

#endif
