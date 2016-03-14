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
#include <evt/EventHeader.h>

#include <det/TPCConst.h>
#include <det/Target.h>
#include <det/TargetConst.h>
#include <det/PSD.h>
#include <det/BPD.h>
#include <det/Beam.h>
#include <det/Detector.h>
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

#ifndef CUTLIST
#define CUTLIST
#include "NA61_LRC_2_Cut_List.h"
#endif

#include <iostream>
#include <fstream>

using namespace std; 
using namespace io;
using namespace utl;
using namespace evt;
using namespace evt::rec;

class StatEventInfo
{
public:
	StatEventInfo() { myReset == false; this->Reset(); }
	~StatEventInfo() {}
	void Reset();
	void DissectEvent(Event &event);	
	
	double_t myFitVtxZ;
	double_t myFitVtxX;
	double_t myFitVtxY;
	double_t myBPD[6];
    double_t myBeamSlopeZX;
    double_t myBeamSlopeZY;
    double_t myBeamPositionAtPSDX;
    double_t myBeamPositionAtPSDY;

    double_t myBeamPositionAtS1X;
    double_t myBeamPositionAtS1Y;
    double_t myBeamPositionAtS2X;
    double_t myBeamPositionAtS2Y;
    double_t myBeamPositionAtS5X;
    double_t myBeamPositionAtS5Y;

	double_t myEnergyPSD;

    vector<double> myTimeStructureWFA;
    vector<double> myTimeStructureWFAT4;
    vector<double> myTimeStructureMHTDC;
	unsigned int myRunNumber; 
	double_t myS5ADC;
    double_t myS1ADC;
    double_t myS2ADC;

    double_t myBPD3SignalX;
    double_t myBPD3SignalY;
    double_t myBPD3SumAllX;
    double_t myBPD3SumAllY;
    double_t myBPD3XRMS;
    double_t myBPD3YRMS;

    double_t myEnergyPSD44;
    double_t myEnergyPSD16;
    double_t myEnergyPSD28per;

private:	
	bool myReset;
};

class StatPSDInfo
{
public:
	StatPSDInfo() { myReset == false; this->Reset(); }
	~StatPSDInfo(){}
	void Reset();
	void DissectEvent(Event &event);

	double_t myEnergyPSD;
private:
	bool myReset;
};


class BaseHandler
{
public:
	BaseHandler(const char* nameOut);
	BaseHandler(const char* nameOut, bool simE);
	void Raw();
	virtual void Simulation();
	bool IsItRaw();
	virtual ~BaseHandler() {}
	virtual void EndOfEvent(Event& ev) {}
	virtual void PutTrack(const evt::rec::VertexTrack& vtxTrack, Event& ev) {}
	virtual void PutTrack(const evt::sim::VertexTrack& vtxTrack, Event& ev) {}
	virtual void Init() {}
	bool EventTest(Event& ev);
    void SetEventNumber(int nEvent); /// Does it exist?
	bool GetSim() {return bSim;}

	void AddEventCut(EventCut* cut);
	void AddTrackCut(TrackCut* cut);

	void AddTrigger(eTrigger trigger);
	void AddT2Cut();
	void AddS1_1Cut();
	void AddWFACut();
	void AddWFACut(double wfa_Time1, double wfa_Time2, double wfa_TimeCut);
    void AddWFAT4Cut(double wfa_Time1, double wfa_Time2, double wfa_TimeCut);
	void AddChargeCut();
	void AddBPDCut();
	void AddStrongPBDCut();
	void AddDirectBPDCut(double minX, double maxX, double minY, double maxY, eMyBPD bpd);
	void AddMainVtxCut();
	void AddFittedVtxCut();
	void AddFitQualityCut();
	void AddFitQualityCut(Int_t qual);
	void AddNVtxTracksCut();
	void AddNVtxTracksCut(unsigned int nVtxTracks);
	void AddZVtxCut();
	void AddZVtxCut(double_t minZ, double_t maxZ);
    void AddCentrality(int minPercent, int maxPercent);
	void AddPSDEnergyCut(double maxE, ePSDModulCombinations psdSet);
	void AddPSDEnergyCut(double minE, double maxE, ePSDModulCombinations psdSet);
	void AddPSDCloudsCut();
	void AddPSDCloudsCut(ePSDNClouds eCloudType);
	void AddS5Cut(double upLimit);
    void AddS2Cut(double upLimit);
	void AddRunNumberCut(int lowLimit, int upLimit);
	void Remove0EPSDEvents(ePSDModulCombinations ePSDMod);
	void RemoveBadRuns();
    void AddPSDTimeStampCut(unsigned int maxOkSectionsWith0);
	void AddBeamSlopeCut(double minSlope, double maxSlope, eBeamSlopePlane slope);
    void AddBPD3ClusterSignalCut(double minX, double maxX, double minY, double maxY);
    void AddNTrackCut(int min, int max);
    void AddNFittedVtxTrackCut(int min, int max);
    void AddTrackVtxFittedTrackRatioCut(double minRation);
    void AddLocalRatioCut(double minRatio, int minBoundOfUsing, int maxBoundOfUsing);
	void AddZeroPositiveTracksCut();
    void AddBeamPositionInSCut(eMyS sType, eMyCoordinate coord, double lowBound, double upBound);
    void AddBPD3RMS();
    void Remove0BinsEvents();
	void AddFPGACut(ePSDModulCombinations comb);
	void AddS5TracksCloudCut(double S5, double maxN, double maxPSD28, eMultiplicityType multType);

