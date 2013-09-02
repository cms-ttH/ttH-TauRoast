// vim: ts=4:sw=4:et:sta
#include <iostream>

#include "boost/lexical_cast.hpp"

#include "TRandom.h"
#include "TTimeStamp.h"

#include "../interface/HWrapper.h"

using namespace std;
using namespace roast;

// Default constructor
HWrapper::HWrapper() : histo(0), fill(0), max(0) {}

// Copy constructor
HWrapper::HWrapper(const HWrapper& iHWrapper) :
    histo(0),
    fill(0),
    max(0)
{
    if (iHWrapper.GetHisto())
        this->SetHisto(iHWrapper.GetHisto());

    fill = iHWrapper.fill;
    max = iHWrapper.max;

    name = iHWrapper.GetName();
    subdir = iHWrapper.GetSubDir();
    NOEraw = iHWrapper.GetNOEraw();
}

HWrapper::HWrapper(const std::string& subdir, TH1* hist, const std::string& fill_f) :
    subdir(subdir),
    histo(0),
    fill(0),
    max(0)
{
    histo = dynamic_cast<TH1*>(hist->Clone());

    fill = get_accessor(fill_f);
}

HWrapper::HWrapper(const std::string& subdir, TH1* hist, const std::string& fill_f, const std::string& max_f) :
    subdir(subdir),
    histo(0),
    fill(0),
    max(0)
{
    histo = dynamic_cast<TH1*>(hist->Clone());

    fill = get_accessor(fill_f);
    max = get_accessor(max_f);
}

HWrapper::~HWrapper()
{
    if (histo)
        delete histo;
}

void
HWrapper::Fill(Branches* b, int i, float w)
{
    if (!max) {
        histo->Fill(fill(b, i, -1), w);
    } else {
        for (int n = 0; n < max(b, i, -1); ++n)
            histo->Fill(fill(b, i, n), w);
    }
}

// Getter methods
string			HWrapper::GetName() const { return name; }
string			HWrapper::GetSubDir() const { return subdir; }
double const	HWrapper::GetNOEraw() const { return NOEraw; }

void
HWrapper::SetHisto(const TH1* hist)
{
    TTimeStamp timestamp;
    TRandom random(timestamp.GetTimeSpec().tv_nsec);
    stringstream ssName; ssName.str("");
    ssName << hist->GetName() << timestamp.GetTimeSpec().tv_sec << timestamp.GetTimeSpec().tv_nsec << random.Gaus();

    if (histo)
        delete histo;

    histo = dynamic_cast<TH1F*>(hist->Clone((ssName.str()).c_str()));
}

void HWrapper::SetMaximum(double const iValue){ histo->SetMaximum(iValue); }
void HWrapper::ResetMaximum(double const iFactor){ histo->SetMaximum(iFactor*histo->GetBinContent(histo->GetMaximumBin())); }

// Other methods
void HWrapper::Add(TH1 const & iHisto, double iFactor){ 
	if(iFactor != iFactor){ cerr << "ERROR: trying to Add(TH1&, nan)" << endl; exit(1); }
	histo->Add(&iHisto, iFactor);
}
void HWrapper::Add(HWrapper const & iHisto, double const iFactor){ 
	histo->Add((iHisto.GetHisto()), iFactor);
}
void HWrapper::ScaleBy(double const iFactor){
	if(iFactor != iFactor){ cerr << "ERROR: trying to ScaleBy(nan)" << endl; exit(1); }
	histo->Scale(iFactor);
}

void HWrapper::ScaleErrorBy(double const iFactor){	
	if(iFactor != iFactor){ cerr << "ERROR: trying to ScaleErrorBy(nan)" << endl; exit(1); }
	for(int b=0; b<=histo->GetNbinsX(); b++){ histo->SetBinError(b, iFactor*(histo->GetBinError(b))); }
}

void HWrapper::AddRelErrorInQuadrature(double const iError){	
	if(iError != iError){ cerr << "ERROR: trying to AddRelErrorInQuadrature(nan)" << endl; exit(1); }
	for(int b=0; b<=histo->GetNbinsX(); b++){ 
        AddRelErrorInQuadrature(iError, b); 
    }
}

void HWrapper::AddRelErrorInQuadrature(double const iError, int iBin){	
	if(iError != iError){ cerr << "ERROR: trying to AddRelErrorInQuadrature(nan)" << endl; exit(1); }
    double newError = sqrt(histo->GetBinError(iBin)*histo->GetBinError(iBin) + iError*histo->GetBinContent(iBin)*iError*histo->GetBinContent(iBin));
    histo->SetBinError(iBin,newError); 
}

void HWrapper::NormalizeTo(double const iNormalization){	ScaleBy(iNormalization/(double)histo->Integral()); }

// Zero negative bins only
void HWrapper::Positivize(){
    for(int b = 1; b <= histo->GetNbinsX(); b++){
        float oldContent = histo->GetBinContent(b);
        if(oldContent < 0){ histo->SetBinContent(b, 0); }
    }
}

double const HWrapper::GetMaximum() const { return histo->GetBinContent(histo->GetMaximumBin()); }
double const HWrapper::GetMaximumWithError() const {
	double result = 0;
	for(int b = 1; b <= histo->GetNbinsX(); b++){
		double content = histo->GetBinContent(b);	
		double error = histo->GetBinError(b);	
		if((content+error) > result){ result = content+error; }
	}
	return result;
}

void HWrapper::PrintInfo(){
	cout << "\n\n" << endl;
	cout << "\tName......." << GetName() << endl;
	cout << "\tNbinsX....." << histo->GetNbinsX() << endl;
	cout << "\tMin X......" << histo->GetBinLowEdge(1) << endl;
	cout << "\tMax X......" << histo->GetBinLowEdge(histo->GetNbinsX()) << endl;
	cout << "\tIntegral..." << histo->Integral() << endl;
	cout << "\tMax........" << GetMaximum() << endl;
	cout << "\tMax w err.." << GetMaximumWithError() << endl;
	cout << "--------------------------" << endl;

	for(int b=0; b <= histo->GetNbinsX(); b++){
		cout << "\t" << b << "\t" << histo->GetBinLowEdge(b) << "\t" << histo->GetBinContent(b) << "\t+/-\t" << histo->GetBinError(b) << endl;
	}
	cout << "==========================\n\n" << endl;



}

ClassImp(roast::HWrapper)
