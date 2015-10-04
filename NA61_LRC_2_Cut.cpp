#ifndef CUTS
#define CUTS
#include "NA61_LRC_2_Cut.h"
#endif

#ifndef CONST
#define CONST
#include "NA61_LRC_2_Const.h" //FIXME
#endif 

EventCut::EventCut(){}

T2Cut::T2Cut()
{
	my_Name = "T2";
	my_Short_Name = "T2"; 
}

bool T2Cut::CheckEvent(Event& event, bool bSim)
{
//	cout<<"T2"<<endl;
	
  	const raw::Trigger& trigger = event.GetRawEvent().GetBeam().GetTrigger();
//	cout<<"_"<<trigger.IsTrigger(det::TriggerConst::eT2, det::TriggerConst::ePrescaled)<<"\t";
   	if (!trigger.IsTrigger(det::TriggerConst::eT2, det::TriggerConst::ePrescaled))
		return 0;
	myNEntries++;
	return 1;
}

S1_1Cut::S1_1Cut()
{
	my_Name = "S1_1";
	my_Short_Name = "S1_1"; 
}

bool S1_1Cut::CheckEvent(Event& event, bool bSim)
{
//	cout<<"S1_1"<<endl;
	const int nbeam1 = event.GetRawEvent().GetBeam().GetTrigger().GetNumberOfSignalHits
		(det::TimeStructureConst::eWFA,det::TriggerConst::eS1_1);
	if (nbeam1 != 1) return 0;
	myNEntries++;
	return 1;
}

WFACut::WFACut():
	myWfaTime1(wfaTime1),
	myWfaTime2(wfaTime2),
	myWfaTimeCut(wfaTimeCut)
{
	my_Name = "WFA";
	my_Short_Name = "WFA"; 
}

WFACut::WFACut(double wfa_Time1, double wfa_Time2, double wfa_TimeCut):
	myWfaTime1(wfa_Time1),
	myWfaTime2(wfa_Time2),
	myWfaTimeCut(wfa_TimeCut)
{
	my_Name = "WFA";
	my_Short_Name = "WFA"; 
}

TString WFACut::GetShortNameWithPar()
{
	TString name;
	char name2[50];
        sprintf(name2,"_%f_%f_%f",myWfaTime1,myWfaTime2,myWfaTimeCut);
	name = my_Short_Name + name2;
	return name;
}

bool WFACut::CheckEvent(Event& event, bool bSim)
{
//	cout<<"WFA"<<endl;
	const raw::Trigger& trigger = event.GetRawEvent().GetBeam().GetTrigger();
	
	vector<double> timeStructureWFA;
	
	if (!trigger.HasTimeStructure(det::TimeStructureConst::eWFA, det::TriggerConst::eS1_1))
		return 0;

    timeStructureWFA = trigger.GetTimeStructure(det::TimeStructureConst::eWFA, det::TriggerConst::eS1_1);
    
    bool beamExist = false;
    for (unsigned int i = 0; i < timeStructureWFA.size(); ++i)
	{
		if (!beamExist && (timeStructureWFA.at(i) == myWfaTime1 || timeStructureWFA.at(i) == myWfaTime2))
			beamExist = true;
		else if (fabs(timeStructureWFA.at(i) - (myWfaTime1 + myWfaTime2) / 2.) < 1000. * myWfaTimeCut)
		{
			beamExist = false;
			break;
		}
    }
    if (!beamExist)	return 0; 
	myNEntries++;
	return 1;
}

ChargeCut::ChargeCut():
	myConfigPath(configPath)
{
	my_Name = "Ion_Charge";
	my_Short_Name = "ICh"; 
}

ChargeCut::ChargeCut(string yourConfigPath):
	myConfigPath(yourConfigPath)
{
	my_Name = "Ion_Charge";
	my_Short_Name = "ICh"; 
}

	TFile ChargeCut::zFile(string(configPath + "/cutFiles/zCut.root").c_str());
	TCutG* ChargeCut::zCut = (TCutG*) zFile.Get("ZvsDelayedZCut");


bool ChargeCut::CheckEvent(Event& event, bool bSim)
//by Emil Kaptur 08.2013
{
//	cout<<"ChargeCut"<<endl;
	const raw::Trigger& trigger = event.GetRawEvent().GetBeam().GetTrigger();
    const double_t detZADC1 =				trigger.GetADC(det::TriggerConst::eZdet_1);
    const double_t detZADC2 =				trigger.GetADC(det::TriggerConst::eZdet_2);
    const double_t detZADCAnalog =		trigger.GetADC(det::TriggerConst::eZdet_sum);
    const double_t detZADCAnalogDelayed = trigger.GetADC(det::TriggerConst::eZdet_sum_del);
	const EventHeader& eventHeader =    event.GetEventHeader();
    
	int runNumber = 0;
	runNumber = eventHeader.GetRunNumber();
    // sanity check
    if (runNumber == 0) return 0;
    
	map<int, double_t*> zCalibrationMap;

 	ifstream calibrationFile;
	calibrationFile.open(string(myConfigPath + "/calibration/calibrationZtest.txt").c_str());
       
	double_t* pArrayCalib[114];
	
	int iRun = 0;
	while (calibrationFile.good())
	{
		int runNo;	
		pArrayCalib[iRun] = new double_t[4];
		for (int l = 0; l<4; l++)
			(pArrayCalib[iRun])[l] = 0;
		calibrationFile >> runNo;
		calibrationFile >> (pArrayCalib[iRun])[0];
		calibrationFile >> (pArrayCalib[iRun])[1];
		calibrationFile >> (pArrayCalib[iRun])[2];
		calibrationFile >> (pArrayCalib[iRun])[3];
		if ((pArrayCalib[iRun])[0] > (pArrayCalib[iRun])[1])
		{
			double_t temp = (pArrayCalib[iRun])[0];
			(pArrayCalib[iRun])[0] = (pArrayCalib[iRun])[1];
			(pArrayCalib[iRun])[1] = temp;
		}
		if ((pArrayCalib[iRun])[2] > (pArrayCalib[iRun])[3])
		{
			double_t temp = (pArrayCalib[iRun])[2];
			(pArrayCalib[iRun])[2] = (pArrayCalib[iRun])[3];
			(pArrayCalib[iRun])[3] = temp;
		}
		zCalibrationMap.insert(make_pair(runNo, pArrayCalib[iRun]));
		iRun++;

	}
	calibrationFile.close();
	if (iRun!=114) cout<<"EMIIIIIIILL!!!!!!!!!!!!!!"<<iRun<<endl;
    
	// file with Z outliers cut defined (for 150A GeV only)
//	TFile zFile(string(myConfigPath + "/cutFiles/zCut.root").c_str());
//	TCutG* zCut = (TCutG*) zFile.Get("ZvsDelayedZCut");
    
    // applying temprorary calibration (for Z-cuts)
    double_t calibratedZDet = 0;
    double_t calibratedZDetDelayed = 0;
    if (zCalibrationMap.find(runNumber) != zCalibrationMap.end())
	{
		double_t c0 = zCalibrationMap.find(runNumber)->second[0];
		double_t c1 = zCalibrationMap.find(runNumber)->second[1];
		double_t c2 = zCalibrationMap.find(runNumber)->second[2];
		double_t c3 = zCalibrationMap.find(runNumber)->second[3];
		calibratedZDet = detZADCAnalog * 3/(c1-c0) + 1-(3*c0)/(c1-c0);
		if (detZADCAnalog < 31)
			calibratedZDet = (detZADC1 + detZADC2) * 3/(c1-c0) + 1-(3*c0)/(c1-c0);
		calibratedZDetDelayed = detZADCAnalogDelayed * 3/(c3-c2) + 1-(3*c2)/(c3-c2);
    } 
    
	for (int i=0; i<114; i++)
		delete [](pArrayCalib[i]);
    
    //Z Detector Cut
    if (calibratedZDet < 12 || calibratedZDet > 21)
        return 0;
    
    //Z Detector Outliers Cut
    if(!zCut->IsInside(calibratedZDet, calibratedZDetDelayed))
      return 0;
	myNEntries++;
	return 1;
}

