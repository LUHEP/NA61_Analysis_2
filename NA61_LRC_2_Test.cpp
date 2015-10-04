
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
    offBranches.push_back("eventTree.fEvent.fRecEvent.*");
    offBranches.push_back("eventTree.fEvent.fRawEvent.*");
    std::vector<std::string> onBranches;
    onBranches.push_back("eventTree.fEvent.fRawEvent.fBeam");
    onBranches.push_back("eventTree.fEvent.fRecEvent.fBeam");
    onBranches.push_back("eventTree.fEvent.fRecEvent.fPSD");
    onBranches.push_back("eventTree.fEvent.fRecEvent.fTracks");
    onBranches.push_back("eventTree.fEvent.fRecEvent.fVertexTracks");
    onBranches.push_back("eventTree.fEvent.fRecEvent.fVertices");
    onBranches.push_back("eventTree.fEvent.fRecEvent.fTrackList");
    onBranches.push_back("eventTree.fEvent.fRecEvent.fVertexTrackList");
    onBranches.push_back("eventTree.fEvent.fRecEvent.fVertexList");
    onBranches.push_back("eventTree.fEvent.fRecEvent.fMainVertexIndex");
    onBranches.push_back("eventTree.fEvent.fRecEvent.fPrimaryVertexMap");
    

    
    int i=0;
	time_t     now;
	struct tm  *ts;
	char       buf[80];

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
			// --- working status output
			if(i%1000==0){
				// Get the current time
				now = time(NULL);          
				// Format and print the time, "ddd yyyy-mm-dd hh:mm:ss zzz" 
				ts = localtime(&now);
				strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", ts);
				printf("running event %5d time: %s\n",(int) i,buf);
			}  
        }
    }      
    
	return 0;
}
