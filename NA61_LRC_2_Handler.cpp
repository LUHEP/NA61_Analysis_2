#ifndef HANDLERS
#define HANDLERS
#include "NA61_LRC_2_Handler.h"
#endif

#ifndef CONST
#define CONST
#include "NA61_LRC_2_Const.h"
#endif

BaseHandler::BaseHandler(const char* nameOut)
{
	nameOutFile=nameOut;
    mynEvent = 0; 
	myHistInSparseCounter = 0;

	bSim=false;
	bRaw = false;
	init = false;
	myEventCutList = new CutList();
	myTrackCutList = new CutList();	
	myEventCutList->SetFriendCutList(myTrackCutList);
}

BaseHandler::BaseHandler(const char* nameOut, bool simE) :
	bSim(simE)
{
	nameOutFile=nameOut;
    mynEvent = 0;
	myHistInSparseCounter = 0;
	bRaw = false;
	init = false;
//	psdEnergyHist=new TH1D("PSDHist",	"PSD energy distribution;PSD(28);entries",500,0,2500);
	myEventCutList = new CutList();
	myTrackCutList = new CutList();	
	myEventCutList->SetFriendCutList(myTrackCutList);
}

void BaseHandler::Raw() {bRaw = true;}
void BaseHandler::Simulation() {bRaw = false;} //FIXME bad realization 5:55am =/
bool BaseHandler::IsItRaw() {return bRaw;}


void BaseHandler::SetEventNumber(int nEvent)
{
	mynEvent = nEvent;
}

bool BaseHandler::EventTest(Event& ev)
{
	return myEventCutList->EventTest(ev, bSim);
}

void BaseHandler::AddEventCut(EventCut* cut)
{
    myEventCutList->AddCut(cut);
}

void BaseHandler::AddTrackCut(TrackCut* cut)
{
    myTrackCutList->AddCut(cut);
}


StatEventInfo OneWindHandler::myEventInfo;
bool OneWindHandler::bEventInfoFull = false;

OneWindHandler::OneWindHandler(const char* nameOut):BaseHandler(nameOut)
{}

OneWindHandler::OneWindHandler(const char* nameOut, bool simE):BaseHandler(nameOut, simE)
{}


OneWindHandler::~OneWindHandler()
{
	cout<<"Onewind Handler destructor is starting ... "<<endl;
	//--- output results
	cout<<"name outFile = "<<nameOutFile<<endl;
	TFile outputFile(nameOutFile, "RECREATE");
	outputFile.cd();

//	cout<<"writing..."<<endl;

	TH1F* eventStat = myEventCutList->GetStatHist(nameOutFile+myNameHist, bSim);

	TH1F* trackStat = myTrackCutList->GetStatHist(nameOutFile+myNameHist, bSim); 

	eventStat->Write();
	trackStat->Write();

	chargeHist->Write();
	multHist->Write();
	psdEnergyHist->Write();
    //acceptHist->Write();
    
  //  plusminusHist->Write();
    etaptHist->Write();
    etaptHist0->Write();
    etaptHist1->Write();
	rapPtHist->Write();
	rapPtHist0->Write();
	rapPtHist1->Write();
    ptPhiHist->Write();
	ptPhiHist0->Write();
	ptPhiHist1->Write();
    
	pHist->Write();			pHist0->Write();			pHist1->Write();
	pxHist->Write();		pxHist0->Write();			pxHist1->Write();
	pyHist->Write();		pyHist0->Write();			pyHist1->Write();
	pzHist->Write();		pzHist0->Write();			pzHist1->Write();
	ptHist->Write();		ptHist0->Write();			ptHist1->Write();
	etaHist->Write();		etaHist0->Write();			etaHist1->Write();
	rapHist->Write();		rapHist0->Write();			rapHist1->Write();
	phiHist->Write();		phiHist0->Write();			phiHist1->Write();
	
	BPD1Hist->Write();
	BPD2Hist->Write();
	BPD3Hist->Write();
	ImpParHist->Write();
	WFAHist->Write();
	fitVtxHist->Write();
    
    dEdxPHist0->Write();
	dEdxPHist1->Write(); //Ya pomenyal
    fitVtxNHist->Write(); //Ya pomenyal
    
    psdEnergyNHist->Write(); 
	psdEnergyS5Hist->Write();
	multS5Hist->Write();
    
    totClusterHist->Write(); //Ya pomenyal
    verClusterHist->Write(); //Ya pomenyal
    gapClusterHist->Write(); //Ya pomenyal
    
    nBeamHist->Write(); //Ya pomenyal

    BPD1XMultHist->Write();  BPD1YMultHist->Write();
	BPD2XMultHist->Write();  BPD2YMultHist->Write();
    BPD3XMultHist->Write();  BPD3YMultHist->Write();
	SlopeBeamXZHist->Write();
	SlopeBeamYZHist->Write();

    cout<<nameOutFile<<endl;
	cout<<"nEvent:  "<<multHist->GetEntries()<<endl;
	cout<<"nTracks: "<<etaptHist->GetEntries()<<endl<<endl;
	
	outputFile.Close();
}

void OneWindHandler::Init()
{
	cout<<"init"<<endl;
	init=true;
	nTracks1= 0;
	nTracks0= 0;
	TString string1;
	if (bSim == true) string1 = "Sim";
		else string1 = "Rec";
	if (bRaw==true) string1 = "Raw";
	TString name = myEventCutList->GetName() + myTrackCutList->GetName()+ string1 ;
	myNameHist = name;

	double n = 0;
	switch (systemType)
	{
	case pp:
		n = 1;
		break;
	case BeBe:
		n = 7;
		break;
	case ArSc:
		n = 40; 
		break;
	case PbPb:
		n = 208; 
		break;
	default:
		break;
	}
	double maxPSDEnergy = beamMomentum * n * 2;
	psdEnergyHist = new TH1D("PSDHist_" + name, "PSD energy distribution" + name + ";PSD(28);entries", 1000, 0, maxPSDEnergy);
	//acceptHist		= new TH3D("acceptance_"+name, "acceptance_"+name+ "; phi;Pt;eta;entries",31,-3.5,3.5,31,0,1.5, 61,0,10);
	
	chargeHist	= new TH1D("chargeHist_"+name,"chargeHist_"+name+";charge;entries",	5,		-2.5,	2.5);
	multHist	= new TH1D("multHist_"+name,"multHist_"+name+	";nTracks;entries",		301,	-0.5,	300.5);
    
    
 //   plusminusHist	= new TH2D("plusminus_correlation_cloud_"+name,	"plusminus_correlation_cloud_"+name+"; N-; N+; entries",	31,-0.5,300.5,	31,-0.5,300.5);
    etaptHist		= new TH2D("etapt_cloud_"+ name,"etapt_cloud_"+ name+	"; eta;Pt;entries",			600,0,10, 300,0,1.5);
    etaptHist0		= new TH2D("etapt_cloud0_"+name,"etapt_cloud0_"+name+	"; eta;Pt;entries (-)",		600,0,10, 300,0,1.5);
    etaptHist1		= new TH2D("etapt_cloud1_"+name,"etapt_cloud1_"+name+	"; eta;Pt;entries (+)",		600,0,10, 300,0,1.5);
	
	rapPtHist = new TH2D("rap_pt_cloud_" + name, "rap_pt_cloud_" + name + "; rapidity;Pt;entries",			600, -4, 4, 300, 0, 1.5);
	rapPtHist0 = new TH2D("rap_pt_cloud0_" + name, "rap_pt_cloud0_" + name + "; rapidity;Pt;entries (-)",	600, -4, 4, 300, 0, 1.5);
	rapPtHist1 = new TH2D("rap_pt_cloud1_" + name, "rap_pt_cloud1_" + name + "; rapidity;Pt;entries (+)",	600, -4, 4, 300, 0, 1.5);


    ptPhiHist		= new TH2D("ptPhi_cloud_"+name,"ptPhi_cloud_"+name+		"; phi;Pt;entries",			150,-3.5,3.5, 300,0,1.5); //!
    ptPhiHist0		= new TH2D("ptPhi_cloud0_"+name,"ptPhi_cloud0_"+name+	"; phi;Pt;entries (-)",		150,-3.5,3.5, 300,0,1.5);//!
    ptPhiHist1		= new TH2D("ptPhi_cloud1_"+name,"ptPhi_cloud1_"+name+	"; phi;Pt;entries (+)",		150,-3.5,3.5, 300,0,1.5);//!
	
	BPD1Hist = new TH2D("BPD1_cloud_" + name, "BPD1_cloud_" + name + "; x [cm]; y [cm]; entries", 100, -2, 2, 100, -2, 2); //!
	BPD2Hist = new TH2D("BPD2_cloud_" + name, "BPD2_cloud_" + name + "; x [cm]; y [cm]; entries", 100, -2, 2, 100, -2, 2);
	BPD3Hist = new TH2D("BPD3_cloud_" + name, "BPD3_cloud_" + name + "; x [cm]; y [cm]; entries", 100, -2, 2, 100, -2, 2);
	ImpParHist 		= new TH2D("ImpPar_"+name,"ImpPar_"+name+ 				"; b_x [cm]; b_y [cm]; entries", 400, -5, 5, 400, -5, 5); //Ya pomenyal (sdelal poshire dlya QA plots)
	
	WFAHist 	= new TH1D("WFA_"+ name,"WFA_"+ name+ ";time [ns]; entries",			251,	-30000,	30000); //Ya pomenyal
	fitVtxHist  = new TH1D("Fitted_Vtx_Position_"+name, "fitVtx_"+name+";z [cm]; entries", 7001,-700,0);
	
	dEdxPHist0 = new TH2D("dEdxP0_cloud_" + name, "dEdxP0_cloud_" + name + "; log10(p[GeV/c]); dE/dx (negative); entries", 601, -3, 3, 301, -0.05, 3.95); //Ya pomenyal (nado esche pomenyat', chtoby klalsya srazu log(p) v hist
	dEdxPHist1   = new TH2D("dEdxP1_cloud_"+name,"dEdxP1_cloud_"+name+ 		"; log10(p[GeV/c]); dE/dx (positive); entries", 601, -3, 3, 301, -0.05, 3.95); //Ya pomenyal (nado esche pomenyat', chtoby klalsya srazu log(p) v hist
    fitVtxNHist   = new TH2D("fitVtxN_cloud_"+name,"fitVtxN_cloud_"+name+ 		"; z [cm]; N; entries", 7001, -700, 0, 31,	-0.5,	300.5); //Ya pomenyal
    
	psdEnergyNHist = new TH2D("psdEnergyN_cloud_" + name, "psdEnergyN_cloud_" + name + "; PSD(28) [GeV]; N; entries", 1000, 0, maxPSDEnergy, 301, -0.5, 300.5); //Ya pomenyal
	psdEnergyS5Hist = new TH2D("psdEnergyS5_cloud_" + name, "psdEnergyS5_cloud_" + name + "; PSD(28) [GeV]; S5; entries", 1000, 0, maxPSDEnergy, 101, -0.5, 500.5);
	multS5Hist = new TH2D("S5N_cloud_" + name, "S5N_cloud_" + name + "; S5; N; entries", 101, -0.5, 500.5, 301, -0.5, 300.5);
    
    totClusterHist	= new TH1D("totClusterHist_"+name,"totClusterHist_"+name+	";nTotalClusters;entries",		301,	-0.5,	300.5); //Ya pomenyal
    verClusterHist	= new TH1D("verClusterHist_"+name,"verClusterHist_"+name+	";nVTPCClusters;entries",		301,	-0.5,	300.5); //Ya pomenyal
    gapClusterHist	= new TH1D("gapClusterHist_"+name,"gapClusterHist_"+name+	";nGTPCClusters;entries",		31,	-0.5,	30.5); //Ya pomenyal
    
    nBeamHist	= new TH1D("nBeamHist_"+name,"nBeamHist_"+name+	";nBeamParticles;entries",		31,	-0.5,	30.5); //Ya pomenyal

	pHist		= new TH1D("pHist_"+   name,"pHist_"+   name+	";p;entries",			500,	-1,		beamMomentum*2.5);
	pHist0 = new TH1D("pHist0_" + name, "pHist0_" + name + ";p;entries (-)", 500, -1, beamMomentum*2.5);	pHist0->SetLineColor(kBlue);
	pHist1 = new TH1D("pHist1_" + name, "pHist1_" + name + ";p;entries (+)", 500, -1, beamMomentum*2.5);	pHist1->SetLineColor(kRed);

	pxHist		= new TH1D("pxHist_"+  name,"pxHist_"+  name+";px;entries"    ,		300,	-2,		2);
	pxHist0		= new TH1D("pxHist0_"+ name,"pxHist0_"+ name+";px;entries (-)",		300,	-2,		2);		pxHist0->SetLineColor(kBlue);
	pxHist1		= new TH1D("pxHist1_"+ name,"pxHist1_"+ name+";px;entries (+)",		300,	-2,		2);		pxHist1->SetLineColor(kRed);

	pyHist		= new TH1D("pyHist_" + name,"pyHist_" + name+";py;entries"    ,		300,	-2,		2);
	pyHist0		= new TH1D("pyHist0_"+ name,"pyHist0_"+ name+";py;entries (-)",		300,	-2,		2);		pyHist0->SetLineColor(kBlue);
	pyHist1		= new TH1D("pyHist1_"+ name,"pyHist1_"+ name+";py;entries (+)",		300,	-2,		2);		pyHist1->SetLineColor(kRed);

	pzHist = new TH1D("pzHist_" + name, "pzHist_" + name + ";pz;entries", 500, -1, beamMomentum*2.5);
	pzHist0 = new TH1D("pzHist0_" + name, "pzHist0_" + name + ";pz;entries (-)", 500, -1, beamMomentum*2.5);	pzHist0->SetLineColor(kBlue);
	pzHist1 = new TH1D("pzHist1_" + name, "pzHist1_" + name + ";pz;entries (+)", 500, -1, beamMomentum*2.5);	pzHist1->SetLineColor(kRed);

	ptHist		= new TH1D("ptHist_" + name,"ptHist_" + name+";pt;entries "   ,		300,    0,		2);
	ptHist0		= new TH1D("ptHist0_"+ name,"ptHist0_"+ name+";pt;entries (-)",		300,    0,		2);		ptHist0->SetLineColor(kBlue);
	ptHist1		= new TH1D("ptHist1_"+ name,"ptHist1_"+ name+";pt;entries (+)",		300,    0,		2);		ptHist1->SetLineColor(kRed);

	etaHist		= new TH1D("etaHist_" +name,"etaHist_" +name+";etaLab;entries"    ,	600,	0,		10);
	etaHist0	= new TH1D("etaHist0_"+name,"etaHist0_"+name+";etaLab;entries (-)",	600,	0,		10);	etaHist0->SetLineColor(kBlue);
	etaHist1	= new TH1D("etaHist1_"+name,"etaHist1_"+name+";etaLab;entries (+)",	600,	0,		10);	etaHist1->SetLineColor(kRed);

	rapHist = new TH1D("rapHist_" + name, "rapHist_" + name + ";rapidity;entries", 600, -5, 5);
	rapHist0 = new TH1D("rapHist0_" + name, "rapHist0_" + name + ";rapidity;entries (-)", 600, -5, 5);	rapHist0->SetLineColor(kBlue);
	rapHist1 = new TH1D("rapHist1_" + name, "rapHist1_" + name + ";rapidity;entries (+)", 600, -5, 5);	rapHist1->SetLineColor(kRed);

	phiHist		= new TH1D("phiHist_" +name,"phiHist_" +name+";phi;entries",			150,	-3.5,	3.5);
	phiHist0	= new TH1D("phiHist0_"+name,"phiHist0_"+name+";phi;entries (-)",	150,	-3.5,	3.5);	phiHist0->SetLineColor(kBlue);
	phiHist1	= new TH1D("phiHist1_"+name,"phiHist1_"+name+";phi;entries (+)",	150,	-3.5,	3.5);	phiHist1->SetLineColor(kRed);

    BPD1XMultHist = new TH2D("BPD1X_Mult_cloud_" + name, "BPD1X_Mult_cloud_" + name + "; N; x [cm]; entries", 301,	-0.5,	300.5, 50, -2, 2);
    BPD1YMultHist = new TH2D("BPD1Y_Mult_cloud_" + name, "BPD1Y_Mult_cloud_" + name + "; N; y [cm]; entries", 301,	-0.5,	300.5, 50, -2, 2);
	BPD2XMultHist = new TH2D("BPD2X_Mult_cloud_" + name, "BPD2X_Mult_cloud_" + name + "; N; x [cm]; entries", 301,	-0.5,	300.5, 50, -2, 2);
	BPD2YMultHist = new TH2D("BPD2Y_Mult_cloud_" + name, "BPD2Y_Mult_cloud_" + name + "; N; y [cm]; entries", 301,	-0.5,	300.5, 50, -2, 2);
	BPD3XMultHist = new TH2D("BPD3X_Mult_cloud_" + name, "BPD3X_Mult_cloud_" + name + "; N; x [cm]; entries", 301,	-0.5,	300.5, 50, -2, 2);
    BPD3YMultHist = new TH2D("BPD3Y_Mult_cloud_" + name, "BPD3Y_Mult_cloud_" + name + "; N; y [cm]; entries", 301,	-0.5,	300.5, 50, -2, 2);

	SlopeBeamXZHist = new TH2D("BeamSlopeXZ_Mult_cloud_" + name, "BeamSlopeXZ_Mult_cloud_" + name + "; N; Slope; entries", 301,	-0.5,	300.5, 100, -0.001, 0.001);
	SlopeBeamYZHist = new TH2D("BeamSlopeYZ_Mult_cloud_" + name, "BeamSlopeYZ_Mult_cloud_" + name + "; N; Slope; entries", 301,	-0.5,	300.5, 100, -0.001, 0.001);

}