BPDCut::BPDCut()
{
	my_Name = "BPD";
	my_Short_Name = "BPD"; 
}

bool BPDCut::CheckEvent(Event& event, bool bSim)
{
//	cout<<"BPD"<<endl;
	if (bSim == false){
	//	RecEvent* pRecEvent= event.GetRecEvent();
		const int beamStatus =  event.GetRecEvent().GetBeam().GetStatus();
		if (beamStatus && (BeamConst::eNotFitted||BeamConst::eBadBPD3)){
		//	cout<<"beamStatus: "<<beamStatus<<endl;
			return 0;
		}
	} else {
//		SimEvent* pSimEvent=&event.GetSimEvent();
//		const int beamStatus =  pSimEvent->GetBeam().GetStatus();
//		if (beamStatus & (BeamConst::eNotFitted|BeamConst::eBadBPD3))
//			return 0;
	}
	myNEntries++;
	return 1;
}

DirectBPDCut::DirectBPDCut(double minX, double maxX, double minY, double maxY, eMyBPD bpd)
{
	if (minX > maxX){
		cout << "Error: MinX>MaxX! myMinX=maxX, myMaxX=minX" << endl;
		double a; 
		a = minX;
		minX = maxX;
		maxX = a;
	}
	if (minY > maxY){
		cout << "Error: MinY>MaxY! myMinY=maxY, myMaxY=minY" << endl;
		double a;
		a = minY;
		minY = maxY;
		maxY = a;
	}


	myMinX = minX;
	myMaxX = maxX;
	myMinY = minY;
	myMaxY = maxY;
	myBPD = bpd;
	my_Name = "Direct_BPD";
	my_Short_Name = "DirBPD";
}

bool DirectBPDCut::CheckEvent(Event& event, bool bSim)
{

	if (bSim == false){
		evt::rec::Beam beam = event.GetRecEvent().GetBeam();
		double BPDcoord[6] = { 0 };
		for (int i = 0; i<6; i++)
			BPDcoord[i] = beam.GetBPDPlane((det::BPDConst::EPlaneId)i).GetPosition();
		double X, Y;
		switch (myBPD)			
		{
		case BPD1:
			X = BPDcoord[0]; Y = BPDcoord[1]; break;
		case BPD2:
			X = BPDcoord[2]; Y = BPDcoord[3]; break;
		case BPD3:
			X = BPDcoord[4]; Y = BPDcoord[5]; break;
		default:
			break;
		} 
		if (X>myMaxX || X<myMinX || Y>myMaxY || Y<myMinY )
			return 0;
	}
	myNEntries++;
	return 1;
}

TString DirectBPDCut::GetShortNameWithPar()
{
	TString name;
	char name2[50];
	int i = myBPD;
	sprintf(name2, "_%i_%f_%f_%f_%f",i, myMinX, myMaxX, myMinY, myMaxY);
	name = my_Short_Name + name2;
	return name;
}

MainVertexCut::MainVertexCut()
{
	my_Name = "Main_Vtx";
	my_Short_Name = "MV"; 
}

bool MainVertexCut::CheckEvent(Event& event, bool bSim)
{
//	cout<<"MVtx"<<endl;
	RecEvent* pRecEvent=&event.GetRecEvent();
	if (pRecEvent->HasMainVertex()==1) 
	{
		myNEntries++;
		return 1;
		
	}
	else return 0;

}

FittedVertexCut::FittedVertexCut()
{
	my_Name = "Fitted_Vtx";
	my_Short_Name = "FV"; 
}

bool FittedVertexCut::CheckEvent(Event& event, bool bSim)
{
//	cout<<"FVtxCut"<<endl;
	
	if (bSim == false){
		RecEvent* pRecEvent=&event.GetRecEvent();
		if (pRecEvent->HasPrimaryVertex(VertexConst::ePrimaryFitZ)){
			myNEntries++;
			return 1;
		}
		else return 0;
	}else{
//		SimEvent* pSimEvent=&event.GetSimEvent();
//		if (pSimEvent->HasPrimaryVertex(VertexConst::ePrimaryFitZ)){
//			myNEntries++;
//			return 1;
//		}
//		else return 0;
		myNEntries++;
		return 1;
	}
}

FitQualityCut::FitQualityCut():
	my_Quality(track_Quality)
{
	my_Name = "Fit_Quality";
	my_Short_Name = "FQu"; 
}

FitQualityCut::FitQualityCut(Int_t qual):
	my_Quality(qual)
{
	my_Name = "Fit_Quality";
	my_Short_Name = "FQu";
}

bool FitQualityCut::CheckEvent(Event& event, bool bSim)
{
//	cout<<"FitQua"<<endl;
	if (bSim==false){
		RecEvent* pRecEvent=&event.GetRecEvent();
		Vertex* pMainVertex=&pRecEvent->GetMainVertex();
		rec::Vertex& primaryVertex = pRecEvent->GetPrimaryVertex(rec::VertexConst::ePrimaryFitZ);
		if (/*pMainVertex->*/primaryVertex.GetFitQuality() != rec::FitQuality::ePerfect)//my_Quality)

			return 0;
	} else{
//		SimEvent* pSimEvent=&event.GetSimEvent();
//		evt::sim::Vertex* pMainVertex=&pSimEvent->GetMainVertex();
//
//		if (pMainVertex->GetFitQuality()!=my_Quality) return 0;
	}
	
	myNEntries++;
	return 1;
}


TString FitQualityCut::GetShortNameWithPar()
{
	TString name;
	char name2[50];
    	sprintf(name2,"_%i",my_Quality);
	name = my_Short_Name + name2;
	return name;
}

NVertexTracksCut::NVertexTracksCut():
	myMinNVtxTrack(nMinVertexTracks)
{
	my_Name = "N_Vtx_Track";
	my_Short_Name = "NVTr";
}


NVertexTracksCut::NVertexTracksCut(unsigned int nVtxTrack):
	myMinNVtxTrack(nVtxTrack)
{
	my_Name = "N_Vtx_Track";
	my_Short_Name = "NVTr";
}


bool NVertexTracksCut::CheckEvent(Event& event, bool bSim)
{
//	cout<<"NVtx"<<endl;
	unsigned int nGoodVertexTracks = 0;
	if (bSim == false){
		RecEvent& recEvent = event.GetRecEvent();
		for (list<evt::rec::VertexTrack>::const_iterator iTrack = recEvent.Begin<evt::rec::VertexTrack>();
				iTrack != recEvent.End<evt::rec::VertexTrack>(); ++iTrack) {
			nGoodVertexTracks++;
			//const evt::rec::VertexTrack& vtxTrack = *iTrack;
		}
//		RecEvent* pRecEvent=&event.GetRecEvent();
//		Vertex* pMainVertex=&event.GetRecEvent().GetMainVertex();
//		for (VertexTrackIndexIterator vtxTrackIter = pMainVertex->DaughterTracksBegin();
//			vtxTrackIter != pMainVertex->DaughterTracksEnd(); ++vtxTrackIter){
//			VertexTrack& vtxTrack = (*pRecEvent).Get(*vtxTrackIter);
//			if ((vtxTrack.GetStatus() == 0) && (vtxTrack.HasTrack()))
//				nGoodVertexTracks++;
//			if (nGoodVertexTracks >= myMinNVtxTrack) {myNEntries++; return 1;}
//		}
	} else {
		evt::SimEvent& simEvent = event.GetSimEvent();
		RecEvent& recEvent = event.GetRecEvent();
		for (list<evt::sim::VertexTrack>::const_iterator iTrack = simEvent.Begin<evt::sim::VertexTrack>();
			iTrack != simEvent.End<evt::sim::VertexTrack>(); ++iTrack) {
			if (iTrack->GetType() == sim::VertexTrackConst::eGeneratorFinal) {
        		//const evt::sim::VertexTrack& vtxTrack = *iTrack;
				nGoodVertexTracks++;
			}
		}
//		SimEvent* pSimEvent=&event.GetSimEvent();
//		Vertex* pMainVertex=&event.GetSimEvent().GetMainVertex();
//		for (VertexTrackIndexIterator vtxTrackIter = pMainVertex->DaughterTracksBegin();
//			vtxTrackIter != pMainVertex->DaughterTracksEnd(); ++vtxTrackIter){
//			VertexTrack& vtxTrack = (*pSimEvent).Get(*vtxTrackIter);
//			if ((vtxTrack.GetStatus() == 0) && (vtxTrack.HasTrack()))
//				nGoodVertexTracks++;
//			if (nGoodVertexTracks >= myMinNVtxTrack) {myNEntries++; return 1;}
//		}
	}
	if (nGoodVertexTracks < myMinNVtxTrack)
		return 0;
	//myNEntries++;
    //cout << "nvertex cut " << myNEntries << endl;
	myNEntries++;
	return 1;
}