    void AddVtxTrackStatusCut();
	void AddImpactPointCut();
	void AddImpactPointCut(double_t max_dX, double_t max_dY);
	void AddVTPCAndGTPCCut();
	void AddVTPCAndGTPCCut(unsigned int minVTPC, unsigned int minGTPC);
	void AddTotalTPCCut();
	void AddTotalTPCCut(unsigned int minN);
	void AddTOFCut();
	void AddPtCut(); 
	void AddPtCut(double_t minPt, double_t maxPt); 
	void AddPCut();
	void AddPCut(double_t minP, double_t maxP);
    void AddAcceptCut();
	void AddAcceptRapidityCut(double minEfficiency);
    void AddAcceptRapidityCut(eAcceptanceType acType,double minEfficiency);
	void AddEECut();
	void AddEtaCut(double_t minEta, double_t maxEta);
    void AddRapidityCut(double_t min, double_t max);
	void AddPhiCut(double_t minPhi, double_t maxPhi);
	void AddChargeTrkCut(int charge);
	void AddDiluteCut(double remove_track_percent);
	void AddStrangeCut();



	void AddStandardCutsRaw(); //declarations in the main file
	void AddStandardCutsRec();
	void AddStandardCutsSim();

	CutList* GetTrackCutList(){ return myTrackCutList; }
	bool		init; //???

protected:
	BaseHandler();
    int mynEvent;
	
	TString nameOutFile;
	TString myNameHist;

	bool		bSim;
	bool		bRaw;

	int myHistInSparseCounter; 

	CutList* myTrackCutList;
	CutList* myEventCutList;

	unsigned int nTracks0, nTracks1;
	
private:

};

//bool PhiGeneralization(double_t& minPhi, double_t& maxPhi);

class OneWindHandler:public BaseHandler
{
public:
	OneWindHandler(const char* nameOutFile);
	OneWindHandler(const char* nameOutFile, bool simE);
	~OneWindHandler();

	void PutTrack(const VertexTrack& vtxTrack, Event& ev);
	void PutTrack(const evt::sim::VertexTrack& vtxTrack, Event& ev);
	void EndOfEvent(Event& ev);
	void Init();
	
	static StatEventInfo myEventInfo;
	static bool bEventInfoFull;

private:
	OneWindHandler();
	// --- list of hists
	TH1D 	*psdEnergyHist, *psdEnergyHist16;
	TH1D	*chargeHist,	*multHist,
//   all charges	  neg		 pos
		*pHist,		*pHist0,	*pHist1,
		*pxHist,	*pxHist0,	*pxHist1,
		*pyHist,	*pyHist0,	*pyHist1,
		*pzHist,	*pzHist0,	*pzHist1,
		*ptHist,	*ptHist0,	*ptHist1,
		*etaHist,	*etaHist0,	*etaHist1,
		*rapHist,	*rapHist0,	*rapHist1,
		*phiHist,	*phiHist0,	*phiHist1;
    
    TH2D  /*plusminusHist,*/ *etaptHist, *etaptHist0, *etaptHist1;
	TH2D  *rapPtHist, *rapPtHist0, *rapPtHist1;
    TH2D  *ptPhiHist, *ptPhiHist0, *ptPhiHist1;
    TH2D  *BPD1Hist, *BPD2Hist, *BPD3Hist, *ImpParHist;
    
    TH1D  *WFAHist;
	TH1D  *WFAT4Hist;
    TH1D  *MHTDCHist;
	TH2D  *dEdxPHist0, *dEdxPHist1, *fitVtxNHist; //Ya pomenyal
    