void OneWindHandler::PutTrack(const evt::rec::VertexTrack& vtxTrack, Event& ev)
{
	if (init==false) this->Init();
	if (bRaw){
		if (bEventInfoFull==false){
			myEventInfo.Reset();
			myEventInfo.DissectEvent(ev);
			bEventInfoFull=true;
		}
	}
	
	if (myTrackCutList->TrackTest(vtxTrack,ev) == 0) return;
	//cout << "put Track" << endl;
	double_t p, pX, pY, pZ, pT, phi, eta, dEdx;
	Vector	vtxMomentum = vtxTrack.GetMomentum();
	pX = vtxMomentum.GetX();
	pY = vtxMomentum.GetY();
	pZ = vtxMomentum.GetZ();
	p = vtxMomentum.GetMag();

	phi = TMath::ATan2(pY, pX); //phi = [-pi;pi] 
	double mass = 0.1396;
	double E = sqrt(pow(mass, 2) + p*p);
	double rapidity = 0.5 * log((E + pZ) / (E - pZ)) - beamRapidity/2;


	pT = sqrt(pX*pX + pY*pY);
	eta = -0.5*TMath::Log((p-pZ)/(p+pZ));

	dEdx = 0;

	chargeHist->Fill(vtxTrack.GetCharge());

	pHist->Fill(p);
	pxHist->Fill(pX);
	pyHist->Fill(pY);
	pzHist->Fill(pZ);
	ptHist->Fill(pT);
	etaHist->Fill(eta);
	rapHist->Fill(rapidity);
	phiHist->Fill(phi);


	if (vtxTrack.GetCharge() == 1)
	{
		pHist1->Fill(p);
		pxHist1->Fill(pX);
		pyHist1->Fill(pY);
		pzHist1->Fill(pZ);
		ptHist1->Fill(pT);
		etaHist1->Fill(eta);
		rapHist1->Fill(rapidity);
		phiHist1->Fill(phi);
		nTracks1++;
		etaptHist1->Fill(eta, pT);
		rapPtHist1->Fill(rapidity, pT);
		ptPhiHist1->Fill(phi, pT);
	}
	else
	{
		pHist0->Fill(p);
		pxHist0->Fill(pX);
		pyHist0->Fill(pY);
		pzHist0->Fill(pZ);
		ptHist0->Fill(pT);
		etaHist0->Fill(eta);
		phiHist0->Fill(phi);
		rapHist0->Fill(rapidity);
		nTracks0++;
		etaptHist0->Fill(eta, pT);
		rapPtHist0->Fill(rapidity, pT);
		ptPhiHist0->Fill(phi, pT);
	}

	//acceptHist->Fill(phi, pT, eta);
	rapPtHist->Fill(rapidity, pT);
	etaptHist->Fill(eta, pT);
	ptPhiHist->Fill(phi, pT);
 
    RecEvent* pRecEvent=&ev.GetRecEvent();
    const Vertex& mainVertex = pRecEvent->GetMainVertex();
	const Point& impactPoint = vtxTrack.GetImpactPoint();
    const Vector vertexToImpact =  mainVertex.GetPosition() - impactPoint;
    const double_t bx = vertexToImpact.GetX();
    const double_t by = vertexToImpact.GetY();
    
    const Track& track = pRecEvent->Get(vtxTrack.GetTrackIndex()); //Ya pomenyal
    dEdx = track.GetEnergyDeposit(evt::rec::TrackConst::eAll); //Ya pomenyal
    
	if (vtxTrack.GetCharge() == 1)
		dEdxPHist1->Fill(log(p)/log(10),dEdx);
	else 
		dEdxPHist0->Fill(log(p) / log(10), dEdx);
	ImpParHist->Fill(bx,by);
    
    int nTotalTPC, nVTPC, nGTPC; //Ya pomenyal
    nTotalTPC   =   track.GetNumberOfClusters(TrackConst::eAll); //Ya pomenyal
    nVTPC       =   track.GetNumberOfClusters(TrackConst::eVTPC1) + track.GetNumberOfClusters(TrackConst::eVTPC2); //Ya pomenyal
    nGTPC       =   track.GetNumberOfClusters(TrackConst::eGTPC); //Ya pomenyal
    
    totClusterHist->Fill(nTotalTPC); //Ya pomenyal
    verClusterHist->Fill(nVTPC); //Ya pomenyal
    gapClusterHist->Fill(nGTPC); //Ya pomenyal
    
}

void OneWindHandler::PutTrack(const evt::sim::VertexTrack& vtxTrack, Event& ev)
{
	if (init==false) this->Init();
	if (myTrackCutList->TrackTest(vtxTrack,ev) == 0) return;

//	cout << "put Track SIM" << endl;
	double_t p, pX, pY, pZ, pT, phi, eta, dEdx;
	Vector	vtxMomentum = vtxTrack.GetMomentum(); 
	pX	=	vtxMomentum.GetX();
	pY	=	vtxMomentum.GetY();
	pZ	=	vtxMomentum.GetZ();
	p	=	vtxMomentum.GetMag();

	phi	=	TMath::ATan2(pY,pX); //phi = [-pi;pi] 
	double mass = 0.1396;
	double E = sqrt(pow(mass, 2) + p*p);
	double rapidity = 0.5 * log((E + pZ) / (E - pZ)) - beamRapidity/2;

    
	pT	=	sqrt(pX*pX+pY*pY);	
	eta=-0.5*TMath::Log((p-pZ)/(p+pZ));
    
    dEdx = 0;

	chargeHist	-> Fill(vtxTrack.GetCharge());
	
	pHist	->	Fill(p);	
	pxHist	->	Fill(pX);	
	pyHist	->	Fill(pY);	
	pzHist	->	Fill(pZ);	
	ptHist	->	Fill(pT);	
	etaHist	->	Fill(eta);
	rapHist->	Fill(rapidity);
	phiHist ->	Fill(phi);

    
	if (vtxTrack.GetCharge()==1)
	{
		pHist1	->	Fill(p);	
		pxHist1	->	Fill(pX);	
		pyHist1	->	Fill(pY);	
		pzHist1	->	Fill(pZ);	
		ptHist1	->	Fill(pT);	
		etaHist1->	Fill(eta);
		rapHist1->	Fill(rapidity);
		phiHist1->	Fill(phi);
        nTracks1++;
        etaptHist1->Fill(eta,pT);
		rapPtHist1->Fill(rapidity, pT);
        ptPhiHist1->Fill(phi,pT);
	} 
	if (vtxTrack.GetCharge() == -1)
	{
		pHist0	->	Fill(p);	
		pxHist0	->	Fill(pX);	
		pyHist0	->	Fill(pY);	
		pzHist0	->	Fill(pZ);	
		ptHist0	->	Fill(pT);	
		etaHist0->	Fill(eta);
		phiHist0->	Fill(phi);
		rapHist0->	Fill(rapidity);
        nTracks0++;
        etaptHist0->Fill(eta,pT);
		rapPtHist0->Fill(rapidity, pT);
        ptPhiHist0->Fill(phi,pT);
	}
	
    //acceptHist  -> Fill(phi,pT,eta);
	rapPtHist->Fill(rapidity, pT);
    etaptHist->Fill(eta,pT);
    ptPhiHist->Fill(phi,pT);
    
	if (vtxTrack.GetCharge() == 1)
		dEdxPHist1->Fill(log(p) / log(10), dEdx);
	if (vtxTrack.GetCharge() == -1)
		dEdxPHist0->Fill(log(p) / log(10), dEdx);
}

