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

#include <TRandom.h>
#include <vector>
#include <iostream>
#include <fstream>

#ifndef CENTRALITY
#define CENTRALITY
#include "NA61_LRC_2_Centrality.h"
#endif

using namespace std; 
using namespace io;
using namespace utl;
using namespace evt;
using namespace evt::rec;



class Cut
{
public:
    Cut(){myNEntries=0;}
    virtual ~Cut() {}
    virtual bool CheckEvent(Event& event, bool bSim) {}
    virtual bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack) {}
    virtual bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack) {}
    virtual TString GetName() {return my_Name;}
	virtual TString GetShortName() {return my_Short_Name;}
	virtual TString GetShortNameWithPar() {return my_Short_Name;}
    int GetNEntries() {return myNEntries;}
	void nEntriesPlusPlus() {myNEntries++;}
	void nEntriesMinusMinus() { myNEntries--; }
	virtual bool DoYouNeedMultiplicity() { return false; }
	virtual void SetMultiplicity(unsigned int mult) {}

protected:
    int myNEntries;
	TString my_Name;
	TString my_Short_Name;
};

class EventCut:public Cut
{
public:
    EventCut(){}
    virtual ~EventCut(){}
    virtual bool CheckEvent(Event& event, bool bSim) {}
	virtual TString GetShortNameWithPar() {}
protected:
};

class T2Cut:public EventCut
{
public:
    T2Cut();
	~T2Cut(){}
	TString GetShortNameWithPar() { return my_Short_Name;}
    bool CheckEvent(Event& event, bool bSim);
private:
    
};

class TriggerCut:public EventCut
{
public:
    TriggerCut(eTrigger trigger);
    ~TriggerCut(){}
    TString GetShortNameWithPar() { return my_Short_Name;}
    bool CheckEvent(Event& event, bool bSim);
private:
    eTrigger myTrigger;
};

class S1_1Cut:public EventCut
{
public:
    S1_1Cut();
    ~S1_1Cut(){}
	TString GetShortNameWithPar() { return my_Short_Name;}
    bool CheckEvent(Event& event, bool bSim);
private:
};

class WFACut:public EventCut
{
public:
    WFACut();
    WFACut(double wfa_Time1, double wfa_Time2, double wfa_TimeCut);
    ~WFACut(){}
    bool CheckEvent(Event& event, bool bSim);
	TString GetShortNameWithPar();
private:
    const double myWfaTime1;
    const double myWfaTime2;
    const double myWfaTimeCut;
};

class ChargeCut:public EventCut
{
public:
    ChargeCut();
    ChargeCut(string configPath); //???
    ~ChargeCut(){}
    bool CheckEvent(Event& event, bool bSim);
	TString GetShortNameWithPar() { return my_Short_Name;}
private:
    const string myConfigPath; 
	static TFile zFile;
	static TCutG* zCut;
};

class BPDCut:public EventCut
{
public:
    BPDCut();
    ~BPDCut(){}
	TString GetShortNameWithPar() { return my_Short_Name;}
    bool CheckEvent(Event& event, bool bSim);
private:
};

class DirectBPDCut :public EventCut
{
public:
	DirectBPDCut(double minX, double maxX, double minY, double maxY, eMyBPD bpd);
	~DirectBPDCut(){}
	TString GetShortNameWithPar();
	bool CheckEvent(Event& event, bool bSim);
private:
	DirectBPDCut();
	double myMinX;
	double myMaxX;
	double myMinY;
	double myMaxY;
	eMyBPD myBPD;
};

class MainVertexCut:public EventCut
{
public:
    MainVertexCut();
    ~MainVertexCut(){}
	TString GetShortNameWithPar() { return my_Short_Name;}
    bool CheckEvent(Event& event, bool bSim);
private:
};

class FittedVertexCut:public EventCut
{
public:
    FittedVertexCut();
    ~FittedVertexCut(){}
	TString GetShortNameWithPar() { return my_Short_Name;}
    bool CheckEvent(Event& event, bool bSim);
private:
};

class FitQualityCut:public EventCut
{
public:
    FitQualityCut();
    FitQualityCut(Int_t qual);
    ~FitQualityCut(){}
    bool CheckEvent(Event& event, bool bSim);
	TString GetShortNameWithPar();
    //char* GetName() {return "FitQual";}
private:
	const Int_t my_Quality;
};

