// vim: ts=4:sw=4:et:sta
/*
    Author:			Nil Valls <nil.valls@cern.ch>
    Date:			11 Jun 2011
    Description:	Plot stacking class.
    Notes:

*/

#include "TFile.h"

#include "../interface/Branches.h"

using namespace std;
using namespace roast;

// Default constructor
Branches::Branches() : fChain(0), caches_dirty(true)
{
    Null();
    SetBranchAddresses();
}

Branches::Branches(const string& tree_name, vector<string> const & iPath) :
    caches_dirty(true)
{
    fChain = new TChain(tree_name.c_str());
    for (const auto& p: iPath)
        fChain->Add((p + "/*.root").c_str());

    // Set branch addresses and branch pointers
    if (!fChain){ cerr << "ERROR: Trying to initialize NULL TChain" << endl; exit(1); }
    fChain->SetMakeClass(1);
    Init();

    Null();
    SetBranchAddresses();
}

// Default destructor
Branches::~Branches()
{
    if (fChain) {
        fChain->ResetBranchAddresses();
        delete fChain->GetCurrentFile();
    }
    delete fChain;
}

void Branches::SetChain(TChain* iChain){ fChain = iChain; } 

void Branches::Init()
{
	// Set branch addresses and branch pointers
	fChain->SetMakeClass(1);
	fChain->ResetBranchAddresses();

	//Clear();
}

void
Branches::GetEntry(double iEntry)
{
    caches_dirty = true;
    fChain->GetEntry(iEntry);
}

Long64_t Branches::GetEntries(){
	if(fChain == NULL){ cerr << "ERROR: Trying to GetEntries() from a NULL TChain." << endl; exit(1); }
	return fChain->GetEntries();
}

void
Branches::RegenerateCaches()
{
    if (caches_dirty) {
        clean_btag_cache.clear();
        clean_nonbtag_cache.clear();

        for (const auto& v: *CleanJetIndices) {
            std::vector<int> btag_indices;
            std::vector<int> nonbtag_indices;

            for (const auto& i: v) {
                if ((*J_combSecVtxMediumBTag)[i])
                    btag_indices.push_back(i);
                else
                    nonbtag_indices.push_back(i);
            }

            clean_btag_cache.push_back(btag_indices);
            clean_nonbtag_cache.push_back(nonbtag_indices);
        }
        caches_dirty = false;
    }
}

float
Branches::GetCleanJetBTagEta(unsigned int idx, unsigned int n)
{
    RegenerateCaches();
    return J_Eta->at(clean_btag_cache[idx][n]);
}

float
Branches::GetCleanJetBTagPt(unsigned int idx, unsigned int n)
{
    RegenerateCaches();
    return J_Pt->at(clean_btag_cache[idx][n]);
}

float
Branches::GetCleanJetNonBTagEta(unsigned int idx, unsigned int n)
{
    RegenerateCaches();
    return J_Eta->at(clean_nonbtag_cache[idx][n]);
}

float
Branches::GetCleanJetNonBTagPt(unsigned int idx, unsigned int n)
{
    RegenerateCaches();
    return J_Pt->at(clean_nonbtag_cache[idx][n]);
}

ClassImp(roast::Branches)