void OneWindHandler::EndOfEvent(Event& ev)
{
	if (init==false) {this->Init();}
    double N = nTracks0 + nTracks1;
	multHist->Fill(N);
//    plusminusHist -> Fill(nTracks0,nTracks1);
 
	
    
	bEventInfoFull = false;
	fitVtxHist	->Fill(myEventInfo.myFitVtxZ);
	fitVtxNHist->Fill(myEventInfo.myFitVtxZ, N); //Ya pomenyal
	BPD1Hist	->Fill(myEventInfo.myBPD[0],myEventInfo.myBPD[1]);
	BPD2Hist	->Fill(myEventInfo.myBPD[2],myEventInfo.myBPD[3]);
	BPD3Hist	->Fill(myEventInfo.myBPD[4],myEventInfo.myBPD[5]);
	psdEnergyHist->Fill(myEventInfo.myEnergyPSD);
    BPD1XMultHist -> Fill(N,myEventInfo.myBPD[0]);
    BPD1YMultHist -> Fill(N,myEventInfo.myBPD[1]);
    BPD2XMultHist -> Fill(N,myEventInfo.myBPD[2]);
    BPD2YMultHist -> Fill(N,myEventInfo.myBPD[3]);
    BPD3XMultHist -> Fill(N,myEventInfo.myBPD[4]);
    BPD3YMultHist -> Fill(N,myEventInfo.myBPD[5]);

    SlopeBeamXZHist ->Fill(N, myEventInfo.myBeamSlopeZX);
    SlopeBeamYZHist ->Fill(N, myEventInfo.myBeamSlopeZY);

	psdEnergyNHist->Fill(myEventInfo.myEnergyPSD, N);
	psdEnergyS5Hist->Fill(myEventInfo.myEnergyPSD, myEventInfo.myS5ADC);
	multS5Hist->Fill(myEventInfo.myS5ADC, N);
    
    nBeamHist->Fill(myEventInfo.myTimeStructureWFA.size()); //Ya pomenyal
    for (unsigned int i = 0; i < myEventInfo.myTimeStructureWFA.size(); ++i){ //Ya pomenyal
        WFAHist->Fill(myEventInfo.myTimeStructureWFA.at(i)-(wfaTime1 + wfaTime2) / 2.); //Ya pomenyal
    }; //Ya pomenyal
    
    
    nTracks0=0; //Ya pomenyal
    nTracks1=0; //Ya pomenyal
}

LRCHandler::LRCHandler(const char* nameOut):BaseHandler(nameOut)
{
	cout<<"LRCHandler constructor ..."<<endl;
	myForwardTrackCutList = new CutList();
	myBackwardTrackCutList= new CutList();
}

LRCHandler::LRCHandler(const char* nameOut, bool simE):BaseHandler(nameOut, simE)
{
	cout<<"LRCHandler constructor ..."<<endl;
	myForwardTrackCutList = new CutList();
	myBackwardTrackCutList= new CutList();
}

void LRCHandler::Init()
{
//	cout<<"INIT"<<endl;
    init = true;
	// --- counters
	nTracksB	= 0;		nTracksF	= 0;
	nTracksB0	= 0;		nTracksF0	= 0;
	nTracksB1	= 0;		nTracksF1	= 0;
	nTracks0	= 0;		nTracks1	= 0;
	
	chargeB		= 0;		chargeF		= 0;

	sumPtB		= 0;		sumPtF		= 0;
	sumPtB0		= 0;		sumPtF0		= 0;
	sumPtB1		= 0;		sumPtF1		= 0;
    
    
    shalala     = 0;
    
	// --- hists
	TString name = myEventCutList->GetName() + myTrackCutList->GetName() + "_Back_" +
		myBackwardTrackCutList->GetName() + "_For_" + myForwardTrackCutList->GetName();

	if (bSim==true) name = name + "_Sim";
	else { 
		if (bRaw == false)
			name = name + "_Rec";
		else 
			name = name + "_Raw";
	}
	myNameHist = name;
	NetChHist	= new TH2D("NetCh_correlation_cloud_all"+myNameHist,myNameHist+			"; Qf; Qb; entries",	
		21,-10.5,10.5,	21,-10.5,10.5);
	TestPtPtHist = new TH2D("TEST_ALL_PTPT"+name, "TEST_ALL_PTPT_"+name+ ";pt_f_all;pt_b_all; entries", 
		arNBins[2], arXmin[2], arXmax[2],  arNBins[2], arXmin[2], arXmax[2]);


	myAllChargeTHSparse		 = new THnSparseD("All_charges_"+name, name, nBins, arNBins, arXmin, arXmax);
	myPositiveTHSparse		 = new THnSparseD("Positive_"+name, name, nBins, arNBins, arXmin, arXmax);
	myNegativeTHSparse		 = new THnSparseD("Negative_"+name, name, nBins, arNBins, arXmin, arXmax);
	myPosBackNegForTHSparse	 = new THnSparseD("PosBackNegFor_"+name, name, nBins, arNBins, arXmin, arXmax);
	myNegBackPosForTHSparse	 = new THnSparseD("NegBackPosFor_"+name, name, nBins, arNBins, arXmin, arXmax);

}

void LRCHandler::AddForwardCut(TrackCut* cut)
{
    myForwardTrackCutList->AddCut(cut);
}

void LRCHandler::AddBackwardCut(TrackCut* cut)
{
    myBackwardTrackCutList->AddCut(cut);
}

LRCHandler::~LRCHandler()
{
	cout<<"LRCHandler destructor are starting ..."<<endl;
	//--- output results 
	cout<<"name outFile = "<<nameOutFile<<endl;
	TFile outputFile(nameOutFile, "RECREATE");
	outputFile.cd();
	cout<<"writing..."<<endl; 

//	TH2D*	NetChHist	= new TH2D("NetCh_correlation_cloud_all"+myNameHist,myNameHist+			"; Qf; Qb; entries",		21,-10.5,10.5,	21,-10.5,10.5);

	TH2D*   ArNNHist[nChargeComb];
	TH2D*	ArPtNHist[nChargeComb];
	TH2D*	ArPtPtHist[nChargeComb]; 

	//ALL__POS__NEG___ForNEG_BackPOS____ForPOS_BackNEG

	ArNNHist[0]		= new TH2D("NN_correlation_cloud_all"+myNameHist,myNameHist+			"; nf;nb;entries",			301,-0.5,300.5,	301,-0.5,300.5);
	ArNNHist[1]		= new TH2D("NN_correlation_cloud_neg_neg"+myNameHist,myNameHist+		"; nf(-);nb(-);entries",	301,-0.5,300.5,	301,-0.5,300.5);
	ArNNHist[2]		= new TH2D("NN_correlation_cloud_pos_pos"+myNameHist,myNameHist+		"; nf(+);nb(+);entries",	301,-0.5,300.5,	301,-0.5,300.5);
	ArNNHist[3]		= new TH2D("NN_correlation_cloud_neg_pos"+myNameHist,myNameHist+	"; nf(-);nb(+);entries",	301,-0.5,300.5,	301,-0.5,300.5);
	ArNNHist[4]		= new TH2D("NN_correlation_cloud_pos_neg"+myNameHist,myNameHist+	"; nf(+);nb(-);entries",	301,-0.5,300.5,	301,-0.5,300.5);

	ArPtNHist[0]	= new TH2D("PtN_correlation_cloud_all"+myNameHist,myNameHist+			"; nf;PtB;entries",			301,-0.5,300.5,	31,0,1.5);
	ArPtNHist[1]	= new TH2D("PtN_correlation_cloud_neg_neg"+myNameHist,myNameHist+		"; nf(-);PtB(-);entries",	301,-0.5,300.5,	31,0,1.5);
	ArPtNHist[2]	= new TH2D("PtN_correlation_cloud_pos_pos"+myNameHist,myNameHist+		"; nf(+);PtB(+);entries",	301,-0.5,300.5,	31,0,1.5);
	ArPtNHist[3]	= new TH2D("PtN_correlation_cloud_neg_pos"+myNameHist,myNameHist+	"; nf(-);PtB(+);entries",	301,-0.5,300.5,	31,0,1.5);
	ArPtNHist[4]	= new TH2D("PtN_correlation_cloud_pos_neg"+myNameHist,myNameHist+	"; nf(+);PtB(-);entries",	301,-0.5,300.5,	31,0,1.5);

	ArPtPtHist[0]	= new TH2D("PtPt_correlation_cloud_all"+myNameHist,myNameHist+			"; PtF;PtB;entries",		31,0,1.5,	31,0,1.5);
	ArPtPtHist[1]	= new TH2D("PtPt_correlation_cloud_neg_neg"+myNameHist,myNameHist+		"; PtF(-);PtB(-);entries",	31,0,1.5,	31,0,1.5);
	ArPtPtHist[2]	= new TH2D("PtPt_correlation_cloud_pos_pos"+myNameHist,myNameHist+		"; PtF(+);PtB(+);entries",	31,0,1.5,	31,0,1.5);
	ArPtPtHist[3]	= new TH2D("PtPt_correlation_cloud_neg_pos"+myNameHist,myNameHist+	"; PtF(-);PtB(+);entries",	31,0,1.5,	31,0,1.5);
	ArPtPtHist[4]	= new TH2D("PtPt_correlation_cloud_pos_neg"+myNameHist,myNameHist+	"; PtF(+);PtB(-);entries",	31,0,1.5,	31,0,1.5);

	THnSparseD* ArSparse[nChargeComb];
	ArSparse[0] =		myAllChargeTHSparse;	
	ArSparse[1] =		myPositiveTHSparse;		
	ArSparse[2] =		myNegativeTHSparse;		
	ArSparse[3] =		myPosBackNegForTHSparse;
	ArSparse[4] =		myNegBackPosForTHSparse;
	
	Long64_t nEntries=0;
	for (Long64_t i = 0; i<nChargeComb; i++){
		nEntries = ArSparse[i]->GetNbins();
		double_t NF, NB, PtF, PtB;
		double_t binContent = 0;
		Int_t pCoord[nBins] = {0}; 
		double_t arVal[nBins] = {0};
		for (int j=0; j<nEntries; j++){
			binContent = ArSparse[i] -> GetBinContent(j,pCoord);
			for (int k=0; k<nBins; k++)
				arVal[k] = arXmin[k] + (pCoord[k]-0.5)*(arXmax[k]-arXmin[k])/arNBins[k];
			NF = arVal[0];
			NB = arVal[1];
			PtF = arVal[2];
			PtB = arVal[3]; 
			
			ArNNHist[i]			->Fill(NF, NB, binContent);
			if (NB != 0)
				ArPtNHist[i]	->Fill(NF, PtB, binContent);
			if (NB*NF != 0)
				ArPtPtHist[i]	->Fill(PtF, PtB, binContent);

		}
	}
	
	//psdEnergyHist->Write();
	TH1F* eventStat = myEventCutList->GetStatHist(nameOutFile+myNameHist, bSim);
	TH1F* trackStat = myTrackCutList->GetStatHist(nameOutFile+myNameHist, bSim);
	TestPtPtHist->Write();
	eventStat->Write();
	trackStat->Write();
	NetChHist->Write();

	for (int i=0; i<nChargeComb; i++){
		ArSparse[i]->Write();
		ArNNHist[i]->Write();
		ArPtNHist[i]->Write();
		ArPtPtHist[i]->Write();
	}
	

	cout<<"closing..."<<endl;
	outputFile.Close();
	cout<<"deleting ..."<<endl;

	cout<<"LRCHandler destructor has finish"<<endl;
}



