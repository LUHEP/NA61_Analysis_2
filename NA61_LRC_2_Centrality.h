#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>

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

#ifndef CONST
#define CONST
#include "NA61_LRC_2_Const.h"
#endif


#include "TGraphErrors.h"
#include "TRandom3.h"
#include "TF2.h"




class LegacyCentrality
{
  public:
    LegacyCentrality() : 
		fDataPath(str_configPath+"/LegacyCentrality/"),// <- "/afs/cern.ch/user/e/ekaptur/h-analysis/include/LegacyCentrality/"
		fCentralityGraph8s0l(0), fDataSet(0) {}
    ~LegacyCentrality(){std::remove("centrality.root");}
    
    unsigned int GetCentralityClass(const evt::Event &event, std::vector<int> *centralityClass = 0);
    
    void SetCentralityEdges(const std::vector<double> &centralityEdges){
      fCentralityEdges = centralityEdges;
      std::sort(fCentralityEdges.begin(), fCentralityEdges.end());
    }
    
    template <int N>
    void SetCentralityEdges(double (&centralityEdges) [N]){
      fCentralityEdges.assign(centralityEdges, centralityEdges + N);
      std::sort(fCentralityEdges.begin(), fCentralityEdges.end());
    }
    
    void SetDataPath(const char* path){fDataPath = path;}
    
    void GetSpectators(const evt::Event& event, std::vector< evt::Index<evt::sim::VertexTrack> >& indexList);
    
    void PSDSimulation(const evt::Event& event, double (&moduleEnergy)[45], std::vector< evt::Index<evt::sim::VertexTrack> > &specList);
    
    //TH2D* neg;
    //TH2D* neu;
    //TH2D* pos;
    //TH1D* parEne;
    
  private:
    std::vector<double> fCentralityEdges;
    void Init(const evt::Event &event);
    std::string fDataPath;
    TGraphErrors *fCentralityGraph8s0l;
    TGraphErrors *fCentralityGraph8s2l;
    TGraphErrors *fCentralityGraph8s4l;
    TGraphErrors *fCentralityGraph16s12l;
    int fDataSet;
    TRandom3 fRandGen;

	static int nObjects;
};