    TH2D	*psdEnergyNHist;
	TH2D	*psdEnergyTracksInFitHist;
	TH2D	*psdEnergyAllTracksHist;
	TH2D	*psdEnergyS5Hist;
	TH2D	*multS5Hist;
    TH2D    *S1S2Hist;
    TH1D	*totClusterHist, *verClusterHist, *gapClusterHist; //Ya pomenyal
    TH1D    *nBeamHist; //Ya pomenyal

    TH2D    *BPD1XMultHist, *BPD1YMultHist,
			*BPD2XMultHist, *BPD2YMultHist,
            *BPD3XMultHist, *BPD3YMultHist,
            *SlopeBeamXZHist, *SlopeBeamYZHist,
	*BeamPositionHistPSDLevelHist;

	TH2D    *VtxTracksVsAllTracksHist;

    TH2D    *BPD3XSignalNVtxTracks;
    TH2D    *BPD3YSignalNVtxTracks;
    TH2D    *BPD3XSignalPSD44;
    TH2D    *BPD3YSignalPSD44;
    TH2D    *PSD16c28p;

    TH2D    *BPD3SignalRatioXTrackRatio;
    TH2D    *BPD3SignalRatioYTrackRatio;
    TH2D    *BPD3SignalDifXTrackRatio;
    TH2D    *BPD3SignalDifYTrackRatio;

    TH2D    *BeamPositionS1LevelHist;
    TH2D    *BeamPositionS2LevelHist;
    TH2D    *BeamPositionS5LevelHist;
	TH2D    *BeamPositionS1XSignalLevelHist;
	TH2D    *BeamPositionS1YSignalLevelHist;
	TH2D    *BeamPositionS2XSignalLevelHist;
	TH2D    *BeamPositionS2YSignalLevelHist;
	TH2D    *BeamPositionS5XSignalLevelHist;
	TH2D    *BeamPositionS5YSignalLevelHist;
	TH3D	*BeamPositionS1vsSignalHist;
	TH3D	*BeamPositionS2vsSignalHist;
	TH3D	*BeamPositionS5vsSignalHist;
    TH2D    *BPD3RMSHist;

 //   TH3D    *acceptHist;
    
	// --- counters
	int nTracks;
//    int nTracks1, nTracks0;
};

class LRCHandler:public BaseHandler
{
public:
	LRCHandler(const char* nameOutFile);
	LRCHandler(const char* nameOutFile, bool simE);
	~LRCHandler();
	
	void PutTrack(const VertexTrack& vtxTrack, Event& ev);
	void PutTrack(const evt::sim::VertexTrack& vtxTrack, Event& ev);
	void EndOfEvent(Event& ev);
	void Init();
	
	void AddForwardCut(TrackCut* cut);
	void AddBackwardCut(TrackCut* cut);

	void AddPhiForward(double_t minPhi, double_t maxPhi);
	void AddPhiBackward(double_t minPhi, double_t maxPhi);
	void AddEtaForward(double_t minEta, double_t maxEta);
	void AddEtaBackward(double_t minEta, double_t maxEta);
	void AddPtForward(double_t minPt, double_t maxPt);
	void AddPtBackward(double_t minPt, double_t maxPt);

private:
	LRCHandler();
	// --- list of hists 
	TH2D	*NetChHist;	
	TH2D	*TestPtPtHist;
/*	//	all				neg				pos				neg-pos			pos-neg		
		*NNHist,		*NNHist0,		*NNHist1,		*NNHist01,		*NNHist10,
		*PtNHist,		*PtNHist0,		*PtNHist1,		*PtNHist01,		*PtNHist10,
		*PtPtHist,		*PtPtHist0,		*PtPtHist1,		*PtPtHist01,	*PtPtHist10; */

	THnSparseD *myAllChargeTHSparse;
	THnSparseD *myPositiveTHSparse;
	THnSparseD *myNegativeTHSparse;
	THnSparseD *myPosBackNegForTHSparse;
	THnSparseD *myNegBackPosForTHSparse;
	
	// --- counters
	unsigned int			nTracksB,	nTracksB0,	nTracksB1,	nTracksF,	nTracksF0,	nTracksF1, shalala;
	int						chargeB,		chargeF;
	double_t	sumPtB,		sumPtB0,	sumPtB1,	sumPtF,		sumPtF0,	sumPtF1;

	// --- windows 
	double_t	myMinEtaB,	myMaxEtaB,	myMinEtaF,	myMaxEtaF,
				myMinPhiB,	myMaxPhiB,	myMinPhiF,	myMaxPhiF;
	bool phiWindow;

	CutList* myForwardTrackCutList;
    CutList* myBackwardTrackCutList;
};

