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

#include <iostream>
#include <fstream>

using namespace std; 
using namespace io;
using namespace utl;
using namespace evt;
using namespace evt::rec;
using namespace fwk;

// type of data 
//const bool realData = true;

const int beamMomentum = 150;
const double dBeamMomentum = beamMomentum; //we stil need the double and the int varialbes
const double pMass = 0.938; 
const double beamRapidity = 0.5*log((sqrt(pow(pMass, 2) + dBeamMomentum*dBeamMomentum) + dBeamMomentum) /
	(sqrt(pow(pMass, 2) + dBeamMomentum*dBeamMomentum) - dBeamMomentum));

enum eSystemType{pp, BeBe, ArSc, PbPb};
const eSystemType systemType = ArSc;
enum eMyBPD{BPD1, BPD2, BPD3};

// Change it to your path
const TString configPath = "/afs/cern.ch/work/a/aseryako/Runs_BeBe_158_TargetIN";
//const TString configPath =	 "/afs/cern.ch/work/a/aseryako/lxbutch/fromZorg";
const string str_configPath ="/afs/cern.ch/work/a/aseryako/Runs_BeBe_158_TargetIN"; 
//const string str_configPath = "/afs/cern.ch/work/a/aseryako/lxbutch/fromZorg";

// --- PSD consts
const Int_t nPSDModules = 44;
const Int_t nPSDMods = 28; 
const double_t class1Min = 0; 
const double_t class2Min = 584.135;
const double_t class3Min = 657.519;
const double_t class4Min = 719.161;
const double_t class5Min = 798.416; 
const double_t maxPSDEnergy = 2000;

// --- "standart" eta windows
const double_t etaBUp   = 4;
const double_t etaBDown = 3.4; 
const double_t etaFUp   = 6;
const double_t etaFDown = 5;


// --- EVENT CUTS 
const unsigned int nMinGoodVTPCTracks = 3;
const unsigned int	nMinVertexTracks	= 3;
const unsigned int	nMinVTPCClusters	= 15;
const unsigned int	nMinGTPCClusters	= 5;
const unsigned int	nMinMTPCClusters	= 10;
const double_t		minZVertex			= -581.5;
const double_t		maxZVertex			= -579.1;

// --- TRACK CUTS
const Int_t track_Quality = 0;
const double_t maxImpactX	= 4*cm;
const double_t maxImpactY	= 2*cm;
const unsigned int nMinTotalClusters = 15;
const double_t maxPt	= 1.5;
const double_t minP		= 0.3;
const double_t minPt	= 0;
const double_t maxP		= 200;

// дл€ wfa катов от Ёмил€
const double_t wfaTime1 = -100;
const double_t wfaTime2 = -200;
const double_t wfaTimeCut = 1.5;

enum dataType {SIM,REC,RAW};
enum checking {unknown, fail, pass}; 

// --- For LRC THnSparce 

// NF NB PtF PtB 
const int nBins = 5;
const int nChargeComb = 5;
const Int_t arNBins[nBins] =   {301,301,		31,31,		20};
const Double_t arXmin[nBins] = {-0.5,-0.5,		0,0,		-0.5};
const Double_t arXmax[nBins] = {300.5,300.5,	1.5,1.5,	19.5};
//Double_t arValLRC[nBins] = {0,0,0,0, 0,0,0,0,		0,0};

// N Pt Pt2 
const int nBinsPtNFluc = 4;
const int arNBinsPtNFluc[nBinsPtNFluc]	= { 301, 600, 600, 30 };
const double arXminPtNFluc[nBinsPtNFluc] = { -0.5, 0, 0, -0.5 };
const double arXmaxPtNFluc[nBinsPtNFluc] = { 300.5, 1.5, 1.5, 29.5 };

// N Pt Ebiam-EPSD
const int nBinsPSDFluc = 4;
const int nChargeCombPSD = 3;
const int arNBinsPSDFluc[nBinsPSDFluc]	 = { 301, 600, 1700, 30 };
const double arXminPSDFluc[nBinsPSDFluc] = { -0.5, 0, -2000, -0.5 };
const double arXmaxPSDFluc[nBinsPSDFluc] = { 300.5, 1.5, 6500, 29.5 };

// --- const for PSDHanler
// iModule PSD energy
const int nBinsPSDModules = nPSDModules + 1;
const int arNBinsPSDModules[nBinsPSDModules] = {151,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
												1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
												1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
const double arXminPSDModules[nBinsPSDModules] = {-0.5,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,
												  -10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,
												  -10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,
												  -10,-10,-10};
const double arXmaxPSDModules[nBinsPSDModules] = {150.5,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,
                                                  1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,
                                                  1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,
                                                  1200,1200,1200};



//consts for TimeHandler
const double minRunNumberArSc150 = 20300;
const double maxRunNumberArSc150 = 20500; 
const double minRunNumberArSc13 = 20500;
const double maxRunNumberArSc13 = 20650;
const double minRunNumberArSc19 = 20660;
const double maxRunNumberArSc19 = 20810;
const double minRunNumberArSc30 = 20810;
const double maxRunNumberArSc30 = 21050;
const double minRunNumberArSc40 = 21050;
const double maxRunNumberArSc40 = 21270;

//for strange PSD_N clouds 
enum ePSDNClouds {good, garbageHighPSD, garbageLowPSDLowN,garbageLowPSDHighN};
enum ePSDModulCombinations {eAll, e28Central, e16Central,e28Periferal};

//							0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22
//double weightsPSD[45] = {	0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
//							23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   43   44
//							1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };


// e+e- cut
//const double electronCut150_x[13] = { -0.120056, 0.323586, 0.545407, 0.80539, 1.07969, 1.19894, 1.32297, 1.14647, 0.86502, 0.430919, -0.162989, -0.227389, -0.120056 };
//const double electronCut150_y[13] = { 1.41806, 1.3779, 1.45056, 1.49263, 1.52131, 1.53087, 1.5978, 1.68767, 1.73547, 1.87888, 1.94772, 1.60545, 1.41806 };
//TCutG electronCut150("electronCut150", 13, electronCut150_x, electronCut150_y);

/*const double electronCut75_x[13] = { -0.374563, 0.0740766, 0.306915, 0.568148, 0.846418, 0.965676, 1.1758, 1.06222, 0.670369, 0.193335, -0.397279, -0.618759, -0.374563 };
const double electronCut75_y[13] = { 1.32392, 1.31977, 1.4029, 1.44447, 1.47357, 1.48188, 1.51929, 1.66062, 1.74376, 1.86846, 1.90172, 1.53592, 1.32392 };
TCutG electronCut75("electronCut75", 13, electronCut75_x, electronCut75_y);

const double electronCut40_x[10] = { -0.703944, -0.226909, 0.0911135, 1.02247, 1.07358, 0.647653, 0.000249865, -0.675549, -0.732338, -0.703944 };
const double electronCut40_y[10] = { 1.36965, 1.31561, 1.34886, 1.48188, 1.61905, 1.68556, 1.88509, 1.84352, 1.63568, 1.36965 };
TCutG electronCut40("electronCut40", 10, electronCut40_x, electronCut40_y);*/
