#ifndef CUTLIST
#define CUTLIST
#include "NA61_LRC_2_Cut_List.h"
#endif

CutList::CutList():
myNameOutFile("CutStat.root")
{
    myNext=NULL;
    myValue=NULL;
    myNTest=0;
//    myNameOutFile = "NoName.root";
}

CutList::CutList(TString name):
myNameOutFile(name)
{
    myNext=NULL;
    myValue=NULL;
    myNTest=0;
//    myNameOutFile = name;
}

CutList::CutList(Cut* cut):
myNameOutFile("NoName.root")
{
    myNext=NULL;
    myValue=cut;
    myNTest=0;
  //  myNameOutFile = "NoName.root";
}

CutList::~CutList()
{
    CutList *current = this;
    Cut *currentCut = 0;
    
    TH1D StatHist("StatHist",";cuts    ;entries",20, -0.5, 10.5);
    
    StatHist.GetXaxis()->SetBinLabel(1,"Total");
    int nBins = 1; 
    StatHist.SetBinContent(1,myNTest);
    
    do
    {
        currentCut = current -> GetValue(0);
        current = current -> myNext;
        nBins++;
        StatHist.SetBinContent(nBins, currentCut->GetNEntries());
        delete currentCut;
    } while(current -> myNext !=0);
    
    TFile outputFile(myNameOutFile, "RECREATE");
    outputFile.cd();
    StatHist.Write();
}

void CutList::AddCut(Cut *cut)
{
    CutList *current = this;
    if (current ->myValue == 0)             myValue=cut;
    else
    {
        while(current -> myNext != 0)
            current = current -> myNext;
        current -> myNext = new CutList(cut);
    }
}

Cut* CutList::GetValue(int index)
{
    int length = this->GetLength();
    if (length<=index) return 0;
    CutList *current = this;
    for (int i=0; i<index; i++)
        current = current->myNext;
    return current->myValue;
}

int CutList::GetLength()
{
    if (myValue == 0) return 0;
    CutList *current = this;
    int length = 1; 
    while (current -> myNext !=0){
        current=current -> myNext;
        length++;
    }
    return length;
}

TString CutList::GetName()
{
	if (myValue == 0) return "no_name";
	CutList *current = this;
	TString name ="CL";
	while (current->myNext != 0){
		name = name + '_' + current->GetValue(0)->GetShortNameWithPar();
		current=current -> myNext;
	}
	name = name + '_' + current->GetValue(0)->GetShortNameWithPar();
	return name;
}

bool CutList::TrackTest(const evt::rec::VertexTrack& vtxTrack, Event& ev)
{
//	cout << "track_test" << endl;
	myNTest++;
	if (myValue == 0) return true;
	CutList *current = this;
	bool res = 1;
	Cut* pCut;
	checking currentCutStatus;
	RecEvent& recEvent = ev.GetRecEvent();
	while (true){
		pCut = current->GetValue(0);
		currentCutStatus = trackCutChecking.CheckCut(pCut->GetShortNameWithPar(),REC);
		if (currentCutStatus == fail)
			return false;
		if (currentCutStatus == pass){
			pCut->nEntriesPlusPlus();

		}else{
			res = pCut->CheckTrack(recEvent, vtxTrack);
			if (res == true) 
				trackCutChecking.CheckedCut(pCut->GetShortNameWithPar(),REC, pass);
			if (res == false) {
				trackCutChecking.CheckedCut(pCut->GetShortNameWithPar(),REC, fail);
				return false;
			}
		}
		if (current->myNext == 0) break;
		current=current -> myNext;
	}

	//	if (res == true) cin>>res;
	if (res == false)
		cout<<"CutList::TrackTest rec: Error. not all cuts are used"<<endl; 
	return res;
}

bool CutList::TrackTest(const evt::sim::VertexTrack& vtxTrack, Event& ev)
{
	myNTest++;
	if (myValue == 0) return true;
	CutList *current = this;
	bool res = 1;
	Cut* pCut;
	checking currentCutStatus;
	SimEvent& simEvent = ev.GetSimEvent();
	while (true){
		pCut = current->GetValue(0);
		currentCutStatus = trackCutChecking.CheckCut(pCut->GetShortNameWithPar(),SIM);
		if (currentCutStatus == fail)
			return false;
		if (currentCutStatus == pass){
			pCut->nEntriesPlusPlus();

		}else{
			res = pCut->CheckTrack(simEvent, vtxTrack);
			if (res == true) 
				trackCutChecking.CheckedCut(pCut->GetShortNameWithPar(),SIM, pass);
			if (res == false) {
				trackCutChecking.CheckedCut(pCut->GetShortNameWithPar(),SIM, fail);
				return false;
			}
		}
		if (current->myNext == 0) break;
		current=current -> myNext;
	}

	//	if (res == true) cin>>res;
	if (res == false)
		cout<<"CutList::TrackTest sim: Error. not all cuts are used"<<endl; 
	return res;
}


