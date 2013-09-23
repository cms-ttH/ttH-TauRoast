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
       public:
          static HWrapper* Create1D(const std::string&, TH1*, const std::string&, const std::string="");
          static HWrapper* Create2D(const std::string&, TH1*, const std::string&, const std::string&, const std::string="");
          HWrapper();
          HWrapper(const roast::HWrapper&);
          HWrapper(const std::string&, TH1*, GetValue_t, GetValue_t, GetValue_t);
          virtual ~HWrapper();

          inline TH1 const * GetHisto() const { return histo; };
          inline TH1 * GetHisto() { return histo; };
          inline TH1* operator*() { return histo; };
          inline const TH1* operator*() const { return histo; };
          inline TH1* operator->() { return histo; };
          // inline const TH1* operator->() { return histo; };

          void SetTranslate(bool t=true) { translate = t; };

          void Fill(Branches*, int, float);

          inline std::string GetName() const { return name; };
          inline std::string GetSubDir() const { return subdir; };
          inline double GetNOEraw() const { return NOEraw; };
          double GetMaximum() const;
          double GetMaximumWithError() const;

          void SetAdditionalXInfo(const std::string& n) { xadd = get_accessor(n); };
          void SetAdditionalYInfo(const std::string& n) { yadd = get_accessor(n); };

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

          GetValue_t xval;
          GetValue_t yval;
          GetValue_t max;

          // additional info to be handled (e.g., for translation)
          GetValue_t xadd;
          GetValue_t yadd;

          bool translate;
          double NOEraw;

          ClassDef(HWrapper, 1);
    };
}

#endif
