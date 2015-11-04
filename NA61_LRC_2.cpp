/**************************************************************************************************************
* Author: Andrey Seryakov seryakov@yahoo.com

* --- Information about centrality classes (Emil 10.10.2013	emil.aleksander.kaptur@cern.ch)
* We take into consideration only first 28 mods of PSD with equal weights
* Determination of classes 
*			%				min PSD energy				max	PSD energy
* Be7+Be9 150A GeV/c
*	1.		0-5%			0							584.135				
*	2.		5-10%			584.135						657.519
*	3.		10-15%			657.519						719.161
*	4.		15-25%			719.161						798.416
*           20              773.1
* 
*
* BPD, FittedVertex, FitQuality, NVertexTracks, ZVertex, ImpactPoint,  cuts don't work for simEvents
*
**************************************************************************************************************/

//FIXME имена гистограмм psd а так же гистограммы катов.
//FIXME не заходить в sim петлю если нет simHandlers


#ifndef CONST
#define CONST
#include "NA61_LRC_2_Const.h"
#endif

#ifndef HANDLERLIST
#define HANDLERLIST
#include "NA61_LRC_2_Handler_List.h"
#endif

#ifndef CUTLIST
#define CUTLIST
#include "NA61_LRC_2_Cut_List.h"
#endif


#ifndef HANDLERS
#define HANDLERS
#include "NA61_LRC_2_Handler.h"
#endif

#ifndef CUT
#define CUT
#include "NA61_LRC_2_Cut.h"
#endif

#include <iostream>
#include <iomanip>
#include <fstream>

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
#endif



