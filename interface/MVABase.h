#ifndef __MVABase_h
#define __MVABase_h

#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

#include "Branches.h"
#include "Process.h"

namespace roast {
    class MVABase {
        public:
            static std::map<std::string, MVABase*> gMVA;
            static std::map<std::string, MVABase*> gComboMVA;

            MVABase(const std::string&, const std::vector<std::string>&, const int rnk=1);
            ~MVABase();

            void CreateTrainingSample(roast::Process*, const std::vector<roast::Process*>&);
            void CreateTrainingSample(roast::Process*);
            float Evaluate(Branches*, int);
            void FillTree(TTree*, const roast::Process*);
            void FillTree(TTree*, TTree*, const roast::Process*);
            virtual void FillVariables(Branches*, const int) = 0;
            bool BookMVA(const string&);
            void TrainMVA(const map<string, string>&);

        protected:
            virtual Branches* GetBranches(const roast::Process*) = 0;
            virtual void SetupVariables(TMVA::Factory*) = 0;
            virtual void SetupVariables(TMVA::Reader*) = 0;
            virtual void SetupVariables(TTree*) = 0;

            template<typename W, typename T> void AddVariableConditionally(W*, const string&, const char&, T&);
            template<typename T> void AddVariable(TMVA::Factory*, const string&, const char&, T&);
            template<typename T> void AddVariable(TMVA::Reader*, const string&, const char&, T&);
            template<typename T> void AddVariable(TTree*, const string&, const char&, T&);

            TMVA::Reader* reader;

            std::string basedir;
            std::string method;
            std::vector<std::string> variables;

            int rank;

            std::map<std::string, double> backgrounds;

            std::string log_filename;
            std::string output_filename;
            std::string sample_filename;

            ClassDef(MVABase, 1);
    };

    template<typename W, typename T>
    void
    MVABase::AddVariableConditionally(W* obj, const string& name, const char& type, T& var)
    {
        // FIXME should actually be the other way around:  go through variables
        // and add them
        if (find(variables.begin(), variables.end(), name) != variables.end())
            AddVariable(obj, name, type, var);
    }

    template<typename T>
    void
    MVABase::AddVariable(TMVA::Factory *factory, const string& name, const char& type, T& var)
    {
        factory->AddVariable(name, type);
    }

    template<typename T>
    void
    MVABase::AddVariable(TMVA::Reader *reader, const string& name, const char& type, T& var)
    {
        reader->AddVariable(name, &var);
    }

    template<typename T>
    void
    MVABase::AddVariable(TTree *tree, const string& name, const char& type, T& var)
    {
        tree->Branch(name.c_str(), &var);
    }
}

#endif