bool CutList::EventTest(Event& ev, bool bSim)
{
//	cout<<"event test"<<endl;
	myNTest++;
	if (myValue == 0) return true;
	CutList *current = this;
	bool res = 1;
	Cut* pCut;
	dataType type;
	if (bSim) type = SIM;
	else type = REC;
	checking currentCutStatus;
	while(true){
		pCut = current->GetValue(0);
		currentCutStatus = eventCutChecking.CheckCut(pCut->GetShortNameWithPar(),type);
		if (currentCutStatus == fail)
			return false;
		if (currentCutStatus == pass){
			pCut->nEntriesPlusPlus();
		}else{
			if (pCut->DoYouNeedMultiplicity()){
//				cout << "YOUNEEDIT" << endl;
				unsigned int multiplicity = 0;
				RecEvent& recEvent = ev.GetRecEvent();
				Vertex& mainVertex = recEvent.GetMainVertex();
				for (VertexTrackIndexIterator vtxTrackIter = mainVertex.DaughterTracksBegin();
					vtxTrackIter != mainVertex.DaughterTracksEnd(); ++vtxTrackIter){
					CutList::trackCutChecking.Reset();
					VertexTrack& vtxTrack = recEvent.Get(*vtxTrackIter);

					if (myFriendCutList->TrackTest(vtxTrack, ev) == false) continue;
					multiplicity++;
				}
				pCut->SetMultiplicity(multiplicity);
//				cout << "set_mult" << endl;
			}

			res = pCut->CheckEvent(ev, bSim);
			if (res == true) 
				eventCutChecking.CheckedCut(pCut->GetShortNameWithPar(),type, pass);
			if (res == false) {
				eventCutChecking.CheckedCut(pCut->GetShortNameWithPar(),type, fail);
				return false;
			}		
		}
		if (current->myNext == 0) break;
		current=current -> myNext;
	}

	if (res == false)
		cout<<"CutList::EventTest: Error. not all cuts are used"<<endl; 
	return res;
}

TH1F* CutList::GetStatHist(TString topName, bool bSim)
{
	int N = this->GetLength();
	TH1F* histStat;
	if (N>0){ 
		CutList *current = this;
    		Cut *currentCut = 0;
		currentCut = current -> GetValue(0);
		TString name = currentCut->GetName();
		
		if (bSim)
			histStat = new TH1F("SimStatCutHist"+name+topName , "Sim"+name+topName + ";cuts;entries", N+1, -0.5, N+0.5);
		else 
			histStat = new TH1F("StatCutHist"+name+topName , name+topName + ";cuts;entries", N+1, -0.5, N+0.5);

		histStat->SetBinContent(1,myNTest);
	    histStat->GetXaxis()->SetBinLabel(1,"before");
		int iBin = 1;

		do {
			iBin++;
	//		if (iBin>N+1) break;
	 	       if (iBin!=2) 
				current = current -> myNext;
			currentCut = current -> GetValue(0);
		//	cout<<currentCut->GetName()<<endl;
			histStat->SetBinContent(iBin, currentCut->GetNEntries());
			histStat->GetXaxis()->SetBinLabel(iBin,currentCut->GetName());
		} while(current -> myNext !=0);
	}else{
		histStat = new TH1F("SimStatCutHist"+topName , "Sim"+topName + ";cuts;entries", 10, -0.5, 10+0.5);
		histStat->SetBinContent(1,myNTest);
	}
	return histStat;
}

CutChecking::CutChecking()
{
	lastSim = 0;
	lastRec = 0;
	myLength = 0;
}

void CutChecking::Reset()
{
	for (int i=0; i<myLength; i++)
		vtrStatus[i]=unknown;
	return;
}

void CutChecking::AddCut(TString nameCut, dataType type)
{
	for (int i=0; i<myLength; i++)
		if ((vtrNames[i]==nameCut) && (vtrTypes[i] == type))
			return; 
//	cout<<"pushing ...... ";
	vtrNames.push_back(nameCut);
	vtrTypes.push_back(type);
	vtrStatus.push_back(unknown);
	myLength++;
//	cout<<"oK"<<myLength<<endl;
	return;
}

checking CutChecking::CheckCut(TString nameCut, dataType type)
{
	if (myLength!=0){
		if ((type==REC) || (type==RAW))
			if (vtrNames[lastRec] == nameCut)
				return vtrStatus[lastRec];
		if (type==SIM)
			if (vtrNames[lastSim] == nameCut)
				return vtrStatus[lastSim];
	
		for (int i=0; i<myLength; i++){
			if ((vtrNames[i]==nameCut) && (vtrTypes[i] == type)){
				if ((type==REC) || (type==RAW))
					lastRec = i;
				else lastSim = i;
				return vtrStatus[i];
			}
		}
	}

//	cout<<"CutChecking::CheckCut: Error: there is now cut: "<<nameCut<<" for "<<type<<" in the list!!! return unknown"<<endl;
	return unknown;
}

void CutChecking::CheckedCut(TString nameCut, dataType type, checking status)
{
//	cout<<"CheckedCut"<<endl;
	if (myLength != 0){
		if ((type==REC) || (type==RAW))
			if (vtrNames[lastRec] == nameCut){
				vtrStatus[lastRec] = status;
				return;
			}
		if (type==SIM)
			if (vtrNames[lastSim] == nameCut){
				vtrStatus[lastSim] = status;
				return;
			}
//		cout<<"Warning! problem CutChecking::CheckedCut with lastRec or lastSim determination"<<endl;
	}
	for (int i=0; i<myLength; i++){
		if ((vtrNames[i]==nameCut) && (vtrTypes[i] == type)){
			vtrStatus[i] = status;
			return;
		}
	}
//	cout<<"CutChecking::CheckedCut: Error: there is no cut: "<<nameCut<<" for "<<type<<" in the list!!! AddCut"<<endl;

	this->AddCut(nameCut, type);
//	cout<<"Add is done"<<endl;
	if ((vtrNames[myLength-1]==nameCut) && (vtrTypes[myLength-1] == type)){
			vtrStatus[myLength-1] = status;
			return;
	}
//	cout<<"CutChecking::CheckedCut: Error: AddCut didn't help!! please break the program and find the answer by hands"<<endl;
	return;

}

CutChecking CutList::eventCutChecking = CutChecking();
CutChecking CutList::trackCutChecking = CutChecking();
