// vim: ts=4:sw=4:et:sta
#include <iostream>

#include <stdint.h>

#include "boost/lexical_cast.hpp"

#include "TRandom.h"
#include "TTimeStamp.h"

#include "../interface/Branches.h"
#include "../interface/HWrapper.h"

using namespace std;
using namespace roast;

// Default constructor
HWrapper::HWrapper() : histo(0), xval(0), yval(0), max(0), xadd(0), yadd(0), unweigh(0), mode(kAll), trafo(kNone) {}

// Copy constructor
HWrapper::HWrapper(const HWrapper& iHWrapper) :
    histo(0),
    xval(0),
    yval(0),
    max(0),
    xadd(iHWrapper.xadd),
    yadd(iHWrapper.yadd),
    unweigh(iHWrapper.unweigh)
{
    if (iHWrapper.GetHisto())
        this->SetHisto(iHWrapper.GetHisto());

    xval = iHWrapper.xval;
    yval = iHWrapper.yval;
    max = iHWrapper.max;

    trafo = iHWrapper.trafo;
    mode = iHWrapper.mode;

    name = iHWrapper.GetName();
    subdir = iHWrapper.GetSubDir();
}

HWrapper::HWrapper(const std::string& subdir, TH1* hist, const Weight& w) :
    subdir(subdir),
    xval(w.GetVal),
    yval(0),
    max(0),
    xadd(0),
    yadd(0),
    unweigh(0),
    mode(kAll),
    trafo(kNone)
{
    histo = dynamic_cast<TH1*>(hist->Clone());
}

HWrapper::HWrapper(const std::string& subdir, TH1* hist, GetValue_t x, GetValue_t y, GetValue_t m) :
    subdir(subdir),
    xval(x),
    yval(y),
    max(m),
    xadd(0),
    yadd(0),
    unweigh(0),
    mode(kAll),
    trafo(kNone)
{
    histo = dynamic_cast<TH1*>(hist->Clone());
}

HWrapper::~HWrapper()
{
    if (histo)
        delete histo;
}

HWrapper *
HWrapper::Create1D(const std::string& subdir, TH1* hist, const std::string& xfill, const std::string max)
{
    auto x = get_accessor(xfill);
    auto m = (max != "") ? get_accessor(max) : 0;

    return new HWrapper(subdir, hist, x, 0, m);
}

HWrapper *
HWrapper::Create2D(const std::string& subdir, TH1* hist, const std::string& xfill, const std::string& yfill, const std::string max)
{
    auto x = get_accessor(xfill);
    auto y = get_accessor(yfill);
    auto m = (max != "") ? get_accessor(max) : 0;

    return new HWrapper(subdir, hist, x, y, m);
}

float
HWrapper::Translate(GetValue_t val, GetValue_t aux, Branches* b, int i, int n)
{
    if (trafo == kTranslateId) {
        if (aux) {
            return b->TranslateMatchIndex(val(b, i, n), aux(b, i, n));
        } else {
            return b->TranslateMatchIndex(val(b, i, n));
        }
    } else if (trafo == kTranslateJetId) {
        return b->TranslateJetMatchIndex(val(b, i, n));
    }
    return val(b, i, n);
}

