#include <iomanip>

#include "boost/lexical_cast.hpp"

#include "../interface/Helper.h"
#include "../interface/Process.h"

using namespace std;
using namespace roast;

Process::Process()
{
   analyzed			= false;
   goodEventsForSignal.clear();
   plot				= false;
}

Process::Process(Process const & iProcess){

	goodEventsForSignal				= iProcess.GetGoodEventsForSignal();

    for (auto& pair: hContainerForSignal)
        delete pair.second;

	hContainerForSignal				= iProcess.GetHContainerForSignal();
	cutFlow							= CutFlow(*iProcess.GetCutFlow());
	normalizedCutFlow				= CutFlow(*iProcess.GetNormalizedCutFlow());

	shortName						= iProcess.GetShortName();
	niceName						= iProcess.GetNiceName();
	labelForLegend					= iProcess.GetLabelForLegend();
	type							= iProcess.GetType();
   treename = iProcess.GetTreeName();
	checkReality					= iProcess.CheckReality();
	ntuplePaths						= iProcess.GetNtuplePaths();
	color							= iProcess.GetColor();

	crossSection					= iProcess.GetCrossSection();
	branchingRatio					= iProcess.GetBranchingRatio();
	otherScaleFactor				= iProcess.GetOtherScaleFactor();
    relSysUncertainty               = iProcess.GetRelSysUncertainty();

	analyzed						= iProcess.Analyzed();
	NOEinDS							= iProcess.GetNOEinDS();
	NoEreadByNUTter					= iProcess.GetNoEreadByNUTter();
	NOEinNtuple						= iProcess.GetNOEinNtuple();
	NOEanalyzed						= iProcess.GetNOEanalyzed();
	NOEexpectedForSignal			= iProcess.GetNOEexpectedForSignal();
	plot							= iProcess.Plot();

	obtainedGoodEventsForSignal		= iProcess.ObtainedGoodEventsForSignal();
	filledHistosForSignal			= iProcess.FilledHistosForSignal();
	normalizedHistosForSignal		= iProcess.NormalizedHistosForSignal();

}

Process::Process(const std::string& name, const std::string& alias, const std::string& title,
      const std::string& type, const std::string& tree, const std::vector<std::string>& paths, int color,
      int ds_count, int nut_count, double xsec, double branch, bool genmatch):
   goodEventsForSignal(),
   shortName(name),
   niceName(alias),
   labelForLegend(title),
   type(type),
   treename(tree),
   checkReality(genmatch),
   ntuplePaths(paths),
   color(color),
   analyzed(false),
   plot(false),
   crossSection(xsec),
   branchingRatio(branch),
   otherScaleFactor(1.),
   NOEinDS(ds_count),
   NoEreadByNUTter(nut_count),
   NOEanalyzed(0),
   NOEexpectedForSignal(0),
   obtainedGoodEventsForSignal(false),
   filledHistosForSignal(false),
   normalizedHistosForSignal(false),
   relSysUncertainty(0.)
{
}

Process::~Process()
{
    for (auto& pair: hContainerForSignal)
        delete pair.second;
}

// Update process
void Process::Update(Process const * iProcess){

   treename = iProcess->GetTreeName();
	niceName						= iProcess->GetNiceName();
	labelForLegend					= iProcess->GetLabelForLegend();
	type							= iProcess->GetType();
	checkReality					= iProcess->CheckReality();
	color							= iProcess->GetColor();

	crossSection					= iProcess->GetCrossSection();
	branchingRatio					= iProcess->GetBranchingRatio();
	otherScaleFactor				= iProcess->GetOtherScaleFactor();
    relSysUncertainty               = iProcess->GetRelSysUncertainty();

	plot							= iProcess->Plot();

	normalizedHistosForSignal		= false;

	// Update cutflow
	cutFlow.SetCutCounts("Read from DS", iProcess->GetNOEinDS());
	cutFlow.SetCutCounts("skimming + PAT", iProcess->GetNoEreadByNUTter());
	
}


vector<Process::Event> const Process::GetGoodEventsForSignal() const { return goodEventsForSignal; }
void Process::SetCutFlow(CutFlow const & iCutFlow){ cutFlow	= CutFlow(iCutFlow); }
void Process::SetNormalizedCutFlow(CutFlow const & iCutFlow){ normalizedCutFlow	= CutFlow(iCutFlow); }
void Process::SetNOEanalyzed(double const iEvents){ NOEanalyzed = iEvents; }
void Process::SetNOEinNtuple(double const iEvents){ NOEinNtuple = iEvents; }
void Process::SetRelSysUncertainty(double const iError){ relSysUncertainty = iError; }
void Process::SetColor(int const iColor){ color = iColor; }
CutFlow* Process::GetCutFlow() { return &cutFlow; }
CutFlow const * Process::GetCutFlow() const { return &cutFlow; }
CutFlow* Process::GetNormalizedCutFlow() { return &normalizedCutFlow; }
CutFlow const * Process::GetNormalizedCutFlow() const { return &normalizedCutFlow; }
string const Process::GetShortName() const {	return shortName;		}
string const Process::GetNiceName() const {			return niceName;		}
string const Process::GetLabelForLegend() const {	return labelForLegend;	}
string const Process::GetType() const {				return type;			}
bool const Process::IsMC() const { return ((type.compare("mcBackground")==0) || (type.compare("signal")==0)); }
bool const Process::Plot() const { return plot; }
void Process::SetNtuplePaths(vector<string> const iPath){ ntuplePaths = iPath; }