class NVertexTracksCut:public EventCut
{
public:
    NVertexTracksCut();
    NVertexTracksCut(unsigned int nVtxTrack);
    ~NVertexTracksCut(){}
    bool CheckEvent(Event& event, bool bSim);
	TString GetShortNameWithPar();
private:
    const unsigned int myMinNVtxTrack;
};

class ZVertexCut:public EventCut
{
public:
    ZVertexCut();
    ZVertexCut(double minZ, double maxZ);
    ~ZVertexCut(){}
    bool CheckEvent(Event& event, bool bSim);
	TString GetShortNameWithPar();
private:
    const double myMinVtxZ;
    const double myMaxVtxZ;
};

class CentralityCut:public EventCut
{
public:
    CentralityCut(double lowPer, double upPer, bool bRaw);
    ~CentralityCut(){}
	void InitLegacyCentrality();
    bool CheckEvent(Event& event, bool bSim);
	TString GetShortNameWithPar();
private:
	CentralityCut();
    double myMinEPSD;
    double myMaxEPSD;
    const double myLowPer;
    const double myUpPer;
    LegacyCentrality* pDetermineCentrality;
	bool bInit;
};

class MultPSDCloudCut : public EventCut
{
public:
	MultPSDCloudCut(bool bRaw, ePSDNClouds eCloud);
	~MultPSDCloudCut(){}
	bool CheckEvent(Event& event, bool bSim);
	TString GetShortNameWithPar();
	bool DoYouNeedMultiplicity();
	void SetMultiplicity(unsigned int mult) { myMultiplicity = mult; }
private:
	MultPSDCloudCut();
	ePSDNClouds myECloud;
	static TCutG *myTCutG;
	bool bInit;
	unsigned int myMultiplicity;
};



class TrackCut:public Cut
{
public:
    TrackCut(){}
    virtual ~TrackCut(){}
    virtual bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack) {}
	virtual bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack) {}
	virtual TString GetShortNameWithPar() {}
private:
};

class VtxTrackStatusCut:public TrackCut
{
public:
    VtxTrackStatusCut();
    ~VtxTrackStatusCut(){}
    bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack);
    bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack);
	TString GetShortNameWithPar() { return my_Short_Name;}
private:
};

class ImpactPointCut:public TrackCut
{
public:
    ImpactPointCut();
    ImpactPointCut(double maxX, double maxY);
    ~ImpactPointCut(){}
    bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack);
    bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack);
	TString GetShortNameWithPar();
private:
    const double myMaxImpX;
    const double myMaxImpY;
};

class VTPCAndGTPCClustersCut:public TrackCut
{
public:
    VTPCAndGTPCClustersCut();
    VTPCAndGTPCClustersCut(unsigned int minVTPC, unsigned int minGTPC);
    ~VTPCAndGTPCClustersCut(){}
    bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack);
    bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack);
	TString GetShortNameWithPar();
private:
    const unsigned int myMinVTPCClusters;
    const unsigned int myMinGTPCClusters;
};

class TotalTPCClustersCut:public TrackCut
{
public:
    TotalTPCClustersCut();
    TotalTPCClustersCut(unsigned int minTotal);
    ~TotalTPCClustersCut(){}
    bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack);
    bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack);
	TString GetShortNameWithPar();
private:
    const unsigned int myMinTotalClusters;
};

class TOFCut:public TrackCut
{
public:
    TOFCut();
    ~TOFCut(){}
    bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack);
    bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack);
	TString GetShortNameWithPar() { return my_Short_Name;}
private:
};

class PtCut:public TrackCut
{
public:
    PtCut();
    PtCut(double min_Pt, double max_Pt);
    ~PtCut(){}
    bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack);
    bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack);
	TString GetShortNameWithPar();
private:
    const double myMaxPt;
    const double myMinPt;
};

class PCut:public TrackCut
{
public:
    PCut();
    PCut(double min_P, double max_P);
    ~PCut(){}
    bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack);
    bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack);
	TString GetShortNameWithPar();
private:
    const double myMaxP;
    const double myMinP;
};

class AcceptCut:public TrackCut
{
public:
    AcceptCut();
    AcceptCut(TString configPath); 
    ~AcceptCut();
    bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack);
    bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack);
	TString GetShortNameWithPar() { return my_Short_Name;}