TString NVertexTracksCut::GetShortNameWithPar()
{
	TString name;
	char name2[50];
    sprintf(name2,"_%i",myMinNVtxTrack);
	name = my_Short_Name + name2;
	return name;
}

ZVertexCut::ZVertexCut():
	myMinVtxZ(minZVertex),
	myMaxVtxZ(maxZVertex)
{
	my_Name = "Z_Coord";
	my_Short_Name = "Z";
}

ZVertexCut::ZVertexCut(double minZ, double maxZ):
	myMinVtxZ(minZ),
	myMaxVtxZ(maxZ)
{
	my_Name = "Z_Coord";
	my_Short_Name = "Z";
}

bool ZVertexCut::CheckEvent(Event& event, bool bSim)
{
//	cout<<"ZVtx"<<endl;
	if (bSim == false){
//		RecEvent* pRecEvent=&event.GetRecEvent();
		Vertex* pPrimaryVertex = &event.GetRecEvent().GetPrimaryVertex(rec::VertexConst::ePrimaryFitZ);
		const utl::Point& Vertex = pPrimaryVertex->GetPosition();
	//	Vertex* pMainVertex=&event.GetRecEvent().GetMainVertex();
	//	const utl::Point& Vertex = pMainVertex->GetPosition();
		double_t ZVertex = Vertex.GetZ();
        
		if (ZVertex > myMaxVtxZ || ZVertex < myMinVtxZ) return 0;
	} else {
//		SimEvent* pSimEvent=&event.GetSimEvent();
//		Vertex* pMainVertex=&event.GetSimEvent().GetMainVertex();
//
//		const utl::Point& Vertex = pMainVertex->GetPosition();
//		double_t ZVertex = Vertex.GetZ();
 //       
//		if (ZVertex > myMaxVtxZ || ZVertex < myMinVtxZ) return 0;
		myNEntries++;
		return 1;
	}
	myNEntries++;
	return 1;
}

TString ZVertexCut::GetShortNameWithPar()
{
	TString name;
	char name2[50];
    sprintf(name2,"_%f_%f",myMinVtxZ,myMaxVtxZ);
	name = my_Short_Name + name2;
	return name;
}

CentralityCut::CentralityCut(double minPer, double maxPer, bool bRaw):
	myLowPer(minPer),
	myUpPer(maxPer)
{
	my_Name = "Centrality";
	my_Short_Name = "PSD";
	myMaxEPSD=0;
	myMinEPSD=0;
	double per;
	double E = 0;
	if (bRaw==true){
		//cout << "centrality_raw" << endl;
		ifstream in(configPath+"/psdByEmil/150BeBeCentrality28Mod.txt");
		while(in>>per){
			if(per>myLowPer){
				myMinEPSD = E;
				in>>E;
				break;
			}
			in>>E;
		}
		while(in>>per){
			if(per>myUpPer){
				myMaxEPSD = E;
				in>>E;
				break;
			}
			in>>E;
		}
		if(myMaxEPSD == 0){
			cout<<"WARNING: max percentile can't be so large (problem with simulation of T2 trigger)"<<endl;
			cout<<"it has reduced to the largest value: "<<per<<endl;
			myMaxEPSD=E;
		}
		in.close();
		cout<<myLowPer*100<<"% "<<myMinEPSD<<endl;
		cout<<myUpPer*100<<"% "<<myMaxEPSD<<endl;
	}else{
		//cout << "Centrality_sim" << endl;
		this->InitLegacyCentrality();//FIXME bad realization!
	}
}

void CentralityCut::InitLegacyCentrality()
{
	pDetermineCentrality = new LegacyCentrality();
//	pDetermineCentrality->SetDataPath(configPath); //change it to your path
	double centralityEdges[4] = {myLowPer, myUpPer, myUpPer+0.1, myUpPer+0.2};  // Can be changed if statistics too low
	pDetermineCentrality->SetCentralityEdges(centralityEdges);
	bInit=true;
}

//fwk::CentralConfig::GetInstance("bootstrap.xml.in");
CentralConfig& cc = CentralConfig::GetInstance(str_configPath + "/bootstrap.xml");
bool CentralityCut::CheckEvent(Event& event, bool bSim)
{
//	cout<<"Centr"<<endl;
	if (bInit==false){ //RAW data
		double_t eventPSDEnergy = 0;
		if (bSim == false)
		{//DON'T FORGET TO CHANGE the Handler
			RecEvent* pRecEvent=&event.GetRecEvent();
			PSD& psd = pRecEvent->GetPSD();
			for (int i=0; i<nPSDMods; i++)
				eventPSDEnergy = eventPSDEnergy + psd.GetModule(i+1).GetEnergy();
		} else{
//			SimEvent* pSimEvent=&event.GetSimEvent();
//			PSD& psd = pSimEvent->GetPSD();
//			for (int i=0; i<nPSDMods; i++)
//				eventPSDEnergy = eventPSDEnergy + psd.GetModule(i+1).GetEnergy();
			myNEntries++;
			return 1;
		}
		if ((eventPSDEnergy > myMinEPSD)&&(eventPSDEnergy < myMaxEPSD)) {
			myNEntries++;
			return 1;
		}
		else return 0;

	}else{
//		cout<<"CheckEvent\t";
		const EventHeader& eventHeader = event.GetEventHeader();
		unsigned int runNumber = eventHeader.GetRunNumber();
		det::Detector::GetInstance().Update(eventHeader.GetTime(), runNumber);
		vector<int> centralityClasses;
		int centralityClass = pDetermineCentrality->GetCentralityClass(event, &centralityClasses);
//		cout<<"centrality:"<<centralityClass<<endl;
		if (centralityClass == 2){ 
			myNEntries++;
			return 1;
		}
		return 0;
	}
}

TString CentralityCut::GetShortNameWithPar()
{
	TString name;
	char name2[50];
    sprintf(name2,"_%f_%f",myLowPer,myUpPer);
	name = my_Short_Name + name2;
	return name;
}

VtxTrackStatusCut::VtxTrackStatusCut()
{
	my_Name = "Vtx_Tr_Status";
	my_Short_Name = "VTrSt";
}


bool VtxTrackStatusCut::CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack)
{
//	cout<<"VtxStatus"<<endl;
	if (vtxTrack.GetStatus() != 0 || !vtxTrack.HasTrack()) return 0;
	myNEntries++;
	return 1;
}

bool VtxTrackStatusCut::CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack)
{
//	cout<<"VtxStatus"<<endl;
//	if (vtxTrack.GetStatus() != 0 || !vtxTrack.HasTrack()) return 0;
	myNEntries++;
	return 1;
}

ImpactPointCut::ImpactPointCut():
	myMaxImpX(maxImpactX),
	myMaxImpY(maxImpactY)
{
	my_Name = "Impact_Point";
	my_Short_Name = "ImP";
}