bool const Process::IsCollisions() const { return ((type.compare("collisions")==0)); }
bool const Process::IsMCbackground() const { return ((type.compare("mcBackground")==0)); }
bool const Process::IsSignal() const { return ((type.compare("signal")==0)); }
bool const Process::CheckReality() const { return checkReality; }
vector<string> const Process::GetNtuplePaths() const { return ntuplePaths; }
int const Process::GetColor() const { return color; }
int const Process::GetNOEinDS() const {			return NOEinDS;		}
int const Process::GetNoEreadByNUTter() const {	return NoEreadByNUTter;}
int const Process::GetNOEinNtuple() const {		return NOEinNtuple; }
int const Process::GetNOEanalyzed() const {		return NOEanalyzed; }

double const Process::GetCrossSection() const{ return crossSection;}
double const Process::GetBranchingRatio() const{ return branchingRatio;}
double const Process::GetOtherScaleFactor() const{ return otherScaleFactor;}
double const Process::GetNOEexpectedForSignal() const{ return NOEexpectedForSignal;}
double const Process::GetRelSysUncertainty() const{ return relSysUncertainty;}
bool const Process::ObtainedGoodEventsForSignal() const{ return obtainedGoodEventsForSignal;}
bool const Process::FilledHistosForSignal() const{ return filledHistosForSignal;}
bool const Process::NormalizedHistosForSignal() const{ return normalizedHistosForSignal;}


void Process::SetShortName(string const iVal){ 
	shortName = iVal;
}
void Process::SetNiceName(string const iVal){ niceName = iVal; }
void Process::SetLabelForLegend(string const iVal){ labelForLegend = iVal; }
void Process::SetAnalyzed(){ analyzed = true; }
bool const Process::Analyzed() const { return analyzed; }

void
Process::ResetHistograms()
{
    for (auto& pair: hContainerForSignal)
        pair.second->GetHisto()->Reset("M");
}

void
Process::ScaleHistograms(double factor)
{
    for (auto& pair: hContainerForSignal)
        pair.second->ScaleBy(factor);
}

// Massive set histogram properties
void Process::SetGoodEventsForSignal(const vector<Process::Event>& iVector){ goodEventsForSignal = iVector; }


void
Process::NormalizeToLumi(double const iIntLumi)
{
	if ((!IsCollisions()) && (!normalizedHistosForSignal)) {
		double NOElumi				= iIntLumi*crossSection*branchingRatio;
		double NOEraw				= GetNOEinDS()*(GetNOEanalyzed()/(double)GetNOEinNtuple());	
		double lumiNormalization	= NOElumi/NOEraw;
		lumiNormalization 			*= GetOtherScaleFactor();
		cout << setprecision(7) << "\n" << shortName << " normalization: \n" 
			<< "\t---------------------------------------\n"
			<< "\tintLumi...." << iIntLumi << "\n" 
			<< "\tcrossSec..." << crossSection << "\n" 
			<< "\tBR............" << branchingRatio << "\n" 
			<< "\tNOEinDS......." << GetNOEinDS() << "\n" 
			<< "\tNOEanalyzed..." << GetNOEanalyzed() << "\n" 
			<< "\tNOEinNtuple..." << GetNOEinNtuple() << "\n" 
			<< "\t---------------------------------------\n"
			<< "\tNOElumi......." << NOElumi << "\n" 
			<< "\tNOEraw........" << NOEraw << "\n" 
			<< "\tOtherSF......." << GetOtherScaleFactor() << "\n" 
			<< "\t---------------------------------------\n"
			<< "\tSF............" << lumiNormalization << "\n" 
			<< "\trelSysSuncertainty....." << relSysUncertainty*100 << "%" << "\n" << endl; //*/

        // FIXME
                // ScaleBy(lumiNormalization);
		GetCutFlow()->RegisterCutFromLast("Lumi norm", 2, lumiNormalization);

      // FIXME
        // hContainerForSignal.AddRelErrorInQuadrature(relSysUncertainty);
	}
	normalizedHistosForSignal	= true;
}

void Process::BuildNormalizedCutFlow(){ normalizedCutFlow.BuildNormalizedCutFlow(&cutFlow); }

void Process::Add(Process* iProcess){
    for (const auto& p: iProcess->GetNtuplePaths())
        ntuplePaths.push_back(p);
    // FIXME
        // hContainerForSignal.Add(*(iProcess->GetHContainerForSignal()));
	cutFlow.Add(*(iProcess->GetCutFlow()));
	normalizedCutFlow.Add(*(iProcess->GetNormalizedCutFlow()));
}

ClassImp(roast::Process)
ClassImp(roast::Process::Event)