private:
    const TString myConfigPath; 
    TH3D* itsAcceptHist;

	double_t myXAxLowEdge, myYAxLowEdge, myZAxLowEdge;
	double_t myXAxBinWidth, myYAxBinWidth, myZAxBinWidth;
	unsigned int myNXBins, myNYBins, myNZBins;
	
};

class AcceptRapidityCut :public TrackCut
{
public:
	AcceptRapidityCut();
	~AcceptRapidityCut();
	bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack);
	bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack);
	TString GetShortNameWithPar() { return my_Short_Name; }
private:
	TString myPath;
	//TH3C* myAcceptHist;
	TH3F* myAcceptHist;


	double myXAxLowEdge, myYAxLowEdge, myZAxLowEdge;
	double myXAxBinWidth, myYAxBinWidth, myZAxBinWidth;
	double myNXBins, myNYBins, myNZBins;
};


class EtaCut:public TrackCut
{
public:

	EtaCut(double minEta, double maxEta);
	~EtaCut(){}
    bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack);
    bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack);
	TString GetShortNameWithPar();
private:
	EtaCut();
	const double myMinEta;
	const double myMaxEta;
};

class PhiCut:public TrackCut //phi = [-pi;pi] 
{
public:

	PhiCut(double minPhi, double maxPhi);
	~PhiCut(){}
    bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack);
    bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack);
	TString GetShortNameWithPar();
private:
	PhiCut();
	const double myMinPhi;
	const double myMaxPhi;
};

class ChargeTrCut: public TrackCut
{
public:
	ChargeTrCut(int redCh);
	~ChargeTrCut(){}
    bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack);
    bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack);
	TString GetShortNameWithPar();
private:
	ChargeTrCut();
	const int myCharge;

};

class DiluteCut: public TrackCut
{
public:
	DiluteCut(double remove_track_percent);
	~DiluteCut(){}
    bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack);
    bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack);
	TString GetShortNameWithPar();

private:
	DiluteCut();
	double my_remove_track_percent;
};

class eeCut : public TrackCut
{
public:
	eeCut();
	~eeCut(){}
	bool CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack);
	bool CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack);
	TString GetShortNameWithPar();

private:
	static TCutG *myTCutG;
};

class StrangeCut: public TrackCut
{
public:
    StrangeCut();
//     ~StrangeCut();
    bool CheckTrack ( SimEvent& simEvent, const sim::VertexTrack& vtxTrack );
    bool CheckTrack ( RecEvent& recEvent, const VertexTrack& vtxTrack ) {};
    TString GetShortNameWithPar();
private:
    static const int strangeIdList[];
    static const int strangeIdListLength;
};

class PSDEnergyCut :public EventCut
{
public:
	PSDEnergyCut(double lowE, double upE, ePSDModulCombinations ePSDSet, bool bRaw);
	~PSDEnergyCut(){}
	bool CheckEvent(Event& event, bool bSim);
	TString GetShortNameWithPar();

private:
	PSDEnergyCut();
	double myMinEPSD;
	double myMaxEPSD;
	bool myBRaw;
	ePSDModulCombinations myPSDModSet;
	bool myPSDModArray[45];
};

class S5Cut :public EventCut
{
public:
	S5Cut(double upLimit, bool bRaw);
	~S5Cut(){}
	bool CheckEvent(Event& event, bool bSim);
	TString GetShortNameWithPar();

private:
	S5Cut();
	double myUpLimit;
	bool myBRaw;
};

//Run number cut, for example if you want to analyse a special set of runs
class RunNumberCut : public EventCut
{
public:
    RunNumberCut(int lowLimit, int upLimit, bool bRaw);
    ~RunNumberCut(){}
    bool CheckEvent(Event& event, bool bSim);
    TString GetShortNameWithPar();

private:
    RunNumberCut();
    int myLowLim;
    int myUpLim;
    bool myBRaw;
};

// to exclude events with not working PSD modules
// написан, конечно, совершенно не оптимально =/
class RunWith0EnergyInOneModuleCut : public EventCut
{
public:
    RunWith0EnergyInOneModuleCut(bool bRaw);
    ~RunWith0EnergyInOneModuleCut(){};
    bool CheckEvent(Event& event, bool bSim);
    TString GetShortNameWithPar();

private:
    RunWith0EnergyInOneModuleCut();
    bool myBRaw;

};