int main(int argc, char* argv[])
{
	// --- reading of filenames
	cout<<"START"<<endl;
	if (argc < 2) 
	{
		cerr << "usage: " << argv[0] << " <file list>" << endl;
		return 1;
	}

    cout << " reading file names from " << argv[1] << endl;
    ifstream fileList(argv[1]);
    
    if (!fileList.good() || !fileList.is_open())
    {
        cerr << " Error, cannot read file list " << argv[1] << endl;
        return 1;
    }

    //FIXME
    std::vector<std::string> offBranches;  
    offBranches.push_back("eventTree.fEvent.fRecEvent.fBeam");
    offBranches.push_back("eventTree.fEvent.fRawEvent.fBeam");
    std::vector<std::string> onBranches;
    onBranches.push_back("eventTree.fEvent.fRawEvent.fBeam");
    onBranches.push_back("eventTree.fEvent.fRecEvent.fBeam");
    
    // --- creating of handlers 
    // ----- List of handlers 
	HandlerList* HandList = new HandlerList();
	// ----- Handlers

	const int N0 = 0;
	OneWindHandler* arOneWindHandler[N0];
	for (int i = 0; i<N0; i++){
		TString name;
		TString nameBasic = "One_Wind";
		char name2[50];
		sprintf(name2, "_%i", i);
		name = nameBasic + name2;
		arOneWindHandler[i] = new OneWindHandler(name + "Raw.root", false);
		if (i==0 || i==1)
			arOneWindHandler[i]->AddRunNumberCut(0,20445);
		if (i==2 || i==3)
			arOneWindHandler[i]->AddRunNumberCut(20445,20500);
		arOneWindHandler[i]->AddStandardCutsRaw();
		if (i==1 || i==3)
			arOneWindHandler[i]->AddPSDEnergyCut(1850, e28Central);
		HandList->AddHandler(arOneWindHandler[i]);
	}
	
    const int N1=0;//1;
	PtNFluctuationHandler* arHandlerFlucPtNRaw[N1];
    for (int i =1; i<N1; i++){
        TString name;
        TString nameBasic="Full_FlucPt";
        char name2[50];
        sprintf(name2,"_%i",i);
        name = nameBasic + name2;
		arHandlerFlucPtNRaw[i] = new PtNFluctuationHandler(name + "Raw.root", false);
		arHandlerFlucPtNRaw[i]->AddStandardCutsRaw();


		//arHandlerFlucPtNRaw[i]->AddPSDEnergyCut(0, 80);
//		if (i == 1)
	//		arHandlerFlucPtNRaw[i]->AddAcceptRapidityCut();
//			arHandlerFlucPtNRaw[i]->AddPSDEnergyCut(0, 1850);
//		if (i == 1)
//			arHandlerFlucPtNRaw[i]->AddPSDEnergyCut(0, 4*beamMomentum);
//		if (i == 2)
//			arHandlerFlucPtNRaw[i]->AddPSDEnergyCut(0, 2600);
//		if (i == 3)
//			arHandlerFlucPtNRaw[i]->AddPSDEnergyCut(0, 17 * beamMomentum);


        HandList->AddHandler(arHandlerFlucPtNRaw[i]);
    }

	const int N2 = 0;//3;
	PSDFluctuationHandler* arHandlerFlucPSDRaw[N2];
	for (int i = 0; i<N2; i++){
		TString name;
		TString nameBasic = "Full_FlucPSD";
		char name2[50];
		sprintf(name2, "_%i", i);
		name = nameBasic + name2;
		arHandlerFlucPSDRaw[i] = new PSDFluctuationHandler(name + "Raw.root", false);
		arHandlerFlucPSDRaw[i]->AddStandardCutsRaw();
/*		if (i == 1)
			arHandlerFlucPSDRaw[i]->AddPSDEnergyCut(0, 4 * beamMomentum);
		if (i == 2)
			arHandlerFlucPSDRaw[i]->AddPSDEnergyCut(0, 12 * beamMomentum);
		if (i == 3)
			arHandlerFlucPSDRaw[i]->AddPSDEnergyCut(0, 17 * beamMomentum);
		*/
		HandList->AddHandler(arHandlerFlucPSDRaw[i]);
	}

	const int N3 =0;//21;
	PtNFluctuationHandler* arHandlerEtaFlucPtNRaw[N3];
	for (int i = 0; i<N3; i++){
		TString name;
		TString nameBasic = "Eta_FlucPtRaw";
		char name2[50];
		sprintf(name2, "_%i", i);
		name = nameBasic + name2;
		arHandlerEtaFlucPtNRaw[i] = new PtNFluctuationHandler(name + "Raw.root", false);
		arHandlerEtaFlucPtNRaw[i]->AddStandardCutsRaw();
		arHandlerEtaFlucPtNRaw[i]->AddCentrality(0, 0.2);
		//arHandlerEtaFlucPtNRaw[i]->AddEtaCut(2 + i*0.2, 2.5 + i*0.2);
		HandList->AddHandler(arHandlerEtaFlucPtNRaw[i]);
	}

	const int N4 = 0;
	TimeHandler* arTime[N4];
	for (int i = 0; i<N4; i++){
		TString name;
		TString nameBasic = "Time";
		char name2[50];
		sprintf(name2, "_%i", i);
		name = nameBasic + name2;
		arTime[i] = new TimeHandler(name + "Raw.root", false);
		arTime[i]->AddStandardCutsRaw();
//		arTime[i]->AddPSDCloudsCut();
        if (i==1)
    		arTime[i]->AddPSDEnergyCut(0, 1850, e28Central);
		HandList->AddHandler(arTime[i]);
	}

	const int N6 = 0;
	OneWindHandler* arOneWindHandler1[N6];
	for (int i = 0; i<N6; i++){
		TString name;
		TString nameBasic = "One_Wind";
		char name2[50];
		sprintf(name2, "_%i", i);
		name = nameBasic + name2;
		arOneWindHandler1[i] = new OneWindHandler(name + "Raw.root", false);
		arOneWindHandler1[i]->AddStandardCutsRaw();
//		arOneWindHandler1[i]->AddPSDEnergyCut(0, 2400);
		if (i == 1)
			arOneWindHandler1[i]->AddAcceptRapidityCut();

//		arOneWindHandler1[i]->AddBPDCut(0.2, BPD3);
		HandList->AddHandler(arOneWindHandler1[i]);
	}

	const int N7 = 0;
	LRCHandler* arLRCHandler[N7];
	for (int i = 0; i<N7; i++){
		TString name;
		TString nameBasic = "LRC_Wind";
		char name2[50];
		sprintf(name2, "_%i", i);
		name = nameBasic + name2;
		arLRCHandler[i] = new LRCHandler(name + "Raw.root", false);
		arLRCHandler[i]->AddStandardCutsRaw();

		arLRCHandler[i]->AddPSDCloudsCut();
		arLRCHandler[i]->AddEtaForward(5.5, 6);
		arLRCHandler[i]->AddEtaBackward(3.2 + i*0.2, 3.7 + i*0.2);
		HandList->AddHandler(arLRCHandler[i]);
	}

	const int N8 = 1;
	PSDHandler* arPSDHandler[N8];
	for (int i = 0; i<N8; i++){
		TString name;
		TString nameBasic = "PSD";
		char name2[50];
		sprintf(name2, "_%i", i);
		name = nameBasic + name2;
		arPSDHandler[i] = new PSDHandler(name + "Raw.root", false);
		arPSDHandler[i]->AddRunNumberCut(0,20445);
		arPSDHandler[i]->AddStandardCutsRaw();
		HandList->AddHandler(arPSDHandler[i]);
	}

	//FIXME: add information about energy and system type inside names of hists!

	cout<<"Initialization"<<endl;
	HandList->Init();
    	int i=0;
	time_t     now;
	struct tm  *ts;
	char       buf[80];
	int nFiles = 0;

    while (true){
        string fileName;
        fileList >> fileName;
        if(!fileList.good())
            break;
        cout << " ->processing " << fileName << endl;
        EventFile eventFile(fileName, eRead, Verbosity::eSilent, offBranches, onBranches);
        Event event;

        cout<<"event loop ... "<<endl;
        // --- loop over events
        
        while (eventFile.Read(event) == eSuccess){
			i++;
//			cout<<"event_event"<<i<<endl;
			// --- working status output
			cout<<i<<"\t";
			if(i%1000==0){
				// Get the current time
				now = time(NULL);          
				// Format and print the time, "ddd yyyy-mm-dd hh:mm:ss zzz" 
				ts = localtime(&now);
				strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", ts);
				printf("running event %5d time: %s\n",(int) i,buf);
			} 
			CutList::eventCutChecking.Reset();
//			cout<<"eat"<<endl;
            HandList->EatEvent(event);
        }
    }      

    delete HandList; 
    
	return 0;
}