ImpactPointCut::ImpactPointCut(double maxX, double maxY):
	myMaxImpX(maxX),
	myMaxImpY(maxY)
{
	my_Name = "Impact_Point";
	my_Short_Name = "ImP";
}

bool ImpactPointCut::CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack)
{
//	cout<<"Impact Point Cut"<<endl;
	const Vertex& mainVertex = recEvent.GetMainVertex();
	const Point& impactPoint = vtxTrack.GetImpactPoint();
    const Vector vertexToImpact =  mainVertex.GetPosition() - impactPoint;
    const double_t bx = vertexToImpact.GetX();
    const double_t by = vertexToImpact.GetY();
    if (fabs(bx) >= myMaxImpX || fabs(by) >= myMaxImpY)
        return 0;
	myNEntries++;
	return 1;
}

bool ImpactPointCut::CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack)
{
//	cout<<"Impact Point Cut"<<endl;
/*	const Vertex& mainVertex = simEvent.GetMainVertex();
	const Point& impactPoint = vtxTrack.GetImpactPoint();
    const Vector vertexToImpact =  mainVertex.GetPosition() - impactPoint;
    const double_t bx = vertexToImpact.GetX();
    const double_t by = vertexToImpact.GetY();
    if (fabs(bx) >= myMaxImpX || fabs(by) >= myMaxImpY)
        return 0;*/
	myNEntries++;
	return 1;
}


TString ImpactPointCut::GetShortNameWithPar()
{
	TString name;
	char name2[50];
    sprintf(name2,"_%f_%f",myMaxImpX,myMaxImpY);
	name = my_Short_Name + name2;
	return name;
}

TotalTPCClustersCut::TotalTPCClustersCut():
	myMinTotalClusters(nMinTotalClusters)
{
	my_Name = "Total_TPC_Clusters";
	my_Short_Name = "TPC";
}

TotalTPCClustersCut::TotalTPCClustersCut(unsigned int minTotal):
	myMinTotalClusters(minTotal)
{
	my_Name = "Total_TPC_Clusters";
	my_Short_Name = "TPC";
}

bool TotalTPCClustersCut::CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack)
{
//	cout<<"Total TPC Cut"<<endl;
	const Track& track = recEvent.Get(vtxTrack.GetTrackIndex());
	int nClusters = 0;
	if (systemType == ArSc)
		nClusters = track.GetNumberOfClusters(TrackConst::eAll) - track.GetNumberOfClusters(TrackConst::eGTPC);
	else
		nClusters = track.GetNumberOfClusters(TrackConst::eAll);

	if (nClusters < myMinTotalClusters) 
		return 0;
	myNEntries++;
	return 1;
}

bool TotalTPCClustersCut::CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack)
{
//	cout<<"Total TPC Cut"<<endl;
//	const Track& track = simEvent.Get(vtxTrack.GetTrackIndex());
//	if (track.GetNumberOfClusters(TrackConst::eAll) < myMinTotalClusters) 
//		return 0;
	myNEntries++;
	return 1;
}

TString TotalTPCClustersCut::GetShortNameWithPar()
{
	TString name;
	char name2[50];
    sprintf(name2,"_%i",myMinTotalClusters);
	name = my_Short_Name + name2;
	return name;
}

 VTPCAndGTPCClustersCut::VTPCAndGTPCClustersCut():
	  myMinVTPCClusters(nMinVTPCClusters),
	  myMinGTPCClusters(nMinGTPCClusters)
 {
 	my_Name = "VTPC_GTPC_Clusters";
	my_Short_Name = "VGTPC";
 }

 VTPCAndGTPCClustersCut::VTPCAndGTPCClustersCut(unsigned int minVTPC, unsigned int minGTPC):
	 myMinGTPCClusters(minGTPC),
	 myMinVTPCClusters(minVTPC)
 {
 	my_Name = "VTPC_GTPC_Clusters";
	my_Short_Name = "VGTPC";
 }

bool VTPCAndGTPCClustersCut::CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack)
{
//	cout<<"VTPC&GTPC Cut"<<endl;
	const Track& track = recEvent.Get(vtxTrack.GetTrackIndex());
	if ((track.GetNumberOfClusters(TrackConst::eVTPC1) + track.GetNumberOfClusters(TrackConst::eVTPC2) < myMinVTPCClusters)
		&& (track.GetNumberOfClusters(TrackConst::eGTPC) < myMinGTPCClusters))
		return 0;
	myNEntries++;
	return 1;
}

bool VTPCAndGTPCClustersCut::CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack)
{
//	cout<<"VTPC&GTPC Cut"<<endl;
//	const Track& track = simEvent.Get(vtxTrack.GetTrackIndex());
//	if ((track.GetNumberOfClusters(TrackConst::eVTPC1) + track.GetNumberOfClusters(TrackConst::eVTPC2) < myMinVTPCClusters)
//		&& (track.GetNumberOfClusters(TrackConst::eGTPC) < myMinGTPCClusters))
//		return 0;
	myNEntries++;
	return 1;
}

TString VTPCAndGTPCClustersCut::GetShortNameWithPar()
{
	TString name;
	char name2[50];
    sprintf(name2,"_%i_%i",myMinVTPCClusters,myMinGTPCClusters);
	name = my_Short_Name + name2;
	return name;
}

TOFCut::TOFCut()
{
	my_Name = "TOF";
	my_Short_Name = "TOF";
}

bool TOFCut::CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack)
{
	if (vtxTrack.GetNumberOfTOFMasses())
		return 0;
	myNEntries++;
	return 1;
}

bool TOFCut::CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack)
{
//	if (vtxTrack.GetNumberOfTOFMasses())
//		return 0;
	myNEntries++;
	return 1;
}


PCut::PCut():
	myMaxP(maxP),
	myMinP(minP)
{
	my_Name = "Momentum";
	my_Short_Name = "P";
}

PCut::PCut(double min_P, double max_P):
	myMaxP(max_P),
	myMinP(min_P)
{
	my_Name = "Momentum";
	my_Short_Name = "P";
}

bool PCut::CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack)
{
//	cout<<"P"<<endl;
	double_t P;
	Vector vtxMomentum = vtxTrack.GetMomentum();
	P=vtxMomentum.GetMag();
	if (P > myMaxP || P < myMinP) return 0;
	myNEntries++;
	return 1;
}

bool PCut::CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack)
{
//	cout<<"P"<<endl;
	double_t P;
	Vector vtxMomentum = vtxTrack.GetMomentum();
	P=vtxMomentum.GetMag();
	if (P > myMaxP || P < myMinP) return 0;
	myNEntries++;
	return 1;
}

TString PCut::GetShortNameWithPar()
{
	TString name;
	char name2[50];
    sprintf(name2,"_%f_%f",myMinP,myMaxP);
	name = my_Short_Name + name2;
	return name;
}

PtCut::PtCut():
	myMaxPt(maxPt),
	myMinPt(minPt)
{
	my_Name = "Transvers_Momentum";
	my_Short_Name = "Pt";
}

PtCut::PtCut(double min_Pt, double max_Pt):
	myMaxPt(max_Pt),
	myMinPt(min_Pt)
{
	my_Name = "Transvers_Momentum";
	my_Short_Name = "Pt";
}

bool PtCut::CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack)
{
//	cout<<"Pt"<<endl;
	double_t Px, Py, Pt;
	Vector vtxMomentum = vtxTrack.GetMomentum();
	Px=vtxMomentum.GetX();
	Py=vtxMomentum.GetY();
	Pt=sqrt(Px*Px+Py*Py);
	if (Pt > myMaxPt || Pt < myMinPt) return 0;
	myNEntries++;
	return 1;
}