void LRCHandler::PutTrack(const evt::rec::VertexTrack& vtxTrack, Event& ev)
{
    if (init==false) this->Init();
	if (myTrackCutList->TrackTest(vtxTrack,ev) == 0) return;
    shalala++;
	bool addB = true;
	bool addF = true;
	int charge = vtxTrack.GetCharge();
	if (charge == 1)
		nTracks1++;
	if (charge == -1)
		nTracks0++;

	if (myBackwardTrackCutList->TrackTest(vtxTrack,ev) == 0) 
		addB = false;
	if (myForwardTrackCutList->TrackTest(vtxTrack,ev) == 0)
		addF = false;
	if (addB == false && addF == false) return;

	double_t pX, pY, pT;
	const Vector vtxMomentum = vtxTrack.GetMomentum(); 
	pX	=	vtxMomentum.GetX();
	pY	=	vtxMomentum.GetY();
	pT	=	sqrt(pX*pX+pY*pY);
    
	if (addB){
		nTracksB++;
		sumPtB = sumPtB + pT;
		if (charge==1){
			nTracksB1++;
			sumPtB1 = sumPtB1 + pT;
		} else {
			nTracksB0++;
			sumPtB0 = sumPtB0 + pT;
		}
		chargeB = chargeB + charge;
	}
	if (addF){
		nTracksF++;
		sumPtF = sumPtF + pT;
		if (charge==1){
			nTracksF1++;
			sumPtF1 = sumPtF1 + pT;
		} else {
			nTracksF0++;
			sumPtF0 = sumPtF0 + pT;
		}
		chargeF = chargeF + charge;
	}
}


void LRCHandler::PutTrack(const evt::sim::VertexTrack& vtxTrack, Event& ev)
{
	if (init==false) {this->Init();}
	if (myTrackCutList->TrackTest(vtxTrack,ev) == 0) return;
    shalala++;
	bool addB = true;
	bool addF = true;
	int charge = vtxTrack.GetCharge();
	if (charge==1)
		nTracks1++;
	if (charge == -1)
		nTracks0++;

	if (myBackwardTrackCutList->TrackTest(vtxTrack,ev) == 0) 
		addB = false;
	if (myForwardTrackCutList->TrackTest(vtxTrack,ev) == 0)
		addF = false;
	if (addB == false && addF == false) return;

	double_t pX, pY, pT;
	const Vector vtxMomentum = vtxTrack.GetMomentum(); 
	pX	=	vtxMomentum.GetX();
	pY	=	vtxMomentum.GetY();
	pT	=	sqrt(pX*pX+pY*pY);
    
	if (addB){
		nTracksB++;
		sumPtB = sumPtB + pT;
		if (charge==1){
			nTracksB1++;
			sumPtB1 = sumPtB1 + pT;
		} else {
			nTracksB0++;
			sumPtB0 = sumPtB0 + pT;
		}
		chargeB = chargeB + charge;
	}
	if (addF){
		nTracksF++;
		sumPtF = sumPtF + pT;
		if (charge==1){
			nTracksF1++;
			sumPtF1 = sumPtF1 + pT;
		} else {
			nTracksF0++;
			sumPtF0 = sumPtF0 + pT;
		}
		chargeF = chargeF + charge;
	}
}
void LRCHandler::EndOfEvent(Event& ev)
{
	if (init==false) {this->Init();}
	// --- filling hists
	int qB = nTracksB1 - nTracksB0;
	int qF = nTracksF1 - nTracksF0;
	NetChHist->Fill(qF, qB);

	if ((nTracksF1 + nTracksF0 != 0) && (nTracksB1 + nTracksB0 != 0))
		TestPtPtHist->Fill((sumPtF1 + sumPtF0)/(nTracksF1 + nTracksF0), 
				(sumPtB1 + sumPtB0)/(nTracksB1 + nTracksB0));

	Double_t arValLRC[nBins] = {0};
	arValLRC[nBins-1] = myHistInSparseCounter;
	myHistInSparseCounter++; 
	if (myHistInSparseCounter == arNBins[nBins-1])
		myHistInSparseCounter = 0;

	//All charges  
	arValLRC[0] = nTracksF1 + nTracksF0;
	arValLRC[1] = nTracksB1 + nTracksB0;
	if (nTracksF1 + nTracksF0 != 0)
		arValLRC[2] = (sumPtF1 + sumPtF0)/(nTracksF1 + nTracksF0);
	else 
		arValLRC[2] = 0;
	if (nTracksB1 + nTracksB0 != 0)
		arValLRC[3] = (sumPtB1 + sumPtB0)/(nTracksB1 + nTracksB0);
	else 
		arValLRC[3] = 0;
	myAllChargeTHSparse->Fill(arValLRC);

	//Positive charges
	arValLRC[0] = nTracksF1;
	arValLRC[1] = nTracksB1;
	if (nTracksF1 != 0)
		arValLRC[2] = (sumPtF1)/(nTracksF1);
	else 
		arValLRC[2] = 0;
	if (nTracksB1 != 0)
		arValLRC[3] = (sumPtB1)/(nTracksB1);
	else 
		arValLRC[3] = 0;
	myPositiveTHSparse->Fill(arValLRC);

	//Negative charges
	arValLRC[0] = nTracksF0;
	arValLRC[1] = nTracksB0;
	if (nTracksF0 != 0)
		arValLRC[2] = (sumPtF0)/(nTracksF0);
	else 
		arValLRC[2] = 0;
	if (nTracksB0 != 0)
		arValLRC[3] = (sumPtB0)/(nTracksB0);
	else 
		arValLRC[3] = 0;
	myNegativeTHSparse->Fill(arValLRC);

	//Positive -> Back; Negative ->Forward
	arValLRC[0] = nTracksF0;
	arValLRC[1] = nTracksB1;
	if (nTracksF0 != 0)
		arValLRC[2] = (sumPtF0)/(nTracksF0);
	else 
		arValLRC[2] = 0;
	if (nTracksB1 != 0)
		arValLRC[3] = (sumPtB1)/(nTracksB1);
	else 
		arValLRC[3] = 0;
	myPosBackNegForTHSparse->Fill(arValLRC);

	//Positive -> Forward, Negative -> Backward 
	arValLRC[0] = nTracksF1;
	arValLRC[1] = nTracksB0;
	if (nTracksF1 != 0)
		arValLRC[2] = (sumPtF1)/(nTracksF1);
	else 
		arValLRC[2] = 0;
	if (nTracksB0 != 0)
		arValLRC[3] = (sumPtB0)/(nTracksB0);
	else 
		arValLRC[3] = 0;
	myNegBackPosForTHSparse->Fill(arValLRC);


	// --- counters
	nTracksB	= 0;		nTracksF	= 0;
	nTracksB0	= 0;		nTracksF0	= 0;
	nTracksB1	= 0;		nTracksF1	= 0;
	nTracks1	= 0;		nTracks0	= 0;
	chargeB		= 0;		chargeF		= 0;

	sumPtB		= 0;		sumPtF		= 0;
	sumPtB0		= 0;		sumPtF0		= 0;
	sumPtB1		= 0;		sumPtF1		= 0; 
    shalala     = 0;
}

//bool PhiGeneralization (double_t& minPhi, double_t& maxPhi)
//{
//    if (minPhi>=maxPhi) 
//    {
//        cout << "maxPhi < or = minPhi => phiSelection is closed" << endl; 
//        return 0;
//    }
//    double pi = TMath::Pi();
//    if ((maxPhi-minPhi)>2*pi)
//    {
//        cout << "Phi interval is more than 2pi => phiSelection is closed" << endl;
//        return 0;
//    }
//    minPhi = minPhi % (2*pi);
//    maxPhi = maxPhi % (2*pi);
//    if (minPhi>pi) minPhi=minPhi-2*pi;
//    if (maxPhi>pi) maxPhi=maxPhi-2*pi;
//    return 1;
//}


void BaseHandler::AddTrigger(eTrigger trigger)
{
	Cut* T = new TriggerCut(trigger);
	myEventCutList->AddCut(T);
}

void BaseHandler::AddT2Cut()
{
    Cut* T2 = new T2Cut();
    myEventCutList->AddCut(T2);
}

void BaseHandler::AddS1_1Cut()
{
    Cut* S1_1 = new S1_1Cut();
    myEventCutList->AddCut(S1_1);
}

void BaseHandler::AddWFACut()
{
    Cut* WFA = new WFACut();
    myEventCutList->AddCut(WFA);
}

void BaseHandler::AddWFACut(double wfa_Time1, double wfa_Time2, double wfa_TimeCut)
{
    Cut* WFA = new WFACut(wfa_Time1, wfa_Time2, wfa_TimeCut);
    myEventCutList->AddCut(WFA);
}

void BaseHandler::AddChargeCut()
{
    Cut* A = new ChargeCut();
    myEventCutList->AddCut(A);
}

void BaseHandler::AddBPDCut()
{
    Cut* A = new BPDCut();
    myEventCutList->AddCut(A);
}

void BaseHandler::AddDirectBPDCut(double minX, double maxX, double minY, double maxY, eMyBPD bpd)
{
	Cut* A = new DirectBPDCut(minX, maxX, minY, maxY, bpd);
	myEventCutList->AddCut(A);
}

void BaseHandler::AddMainVtxCut()
{
    Cut* A = new MainVertexCut();
    myEventCutList->AddCut(A);
}

void BaseHandler::AddFittedVtxCut()
{
    Cut* A = new FittedVertexCut();
    myEventCutList->AddCut(A);
}

void BaseHandler::AddFitQualityCut()
{
    Cut* A = new FitQualityCut();
    myEventCutList->AddCut(A);
}

void BaseHandler::AddFitQualityCut(Int_t qual)
{
    Cut* A = new FitQualityCut(qual);
    myEventCutList->AddCut(A);
}

void BaseHandler::AddNVtxTracksCut()
{
    Cut* A = new NVertexTracksCut();
    myEventCutList->AddCut(A);
}

void BaseHandler::AddNVtxTracksCut(unsigned int n)
{
    Cut* A = new NVertexTracksCut(n);
    myEventCutList->AddCut(A);
}

void BaseHandler::AddZVtxCut()
{
    Cut* A = new ZVertexCut();
    myEventCutList->AddCut(A);
}

void BaseHandler::Remove0EPSDEvents(ePSDModulCombinations ePSDMod)
{
	Cut* A = new RunWith0EnergyInOneModuleCutVer2(bRaw, ePSDMod);
	myEventCutList->AddCut(A);
}
void BaseHandler::RemoveBadRuns()
{
    Cut* A = new BadRunCut(bRaw);
    myEventCutList->AddCut(A);
}

void BaseHandler::AddZVtxCut(double_t minZ, double_t maxZ)
{
    if (minZ>maxZ) 
        cout<<"Error. Can't add ZVertexCut with minZ > maxZ"<<endl;
	else {
        Cut* A = new ZVertexCut(minZ, maxZ);
        myEventCutList->AddCut(A);
    }
}