// Filling the histogram has a special case:  if no filling functions are
// defined, the weight will be plotted.
void
HWrapper::Fill(Branches* b, int i, float w)
{
    float w_orig = w;
    if (trafo == kUnweighed)
        w = 1.;

    std::vector<float> v_weights;
    std::vector<float> v_xvals;
    std::vector<float> v_yvals;

    int begin = -1;
    int end = 0;

    if (max) {
        begin = 0;
        end = max(b, i, -1);
    }

    for (int n = begin; n < end; ++n) {
        float new_w = w;
        if (unweigh)
            new_w /= unweigh(b, i, n);

        v_weights.push_back(new_w);

        if (xval)
            v_xvals.push_back(Translate(xval, xadd, b, i, n));
        else
            v_xvals.push_back(w_orig);

        if (yval)
            v_yvals.push_back(Translate(yval, yadd, b, i, n));
    }

    if (mode != kAll) {
        float nx = 0;
        float ny = 0;
        float nw = 0;

        if (mode == kAvg) {
            float x_denom = 0;

            if (v_xvals.size() > 0) {
                for (unsigned int i = 0; i < v_xvals.size(); ++i) {
                    nx += v_xvals[i] * v_weights[i];
                    x_denom += v_weights[i];
                }

                nx /= x_denom;
            }

            if (v_yvals.size() > 0) {
                float y_denom = 0;

                for (unsigned int i = 0; i < v_yvals.size(); ++i) {
                    ny += v_yvals[i] * v_weights[i];
                    y_denom += v_weights[i];
                }

                ny /= y_denom;
            }

            nw = x_denom / v_weights.size();
        } else if (mode == kMax) {
            if (v_xvals.size() > 0) {
                nx = v_xvals[0];
                nw = v_weights[0];
                for (unsigned int n = 1; n < v_xvals.size(); ++n) {
                    if (v_xvals[n] > nx) {
                        nx = v_xvals[n];
                        nw = v_weights[n];
                    }
                }
            }

            if (v_yvals.size() > 0) {
                ny = v_yvals[0];
                float nw_y = v_weights[0];
                for (unsigned int n = 1; n < v_yvals.size(); ++n) {
                    if (v_yvals[n] > ny) {
                        ny = v_yvals[n];
                        nw_y = v_weights[n];
                    }
                }

                nw = (nw + nw_y) * .5;
            }
        } else if (mode == kMin) {
            if (v_xvals.size() > 0) {
                nx = v_xvals[0];
                nw = v_weights[0];
                for (unsigned int n = 1; n < v_xvals.size(); ++n) {
                    if (v_xvals[n] < nx) {
                        nx = v_xvals[n];
                        nw = v_weights[n];
                    }
                }
            }

            if (v_yvals.size() > 0) {
                ny = v_yvals[0];
                float nw_y = v_weights[0];
                for (unsigned int n = 1; n < v_yvals.size(); ++n) {
                    if (v_yvals[n] < ny) {
                        ny = v_yvals[n];
                        nw_y = v_weights[n];
                    }
                }

                nw = (nw + nw_y) * .5;
            }
        }

        if (v_xvals.size() > 0)
            v_xvals = {nx};
        if (v_yvals.size() > 0)
            v_yvals = {ny};
        v_weights = {nw};
    }

    for (unsigned int i = 0; i < v_xvals.size(); ++i) {
        if (yval) {
            dynamic_cast<TH2*>(histo)->Fill(v_xvals[i], v_yvals[i], v_weights[i]);
        } else {
            histo->Fill(v_xvals[i], v_weights[i]);
        }
    }
}

void
HWrapper::SetHisto(const TH1* hist)
{
    TTimeStamp timestamp;
    TRandom random(timestamp.GetTimeSpec().tv_nsec);
    stringstream ssName; ssName.str("");
    ssName << hist->GetName() << timestamp.GetTimeSpec().tv_sec << timestamp.GetTimeSpec().tv_nsec << random.Gaus();

    if (histo)
        delete histo;

    histo = dynamic_cast<TH1*>(hist->Clone((ssName.str()).c_str()));
    histo->SetDirectory(0);
}

void HWrapper::SetMaximum(double const iValue){ histo->SetMaximum(iValue); }
void HWrapper::ResetMaximum(double const iFactor){ histo->SetMaximum(iFactor*histo->GetBinContent(histo->GetMaximumBin())); }

// Other methods
void HWrapper::Add(TH1 const & iHisto, double iFactor){ 
	if(iFactor != iFactor){ cerr << "ERROR: trying to Add(TH1&, nan)" << endl; exit(1); }
    if (histo) {
        histo->Add(&iHisto, iFactor);
    } else {
        histo = new TH1(iHisto);
        histo->Scale(iFactor);
    }
}
void HWrapper::Add(HWrapper const & iHisto, double const iFactor){ 
    if (histo) {
        histo->Add((iHisto.GetHisto()), iFactor);
    } else {
        histo = new TH1(*iHisto.GetHisto());
        histo->Scale(iFactor);
    }
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

double
HWrapper::GetMaximum() const
{
    return histo->GetBinContent(histo->GetMaximumBin());
}

double
HWrapper::GetMaximumWithError() const
{
	double result = 0;
	for(int b = 1; b <= histo->GetNbinsX(); b++){
		double content = histo->GetBinContent(b);	
		double error = histo->GetBinError(b);	
		if((content+error) > result){ result = content+error; }
	}
	return result;
}

ClassImp(roast::HWrapper)