bool PtCut::CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack)
{
//	cout<<"Pt"<<endl;
	double_t Px, Py, Pt;
	Vector vtxMomentum = vtxTrack.GetMomentum();
	Px=vtxMomentum.GetX();
	Py=vtxMomentum.GetY();
	Pt=sqrt(Px*Px+Py*Py);
	if (Pt > myMaxPt || Pt < myMinPt) return 0;
	myNEntries++;
	return 1;
}

TString PtCut::GetShortNameWithPar()
{
	TString name;
	char name2[50];
    sprintf(name2,"_%f_%f",myMinPt,myMaxPt);
	name = my_Short_Name + name2;
	return name;
}

AcceptCut::AcceptCut():
myConfigPath(configPath)
{
    TFile* in_file = TFile::Open(string(myConfigPath + "/acceptance.root").c_str());
    //TH3D* acceptHist = (TH3D*)in_file->GetObjectChecked("newAcc", "TH3D");
	TH3D* acceptHist = (TH3D*)in_file->GetObjectChecked("recAcceptance", "TH3D");
    itsAcceptHist = (TH3D*) acceptHist->Clone("myAccCopy");

	myXAxLowEdge = itsAcceptHist->GetXaxis()->GetBinLowEdge(1);
	myYAxLowEdge = itsAcceptHist->GetYaxis()->GetBinLowEdge(1);
	myZAxLowEdge = itsAcceptHist->GetZaxis()->GetBinLowEdge(1);

	myXAxBinWidth = itsAcceptHist->GetXaxis()->GetBinWidth(1);
	myYAxBinWidth = itsAcceptHist->GetYaxis()->GetBinWidth(1);
	myZAxBinWidth = itsAcceptHist->GetZaxis()->GetBinWidth(1);

	myNXBins = itsAcceptHist->GetXaxis()->GetNbins() + 1;
	myNYBins = itsAcceptHist->GetYaxis()->GetNbins() + 1;
	myNZBins = itsAcceptHist->GetZaxis()->GetNbins() + 1;

	my_Name = "Acceptance_map";
	my_Short_Name = "Ac";
}

AcceptCut::AcceptCut(TString yourConfigPath):
myConfigPath(yourConfigPath)
{
	TFile* in_file = TFile::Open(string(myConfigPath + "/acceptance.root").c_str());
	//TH3D* acceptHist = (TH3D*)in_file->GetObjectChecked("newAcc", "TH3D");
	TH3D* acceptHist = (TH3D*)in_file->GetObjectChecked("recAcceptance", "TH3D");
	
   itsAcceptHist = (TH3D*) acceptHist->Clone("myAccCopy");

   myXAxLowEdge = itsAcceptHist->GetXaxis()->GetBinLowEdge(1);
   myYAxLowEdge = itsAcceptHist->GetYaxis()->GetBinLowEdge(1);
   myZAxLowEdge = itsAcceptHist->GetZaxis()->GetBinLowEdge(1);

   myXAxBinWidth = itsAcceptHist->GetXaxis()->GetBinWidth(1);
   myYAxBinWidth = itsAcceptHist->GetYaxis()->GetBinWidth(1);
   myZAxBinWidth = itsAcceptHist->GetZaxis()->GetBinWidth(1);

   myNXBins = itsAcceptHist->GetXaxis()->GetNbins() + 1;
   myNYBins = itsAcceptHist->GetYaxis()->GetNbins() + 1;
   myNZBins = itsAcceptHist->GetZaxis()->GetNbins() + 1;

	my_Name = "Acceptance_map";
	my_Short_Name = "Ac";
}

AcceptCut::~AcceptCut()
{
   delete itsAcceptHist;
}

bool AcceptCut::CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack)
{
//	cout<<"Accept"<<endl;
	double_t Px, Py, Pz, P, Pt, eta, phi;
	Vector vtxMomentum = vtxTrack.GetMomentum();
	Px=vtxMomentum.GetX();
	Py=vtxMomentum.GetY();
    Pz=vtxMomentum.GetZ();
	P=vtxMomentum.GetMag();
    phi	=	TMath::ATan2(Py,Px);
    Pt=sqrt(Px*Px+Py*Py);
    eta=-TMath::Log(Pt/(P+Pz));
    double_t a,b,c;
	a = 1 + floor((phi - myXAxLowEdge) / myXAxBinWidth);
	b = 1 + floor((Pt - myYAxLowEdge) / myYAxBinWidth);
	c = 1 + floor((eta - myZAxLowEdge) / myZAxBinWidth);

	if (a < 1 || b < 1 || c < 1) return 0;
    if ((a<myNXBins) && (b<myNYBins) && (c<myNZBins)) {
        double_t N_acc;
        N_acc=itsAcceptHist->GetBinContent(a,b,c);
        if (N_acc==0)
            return 0;
        else {
            myNEntries++;
            return 1;
        }
    } else 
		return 0;
}

bool AcceptCut::CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack)
{
//	cout<<"Accept"<<endl;
	double_t Px, Py, Pz, P, Pt, eta, phi;
	Vector vtxMomentum = vtxTrack.GetMomentum();
	Px=vtxMomentum.GetX();
	Py=vtxMomentum.GetY();
    Pz=vtxMomentum.GetZ();
	P=vtxMomentum.GetMag();
    phi	=	TMath::ATan2(Py,Px);
    Pt=sqrt(Px*Px+Py*Py);
    eta=-TMath::Log(Pt/(P+Pz));
    double_t a,b,c;
	a = 1 + floor((phi - myXAxLowEdge) / myXAxBinWidth);
	b = 1 + floor((Pt - myYAxLowEdge) / myYAxBinWidth);
	c = 1 + floor((eta - myZAxLowEdge) / myZAxBinWidth);
	if (a < 1 || b < 1 || c < 1) return 0;
	if ((a<myNXBins) && (b<myNYBins) && (c<myNZBins)){
        double_t N_acc;
        N_acc=itsAcceptHist->GetBinContent(a,b,c);
        if (N_acc==0)
            return 0;
        else {
            myNEntries++;
            return 1;
        }
    } else 
		return 0;
}



EtaCut::EtaCut(double min_Eta, double max_Eta):
	myMaxEta(max_Eta),
	myMinEta(min_Eta)
{
	my_Name = "Eta";
	my_Short_Name = "Eta";
}

bool EtaCut::CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack)
{
//	cout<<"Eta"<<endl;
	double_t Px, Py, Pt, Pz, P, Eta;
	Vector vtxMomentum = vtxTrack.GetMomentum();
	Px=vtxMomentum.GetX();
	Py=vtxMomentum.GetY();
	Pt=sqrt(Px*Px+Py*Py);
	P =vtxMomentum.GetMag();
	Pz=vtxMomentum.GetZ();
	Eta=-TMath::Log(Pt/(P+Pz));
	if (Eta > myMaxEta || Eta < myMinEta) return 0;
	myNEntries++;
	return 1;
}

bool EtaCut::CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack)
{
//	cout<<"Eta"<<endl;
	double_t Px, Py, Pt, Pz, P, Eta;
	Vector vtxMomentum = vtxTrack.GetMomentum();
	Px=vtxMomentum.GetX();
	Py=vtxMomentum.GetY();
	Pt=sqrt(Px*Px+Py*Py);
	P =vtxMomentum.GetMag();
	Pz=vtxMomentum.GetZ();
	Eta=-TMath::Log(Pt/(P+Pz));
	if (Eta > myMaxEta || Eta < myMinEta) return 0;
	myNEntries++;
	return 1;
}

TString EtaCut::GetShortNameWithPar()
{
	TString name;
	char name2[50];
    sprintf(name2,"_%f_%f",myMinEta,myMaxEta);
	name = my_Short_Name + name2;
	return name;
}



PhiCut::PhiCut(double min_Phi, double max_Phi):
	myMaxPhi(max_Phi),
	myMinPhi(min_Phi)
{
	my_Name = "Phi";
	my_Short_Name = "Phi";
}