void BaseHandler::AddCentrality(double_t minPer, double_t maxPer)
{
    if (minPer>1 || minPer<0) {
		cout<<"Error. Can't add centrality cut with minPer < 0 or >1"<<endl;
		return;
	}
    if (maxPer>1 || maxPer<0) {
		cout<<"Error. Can't add centrality cut with maxPer < 0 or >1"<<endl;
		return;
	}
	
	if (minPer>=maxPer){
        cout<<"Error. Can't add centrality cut with minPer >= maxPer"<<endl;
		return;
	}
	
	if (this->IsItRaw()){
		Cut* A = new CentralityCut(minPer,maxPer,1);
		myEventCutList->AddCut(A);
	}else{
		Cut* A = new CentralityCut(minPer,maxPer,0);
		myEventCutList->AddCut(A);
	}
}

void BaseHandler::AddVtxTrackStatusCut()
{
    Cut* A = new VtxTrackStatusCut();
    myTrackCutList->AddCut(A);
}

void BaseHandler::AddImpactPointCut()
{
    Cut* A = new ImpactPointCut();
    myTrackCutList->AddCut(A);
}

void BaseHandler::AddImpactPointCut(double_t max_dX, double_t max_dY)
{
    if (max_dX <= 0) 
        cout<<"Error. Can't add ImpactPointCut with dX<=0"<<endl;
    else if (max_dY <= 0)
        cout<<"Error. Can't add ImpactPointCut with dY<=0"<<endl;
    else {
        Cut* A = new ImpactPointCut(max_dX, max_dY);
        myTrackCutList->AddCut(A);
    }
}

void BaseHandler::AddVTPCAndGTPCCut()
{
    Cut* A = new VTPCAndGTPCClustersCut;
    myTrackCutList->AddCut(A);
}

void BaseHandler::AddVTPCAndGTPCCut(unsigned int VTPC, unsigned int GTPC)
{
    Cut* A = new VTPCAndGTPCClustersCut(VTPC, GTPC);
    myTrackCutList->AddCut(A);
}

void BaseHandler::AddTotalTPCCut()
{
    Cut* A = new TotalTPCClustersCut();
    myTrackCutList->AddCut(A);
}

void BaseHandler::AddTotalTPCCut(unsigned int N)
{
    Cut* A = new TotalTPCClustersCut(N);
    myTrackCutList->AddCut(A);
}

void BaseHandler::AddTOFCut()
{
    Cut* A = new TOFCut();
    myTrackCutList->AddCut(A);
}

void BaseHandler::AddPtCut()
{
    Cut* A = new PtCut();
    myTrackCutList->AddCut(A);
}

void BaseHandler::AddPtCut(double_t minPt, double_t maxPt)
{
    if (minPt<0)
        cout<<"Error. Error. Can't add Pt Cut with minPt<=0"<<endl;
    else if (minPt>=maxPt)
        cout<<"Error. Error. Can't add Pt Cut with minPt>=maxPt"<<endl;
    else {
        Cut* A = new PtCut(minPt, maxPt);
        myTrackCutList->AddCut(A);
    }
}

void BaseHandler::AddPCut()
{
    Cut* A = new PCut();
    myTrackCutList->AddCut(A);
}

void BaseHandler::AddPCut(double_t minP, double_t maxP)
{
    if (minP>=maxP)
        cout<<"Error. Error. Can't add P Cut with minP>=maxP"<<endl;
    else {
        Cut* A = new PCut(minP, maxP);
        myTrackCutList->AddCut(A);
    }
}

void BaseHandler::AddAcceptCut()
{
    Cut* A = new AcceptCut();
    myTrackCutList->AddCut(A);
}

void BaseHandler::AddAcceptRapidityCut()
{
	Cut* A = new AcceptRapidityCut();
	myTrackCutList->AddCut(A);
}

void BaseHandler::AddEECut()
{
	Cut* A = new eeCut();
	myTrackCutList->AddCut(A);
}


void BaseHandler::AddEtaCut(double_t minEta, double_t maxEta)
{
	if (minEta>maxEta) 
		cout<<nameOutFile<<": Error. minEta>maxEta"<<endl;
	Cut* A = new EtaCut(minEta, maxEta);
	myTrackCutList->AddCut(A);
}

void BaseHandler::AddPhiCut(double_t minPhi, double_t maxPhi)
{
	if (minPhi<(-1*TMath::Pi()) || maxPhi>(TMath::Pi())) 
		cout<<nameOutFile<<": warning. phi = [-pi;pi]"<<endl;
	if (minPhi>maxPhi) 
		cout<<nameOutFile<<": Error. minPhi>maxPhi"<<endl;
	Cut* A = new PhiCut(minPhi, maxPhi);
	myTrackCutList->AddCut(A);
}

void BaseHandler::AddChargeTrkCut(int W)
{
	Cut* A = new ChargeTrCut(W);
	myTrackCutList->AddCut(A);
}

void BaseHandler::AddDiluteCut(double remove_track_percent)
{
	Cut* A = new DiluteCut(remove_track_percent);
	myTrackCutList->AddCut(A);
}

void BaseHandler::AddPSDCloudsCut()
{
	Cut* A = new MultPSDCloudCut(bRaw, good);
	myEventCutList->AddCut(A);
}

void BaseHandler::AddPSDCloudsCut(ePSDNClouds eCloud)
{
	Cut* A = new MultPSDCloudCut(bRaw, eCloud);
	myEventCutList->AddCut(A);
}

void BaseHandler::AddPSDEnergyCut(double minE, double maxE, ePSDModulCombinations psdSet)
{
	Cut* A = new PSDEnergyCut(minE, maxE, psdSet,bRaw);
	myEventCutList->AddCut(A);

}

void BaseHandler::AddPSDEnergyCut(double maxE, ePSDModulCombinations psdSet)
{
	Cut* A = new PSDEnergyCut(0, maxE, psdSet, bRaw);
	myEventCutList->AddCut(A);

}

void BaseHandler::AddS5Cut(double upLimit)
{
	Cut* A = new S5Cut(upLimit, bRaw);
	myEventCutList->AddCut(A);
}

void BaseHandler::AddRunNumberCut(int lowLimit, int upLimit)
{
	Cut* A = new RunNumberCut(lowLimit,upLimit,bRaw);
	myEventCutList->AddCut(A);
}

void BaseHandler::AddStrangeCut()
{
    StrangeCut* cut = new StrangeCut();
    myTrackCutList->AddCut(cut);
}

void BaseHandler::AddBeamSlopeCut(double minSlope, double maxSlope, eBeamSlopePlane slope)
{
	Cut* A = new BeamSlopeCut(minSlope, maxSlope, slope, bRaw);
	myEventCutList->AddCut(A);
}

void LRCHandler::AddPhiForward(double_t minPhi, double_t maxPhi)
{
	if (minPhi<(-1*TMath::Pi()) || maxPhi>(TMath::Pi())) 
		cout<<nameOutFile<<": warning. phi = [-pi;pi]"<<endl;
	if (minPhi>maxPhi) 
		cout<<nameOutFile<<": Error. minPhi>maxPhi"<<endl;
	Cut* A = new PhiCut(minPhi, maxPhi);
	myForwardTrackCutList->AddCut(A);
}

void LRCHandler::AddPhiBackward(double_t minPhi, double_t maxPhi)
{
	if (minPhi<(-1*TMath::Pi()) || maxPhi>(TMath::Pi())) 
		cout<<nameOutFile<<": warning. phi = [-pi;pi]"<<endl;
	if (minPhi>maxPhi) 
		cout<<nameOutFile<<": Error. minPhi>maxPhi"<<endl;
	Cut* A = new PhiCut(minPhi, maxPhi);
	myBackwardTrackCutList->AddCut(A);
}

void LRCHandler::AddEtaForward(double_t minEta, double_t maxEta)
{
	if (minEta>maxEta) 
		cout<<nameOutFile<<": Error. minEta>maxEta"<<endl;
	Cut* A = new EtaCut(minEta, maxEta);
	myForwardTrackCutList->AddCut(A);
	cout<<"EtaFor: "<<minEta<<" "<<maxEta<<endl;
}

void LRCHandler::AddEtaBackward(double_t minEta, double_t maxEta)
{
	if (minEta>maxEta) 
		cout<<nameOutFile<<": Error. minEta>maxEta"<<endl;
	Cut* A = new EtaCut(minEta, maxEta);
	myBackwardTrackCutList->AddCut(A);
	cout<<"EtaBack: "<<minEta<<" "<<maxEta<<endl;
}

void LRCHandler::AddPtForward(double_t minPt, double_t maxPt)
{
	if (minPt<0)
		cout<<nameOutFile<<": Error. minPt<0"<<endl;
	if (minPt>maxPt) 
		cout<<nameOutFile<<": Error. minPt>maxPt"<<endl;
	Cut* A = new PtCut(minPt, maxPt);
	myForwardTrackCutList->AddCut(A);
}

void LRCHandler::AddPtBackward(double_t minPt, double_t maxPt)
{
	if (minPt<0)
		cout<<nameOutFile<<": Error. minPt<0"<<endl;
	if (minPt>maxPt) 
		cout<<nameOutFile<<": Error. minPt>maxPt"<<endl;
	Cut* A = new PtCut(minPt, maxPt);
	myBackwardTrackCutList->AddCut(A);
} 

void BaseHandler::AddPSDTimeStampCut(unsigned int maxOkSectionsWith0)
{
    Cut* A = new PSDTimeStampCut(bRaw, maxOkSectionsWith0);
    myEventCutList->AddCut(A);

}

void StatEventInfo::Reset()
{
	if (myReset == false){
        myFitVtxX = 0;
        myFitVtxY = 0;
		myFitVtxZ = 0;
		for (int i = 0; i<6; i++) 
			myBPD[i] = 0;
        myBeamSlopeZX = 0;
        myBeamSlopeZY = 0;
		myEnergyPSD = 0;
        myTimeStructureWFA.push_back(100); //Ya pomenyal
		myReset = true;
		myRunNumber = 0;
		myS5ADC = 0;
	}
}

void StatPSDInfo::Reset()
{
	if (myReset == false){
		myEnergyPSD = 0;
		myReset = true;
	}
}

void StatEventInfo::DissectEvent(Event &event)
{   
	if (myReset == true){
		
		evt::rec::Beam beam = event.GetRecEvent().GetBeam();
		for (int i = 0; i<6; i++){
			myBPD[i] = beam.GetBPDPlane((det::BPDConst::EPlaneId)i).GetPosition();
		}
		RecEvent* pRecEvent=&event.GetRecEvent();
		
		//Vertex* pMainVertex=&event.GetRecEvent().GetMainVertex();
		Vertex* pPrimaryVertex = &event.GetRecEvent().GetPrimaryVertex(rec::VertexConst::ePrimaryFitZ);
		//const utl::Point& Vertex = pMainVertex->GetPosition();
		const utl::Point& Vertex = pPrimaryVertex->GetPosition();
		myFitVtxZ = Vertex.GetZ();
        myFitVtxX = Vertex.GetX();
        myFitVtxY = Vertex.GetY();

        
        
        const raw::Trigger& trigger = event.GetRawEvent().GetBeam().GetTrigger(); //Ya pomenyal
        
        myTimeStructureWFA.clear();//Ya pomenyal
        
        
        myTimeStructureWFA = trigger.GetTimeStructure(det::TimeStructureConst::eWFA, det::TriggerConst::eS1_1); //Ya pomenyal
        
        
		
		PSD& psd = pRecEvent->GetPSD();
		for (int i=0; i<nPSDMods; i++)
			myEnergyPSD = myEnergyPSD + psd.GetModule(i+1).GetEnergy();

        myBeamSlopeZX = pRecEvent->GetBeam().Get(det::BPDConst::eX).GetSlope();
        myBeamSlopeZY = pRecEvent->GetBeam().Get(det::BPDConst::eY).GetSlope();

		myRunNumber = event.GetEventHeader().GetRunNumber();

		myS5ADC = event.GetRawEvent().GetBeam().GetTrigger().GetADC(det::TriggerConst::eS5);
		
		myReset = false;
	}
}

void StatPSDInfo::DissectEvent(Event &event)
{
	if (myReset == true){
		RecEvent* pRecEvent = &event.GetRecEvent();
		PSD& psd = pRecEvent->GetPSD();
		for (int i = 0; i<nPSDMods; i++)
			myEnergyPSD = myEnergyPSD + psd.GetModule(i + 1).GetEnergy();
		myReset = false;
	}
}

