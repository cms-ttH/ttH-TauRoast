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
Branches::Branches() : fChain(0), bestCombo(-1) {}

Branches::Branches(const string& tree_name, vector<string> const & iPath) :
    bestCombo(-1)
{
    fChain = new TChain(tree_name.c_str());
    for (const auto& p: iPath)
        fChain->Add((p + "/*.root").c_str());

    bestCombo = -1;

    // Set branch addresses and branch pointers
    if (!fChain){ cerr << "ERROR: Trying to initialize NULL TChain" << endl; exit(1); }
    fChain->SetMakeClass(1);
    Init();
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

void Branches::SetBestCombo(int const iValue){ bestCombo = iValue; }
unsigned int const Branches::GetBestCombo() const { 
	if (bestCombo < 0){ cerr << "ERROR: 'bestCombo' is " << bestCombo << "." << endl; exit(1); }
	return bestCombo; 
}

void Branches::SetChain(TChain* iChain){ fChain = iChain; } 

void Branches::Init()
{
	// Set branch addresses and branch pointers
	fChain->SetMakeClass(1);
	fChain->ResetBranchAddresses();

	//Clear();
}

void Branches::GetEntry(double iEntry){ fChain->GetEntry(iEntry); }

Long64_t Branches::GetEntries(){
	if(fChain == NULL){ cerr << "ERROR: Trying to GetEntries() from a NULL TChain." << endl; exit(1); }
	return fChain->GetEntries();
}

ClassImp(roast::Branches)