void BaseHandler::AddStandardCutsRaw()
{
	this->Raw();
	this->AddT2Cut();
	if (systemType == ArSc)
		this->AddWFACut(-100, -200, 25);
	else 
		this->AddWFACut(-100, -200, 1);
//	this->AddChargeCut();
	this->AddBPDCut();
	this->AddFittedVtxCut();
	this->AddFitQualityCut();
	if (systemType == ArSc){
		if (beamMomentum == 150) {
			this->AddDirectBPDCut(-0.45, -0.12, -0.1, 0.6, BPD1);
			this->AddDirectBPDCut(-0.15, 0.15, -0.37, -0.01, BPD3);
			this->AddZVtxCut(-589.7, -569.7);
			this->AddPSDEnergyCut(2370, e16Central);
			this->AddPSDEnergyCut(800, 5000, e28Periferal);
			this->AddS5Cut(80);
		}
		if (beamMomentum == 13)
			this->AddZVtxCut(-590, -570);
		if (beamMomentum == 19)
			this->AddZVtxCut(-589.9, -569.9);
		if (beamMomentum == 30)
			this->AddZVtxCut(-589.9, -569.9);
		if (beamMomentum == 40)
			this->AddZVtxCut(-589.8, -569.8);
	}
	else
		this->AddZVtxCut(-590.3, -570.3);
//	this->AddPSDCloudsCut(garbageLowPSDLowN);
//	if (systemType == ArSc)
//		this->AddPSDCloudsCut();

	this->AddVtxTrackStatusCut();
//	this->AddChargeTrkCut(0);
	if (systemType != ArSc)
		this->AddVTPCAndGTPCCut(15, 5);
	else
		this->AddVTPCAndGTPCCut(15, 50);

	this->AddTotalTPCCut(30);
	this->AddImpactPointCut(4, 2);
	this->AddPtCut(0, 1.5);
	if (beamMomentum != 13){
		this->AddAcceptRapidityCut();
		if (systemType != ArSc)
			this->AddEECut();
	}
}

void BaseHandler::AddStandardCutsRec()
{
	this->Simulation();
	this->AddNVtxTracksCut(1);
	this->AddZVtxCut(-590.3, -570.3);

	this->AddVtxTrackStatusCut();
	this->AddChargeTrkCut(0);
	if (systemType != ArSc)
		this->AddVTPCAndGTPCCut(15, 5);
	this->AddTotalTPCCut(30);
	this->AddImpactPointCut(4, 2);
	this->AddPtCut(0, 1.5);
	this->AddAcceptRapidityCut();
	this->AddEECut();
}

void BaseHandler::AddStandardCutsSim()
{
	this->Simulation();
	this->AddChargeTrkCut(0);
	this->AddPtCut(0, 1.5);
	this->AddAcceptRapidityCut();
}