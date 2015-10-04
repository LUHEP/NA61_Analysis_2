#ifndef CUT
#define CUT
#include "NA61_LRC_2_Cut.h"
#endif

#ifndef CONST
#define CONST
#include "NA61_LRC_2_Const.h"
#endif

#ifndef SHINELIB
#define SHINELIB
#include <evt/Event.h>
#include <evt/RecEvent.h>
#include <evt/rec/Trigger.h>
#include <fwk/CentralConfig.h>
#include <utl/ShineUnits.h>
#include <io/EventFile.h>
#include <io/EventFileChain.h>
#include <utl/ShineUnits.h>
#include <evt/rec/RecEventConst.h>
#include <det/TriggerConst.h>
#include <utl/MathConst.h>
#include <utl/PhysicalConst.h>

#include <det/TPCConst.h>
#include <det/Target.h>
#include <det/TargetConst.h>
#include <det/PSD.h>
#include <det/BPD.h>
#include <det/Beam.h>
#include <det/Detector.h>
#include <evt/EventHeader.h>
#endif

#ifndef ROOTLIB
#define ROOTLIB
#include <TFile.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TH3.h>
#include <TCutG.h>
#include <TProfile.h>
#include <TList.h>
#include <TString.h>
#include <TMath.h>
#include <utl/Vector.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TLegend.h>
#include <THnSparse.h>

#endif

#include <iostream>
#include <fstream>


using namespace std; 
using namespace io;
using namespace utl;
using namespace evt;
using namespace evt::rec;

class CutChecking
{
public:
	CutChecking();
	~CutChecking(){}
	void Reset();
	void AddCut(TString nameCut, dataType type);
	checking CheckCut(TString nameCut, dataType type);
	void CheckedCut(TString nameCut, dataType type, checking status);


private:
	vector<TString> vtrNames;
	vector<dataType> vtrTypes;
	vector<checking> vtrStatus;
	unsigned int myLength;

	unsigned int lastSim;
	unsigned int lastRec;
};


class CutList
{
public:
    CutList();
    CutList(TString Name);
    CutList(Cut *cut);
    ~CutList();
    
    void AddCut(Cut* cut);
    int GetLength();
    Cut* GetValue(int index);
    bool EventTest(Event& ev, bool bSim);
    bool TrackTest(const evt::rec::VertexTrack& vtxTrack, Event& ev);
    bool TrackTest(const evt::sim::VertexTrack& vtxTrack, Event& ev);
	TH1F* GetStatHist(TString name, bool bSim);
	void SetFriendCutList(CutList* cutList) { myFriendCutList = cutList; }
    int myNTest;

	TString GetName();
    
	static CutChecking eventCutChecking;
	static CutChecking trackCutChecking;
private:
    Cut* myValue;
    CutList* myNext;
    TString myNameOutFile;
	CutList* myFriendCutList;
};