class RunWith0EnergyInOneModuleCutVer2 : public EventCut
{
public:
    RunWith0EnergyInOneModuleCutVer2(bool bRaw, ePSDModulCombinations ePSDSet);
    ~RunWith0EnergyInOneModuleCutVer2(){};
    bool CheckEvent(Event& event, bool bSim);
    TString GetShortNameWithPar();

private:
    RunWith0EnergyInOneModuleCutVer2();
    bool myBRaw;
    ePSDModulCombinations myPSDModSet;
    bool myPSDModArray[45];

};

class PSDTimeStampCut : public EventCut
{
public:
    PSDTimeStampCut(bool bRaw, unsigned int maxSectionsWith0);
    ~PSDTimeStampCut(){}
    bool CheckEvent(Event& event, bool bSim);
    TString GetShortNameWithPar();

private:
    PSDTimeStampCut();
    bool myBRaw;
    unsigned int myMaxOkNSectionsWith0;
};

class BadRunCut : public EventCut
{

public:
    BadRunCut(bool bRaw);
    ~BadRunCut(){}
    bool CheckEvent(Event& event, bool bSim);
    TString GetShortNameWithPar();

private:
    BadRunCut();
    bool myBRaw;
    const int* myArBadRuns;
    int myNBadRuns;
};

class BeamSlopeCut : public EventCut
{
public:
    BeamSlopeCut(double minSlope, double maxSlope, eBeamSlopePlane plane, bool bRaw);
    ~BeamSlopeCut() {}
    bool CheckEvent(Event& event, bool bSim);
    TString GetShortNameWithPar();

private:
    BeamSlopeCut();
    bool myBRaw;
    double myMinSlope;
    double myMaxSlope;
    eBeamSlopePlane myPlane;
};

class BPD3InteractionCut : public EventCut
{
public:
    BPD3InteractionCut(double minX, double maxX, double minY, double maxY, bool bRaw);
    ~BPD3InteractionCut(){}
    bool CheckEvent(Event& event, bool bSim);
    TString GetShortNameWithPar() { return my_Short_Name;}
private:
    BPD3InteractionCut();
    bool myBRaw;
    double myMinXValue;
    double myMaxXValue;
    double myMinYValue;
    double myMaxYValue;
};

class NTracksCut : public EventCut //all tracks
{
public:
    NTracksCut(int min, int max);
    ~NTracksCut(){}
    bool CheckEvent(Event& event, bool bSim);
    TString GetShortNameWithPar() { return my_Short_Name;}
private:
    NTracksCut();
    int myNMin;
    int myNMax;
};

class NFittedVtxTracksCut : public EventCut //tracks which were used for fitting main vtx
{
public:
    NFittedVtxTracksCut(int min, int max);
    ~NFittedVtxTracksCut(){}
    bool CheckEvent(Event& ev, bool bSim);
    TString GetShortNameWithPar() { return my_Short_Name;}
private:
    NFittedVtxTracksCut();
    int myNMin;
    int myNMax;
};

class TrackVtxFittedTrackRatioCut : public EventCut //ratio of tracks which were used for fitting main vtx, to all tracks
{
public:
    TrackVtxFittedTrackRatioCut(double minRatio);
    ~TrackVtxFittedTrackRatioCut(){}
    bool CheckEvent(Event& ev, bool bSim);
    TString GetShortNameWithPar() { return my_Short_Name;}
private:
    TrackVtxFittedTrackRatioCut();
    double myMinRatio;
};

class TrackVtxFittedTrackRatioCutWithFittedVtxTrackBounds : public EventCut // merging of two previous cuts
{
public:
    TrackVtxFittedTrackRatioCutWithFittedVtxTrackBounds(double minRatio, int min, int max);
    ~TrackVtxFittedTrackRatioCutWithFittedVtxTrackBounds(){}
    bool CheckEvent(Event& ev, bool bSim);
    TString GetShortNameWithPar() { return my_Short_Name;}
private:
    TrackVtxFittedTrackRatioCutWithFittedVtxTrackBounds();
    double myMinRatio;
    int myNMin;
    int myNMax;
};

class  StrongBPDCut:public EventCut
{
public:
    StrongBPDCut();
    ~StrongBPDCut(){}
    bool CheckEvent(Event& ev, bool bSim);
    TString GetShortNameWithPar() { return my_Short_Name;}
private:
};