class PtNFluctuationHandler :public BaseHandler
{
public:
	PtNFluctuationHandler(const char* nameOutFile);
	PtNFluctuationHandler(const char* nameOutFile, bool simE);
	~PtNFluctuationHandler();

	void PutTrack(const VertexTrack& vtxTrack, Event& ev);
	void PutTrack(const evt::sim::VertexTrack& vtxTrack, Event& ev);
	void EndOfEvent(Event& ev);
	void Init();

private:
	PtNFluctuationHandler();

//	unsigned int nTracks0, nTracks1;
	double sumPt0, sumPtsqr0, sumPt1, sumPtsqr1;

	THnSparseD *myAllChargeTHSparse;
	THnSparseD *myPositiveTHSparse;
	THnSparseD *myNegativeTHSparse;
	THnSparseD *myPosPtNegNTHSparse;
	THnSparseD *myNegPtPosNTHSparse;
};

class PSDFluctuationHandler :public BaseHandler
{
public:
	PSDFluctuationHandler(const char* nameOutFile);
	PSDFluctuationHandler(const char* nameOutFile, bool simE);
	~PSDFluctuationHandler();

	void PutTrack(const VertexTrack& vtxTrack, Event& ev);
	void PutTrack(const evt::sim::VertexTrack& vtxTrack, Event& ev) {}
	void PutPSDEnergy(Event& ev);
	void EndOfEvent(Event& ev);
	void Init();
	void SetBeamEnergy(double E) { beamEnergy = E; }
	void Simulation();
//	void SetNModules(unsigned int nMod);

private:
	PSDFluctuationHandler();

	THnSparseD *myAllChargeTHSparse;
	THnSparseD *myPositiveTHSparse;
	THnSparseD *myNegativeTHSparse;
	TH1D*		mySpectEnergyHist;

	unsigned int nEvents; 
	double sumPt0, sumPt1;
	double sumEnergyPSD;
	double beamEnergy;
	bool bEnergyIsWriten;
};

/*class AcceptanceHandler :public BaseHandler
{
public:
	AcceptanceHandler(const char* nameOutFile);
	~AcceptanceHandler();

	BaseHandler* GetRecHandlerForCuts();
	void PutTrack(const evt::sim::VertexTrack& vtxTrack, Event& ev);
	void EndOfEvent(Event& ev);
	void Init();

private:
	TH3D *mySimHist;
	TH3D *myRecHist;

	BaseHandler *myRecHandlerForCuts;
};
*/

class TimeHandler :public BaseHandler
{
public:
	TimeHandler(const char* nameOutFile);
	TimeHandler(const char* nameOutFile, bool simE);
	~TimeHandler();

	void PutTrack(const VertexTrack& vtxTrack, Event& ev);
	void PutTrack(const evt::sim::VertexTrack& vtxTrack, Event& ev);
	void EndOfEvent(Event& ev);
	void Init();

	static StatEventInfo myEventInfo;
	static bool bEventInfoFull;
private:
	TimeHandler();
	// --- list of hists
	TH2D*	psdEnergyHist;
	TH2D	*chargeHist,	*multHist,
		*pHist, *ptHist,	*phiHist;
    TH2D  *fitVtxHistX, *fitVtxHistY, *fitVtxHistZ;
//	TH2D  *fitVtxHistXverMult, *fitVtxHistYverMult;
};

// --- search of correlations between different PSD modules
class PSDHandler : public BaseHandler
{
public:
	PSDHandler(const char* nameOut);
	PSDHandler(const char* nameOut, bool sim);
	~PSDHandler();

	void PutTrack(const evt::rec::VertexTrack& vtxTrack, Event& ev);
	void PutTrack(const evt::sim::VertexTrack& vtxTrack, Event& ev);
	void EndOfEvent(Event& ev);
	void Init();

private:
	PSDHandler();
	THnSparseD* myModuleSparse;
};

class PSD0BinsFindHandler:public BaseHandler
{
public:
	PSD0BinsFindHandler(const char* nameOut);
	PSD0BinsFindHandler(const char* nameOut, bool sim);
	~PSD0BinsFindHandler();
	void PutTrack(const evt::rec::VertexTrack& vtxTrack, Event& ev)
	{this->Init();}
	void PutTrack(const evt::sim::VertexTrack& vtxTrack, Event& ev)
	{this->Init();}
	void EndOfEvent(Event& ev);
	void Init();

private:
	PSD0BinsFindHandler();
	TH2D *iRunsIEventsHists[nPSDModules];
    TH1D *myEnergyInModulesWith0Status;
    TH1D *myEnergyInModulesWithNon0Status;
};