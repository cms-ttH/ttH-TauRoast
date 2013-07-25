// vim: ts=4:sw=4:et:sta
#ifndef HWrapper_h
#define HWrapper_h

#include <map>
#include <vector>

#include "TH1F.h"
#include "TH2F.h"

using namespace std;
//using namespace Common;

namespace roast {
    class HWrapper{
       private:
          string  name;
          string	subdir;
          TH1F*	histo;
          bool	isTH2F;
          bool	isTH1F;
          float	xbins[100];
          string	xbinLabels[100];
          float	xMinVis;
          float	xMaxVis;
          float	yMinVis;
          float	yMaxVis;
          float	zMinVis;
          float	zMaxVis;
          bool	logx;
          bool	logy;
          bool	logz;
          bool	showOverFlow;
          bool	showUnderFlow;
          bool	showText;
          bool	centerLabels;
          string	TH1Type;
          bool	hasVariableWidthBins;
          double	NOEraw;


       public :
          // Default constructor
          HWrapper();
          HWrapper(const roast::HWrapper&);
          HWrapper(string, string, string, const std::map<std::string, std::string>&);
          HWrapper(std::string name, std::string subdir, std::string type,
                  std::string xlabel, std::string ylabel, std::string zlabel,
                  bool logx, bool logy, bool logz,
                  int xbins, float xmin, float xmax, float xminvis, float xmaxvis,
                  int ybins, float ymin, float ymax, float yminvis, float ymaxvis,
                  bool showof, bool showuf, bool centerlabels, bool showtext);
          virtual ~HWrapper();

          inline TH1 const * GetHisto() const { return histo; };
          inline TH1 * GetHisto() { return histo; };
          string			GetName() const;
          string			GetSubDir() const;
          float const		GetMinXVis() const;
          float const		GetMaxXVis() const;
          float const		GetMinYVis() const;
          float const		GetMaxYVis() const;
          float const		GetMinZVis() const;
          float const		GetMaxZVis() const;
          string			GetXTitle() const;
          string			GetYTitle() const;
          string			GetZTitle() const;
          bool const		GetLogX() const;
          bool const		GetLogY() const;
          bool const		GetLogZ() const;
          bool const		ShowOverFlow() const;
          bool const		ShowUnderFlow() const;
          bool const		ShowText() const;
          bool const		CenterLabels() const;
          bool const		IsTH1F() const;
          bool const		IsTH2F() const;
          bool const		HasVariableWidthBins() const;
          void			Fill(double, double);
          void			Fill(double, double, double);
          double const	GetNOEraw() const;
          double const	GetMaximum() const;
          double const	GetMaximumWithError() const;


          void SetHisto(const TH1*);
          void	SetIsTH1F(bool const);
          void	SetIsTH2F(bool const);
          void	SetMaximum(double const);
          void	ResetMaximum(double const iFactor=1.1);
          void	Add(TH1 const &, double const iFactor=1.0);
          void	Add(HWrapper const &, double const);
          inline void Add(const HWrapper& w) { Add(w, 1.0); };
          void	NormalizeTo(double const);
          void	ScaleBy(double const);
          void	ScaleErrorBy(double const);
          void	AddRelErrorInQuadrature(double const);
          void	AddRelErrorInQuadrature(double const, int);
          void	Positivize();
          void	FillRandom(string const);

          void	SetLineWidth(int const,int iColor=0);
          void	SetFillStyle(int const,int iColor=0);
          void	SetFillColor(int const);
          void	SetLineColor(int const);
          void	SetMarkerStyle(int const);

          void	PrintInfo();


          ClassDef(HWrapper, 1);
    };
}

#endif
