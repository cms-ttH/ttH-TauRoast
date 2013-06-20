#ifndef HContainer_h
#define HContainer_h

#include <map>
#include <vector>

#include "TH1F.h"
#include "TH2F.h"

#include "HWrapper.h"
#include "CutFlow.h"

using namespace std;
//using namespace Common;

namespace roast {
    class HContainer{
       private:
          map<string, roast::HWrapper> container;

       public :
          // Default constructor
          HContainer();
          HContainer(const HContainer&);
          virtual ~HContainer();

          void 							Add(roast::HWrapper const &);
          void 							Add(string const, roast::HWrapper const &);
          roast::HWrapper *						Get(string const);
          roast::HWrapper const *				Get(string const) const;
          vector<string> const			GetNames() const;
          TH1 const *						GetHisto(string const) const;
          map<string, roast::HWrapper> const &	GetContainer() const; 
          void							clear();
          int const						size() const;
          map<string, roast::HWrapper>::const_iterator begin() const;
          map<string, roast::HWrapper>::const_iterator end() const;


          void Fill(string const, double const, float const);
          void Fill(string const, double const, double const, float const);
          bool Exists(string const);
          void SetMarkerStyle(int const);
          void SetFillStyle(int const);
          void SetFillColor(int const);
          void SetLineColor(int const);
          void SetLineWidth(int const, int const);
          void NormalizeTo(double const);
          void ScaleBy(double const);
          void ScaleErrorBy(double const);
          void AddRelErrorInQuadrature(double const);
          void Add(HContainer const &, double const iFactor=1.0);
          void Positivize();

          ClassDef(HContainer, 1);
    };
}

#endif