bool PhiCut::CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack)
{
//	cout<<"Phi"<<endl;
	double_t Px, Py, Phi;
	Vector vtxMomentum = vtxTrack.GetMomentum();
	Px=vtxMomentum.GetX();
	Py=vtxMomentum.GetY();
	Phi=TMath::ATan2(Py,Px); //phi = [-pi;pi] 
	if (Phi > myMaxPhi || Phi < myMinPhi) return 0;
	myNEntries++;
	return 1;
}

bool PhiCut::CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack)
{
//	cout<<"Phi"<<endl;
	double_t Px, Py, Phi;
	Vector vtxMomentum = vtxTrack.GetMomentum();
	Px=vtxMomentum.GetX();
	Py=vtxMomentum.GetY();
	Phi=TMath::ATan2(Py,Px); //phi = [-pi;pi] 
	if (Phi > myMaxPhi || Phi < myMinPhi) return 0;
	myNEntries++;
	return 1;
}

TString PhiCut::GetShortNameWithPar()
{
	TString name;
	char name2[50];
    	sprintf(name2,"_%f_%f",myMinPhi,myMaxPhi);
	name = my_Short_Name + name2;
	return name;
}

ChargeTrCut::ChargeTrCut(int redCh):
 myCharge(redCh)
{
	my_Name = "Charge_Track";
	my_Short_Name = "ChTrk";
}

bool ChargeTrCut::CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack)
{
	if (vtxTrack.GetCharge()==myCharge) return 0;
	myNEntries++;
	return 1;
}

bool ChargeTrCut::CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack)
{
//	cout<<"problem?"<<endl;
	if (vtxTrack.GetCharge()==myCharge) return 0;
	myNEntries++;
	return 1;
}

TString ChargeTrCut::GetShortNameWithPar() //FIXME
{
	TString name;
//	char name2[50];
//    	sprintf(name2,"_%f",myCharge);
	name = my_Short_Name;// + name2;
	return name;
}


TRandom* pTRandom = new TRandom();
DiluteCut::DiluteCut(double remove_track_percent)
{
	if (remove_track_percent > 1 || remove_track_percent < 0){
		cout<<"Warning! Dilute cut isn't working. remove_track_percent have to be between 0 and 1"<<endl;
		my_Name = "";
		my_Short_Name = "";
		my_remove_track_percent = 0;
	} else {
		my_Name = "DiluteCut";
		my_Short_Name = "Dilute";
		my_remove_track_percent = remove_track_percent;
	}
}

bool DiluteCut::CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack)
{
	double t = pTRandom->Uniform();

	if (t > my_remove_track_percent)
		return 1; 
	return 0;
}

bool DiluteCut::CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack)
{
	if (pTRandom->Uniform() > my_remove_track_percent)
		return 1; 
	return 0;
}

TString DiluteCut::GetShortNameWithPar() //FIXME
{
	TString name;
	char name2[50];
	sprintf(name2,"_%i",my_remove_track_percent);
	name = my_Short_Name + name2;
	return name;
}

AcceptRapidityCut::AcceptRapidityCut()
{
	TFile* in_file;
	TString nameHist; 
	switch (beamMomentum)
	{
	case 150:
		nameHist = "158";
		break;
	case 20:
		nameHist = "20";
		break;
	case 19:
		nameHist = "20";
		break;
	case 31:
		nameHist = "31";
		break;
	case 30:
		nameHist = "31";
		break;
	case 40:
		nameHist = "40";
		break;
	case 80:
		nameHist = "80";
		break;
	case 75:
		nameHist = "80"; //WTF?! FIXME 
		break;
	default:
		nameHist = "158";
		cout << "Error no such beam energy. 158 are setted" << endl;
		break;
	} 
	switch (systemType)
	{
	case BeBe:
		//myPath = "/afs/cern.ch/work/a/aseryako/public/acceptance_maps/ac_map_BeBe_Tobiasz.root";
		myPath = "/afs/cern.ch/work/a/aseryako/public/acceptance_maps/acc_map_BeBe_100.root";
		break;
	default:
		//myPath = "/afs/cern.ch/work/a/aseryako/public/acceptance_maps/ac_map_BeBe_Tobiasz.root";
		myPath = "/afs/cern.ch/work/a/aseryako/public/acceptance_maps/acc_map_BeBe_100.root";
		break;
	}

	in_file= TFile::Open(string(myPath).c_str());
	//TH3D* AcceptRapidityHist = (TH3D*)in_file->GetObjectChecked("newAcc", "TH3D");
	//TH3C* AcceptHist = (TH3C*)in_file->GetObjectChecked(nameHist, "TH3C");
	//myAcceptHist = (TH3C*)AcceptHist->Clone("myAccCopy");
	TH3F* AcceptHist = (TH3F*)in_file->GetObjectChecked(nameHist, "TH3F");
	myAcceptHist = (TH3F*)AcceptHist->Clone("myAccCopy");

	myXAxLowEdge = myAcceptHist->GetXaxis()->GetBinLowEdge(1);
	myYAxLowEdge = myAcceptHist->GetYaxis()->GetBinLowEdge(1);
	myZAxLowEdge = myAcceptHist->GetZaxis()->GetBinLowEdge(1);

	myXAxBinWidth = myAcceptHist->GetXaxis()->GetBinWidth(1);
	myYAxBinWidth = myAcceptHist->GetYaxis()->GetBinWidth(1);
	myZAxBinWidth = myAcceptHist->GetZaxis()->GetBinWidth(1);

	myNXBins = myAcceptHist->GetXaxis()->GetNbins() + 1;
	myNYBins = myAcceptHist->GetYaxis()->GetNbins() + 1;
	myNZBins = myAcceptHist->GetZaxis()->GetNbins() + 1;

	my_Name = "AcceptRapidity_map";
	my_Short_Name = "ARMap";
}

AcceptRapidityCut::~AcceptRapidityCut()
{
	delete myAcceptHist;
}

bool AcceptRapidityCut::CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack)
{
	//	cout<<"AcceptRapidity"<<endl;
	double_t Px, Py, Pz, P2, Pt, eta, phi, rapidity, E, phi_reflected;
	double mass = 0.1396;
	int charge;
	Vector vtxMomentum = vtxTrack.GetMomentum();
	Px = vtxMomentum.GetX();
	Py = vtxMomentum.GetY();
	Pz = vtxMomentum.GetZ();
	P2 = vtxMomentum.GetMag2();
	phi = TMath::ATan2(Py, Px); //[-pi;pi]
	phi = 180 * phi / TMath::Pi(); //[-180;180]
	Pt = sqrt(Px*Px + Py*Py);
	//	eta = -TMath::Log(Pt / (P + Pz));
	E = sqrt(pow(mass, 2) + P2);

	rapidity = 0.5 * log((E + Pz) / (E - Pz)) - beamRapidity / 2;
	charge = vtxTrack.GetCharge();

	if (charge < 0) {
		if (phi < 0) phi_reflected = -180 - phi;
		else phi_reflected = 180 - phi;
	}

	double_t a, b, c;
	a = 1 + floor((rapidity - myXAxLowEdge) / myXAxBinWidth);
	b = 1 + floor((phi_reflected - myYAxLowEdge) / myYAxBinWidth);
	c = 1 + floor((Pt - myZAxLowEdge) / myZAxBinWidth);
	if (a < 1 || b < 1 || c < 1) return 0;
	if ((a<myNXBins) && (b<myNYBins) && (c<myNZBins)) {
		double_t N_acc;
		N_acc = myAcceptHist->GetBinContent(a, b, c);
		if (N_acc<0.9)
		//if (N_acc == 0)
			return 0;
		else {
			myNEntries++;
			return 1;
		}
	}
	else
		return 0;
}

