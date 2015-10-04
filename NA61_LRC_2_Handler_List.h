#ifndef HANDLERS
#define HANDLERS
#include "NA61_LRC_2_Handler.h"
#endif

#ifndef CUTLIST
#define CUTLIST
#include "NA61_LRC_2_Cut_List.h"
#endif

#ifndef CUT
#define CUT
#include "NA61_LRC_Cut.h"
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


class HandlerList
{
public:
	HandlerList();
	HandlerList(BaseHandler* handler);
	~HandlerList();

	void AddHandler(BaseHandler* handler);
	int GetLength();
	BaseHandler* GetValue(int index);
	
	void EatEvent(Event& ev);
	void EndOfEvent(Event& ev);
	void Init();

private:
	bool init;
	bool *pGoodEvent;
	BaseHandler* myValue;
	HandlerList* myNext;
	bool hasSimHandler;
	bool hasRecHandler;
	bool hasRawHandler;
};




















