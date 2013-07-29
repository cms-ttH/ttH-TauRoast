// vim: ts=4:sw=4:et:sta
/*
    Author:			Nil Valls <nil.valls@cern.ch>, Matthias Wolf <matthias.wolf@cern.ch>
    Date:			11 Jun 2011
    Description:	TMVA evaluating class
    Notes:

*/

#include <fstream>

#include "boost/filesystem/operations.hpp"

#include "TMVA/Config.h"
#include "TFile.h"
#include "TLorentzVector.h"

#include "../interface/Branches.h"
#include "../interface/Helper.h"

#include "../interface/MVABase.h"

using namespace std;

namespace roast {
    map<string, MVABase*> MVABase::gMVA;
    map<string, MVABase*> MVABase::gComboMVA;

    MVABase::~MVABase()
    {
        delete reader;
    }

    MVABase::MVABase(const std::string& dir,
            const std::vector<std::string>& vars, const int rnk) :
        basedir(dir),
        method(),
        variables(vars),
        rank(rnk)
    {
        reader = new TMVA::Reader("!Color:Silent:!V");

        log_filename = basedir + "/tmva.log";
        output_filename = basedir + "/tmva.root";
        sample_filename = basedir + "/sample.root";
    }

    bool
    MVABase::BookMVA(const string& method)
    {
        using namespace boost::filesystem;

        this->method = method;
        SetupVariables(reader);

        string weight_filename = basedir + "/TMVAClassification_" + method + ".weights.xml";
        // if (exists(path(weight_filename, &boost::filesystem::native))) {
        if (exists(path(weight_filename))) {
            reader->BookMVA(method + " method", weight_filename);
            return true;
        }
        return false;
    }

    void
    MVABase::CreateTrainingSample(Process* proc)
    {
        TFile outfile(sample_filename.c_str(), "RECREATE");
        outfile.cd();

        TTree* sig_tree = new TTree("TreeS","TreeS");
        TTree* bkg_tree = new TTree("TreeB","TreeB");
        cout << "\tSame sample MVA" << std::endl;
        FillTree(sig_tree, bkg_tree, proc);
        sig_tree->Write();
        bkg_tree->Write();

        cout << "\tSignal tree contains " << sig_tree->GetEntries() << " events." << endl;
        cout << "\tBackground tree contains " << bkg_tree->GetEntries() << " events." << endl;
    }

    void
    MVABase::CreateTrainingSample(Process* signal, const vector<Process*>& bkgs)
    {
        TFile outfile(sample_filename.c_str(), "RECREATE");
        outfile.cd();

        TTree* signalTree = new TTree("TreeS","TreeS");
        FillTree(signalTree, signal);
        signalTree->Write();

        cout << "\tSignal tree contains " << signalTree->GetEntries() << " events." << endl;

        delete signalTree;

        this->backgrounds.clear();
        for (auto& bkg: bkgs) {
            double weight = bkg->GetCrossSection();
            string name = "TreeB_" + bkg->GetShortName();

            TTree* backgroundTree = new TTree(name.c_str(), name.c_str());
            FillTree(backgroundTree, bkg);
            backgroundTree->Write();

            this->backgrounds[name] = weight;

            cout << "\tBackground tree contains " << backgroundTree->GetEntries() << " events." << endl;

            delete backgroundTree;
        }
    }

    void
    MVABase::FillTree(TTree *tree, const Process *process)
    {
        SetupVariables(tree);

        Branches *event = GetBranches(process);

        auto goodEvents = process->GetGoodEvents();
        for (unsigned int i = 0; i < goodEvents.size(); i++) {
            event->GetEntry(goodEvents.at(i).entry);
            int combo = goodEvents[i].combos[0];
            FillVariables(event, combo);

            tree->Fill();
        }
    }

    void
    MVABase::FillTree(TTree *sig, TTree *bkg, const Process *process)
    {
        SetupVariables(sig);
        SetupVariables(bkg);

        Branches *event = GetBranches(process);

        auto goodEvents = process->GetGoodEvents();
        for (unsigned int i = 0; i < goodEvents.size(); i++) {
            event->GetEntry(goodEvents.at(i).entry);

            for (unsigned int i = 0; i < event->GetNumCombos(); ++i) {
                FillVariables(event, i);

                if (event->IsGoodGenMatch(i))
                    sig->Fill();
                else
                    bkg->Fill();
            }
        }

        backgrounds.clear();
        backgrounds["TreeB"] = 1.0;
    }

    void
    MVABase::TrainMVA(const map<string, string>& setup)
    {
        TMVA::gConfig().GetIONames().fWeightFileDir = basedir;

        // ofstream tmp_out(log_filename);
        // streambuf* old_buf = cout.rdbuf();
        // cout.rdbuf(tmp_out.rdbuf());

        TFile outfile(output_filename.c_str(), "RECREATE");
        TMVA::Factory *factory = new TMVA::Factory("TMVAClassification", &outfile,
                "!V:Silent:Transformations=I;D;P;G,D:AnalysisType=Classification");

        SetupVariables(factory);

        TTree *stree;
        TFile infile(sample_filename.c_str());

        infile.GetObject("TreeS", stree);
        factory->AddSignalTree(stree, 1.);

        for (const auto& bkg: backgrounds) {
            TTree *btree;
            infile.GetObject(bkg.first.c_str(), btree);

            if (btree->GetEntries() > 0)
                factory->AddBackgroundTree(btree, bkg.second);
            else
                cerr << "WARNING: skipping " << bkg.first << " for MVA training (no events)" << endl;
        }

        factory->PrepareTrainingAndTestTree("", "", "SplitMode=Random:NormMode=NumEvents:!V");

        // the following can be copied from TMVAClassification.C
        // factory->BookMethod(TMVA::Types::kCuts, "Cuts",
                // "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart");
        for (const auto& m: setup) {
            if (m.first.find("BDT") != string::npos) {
                cout << "Booking BDT: " << m.first << endl;
                factory->BookMethod(TMVA::Types::kBDT, m.first, m.second);
            } else if (m.first == "CFMlpANN") {
                cout << "Booking ANN: " << m.first << endl;
                factory->BookMethod(TMVA::Types::kCFMlpANN, m.first, m.second);
            } else {
                cerr << "ERROR: can't train " << m.first << endl;
            }
        }

        cerr << "TRAINING" << endl;
        factory->TrainAllMethods();
        factory->TestAllMethods();
        factory->EvaluateAllMethods();

        delete factory;

        // cout.rdbuf(old_buf);
    }

    float
    MVABase::Evaluate(Branches* event, int combo)
    {
        FillVariables(event, combo);
        return reader->EvaluateMVA(method + " method");
    }

    void
    register_mva(const string& name, MVABase *mva)
    {
        MVABase::gMVA[name] = mva;
    }
}

ClassImp(roast::MVABase)