bool AcceptRapidityCut::CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack)
{
	//	cout<<"AcceptRapidity"<<endl;
	double_t Px, Py, Pz, P2, Pt, eta, phi, rapidity, E, phi_reflected;
	double mass = 0.1396;
	int charge;
	Vector vtxMomentum = vtxTrack.GetMomentum();
	Px = vtxMomentum.GetX();
	Py = vtxMomentum.GetY();
	Pz = vtxMomentum.GetZ();
	P2 = vtxMomentum.GetMag2();
	phi = TMath::ATan2(Py, Px); //[-pi;pi]
	phi = 180 * phi / TMath::Pi(); //[-180;180]
	Pt = sqrt(Px*Px + Py*Py);
	//	eta = -TMath::Log(Pt / (P + Pz));
	E = sqrt(pow(mass, 2) + P2);
	rapidity = 0.5 * log((E + Pz) / (E - Pz)) - beamRapidity/2;
	charge = vtxTrack.GetCharge();

	if (charge < 0) {
		if (phi < 0) phi_reflected = -180 - phi;
		else phi_reflected = 180 - phi;
	}

	double_t a, b, c;
	a = 1 + floor((rapidity - myXAxLowEdge) / myXAxBinWidth);
	b = 1 + floor((phi_reflected - myYAxLowEdge) / myYAxBinWidth);
	c = 1 + floor((Pt - myZAxLowEdge) / myZAxBinWidth);
	if (a < 1 || b < 1 || c < 1) return 0;
	if ((a<myNXBins) && (b<myNYBins) && (c<myNZBins)) {
		double_t N_acc;
		N_acc = myAcceptHist->GetBinContent(a, b, c);
		if (N_acc<0.9)
		//if (N_acc == 0)
			return 0;
		else {
			myNEntries++;
			return 1;
		}
	}
	else
		return 0;
}

double 	electronCut150_x[13] = { -0.120056, 0.323586, 0.545407, 0.80539, 1.07969, 1.19894, 1.32297, 1.14647, 0.86502, 0.430919, -0.162989, -0.227389, -0.120056 };
double	electronCut150_y[13] = { 1.41806, 1.3779, 1.45056, 1.49263, 1.52131, 1.53087, 1.5978, 1.68767, 1.73547, 1.87888, 1.94772, 1.60545, 1.41806 };

TCutG* electronCut150 = new TCutG("electronCut150", 13, electronCut150_x, electronCut150_y);

double electronCut75_x[13] = { -0.374563, 0.0740766, 0.306915, 0.568148, 0.846418, 0.965676, 1.1758, 1.06222, 0.670369, 0.193335, -0.397279, -0.618759, -0.374563 };
double electronCut75_y[13] = { 1.32392, 1.31977, 1.4029, 1.44447, 1.47357, 1.48188, 1.51929, 1.66062, 1.74376, 1.86846, 1.90172, 1.53592, 1.32392 };
TCutG* electronCut75 = new TCutG("electronCut75", 13, electronCut75_x, electronCut75_y);

double electronCut40_x[10] = { -0.703944, -0.226909, 0.0911135, 1.02247, 1.07358, 0.647653, 0.000249865, -0.675549, -0.732338, -0.703944 };
double electronCut40_y[10] = { 1.36965, 1.31561, 1.34886, 1.48188, 1.61905, 1.68556, 1.88509, 1.84352, 1.63568, 1.36965 };
TCutG* electronCut40 = new TCutG("electronCut40", 10, electronCut40_x, electronCut40_y);

TCutG* eeCut::myTCutG;
//TCutG* eeCut::myTCutG = new TCutG("electronCut150", 13, electronCut150_x, electronCut150_y);
eeCut::eeCut()
{
	my_Name = "Electron";
	my_Short_Name = "EE";
	switch (beamMomentum)
	{
	case 150:
		myTCutG = electronCut150;
		break;
	case 75:
		myTCutG = electronCut75;
	break;
	case 40:
		myTCutG = electronCut40;
		break;
	default:
		cout << "Error with beam energy in eeCut" << endl;
		break;
	}
}

bool eeCut::CheckTrack(RecEvent& recEvent, const VertexTrack& vtxTrack)
{
	//	cout<<"Phi"<<endl;
	double_t P, dEdx;
	Vector vtxMomentum = vtxTrack.GetMomentum();
	P = vtxMomentum.GetMag();
	const Track& track = recEvent.Get(vtxTrack.GetTrackIndex());
	dEdx = track.GetEnergyDeposit(evt::rec::TrackConst::eAll);
	if (myTCutG->IsInside(log10(P), dEdx)) return 0; //Electron!
	myNEntries++;
	return 1;
}

bool eeCut::CheckTrack(SimEvent& simEvent, const evt::sim::VertexTrack& vtxTrack)
{
	if (abs(vtxTrack.GetParticleId()) == 11) return 0;
	myNEntries++;
	return 1;
}

TString eeCut::GetShortNameWithPar()
{
	return my_Short_Name;
}

//double dPSDMultArSc150M[10] = { 178, 178, 156, 127, 50, 42, 64, 124, 238, 238 };
//double dPSDMultArSc150P[10] = { 524, 996, 1284, 1497, 2858, 3777, 4165, 3876, 1246, 524 };
//TCutG* psdMultCloudTCutArSc150 = new TCutG("psdMultCloudTCutArSc150", 10, dPSDMultArSc150M, dPSDMultArSc150P);

//double dPSDMultArSc150M[7] = { 178, 50, 42, 64, 124, 238, 238 };
//double dPSDMultArSc150P[7] = { 0, 2858, 3777, 4165, 3876, 1246, 0 };
//TCutG* psdMultCloudTCutArSc150 = new TCutG("psdMultCloudTCutArSc150", 7, dPSDMultArSc150M, dPSDMultArSc150P);

double dPSDMultArSc150M[6] = { 80, 80, 30, 30, 300, 3000 };
double dPSDMultArSc150P[6] = { 0, 2000, 4000, 4500, 4500,  0 };
TCutG* psdMultCloudTCutArSc150 = new TCutG("psdMultCloudTCutArSc150", 6, dPSDMultArSc150M, dPSDMultArSc150P);

double dPSDMultArSc150GarbageHighPSDM[4] = { 200, 200, 0, 0 };
double dPSDMultArSc150GarbageHighPSDP[4] = { 4500, 8000, 8000, 4500 };
TCutG* psdMultCloudTCutArSc150GarbageHighPSD = new TCutG("psdMultCloudTCutArSc150GarbageHighPSD", 
	4, dPSDMultArSc150GarbageHighPSDM, dPSDMultArSc150GarbageHighPSDP);

double dPSDMultArSc150GarbageLowPSDLowNM[5] = { 50, 50, 42, 0, 0 };
double dPSDMultArSc150GarbageLowPSDLowNP[5] = { 0, 2858, 3777, 3777, 0 };
TCutG* psdMultCloudTCutArSc150GarbageLowPSDLowN = new TCutG("psdMultCloudTCutArSc150GarbageLowPSDLowN",
	5, dPSDMultArSc150GarbageLowPSDLowNM, dPSDMultArSc150GarbageLowPSDLowNP);

double dPSDMultArSc150GarbageLowPSDHighNM[6] = { 178, 178, 156, 127, 50, 50 };
double dPSDMultArSc150GarbageLowPSDHighNP[6] = { 0, 996, 1284, 1497, 2858, 0 }; 
TCutG* psdMultCloudTCutArSc150GarbageLowPSDHighN = new TCutG("psdMultCloudTCutArSc150GarbageLowPSDHighN",
	6, dPSDMultArSc150GarbageLowPSDHighNM, dPSDMultArSc150GarbageLowPSDHighNP);

double dPSDMultArSc13M[5] = { 16, 4, 0, 20, 90};
double dPSDMultArSc13P[5] = { 0, 250, 400, 400, 0};
TCutG* psdMultCloudTCutArSc13 = new TCutG("psdMultCloudTCutArSc13", 5, dPSDMultArSc13M, dPSDMultArSc13P);