PtNFluctuationHandler::PtNFluctuationHandler(const char* nameOut) :BaseHandler(nameOut)
{
	cout << "Fluc_Handler_Construction" << endl;
}

PtNFluctuationHandler::PtNFluctuationHandler(const char* nameOut, bool simE) :BaseHandler(nameOut, simE)
{
	cout << "Fluc_Handler_Constructor ..." << endl;
}

PSDFluctuationHandler::PSDFluctuationHandler(const char* nameOut) : BaseHandler(nameOut)
{
	cout << "PSD_Handler_Construction" << endl;
	beamEnergy = 0;
}

PSDFluctuationHandler::PSDFluctuationHandler(const char* nameOut, bool simE) : BaseHandler(nameOut, simE)
{
	cout << "PSD_Handler_Constructor ..." << endl;
	beamEnergy = 0;
}

void PSDFluctuationHandler::Simulation()
{
	cout << "myERROR: no PSD simulation" << endl;
}

void PtNFluctuationHandler::Init()
{
	init = true;
	// --- counters
	nTracks0 = 0;	sumPt0 = 0;	sumPtsqr0 = 0;
	nTracks1 = 0;	sumPt1 = 0;	sumPtsqr1 = 0;
			

	// --- hists
	TString name = myEventCutList->GetName() + myTrackCutList->GetName();

	if (bSim == true) name = name + "_Sim";
	else {
		if (bRaw == false)
			name = name + "_Rec";
		else
			name = name + "_Raw";
	}
	myNameHist = name;

	myAllChargeTHSparse = new THnSparseD("All_charges_" + name, name, nBinsPtNFluc, arNBinsPtNFluc, arXminPtNFluc, arXmaxPtNFluc);
	myPositiveTHSparse = new THnSparseD("Positive_" + name, name,	nBinsPtNFluc, arNBinsPtNFluc, arXminPtNFluc, arXmaxPtNFluc);
	myNegativeTHSparse = new THnSparseD("Negative_" + name, name,	nBinsPtNFluc, arNBinsPtNFluc, arXminPtNFluc, arXmaxPtNFluc);
	myPosPtNegNTHSparse = new THnSparseD("PosPtNegN_" + name, name, nBinsPtNFluc, arNBinsPtNFluc, arXminPtNFluc, arXmaxPtNFluc);
	myNegPtPosNTHSparse = new THnSparseD("NegPtPosN_" + name, name, nBinsPtNFluc, arNBinsPtNFluc, arXminPtNFluc, arXmaxPtNFluc);

}
void PSDFluctuationHandler::Init()
{
	bEnergyIsWriten = false;
	init = true;
	// --- counters
	nTracks0 = 0;	sumPt0 = 0;	
	nTracks1 = 0;	sumPt1 = 0;
	sumEnergyPSD = 0;
	beamEnergy = beamMomentum;


	// --- hists
	TString name = myEventCutList->GetName() + myTrackCutList->GetName();

	if (bSim == true) name = name + "_Sim";
	else {
		if (bRaw == false)
			name = name + "_Rec";
		else
			name = name + "_Raw";
	}
	myNameHist = name;

	myAllChargeTHSparse = new THnSparseD("All_charges_" + name, name, nBinsPSDFluc, arNBinsPSDFluc, arXminPSDFluc, arXmaxPSDFluc);
	myPositiveTHSparse = new THnSparseD("Positive_" + name, name, nBinsPSDFluc, arNBinsPSDFluc, arXminPSDFluc, arXmaxPSDFluc);
	myNegativeTHSparse = new THnSparseD("Negative_" + name, name, nBinsPSDFluc, arNBinsPSDFluc, arXminPSDFluc, arXmaxPSDFluc);

	mySpectEnergyHist = new TH1D("BeamE_minus_PSD_hist_" + name, "Beam minus PSD energy distribution" + name + ";PSD;entries", arNBinsPSDFluc[2], arXminPSDFluc[2], arXmaxPSDFluc[2]);
}

PtNFluctuationHandler::~PtNFluctuationHandler()
{
	cout << "PtNFluctuationHandler destructor are starting ..." << endl;
	//--- output results 
	cout << "name outFile = " << nameOutFile << endl;
	TFile outputFile(nameOutFile, "RECREATE");
	outputFile.cd();
	cout << "writing..." << endl;

	//	TH2D*	NetChHist	= new TH2D("NetCh_correlation_cloud_all"+myNameHist,myNameHist+			"; Qf; Qb; entries",		21,-10.5,10.5,	21,-10.5,10.5);

	TH2D*   ArPt2NHist[nChargeComb];
	TH2D*	ArPtNHist[nChargeComb];

	//ALL__POS__NEG___ForNEG_BackPOS____ForPOS_BackNEG

	ArPt2NHist[0] = new TH2D("Pt2N_correlation_cloud_all" + myNameHist, myNameHist + "; n;pt2;entries",				arNBinsPtNFluc[0], arXminPtNFluc[0], arXmaxPtNFluc[0], arNBinsPtNFluc[2], arXminPtNFluc[2], arXmaxPtNFluc[2]);
	ArPt2NHist[1] = new TH2D("Pt2N_correlation_cloud_neg_neg" + myNameHist, myNameHist + "; n(-);pt2(-);entries",	arNBinsPtNFluc[0], arXminPtNFluc[0], arXmaxPtNFluc[0], arNBinsPtNFluc[2], arXminPtNFluc[2], arXmaxPtNFluc[2]);
	ArPt2NHist[2] = new TH2D("Pt2N_correlation_cloud_pos_pos" + myNameHist, myNameHist + "; n(+);pt2(+);entries",	arNBinsPtNFluc[0], arXminPtNFluc[0], arXmaxPtNFluc[0], arNBinsPtNFluc[2], arXminPtNFluc[2], arXmaxPtNFluc[2]);
	ArPt2NHist[3] = new TH2D("Pt2N_correlation_cloud_neg_pos" + myNameHist, myNameHist + "; n(-);pt2(+);entries",	arNBinsPtNFluc[0], arXminPtNFluc[0], arXmaxPtNFluc[0], arNBinsPtNFluc[2], arXminPtNFluc[2], arXmaxPtNFluc[2]);
	ArPt2NHist[4] = new TH2D("Pt2N_correlation_cloud_pos_neg" + myNameHist, myNameHist + "; n(+);pt2(-);entries",	arNBinsPtNFluc[0], arXminPtNFluc[0], arXmaxPtNFluc[0], arNBinsPtNFluc[2], arXminPtNFluc[2], arXmaxPtNFluc[2]);

	ArPtNHist[0] = new TH2D("PtN_correlation_cloud_all" + myNameHist, myNameHist + "; n;Pt;entries",			arNBinsPtNFluc[0], arXminPtNFluc[0], arXmaxPtNFluc[0], arNBinsPtNFluc[1], arXminPtNFluc[1], arXmaxPtNFluc[1]);
	ArPtNHist[1] = new TH2D("PtN_correlation_cloud_neg_neg" + myNameHist, myNameHist + "; n(-);Pt(-);entries",	arNBinsPtNFluc[0], arXminPtNFluc[0], arXmaxPtNFluc[0], arNBinsPtNFluc[1], arXminPtNFluc[1], arXmaxPtNFluc[1]);
	ArPtNHist[2] = new TH2D("PtN_correlation_cloud_pos_pos" + myNameHist, myNameHist + "; n(+);Pt(+);entries",	arNBinsPtNFluc[0], arXminPtNFluc[0], arXmaxPtNFluc[0], arNBinsPtNFluc[1], arXminPtNFluc[1], arXmaxPtNFluc[1]);
	ArPtNHist[3] = new TH2D("PtN_correlation_cloud_neg_pos" + myNameHist, myNameHist + "; n(-);Pt(+);entries",	arNBinsPtNFluc[0], arXminPtNFluc[0], arXmaxPtNFluc[0], arNBinsPtNFluc[1], arXminPtNFluc[1], arXmaxPtNFluc[1]);
	ArPtNHist[4] = new TH2D("PtN_correlation_cloud_pos_neg" + myNameHist, myNameHist + "; n(+);Pt(-);entries",	arNBinsPtNFluc[0], arXminPtNFluc[0], arXmaxPtNFluc[0], arNBinsPtNFluc[1], arXminPtNFluc[1], arXmaxPtNFluc[1]);

	THnSparseD*	ArSparse[nChargeComb];
	ArSparse[0] = myAllChargeTHSparse;
	ArSparse[1] = myPositiveTHSparse;
	ArSparse[2] = myNegativeTHSparse;
	ArSparse[3] = myPosPtNegNTHSparse;
	ArSparse[4] = myNegPtPosNTHSparse;

	Long64_t nEntries = 0;
	for (Long64_t i = 0; i<nChargeComb; i++){
		nEntries = ArSparse[i]->GetNbins();
		double_t N, Pt, Pt2;
		double_t binContent = 0;
		Int_t pCoord[nBinsPtNFluc] = { 0 };
		double_t arVal[nBinsPtNFluc] = { 0 };
		for (int j = 0; j<nEntries; j++){
			binContent = ArSparse[i]->GetBinContent(j, pCoord);
			for (int k = 0; k<nBinsPtNFluc; k++)
				arVal[k] = arXminPtNFluc[k] + (pCoord[k] - 0.5)*(arXmaxPtNFluc[k] - arXminPtNFluc[k]) / arNBinsPtNFluc[k];
			N = arVal[0];
			Pt = arVal[1];
			Pt2 = arVal[2];

			ArPt2NHist[i]->Fill(N, Pt2, binContent);
			ArPtNHist[i]->Fill(N, Pt, binContent);
		}
	}

	//psdEnergyHist->Write();
	TH1F* eventStat = myEventCutList->GetStatHist(nameOutFile + myNameHist, bSim);
	TH1F* trackStat = myTrackCutList->GetStatHist(nameOutFile + myNameHist, bSim);
	eventStat->Write();
	trackStat->Write();

	for (int i = 0; i<nChargeComb; i++){
		ArSparse[i]->Write();
		ArPtNHist[i]->Write();
		ArPt2NHist[i]->Write();
	}
	cout << "closing..." << endl;
	outputFile.Close();
	cout << "deleting ..." << endl;

	cout << "PtN Fluc Handler destructor has finish" << endl;
}

