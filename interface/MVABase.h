// vim: ts=4:sw=4:et:sta
#ifndef __MVABase_h
#define __MVABase_h

#include <algorithm>

#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

#include "Accessor.h"
#include "Branches.h"
#include "Process.h"
#include "TLLBranches.h"
#include "TTLBranches.h"

namespace roast {
    class MVABase {
        public:
            struct Var {
                Var() : GetVal(0), name(""), type('F'), value(0.) {};
                Var(const std::string&, char);
                void Update(roast::Branches*, int);

                GetValue_t GetVal;
                std::string name;
                char type;
                float value;
            };

            static std::map<std::string, MVABase*> gMVA;
            static std::map<std::string, MVABase*> gComboMVA;

            struct Get {
                virtual roast::Branches* Branches(const roast::Process*) = 0;
            };

            MVABase(const std::string&, const std::vector<roast::MVABase::Var>&, Get*, const int rnk=1);
            ~MVABase();

            void CreateTrainingSample(roast::Process*, const std::vector<roast::Process*>&);
            void CreateTrainingSample(roast::Process*);
            float Evaluate(Branches*, int);
            void FillTree(TTree*, const roast::Process*);
            void FillTree(TTree*, TTree*, const roast::Process*);
            void FillVariables(Branches*, const int);
            bool BookMVA(const std::string&);
            void TrainMVA(const std::map<std::string, std::string>&);

        protected:
            template<typename T> void SetupVariables(T obj);

            inline void AddVariable(TMVA::Factory *f, roast::MVABase::Var& v) { f->AddVariable(v.name, v.type); };
            inline void AddVariable(TMVA::Reader *r, roast::MVABase::Var& v) { r->AddVariable(v.name, &v.value); };
            inline void AddVariable(TTree *t, roast::MVABase::Var& v) { t->Branch(v.name.c_str(), &v.value); };

            TMVA::Reader* reader;

            Get* get;

            std::string basedir;
            std::string method;
            std::vector<std::string> variables;
            std::vector<roast::MVABase::Var> vars;

            int rank;

            std::map<std::string, double> backgrounds;

            std::string log_filename;
            std::string output_filename;
            std::string sample_filename;

            ClassDef(MVABase, 1);
    };

    void register_mva(const std::string&, MVABase*);

    namespace tll {
        struct Get : roast::MVABase::Get {
            virtual roast::Branches* Branches(const roast::Process *proc)
            {
                return new roast::tll::Branches(proc->GetTreeName(), proc->GetNtuplePaths());
            };
        };
    }

    namespace ttl {
        struct Get : roast::MVABase::Get {
            virtual roast::Branches* Branches(const roast::Process *proc)
            {
                return new roast::ttl::Branches(proc->GetTreeName(), proc->GetNtuplePaths());
            };
        };
    }

    template<typename T>
    void
    MVABase::SetupVariables(T obj)
    {
        for (auto& v: vars)
            AddVariable(obj, v);
    }
}

#endif
