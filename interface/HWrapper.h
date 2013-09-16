// vim: ts=4:sw=4:et:sta
#ifndef HWrapper_h
#define HWrapper_h

#include <map>
#include <string>
#include <vector>

#include "TH1.h"
#include "TH1F.h"
#include "TH2F.h"

#include "Accessor.h"

namespace roast {
    class HWrapper{
       public :
          HWrapper();
          HWrapper(const roast::HWrapper&);
          HWrapper(const std::string& subdir, TH1*, const std::string&);
          HWrapper(const std::string& subdir, TH1*, const std::string&, const std::string&);
          virtual ~HWrapper();

          inline TH1 const * GetHisto() const { return histo; };
          inline TH1 * GetHisto() { return histo; };
          inline TH1* operator*() { return histo; };
          inline const TH1* operator*() const { return histo; };
          inline TH1* operator->() { return histo; };
          // inline const TH1* operator->() { return histo; };

          void Fill(Branches*, int, float);

          inline std::string GetName() const { return name; };
          inline std::string GetSubDir() const { return subdir; };
          inline double GetNOEraw() const { return NOEraw; };
          double GetMaximum() const;
          double GetMaximumWithError() const;

          void SetHisto(const TH1*);
          void SetMaximum(double const);
          void ResetMaximum(double const iFactor=1.1);
          void Add(TH1 const &, double const iFactor=1.0);
          void Add(HWrapper const &, double const);
          inline void Add(const HWrapper& w) { Add(w, 1.0); };
          void NormalizeTo(double const);
          void ScaleBy(double const);
          void ScaleErrorBy(double const);
          void AddRelErrorInQuadrature(double const);
          void AddRelErrorInQuadrature(double const, int);
          void Positivize();

       private:
          std::string name;
          std::string subdir;

          TH1*  histo;

          GetValue_t fill;
          GetValue_t max;

          double NOEraw;

          ClassDef(HWrapper, 1);
    };
}

#endif