PSDFluctuationHandler::~PSDFluctuationHandler()
{
	cout << "PSDFluctuationHandler destructor are starting ..." << endl;
	//--- output results 
	cout << "name outFile = " << nameOutFile << endl;
	TFile outputFile(nameOutFile, "RECREATE");
	outputFile.cd();
	cout << "writing..." << endl;

	//	TH2D*	NetChHist	= new TH2D("NetCh_correlation_cloud_all"+myNameHist,myNameHist+			"; Qf; Qb; entries",		21,-10.5,10.5,	21,-10.5,10.5);

	TH2D*   ArPSDNHist[nChargeCombPSD];
	TH2D*	ArPSDPtHist[nChargeCombPSD];

	//ALL__POS__NEG___ForNEG_BackPOS____ForPOS_BackNEG

	ArPSDNHist[0] = new TH2D("EspectN_correlation_cloud_all" + myNameHist, myNameHist + "; n;Espect;entries", arNBinsPSDFluc[0], arXminPSDFluc[0], arXmaxPSDFluc[0],		arNBinsPSDFluc[2], arXminPSDFluc[2], arXmaxPSDFluc[2]);
	ArPSDNHist[1] = new TH2D("EspectN_correlation_cloud_neg" + myNameHist, myNameHist + "; n(-);Espect;entries", arNBinsPSDFluc[0], arXminPSDFluc[0], arXmaxPSDFluc[0], arNBinsPSDFluc[2], arXminPSDFluc[2], arXmaxPSDFluc[2]);
	ArPSDNHist[2] = new TH2D("EspectN_correlation_cloud_pos" + myNameHist, myNameHist + "; n(+);Espect;entries", arNBinsPSDFluc[0], arXminPSDFluc[0], arXmaxPSDFluc[0], arNBinsPSDFluc[2], arXminPSDFluc[2], arXmaxPSDFluc[2]);

	ArPSDPtHist[0] = new TH2D("EspectPt_correlation_cloud_all" + myNameHist, myNameHist + "; Pt;Espect;entries",		arNBinsPSDFluc[1], arXminPSDFluc[1], arXmaxPSDFluc[1],	 arNBinsPSDFluc[2], arXminPSDFluc[2], arXmaxPSDFluc[2]);
	ArPSDPtHist[1] = new TH2D("EspectPt_correlation_cloud_neg" + myNameHist, myNameHist + "; Pt(-);Espect;entries", arNBinsPSDFluc[1], arXminPSDFluc[1], arXmaxPSDFluc[1], arNBinsPSDFluc[2], arXminPSDFluc[2], arXmaxPSDFluc[2]);
	ArPSDPtHist[2] = new TH2D("EspectPt_correlation_cloud_pos" + myNameHist, myNameHist + "; Pt(+);Espect;entries", arNBinsPSDFluc[1], arXminPSDFluc[1], arXmaxPSDFluc[1], arNBinsPSDFluc[2], arXminPSDFluc[2], arXmaxPSDFluc[2]);

	THnSparseD*	ArSparse[nChargeCombPSD];
	ArSparse[0] = myAllChargeTHSparse;
	ArSparse[1] = myPositiveTHSparse;
	ArSparse[2] = myNegativeTHSparse;

	Long64_t nEntries = 0;
	for (Long64_t i = 0; i<nChargeCombPSD; i++){
		nEntries = ArSparse[i]->GetNbins();
		double_t N, Pt, ESpect;
		double_t binContent = 0;
		Int_t pCoord[nBinsPSDFluc] = { 0 };
		double_t arVal[nBinsPSDFluc] = { 0 };
		for (int j = 0; j<nEntries; j++){
			binContent = ArSparse[i]->GetBinContent(j, pCoord);
			for (int k = 0; k<nBinsPSDFluc; k++)
				arVal[k] = arXminPSDFluc[k] + (pCoord[k] - 0.5)*(arXmaxPSDFluc[k] - arXminPSDFluc[k]) / arNBinsPSDFluc[k];
			N = arVal[0];
			Pt = arVal[1];
			ESpect = arVal[2];
			
			//cout << N<<"  "<<Pt<<" "<<ESpect<<"  "<<binContent<<endl;
			ArPSDNHist[i]->Fill(N, ESpect, binContent);
			ArPSDPtHist[i]->Fill(Pt, ESpect, binContent);
		}
	}
	//psdEnergyHist->Write();
	TH1F* eventStat = myEventCutList->GetStatHist(nameOutFile + myNameHist, bSim);
	TH1F* trackStat = myTrackCutList->GetStatHist(nameOutFile + myNameHist, bSim);
	cout << "writing..." << endl;
	eventStat->Write();
	trackStat->Write();
	mySpectEnergyHist->Write();
	for (int i = 0; i<nChargeCombPSD; i++){
		ArSparse[i]->Write();
		ArPSDNHist[i]->Write();
		ArPSDPtHist[i]->Write();
	}
	cout << "closing..." << endl;
	outputFile.Close();
	cout << "deleting ..." << endl;

	cout << "PSD Fluc Handler destructor has finish" << endl;
}

void PtNFluctuationHandler::PutTrack(const evt::rec::VertexTrack& vtxTrack, Event& ev)
{
	if (init == false) this->Init();
	if (myTrackCutList->TrackTest(vtxTrack, ev) == 0) return;

	double_t pX, pY, pT, pT2;
	const Vector vtxMomentum = vtxTrack.GetMomentum();
	pX = vtxMomentum.GetX();
	pY = vtxMomentum.GetY();
	pT2 = pX*pX + pY*pY;
	pT = sqrt(pT2);

	if (vtxTrack.GetCharge() == 1){
		nTracks1++;
		sumPt1 = sumPt1 + pT;
		sumPtsqr1 = sumPtsqr1 + pT2;
	} else {
		nTracks0++;
		sumPt0 = sumPt0 + pT;
		sumPtsqr0 = sumPtsqr0 + pT2;
	}
}

void PtNFluctuationHandler::PutTrack(const evt::sim::VertexTrack& vtxTrack, Event& ev)
{
	if (init == false) this->Init();
	if (myTrackCutList->TrackTest(vtxTrack, ev) == 0) return;
	double_t pX, pY, pT, pT2;
	const Vector vtxMomentum = vtxTrack.GetMomentum();
	pX = vtxMomentum.GetX();
	pY = vtxMomentum.GetY();
	pT2 = pX*pX + pY*pY;
	pT = sqrt(pT2);

	if (vtxTrack.GetCharge() == 1){
		nTracks1++;
		sumPt1 = sumPt1 + pT;
		sumPtsqr1 = sumPtsqr1 + pT2;
	}else {
		nTracks0++;
		sumPt0 = sumPt0 + pT;
		sumPtsqr0 = sumPtsqr0 + pT2;
	}
}

void PSDFluctuationHandler::PutTrack(const VertexTrack& vtxTrack, Event& ev)
{

	if (init == false) this->Init();

	if (myTrackCutList->TrackTest(vtxTrack, ev) == 0) return;

	double_t pX, pY, pT, pT2;
	const Vector vtxMomentum = vtxTrack.GetMomentum();
	pX = vtxMomentum.GetX();
	pY = vtxMomentum.GetY();
	pT2 = pX*pX + pY*pY;
	pT = sqrt(pT2);

	if (vtxTrack.GetCharge() == 1){
		nTracks1++;
		sumPt1 = sumPt1 + pT;
	} else {
		nTracks0++;
		sumPt0 = sumPt0 + pT;
	}
}

void PSDFluctuationHandler::PutPSDEnergy(Event& event)
{
	RecEvent* pRecEvent = &event.GetRecEvent();
	PSD& psd = pRecEvent->GetPSD();
	for (int i = 0; i<nPSDMods; i++)
		sumEnergyPSD += psd.GetModule(i + 1).GetEnergy();
}

void PtNFluctuationHandler::EndOfEvent(Event& ev)
{
	if (init == false) { this->Init(); }

	// --- filling hists

	Double_t arVal[nBinsPtNFluc] = { 0 };
	arVal[nBinsPtNFluc - 1] = myHistInSparseCounter;
	myHistInSparseCounter++;
	if (myHistInSparseCounter == arNBinsPtNFluc[nBinsPtNFluc-1])
		myHistInSparseCounter = 0;

	//All charges  
	arVal[0] = nTracks1 + nTracks0;
	if (arVal[0] != 0){
		arVal[1] = (sumPt0 + sumPt1) / arVal[0];
		arVal[2] = (sumPtsqr0 + sumPtsqr1) / arVal[0];
	}
	else {
		arVal[1] = 0;
		arVal[2] = 0;
	}
	myAllChargeTHSparse->Fill(arVal);

	//Positive charges
	arVal[0] = nTracks1;
	if (arVal[0] != 0){
		arVal[1] = sumPt1 / arVal[0];
		arVal[2] = sumPtsqr1 / arVal[0];
	}
	else {
		arVal[1] = 0;
		arVal[2] = 0;
	}
	myPositiveTHSparse->Fill(arVal);

	//Negative charges
	arVal[0] = nTracks0;
	if (arVal[0] != 0){
		arVal[1] = sumPt0 / arVal[0];
		arVal[2] = sumPtsqr0 / arVal[0];
	}
	else {
		arVal[1] = 0;
		arVal[2] = 0;
	}
	myNegativeTHSparse->Fill(arVal);

	//Positive -> Pt; Negative ->N
	arVal[0] = nTracks0;
	if (nTracks1 != 0){
		arVal[1] = sumPt1;
		arVal[2] = sumPtsqr1;
	}
	else {
		arVal[1] = 0;
		arVal[2] = 0;
	}
	myPosPtNegNTHSparse->Fill(arVal);

	//Positive -> N; Negative ->Pt
	arVal[0] = nTracks1;
	if (nTracks0 != 0){
		arVal[1] = sumPt0;
		arVal[2] = sumPtsqr0;
	}
	else {
		arVal[1] = 0;
		arVal[2] = 0;
	}

	myNegPtPosNTHSparse->Fill(arVal);


	// --- counters
	nTracks0 = 0;		
	nTracks1 = 0;		

	sumPt0 = 0;		
	sumPt1 = 0;
	sumPtsqr0 = 0;
	sumPtsqr1 = 0;
}

void PSDFluctuationHandler::EndOfEvent(Event& ev)
{

	if (init == false) { this->Init(); }
	this->PutPSDEnergy(ev);
	// --- filling hists

	Double_t arVal[nBinsPSDFluc] = { 0 };
	arVal[nBinsPtNFluc - 1] = myHistInSparseCounter;
	myHistInSparseCounter++;
	if (myHistInSparseCounter == arNBinsPSDFluc[nBinsPSDFluc - 1])
		myHistInSparseCounter = 0;

	switch (systemType)
	{
	case BeBe:
		arVal[2] = 7 * dBeamMomentum - sumEnergyPSD;
		break;
	case pp:
		cout << "myERROR: NO PSD DURING P+P RUNS" << endl;
		return;
		break;
	case ArSc:
		arVal[2] = 40 * dBeamMomentum - sumEnergyPSD;
		break;
	default:
		cout << "myERROR: PSD Fluc Handler is determined for this system type" << endl;
		return;
		break;
	}

	//All charges  
	arVal[0] = nTracks1 + nTracks0;
	if (arVal[0] != 0){
		arVal[1] = (sumPt0 + sumPt1) / arVal[0];
	}
	else {
		arVal[1] = 0;
	}
	myAllChargeTHSparse->Fill(arVal);

	//Positive charges
	arVal[0] = nTracks1;
	if (arVal[0] != 0){
		arVal[1] = sumPt1 / arVal[0];
	}
	else {
		arVal[1] = 0;
	}
	myPositiveTHSparse->Fill(arVal);

	//Negative charges
	arVal[0] = nTracks0;
	if (arVal[0] != 0){
		arVal[1] = sumPt0 / arVal[0];
	}
	else {
		arVal[1] = 0;
	}
	myNegativeTHSparse->Fill(arVal);
	mySpectEnergyHist->Fill(arVal[2]);

	// --- counters
	nTracks0 = 0;
	nTracks1 = 0;

	sumPt0 = 0;
	sumPt1 = 0;
	sumEnergyPSD = 0;
}

/*AcceptanceHandler::AcceptanceHandler(const char* nameOutFile) :BaseHandler(nameOutFile,true)//always SIM 
{}

AcceptanceHandler::~AcceptanceHandler()
{
	cout << "name outFile = " << nameOutFile << endl;
	TFile outputFile(nameOutFile, "RECREATE");
	outputFile.cd();

	TH1F* trackRecStat = this->GetRecHandlerForCuts->GetTrackCutList->GetStatHist(nameOutFile + myNameHist, bSim);

	trackRecStat->Write();

	myRecHist->Write();
	mySimHist->Write();
}

void AcceptanceHandler::Init()
{
	cout << "Init" << endl;
	init = true;

	TString nameSim = myTrackCutList->GetName();
	TString nameRec = myRecHandlerForCuts->GetTrackCutList->GetName();
	myNameHist = nameSim;

	mySimHist = new TH3D("acceptance_" + nameSim +"Sim", "acceptance_" + nameSim + "Sim; phi;Pt;eta;entries", 301, -3.5, 3.5, 31, 0, 1.5, 601, 0, 10);
	myRecHist = new TH3D("acceptance_" + nameRec + "Rec", "acceptance_" + nameRec + "Rec; phi;Pt;eta;entries", 301, -3.5, 3.5, 31, 0, 1.5, 601, 0, 10);
}

void AcceptanceHandler::PutTrack(const evt::sim::VertexTrack& vtxTrackSim, Event& ev)
{
	if (init == false) this->Init();
	if (myTrackCutList->TrackTest(vtxTrackSim, ev) == 0) return;

	double_t p, pX, pY, pZ, pT, phi, eta, dEdx;
	Vector	vtxMomentum = vtxTrackSim.GetMomentum();
	pX = vtxMomentum.GetX();
	pY = vtxMomentum.GetY();
	pZ = vtxMomentum.GetZ();
	p = vtxMomentum.GetMag();
	phi = TMath::ATan2(pY, pX); //phi = [-pi;pi] 
	
	pT = sqrt(pX*pX + pY*pY);
	eta = -0.5*TMath::Log((p-pZ)/(p+pZ));

	evt::Index<rec::Track> recTrack = vtxTrackSim->GetRecTrackWithMaxCommonPoints();//FIXME
}*/