double dPSDMultArSc19M[6] = { 15, 8, 5, 20, 80, 120 };
double dPSDMultArSc19P[6] = { 0, 400, 700, 700, 400, 0 };
TCutG* psdMultCloudTCutArSc19 = new TCutG("psdMultCloudTCutArSc19", 6, dPSDMultArSc19M, dPSDMultArSc19P);

double dPSDMultArSc30M[5] = { 30, 10, 0, 30, 140};
double dPSDMultArSc30P[5] = { 0, 800, 1100, 1100, 0};
TCutG* psdMultCloudTCutArSc30 = new TCutG("psdMultCloudTCutArSc30", 5, dPSDMultArSc30M, dPSDMultArSc30P);

double dPSDMultArSc40M[6] = { 40, 40, 10, 10, 150, 150 };
double dPSDMultArSc40P[6] = { 0, 500, 1100, 1600, 800, 0 };
TCutG* psdMultCloudTCutArSc40 = new TCutG("psdMultCloudTCutArSc40", 6, dPSDMultArSc40M, dPSDMultArSc40P);

TCutG* MultPSDCloudCut::myTCutG;
MultPSDCloudCut::MultPSDCloudCut(bool bRaw, ePSDNClouds eCloud)
{
	myECloud = eCloud;
	my_Name = "Multiplicity_PSD_cloud_cut";
	my_Short_Name = "MultPSDCloud";
	bInit = false;
	if (bRaw){
		switch (beamMomentum)
		{
		case 13:
			switch (systemType)
			{
			case ArSc:
				if (nPSDMods == 28){
					if (eCloud == good){
						myTCutG = psdMultCloudTCutArSc13;
						bInit = true;
						break;
					}
				}
				break;
			default:
				break;
			}
			break;
		case 19:
			switch (systemType)
			{
			case ArSc:
				if (nPSDMods == 28){
					if (eCloud == good){
						myTCutG = psdMultCloudTCutArSc19;
						bInit = true;
						break;
					}
				}
				break;
			default:
				break;
			}
			break;
		case 30:
			switch (systemType)
			{
			case ArSc:
				if (nPSDMods == 28){
					if (eCloud == good){
						myTCutG = psdMultCloudTCutArSc30;
						bInit = true;
						break;
					}
				}
				break;
			default:
				break;
			}
			break;
		case 40:
			switch (systemType)
			{
			case ArSc:
				if (nPSDMods == 28){
					if (eCloud == good){
						myTCutG = psdMultCloudTCutArSc40;
						bInit = true;
						break;
					}
				}
				break;
			default:
				break;
			}
			break;
		case 150:
			switch (systemType)
			{
			case ArSc:
				switch (nPSDMods)
				{
				case 28: 
					switch (eCloud)
					{
					case good:
						myTCutG = psdMultCloudTCutArSc150;
						cout << "good cut" << endl;
						bInit = true;
						break;
					case garbageHighPSD:
						myTCutG = psdMultCloudTCutArSc150GarbageHighPSD;
						cout << "GarbageHighPSD" << endl;
						bInit = true;
						break;
					case garbageLowPSDLowN:
						myTCutG = psdMultCloudTCutArSc150GarbageLowPSDLowN;
						cout << "GarbageLowPSDLowN" << endl;
						bInit = true;
						break;
					case garbageLowPSDHighN:
						myTCutG = psdMultCloudTCutArSc150GarbageLowPSDHighN;
						cout << "GarbageLowPSDHighN" << endl;
						bInit = true;
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	cout << "!!!" << endl;
}

bool MultPSDCloudCut::CheckEvent(Event& event, bool bSim)
{	
	if (bInit == false) {
		cout << "MultPSDCloudCut: INIT == FALSE" << endl;
		myNEntries++;
		return 1;
	}
	if (bSim == true)	{
		myNEntries++;
		cout << "MultPSDCloudCut is not working for sim events" << endl;
		return 1;
	}
	double_t PSDEnergy=0;
	RecEvent* pRecEvent = &event.GetRecEvent();
	PSD& psd = pRecEvent->GetPSD();
	for (int i = 0; i<nPSDMods; i++)
		PSDEnergy = PSDEnergy + psd.GetModule(i + 1).GetEnergy();
	cout << "multPSDCut: " << myMultiplicity <<"  PSD: "<< PSDEnergy << endl;
	if (myTCutG->IsInside(myMultiplicity, PSDEnergy)) {
		myNEntries++;
		return true;
	}
	else return false;

}

TString MultPSDCloudCut::GetShortNameWithPar()
{
	TString name;
	switch (myECloud)
	{
	case good:
		name = "GoodCloud";
		break;
	case garbageHighPSD:
		name = "GarbageHighPSD";
		break;
	case garbageLowPSDLowN:
		name = "GarbageLowPSDLowN";
		break;
	case garbageLowPSDHighN:
		name = "GarbageLowPSDHighN";
		break;
	default:
		break;
	}
	return my_Short_Name + name;
}

bool MultPSDCloudCut::DoYouNeedMultiplicity()
{
	if (bInit == true)
		return true;
	return false;
}

PSDEnergyCut::PSDEnergyCut(double lowE, double upE, ePSDModulCombinations ePSDSet, bool bRaw)
{
	TString name;

	myBRaw = bRaw;
	if (bRaw){
		myMaxEPSD = upE;
		myMinEPSD = lowE;
	}
	else{
		cout << "WARNING: PSDEnergyCut doesn't work with events from generators" << endl;
	}
	for (int i = 0; i < 45; i++)
		myPSDModArray[i] = 0;

	switch (ePSDSet)
	{
	case e28Central:
		for (int i = 1; i < 29; i++)
			myPSDModArray[i] = 1;
		name = "28Central";
		break;
	case e16Central:
		for (int i = 1; i < 17; i++)
			myPSDModArray[i] = 1;
		name = "16Central";
		break;
	case e28Periferal:
		for (int i = 17; i < 45; i++)
			myPSDModArray[i] = 1;
		name = "28Periferal";
		break;
	default:
		for (int i = 1; i < 45; i++)
			myPSDModArray[i] = 1;
		name = "All";
		break;
	}
	my_Name = "PSD_Energy_direct_cut_" + name;
	my_Short_Name = "direct_psd_" + name;

}

bool PSDEnergyCut::CheckEvent(Event& event, bool bSim)
{
	if (myBRaw == false)
		return true; //nothing to do
	
	RecEvent* pRecEvent = &event.GetRecEvent();
	PSD& psd = pRecEvent->GetPSD();
	double eventPSDEnergy = 0;
	for (int i = 0; i<44; i++)
		if (myPSDModArray[i+1])
			eventPSDEnergy = eventPSDEnergy + psd.GetModule(i + 1).GetEnergy();
	if (eventPSDEnergy>myMinEPSD && eventPSDEnergy < myMaxEPSD){
		myNEntries++;
		return true;
	}
	else 
		return false;
}


TString PSDEnergyCut::GetShortNameWithPar()
{
	TString name;
	char name2[50];
	sprintf(name2, "_%f_%f", myMinEPSD, myMaxEPSD);
	name = my_Short_Name + name2;
	return name;
}

S5Cut::S5Cut(double upLimit, bool bRaw)
{
	my_Name = "S5";
	my_Short_Name = "S5";
	myBRaw = bRaw;
	if (bRaw)
		myUpLimit = upLimit;
	else
		cout << "WARNING: S5 is only for RAW data!" << endl;
}

bool S5Cut::CheckEvent(Event& event, bool bSim)
{
	if (myBRaw = false)
		return true;//notrhing to do
	double A = event.GetRawEvent().GetBeam().GetTrigger().GetADC(det::TriggerConst::eS5);
	if (A < myUpLimit){
		myNEntries++;
		return true;
	}
	else 
		return false;
}

TString S5Cut::GetShortNameWithPar()
{
	TString name;
	char name2[50];
	sprintf(name2, "_%f", myUpLimit);
	name = my_Short_Name + name2;
	return name;
}