StatEventInfo TimeHandler::myEventInfo;
bool TimeHandler::bEventInfoFull = false;

TimeHandler::TimeHandler(const char* nameOut) :BaseHandler(nameOut)
{}

TimeHandler::TimeHandler(const char* nameOut, bool simE) : BaseHandler(nameOut, simE)
{}


TimeHandler::~TimeHandler()
{
	//	cout<<"Handler destructor is starting ... "<<endl;
	//--- output results
	cout << "name outFile = " << nameOutFile << endl;
	TFile outputFile(nameOutFile, "RECREATE");
	outputFile.cd();

	//	cout<<"writing..."<<endl;

	TH1F* eventStat = myEventCutList->GetStatHist(nameOutFile + myNameHist, bSim);

	TH1F* trackStat = myTrackCutList->GetStatHist(nameOutFile + myNameHist, bSim);

	eventStat->Write();
	trackStat->Write();

	chargeHist->Write();
	multHist->Write();
	psdEnergyHist->Write();

	pHist->Write();
	ptHist->Write();
	phiHist->Write();

	fitVtxHistX->Write();
	fitVtxHistY->Write();
	fitVtxHistZ->Write();

  //  fitVtxHistXverMult->Write();
   // fitVtxHistYverMult->Write();

    cout << nameOutFile << endl;
	cout << "nEvent:  " << multHist->GetEntries() << endl;
	cout << "nTracks: " << phiHist->GetEntries() << endl << endl;

	outputFile.Close();
}

void TimeHandler::Init() {
	cout << "init" << endl;
	double minBound, maxBound, nBinsRunNumber;
	switch (systemType) {
		case ArSc:
			if (beamMomentum == 13) {
				minBound = minRunNumberArSc13 - 0.5;
				maxBound = maxRunNumberArSc13 + 0.5;
				nBinsRunNumber = maxBound - minBound;
				break;
			}
			if (beamMomentum == 19) {
				minBound = minRunNumberArSc19 - 0.5;
				maxBound = maxRunNumberArSc19 + 0.5;
				nBinsRunNumber = maxBound - minBound;
				break;
			}
			if (beamMomentum == 30) {
				minBound = minRunNumberArSc30 - 0.5;
				maxBound = maxRunNumberArSc30 + 0.5;
				nBinsRunNumber = maxBound - minBound;
				break;
			}
			if (beamMomentum == 150) {
				minBound = minRunNumberArSc150 - 0.5;
				maxBound = maxRunNumberArSc150 + 0.5;
				nBinsRunNumber = maxBound - minBound;
				break;
			}
			if (beamMomentum == 40) {
				minBound = minRunNumberArSc40 - 0.5;
				maxBound = maxRunNumberArSc40 + 0.5;
				nBinsRunNumber = maxBound - minBound;
				break;
			}

		default:
			minBound = -0.5;
			maxBound = 21000.5;
			nBinsRunNumber = maxBound - minBound;
			break;
	}
	init = true;
	nTracks1 = 0;
	nTracks0 = 0;
	TString string1;
	if (bSim == true) string1 = "Sim";
	else string1 = "Rec";
	if (bRaw == true) string1 = "Raw";
	TString name = "Time_" + myEventCutList->GetName() + myTrackCutList->GetName() + string1;
	myNameHist = name;

	psdEnergyHist = new TH2D("PSDHist_" + name, "PSD energy distribution" + name + ";runNumber;PSD(28);entries",
							 nBinsRunNumber, minBound, maxBound, 500, 0, 8500);

	chargeHist = new TH2D("chargeHist_" + name, "chargeHist_" + name + ";runNumber;charge;entries", nBinsRunNumber,
						  minBound, maxBound, 5, -2.5, 2.5);
	multHist = new TH2D("multHist_" + name, "multHist_" + name + ";runNumber;nTracks;entries", nBinsRunNumber, minBound,
						maxBound, 301, -0.5, 300.5);


	fitVtxHistZ = new TH2D("Fitted_Vtx_Position_Z_" + name, "fitVtxZ_" + name + ";runNumber;z [cm]; entries",
						   nBinsRunNumber, minBound, maxBound, 7001, -700, 0);
    fitVtxHistX = new TH2D("Fitted_Vtx_Position_X_" + name, "fitVtxX_" + name + ";runNumber;x [cm]; entries",
                           nBinsRunNumber, minBound, maxBound, 1001, -10, 10);
    fitVtxHistY = new TH2D("Fitted_Vtx_Position_Y_" + name, "fitVtxY_" + name + ";runNumber;y [cm]; entries",
                           nBinsRunNumber, minBound, maxBound, 1001, -10, 10);

	pHist = new TH2D("pHist_" + name, "pHist_" + name + ";runNumber;p;entries", nBinsRunNumber, minBound, maxBound, 301,
					 -1, 150);

	ptHist = new TH2D("ptHist_" + name, "ptHist_" + name + ";runNumber;pt;entries ", nBinsRunNumber, minBound, maxBound,
					  301, 0, 2);

	phiHist = new TH2D("phiHist_" + name, "phiHist_" + name + ";runNumber;phi;entries", nBinsRunNumber, minBound,
					   maxBound, 101, -3.5, 3.5);
/*
	fitVtxHistXverMult = new TH2D("Fitted_Vtx_Position_X_versus_mult_" + name, "fitVtxXverMult_" + name + ";runNumber;x/mult [cm]; entries",
						   nBinsRunNumber, minBound, maxBound, 1001, -1, 1);
	fitVtxHistYverMult = new TH2D("Fitted_Vtx_Position_Y_versus_mult_" + name, "fitVtxYverMult_" + name + ";runNumber;y/mult [cm]; entries",
						   nBinsRunNumber, minBound, maxBound, 1001, -1, 1);*/
}



void TimeHandler::PutTrack(const evt::rec::VertexTrack& vtxTrack, Event& ev)
{
	if (init == false) this->Init();
	if (bRaw){
		if (bEventInfoFull == false){
			myEventInfo.Reset();
			myEventInfo.DissectEvent(ev);
			bEventInfoFull = true;
		}
	}

	if (myTrackCutList->TrackTest(vtxTrack, ev) == 0) return;
	//cout << "put Track" << endl;
	double_t p, pX, pY, pZ, pT, phi, eta, dEdx;
	Vector	vtxMomentum = vtxTrack.GetMomentum();
	pX = vtxMomentum.GetX();
	pY = vtxMomentum.GetY();
	pZ = vtxMomentum.GetZ();
	p = vtxMomentum.GetMag();

	phi = TMath::ATan2(pY, pX); //phi = [-pi;pi] 
	double mass = 0.1396;
	double E = sqrt(pow(mass, 2) + p*p);

	pT = sqrt(pX*pX + pY*pY);
	eta = -0.5*TMath::Log((p-pZ)/(p+pZ));

	dEdx = 0;

	chargeHist->Fill(myEventInfo.myRunNumber, vtxTrack.GetCharge());

	pHist->Fill(myEventInfo.myRunNumber,p);
	ptHist->Fill(myEventInfo.myRunNumber, pT);
	phiHist->Fill(myEventInfo.myRunNumber, phi);


	if (vtxTrack.GetCharge() == 1)
	{
		nTracks1++;
	}
	else
	{
		nTracks0++;
	}
}

void TimeHandler::PutTrack(const evt::sim::VertexTrack& vtxTrack, Event& ev)
{
	cout << "NO, SIM, NO" << endl;
	cout << "No time for you" << endl;
}

void TimeHandler::EndOfEvent(Event& ev)
{
	if (init == false) { this->Init(); }
	multHist->Fill(myEventInfo.myRunNumber, nTracks0 + nTracks1);

	bEventInfoFull = false;
	fitVtxHistZ->Fill(myEventInfo.myRunNumber,myEventInfo.myFitVtxZ);
    fitVtxHistX->Fill(myEventInfo.myRunNumber,myEventInfo.myFitVtxX);
    fitVtxHistY->Fill(myEventInfo.myRunNumber,myEventInfo.myFitVtxY);

//    fitVtxHistXverMult->Fill(myEventInfo.myRunNumber, myEventInfo.myFitVtxX / (nTracks0 + nTracks1));
 //   fitVtxHistYverMult->Fill(myEventInfo.myRunNumber, myEventInfo.myFitVtxY / (nTracks0 + nTracks1));

	psdEnergyHist->Fill(myEventInfo.myRunNumber, myEventInfo.myEnergyPSD);

	nTracks0 = 0;
	nTracks1 = 0;
}

PSDHandler::PSDHandler(const char *nameOut):BaseHandler(nameOut) { }
PSDHandler::PSDHandler(const char *nameOut, bool sim) : BaseHandler(nameOut,sim){ }
PSDHandler::~PSDHandler()
{
    cout << "name outFile = " << nameOutFile << endl;
    TFile outputFile(nameOutFile, "RECREATE");
    outputFile.cd();

    TH1F* eventStat = myEventCutList->GetStatHist(nameOutFile + myNameHist, bSim);
    TH1F* trackStat = myTrackCutList->GetStatHist(nameOutFile + myNameHist, bSim);

    myModuleSparse->Write();
    eventStat->Write();
    trackStat->Write();

    cout << nameOutFile << endl;
    double tmp = nPSDModules+1;
    cout << "nEvent:  " << myModuleSparse->GetEntries()/tmp << endl;
    outputFile.Close();
}

void PSDHandler::Init()
{
    cout << "initialisation of the PSD module handler" << endl;
    init = true;
    TString string1;
    if (bSim == true) string1 = "Sim";
    else string1 = "Rec";
    if (bRaw == true) string1 = "Raw";
    TString name = "PSDModules_" + myEventCutList->GetName()+ myTrackCutList->GetName() + string1;
    myNameHist = name;

    const double* arXMax;
	switch (systemType){
		case ArSc:
			arXMax = arXmaxPSDModulesArSc150;
			break;
		case BeBe:
			arXMax = arXmaxPSDModulesBeBe150;
			break;
		default:
			arXMax = arXmaxPSDModulesArSc150;
			break;
	}

    myModuleSparse = new THnSparseD(name, name, nBinsPSDModules, arNBinsPSDModules, arXminPSDModules, arXMax);
}

void PSDHandler::PutTrack(const evt::rec::VertexTrack& vtxTrack, Event& ev)
{
    if (init == false) this->Init();
    if (myTrackCutList->TrackTest(vtxTrack, ev) == 0) return;

    if (vtxTrack.GetCharge() == 1)
        nTracks1++;
    else
        nTracks0++;
}

void PSDHandler::PutTrack(const evt::sim::VertexTrack& vtxTrack, Event& ev)
{
    if (init == false) this->Init();
    if (myTrackCutList->TrackTest(vtxTrack, ev) == 0) return;

    if (vtxTrack.GetCharge() == 1)
        nTracks1++;
    else
        nTracks0++;
}

void PSDHandler::EndOfEvent(Event & ev) {
    if (init == false) { this->Init(); }
    double arVal[nBinsPSDModules] = {0};
    arVal[0] = nTracks0 + nTracks1;
    RecEvent *pRecEvent = &ev.GetRecEvent();
    PSD &psd = pRecEvent->GetPSD();
    for (int i = 1; i < nBinsPSDModules; i++)
        arVal[i] = psd.GetModule(i).GetEnergy();


    myModuleSparse->Fill(arVal);
    nTracks0 = 0;
    nTracks1 = 0;
}