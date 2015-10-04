#ifndef CENTRALITY
#define CENTRALITY
#include "NA61_LRC_2_Centrality.h"
#endif

/*#ifndef CONST
#define CONST
#include "NA61_LRC_2_Const.h"
#endif*/

int LegacyCentrality::nObjects=0;

unsigned int LegacyCentrality::GetCentralityClass(const evt::Event &event, std::vector<int> *centralityClass){
  bool isMC = event.GetSimEvent().HasMainVertex();
  if (fCentralityEdges.empty()){
    double defaultCentrality[4] = {0.05, 0.1, 0.15, 0.2};
    fCentralityEdges.assign(defaultCentrality, defaultCentrality + 4);
  }
  
  if (!fCentralityGraph8s0l) Init(event);
  
  //                        0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22
  double weights8s0l[45] = {0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  //                         23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   43   44
                            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  
  //                        0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22
  double weights8s2l[45] = {0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
  //                         23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   43   44
                            0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  
  //                        0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22
  double weights8s4l[45] = {0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0,
  //                         23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   43   44
                            0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  
  //                          0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22
  double weights16s12l[45] = {0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
  //                         23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   43   44
                            1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  
  double moduleEnergy[45];
  double psdEnergy[4] = {0, 0, 0, 0};
  if (!isMC){
    for (int i = 0; i < 45; ++i){
      moduleEnergy[i] = 0;
      if (event.GetRecEvent().GetPSD().HasModule(i)){
        evt::rec::PSDModule module = event.GetRecEvent().GetPSD().GetModule(i);
        moduleEnergy[i] = module.GetEnergy();
      }
    }
  } else {
    std::vector< evt::Index<evt::sim::VertexTrack> > spectatorList;
    GetSpectators(event, spectatorList);
    PSDSimulation(event, moduleEnergy, spectatorList);
  }
  
  for (int i = 0; i < 45; ++i){
    psdEnergy[0] += moduleEnergy[i] * weights8s0l[i];
    psdEnergy[1] += moduleEnergy[i] * weights8s2l[i];
    psdEnergy[2] += moduleEnergy[i] * weights8s4l[i];
    psdEnergy[3] += moduleEnergy[i] * weights16s12l[i];
  }
  
  double cent8s0l   = fCentralityGraph8s0l->Eval(psdEnergy[0]);
  double cent8s2l   = fCentralityGraph8s2l->Eval(psdEnergy[1]);
  double cent8s4l   = fCentralityGraph8s4l->Eval(psdEnergy[2]);
  double cent16s12l = fCentralityGraph16s12l->Eval(psdEnergy[3]);
  
  unsigned int centralityClass8s0l = 1;
  unsigned int centralityClass8s2l = 1;
  unsigned int centralityClass8s4l = 1;
  unsigned int centralityClass16s12l = 1;
  
  bool done8s0l = false;
  bool done8s2l = false;
  bool done8s4l = false;
  bool done16s12l = false;
  
  for(std::vector<double>::iterator it  = fCentralityEdges.begin();
                                    it != fCentralityEdges.end();
                                  ++it){
    if (cent8s0l < *it) done8s0l = true;
    if (cent8s2l < *it) done8s2l = true;
    if (cent8s4l < *it) done8s4l = true;
    if (cent16s12l < *it) done16s12l = true;
    if (!done8s0l) centralityClass8s0l++;
    if (!done8s2l) centralityClass8s2l++;
    if (!done8s4l) centralityClass8s4l++;
    if (!done16s12l) centralityClass16s12l++;
  }
  
  if (centralityClass){
    centralityClass->push_back(centralityClass8s0l);
    centralityClass->push_back(centralityClass8s2l);
    centralityClass->push_back(centralityClass8s4l);
    centralityClass->push_back(centralityClass16s12l);
  }
  
  return centralityClass16s12l;
}

void LegacyCentrality::Init(const evt::Event &event){
  bool isMC = event.GetSimEvent().HasMainVertex();
  fRandGen.SetSeed(0);
  unsigned int runNumber = event.GetEventHeader().GetRunNumber();
  
  if (15251 <= runNumber && runNumber <= 15341){
    fDataSet = 40;
  } else if (15097 <= runNumber && runNumber <= 15232) {
    fDataSet = 75;
  } else if ((14961 <= runNumber && runNumber <= 15089) || (15349 <= runNumber && runNumber <= 15411)) {
    fDataSet = 150;
  } else {
    throw utl::ShineException("Centrality not available for given run number.");
  }
  
  det::Detector::GetInstance().Update(event.GetEventHeader().GetTime(), runNumber);

  std::ostringstream s;
  s << "LegacyCentrality_" << ((isMC) ? "MC_" : "Data_") << fDataSet << ".root";
  cout<<"LegacyCentrality_" << ((isMC) ? "MC_" : "Data_") << fDataSet << ".root"<<endl;

  std::string fileName(s.str());
  
	char tmpName[50];
	sprintf(tmpName,"_%i",nObjects);
	nObjects++;
	TString name = "centrality.root";
	name = tmpName + name;

  std::string fullPath = fDataPath;
  cout<<fDataPath<<endl;
  fullPath += fileName;
  cout<<fullPath<<endl;
  {
    std::ifstream src(fullPath.c_str(), std::ios::binary);
    std::ofstream dst(name,   std::ios::binary);
    
    dst << src.rdbuf();

	src.close();
	dst.close();
  }
  TFile *centralityFile = new TFile(name, "read");
  if (!centralityFile->IsOpen()) throw utl::ShineException("Centrality file cannot be opened.");
  
  centralityFile->GetObject("graph8s0l", fCentralityGraph8s0l);
  fCentralityGraph8s0l = (TGraphErrors*) fCentralityGraph8s0l->Clone();
  
  centralityFile->GetObject("graph8s2l", fCentralityGraph8s2l);
  fCentralityGraph8s2l = (TGraphErrors*) fCentralityGraph8s2l->Clone();
  
  centralityFile->GetObject("graph8s4l", fCentralityGraph8s4l);
  fCentralityGraph8s4l = (TGraphErrors*) fCentralityGraph8s4l->Clone();
  
  centralityFile->GetObject("graph16s12l", fCentralityGraph16s12l);
  fCentralityGraph16s12l = (TGraphErrors*) fCentralityGraph16s12l->Clone();
  
  centralityFile->Close();
  delete centralityFile;
  
  //neg = new TH2D("neg", "", 100, 0, 200, 100, 0, 5);
  //neu = new TH2D("neu", "", 100, 0, 200, 100, 0, 5);
  //pos = new TH2D("pos", "", 100, 0, 200, 100, 0, 5);
  //
  //parEne = new TH1D("parEne", "", 1, 0, -1);
}

void LegacyCentrality::GetSpectators(const evt::Event& event, std::vector< evt::Index<evt::sim::VertexTrack> >& indexList)
{
  unsigned int runNumber = event.GetEventHeader().GetRunNumber();
  det::Detector::GetInstance().Update(event.GetEventHeader().GetTime(), runNumber);
  const evt::SimEvent& simEvent = event.GetSimEvent();
  const evt::sim::Vertex mainVertex = simEvent.GetMainVertex();
  //cout << endl <<endl << mainVertex.GetNumberOfParentTracks() << "\t" << mainVertex.GetNumberOfDaughterTracks() << endl;
  
  for (evt::sim::VertexTrackIndexIterator iter  = mainVertex.DaughterTracksBegin(),
                                  endIter  = mainVertex.DaughterTracksEnd();
                                     iter != endIter;
                                   ++iter){
    const evt::sim::VertexTrack primaryTrack = simEvent.Get(*iter);
    if (!simEvent.Has(primaryTrack.GetStopVertexIndex())) continue;
    const evt::sim::Vertex secondVertex = simEvent.Get(primaryTrack.GetStopVertexIndex());
    const int numberOfPrimaryParents = secondVertex.GetNumberOfParentTracks();
    //cout << secondVertex.GetNumberOfParentTracks() << "\t" << secondVertex.GetNumberOfDaughterTracks() << endl;
    
    if (secondVertex.GetNumberOfDaughterTracks() == 1){
      const evt::sim::VertexTrack internalTrack = simEvent.Get(*(secondVertex.DaughterTracksBegin()));
      if (internalTrack.GetType() == evt::sim::VertexTrackConst::eGeneratorFinal && (internalTrack.GetParticleId() == 2212 || internalTrack.GetParticleId() == 2112)){
        indexList.push_back(internalTrack.GetIndex());
      //  cout << "Type 4 found" << endl;
      }
      if (!internalTrack.HasStopVertex()) continue;
      const evt::sim::Vertex finalVertex = simEvent.Get(internalTrack.GetStopVertexIndex());
      const int numberOfFinalTracks = finalVertex.GetNumberOfDaughterTracks();
      if (numberOfFinalTracks == 1){
        const evt::sim::VertexTrack finalTrack = simEvent.Get(*(finalVertex.DaughterTracksBegin()));
        if (finalTrack.GetType() == evt::sim::VertexTrackConst::eGeneratorFinal){
          if (finalTrack.GetParticleId() == 2212 || finalTrack.GetParticleId() == 2112){
            if (numberOfPrimaryParents == 1){
              indexList.push_back(finalTrack.GetIndex());
            //  cout << "Type 3 found" << endl;
            }
          } else if (finalTrack.GetParticleId() > 1000000000) {
            const int A = (finalTrack.GetParticleId() % 10000) / 10;
            if (numberOfPrimaryParents == A){
              indexList.push_back(finalTrack.GetIndex());
            //  cout << "Type 1 found" << endl;
            }
          }
        }
      } else {
        bool isSpectatorVertex = true;
        int finalA = 0;
        for (evt::sim::VertexTrackIndexIterator iterFinal  = finalVertex.DaughterTracksBegin(),
                                        endIterFinal  = finalVertex.DaughterTracksEnd();
                                           iterFinal != endIterFinal;
                                         ++iterFinal){
          const evt::sim::VertexTrack finalTrack = simEvent.Get(*iterFinal);
          if (finalTrack.GetType() != evt::sim::VertexTrackConst::eGeneratorFinal){
            isSpectatorVertex = false;
            break;
          }
          if (finalTrack.GetParticleId() < 1000000000 && (finalTrack.GetParticleId() != 2212 && finalTrack.GetParticleId() != 2112)){
            isSpectatorVertex = false;
            break;
          }
          if (finalTrack.GetParticleId() == 2212 || finalTrack.GetParticleId() == 2112){
            finalA++;
          }
          if (finalTrack.GetParticleId() > 1000000000){
            finalA += (finalTrack.GetParticleId() % 10000) / 10;
          }
        }
        if (finalA == numberOfPrimaryParents && isSpectatorVertex){
          for (evt::sim::VertexTrackIndexIterator iterFinal  = finalVertex.DaughterTracksBegin(),
                                          endIterFinal  = finalVertex.DaughterTracksEnd();
                                             iterFinal != endIterFinal;
                                           ++iterFinal){
            const evt::sim::VertexTrack finalTrack = simEvent.Get(*iterFinal);
            indexList.push_back(finalTrack.GetIndex());
          }
        //  cout << "Type 2 found" << endl;
        }
      }
    }
    
    for (int i = 0; i < numberOfPrimaryParents - 1; ++i){
      ++iter;
    }
  }
  
  std::sort(indexList.begin(), indexList.end());
  indexList.erase(std::unique(indexList.begin(), indexList.end() ), indexList.end());
    
}


Double_t radialShowerFit(Double_t *x, Double_t *par)
{ 
  double xc = x[0]-par[0];
  double yc = x[1]-par[1];
  double nomi = sqrt(xc*xc + yc*yc);
  return par[4]*exp(-nomi/par[2]) + (1-par[4])*exp(-nomi/par[3]);
}

void LegacyCentrality::PSDSimulation(const evt::Event &event, double (&moduleEnergy)[45], std::vector< evt::Index<evt::sim::VertexTrack> > &specList)
{  
  unsigned int runNumber = event.GetEventHeader().GetRunNumber();
  det::Detector::GetInstance().Update(event.GetEventHeader().GetTime(), runNumber);
  const evt::SimEvent& simEvent = event.GetSimEvent();
  if (!simEvent.HasMainVertex())
    return;
  
  double psdPosition = 0;
  double psdShift = 10000;
  if (fDataSet == 40){
    psdPosition = 1667 - 580;
    psdShift = 13;
  } else {
    psdPosition = 2200 - 580;
    psdShift = 18;
  }
  
  for (int i = 0; i < 45; ++i){
    moduleEnergy[i] = 0;
  }
  
  const det::MagneticFieldTracker
        tracker(det::Detector::GetInstance().GetMagneticField(), utl::Tracker::eConservativeTrackStepper);
  const det::PSD &detectorPSD = det::Detector::GetInstance().GetPSD();
  
  TF2 showerProfile("showerProfile", radialShowerFit, -1000, 1000, -1000, 1000, 5);
  double showerDecay     = 1.91085;  // Values from fitting to beam shower from data
  double showerDecaySlow = 7.8342;   // Values from fitting to beam shower from data
  double fraction        = 0.964088; // Values from fitting to beam shower from data
  showerProfile.SetParameters(0, 0, showerDecay, showerDecaySlow, fraction);
  double cutOff = 100;
  double energyNorm = showerProfile.Integral(-cutOff, +cutOff, -cutOff, +cutOff);
  
  
  for (std::list< evt::sim::VertexTrack >::const_iterator vtxTrackIter =  simEvent.Begin<evt::sim::VertexTrack>(),
                                                               endIter =  simEvent.End<evt::sim::VertexTrack>();
                                                          vtxTrackIter != endIter;
                                                        ++vtxTrackIter){
    
    
    evt::sim::VertexTrack vtxTrack = *vtxTrackIter;
    if (vtxTrack.GetType() != evt::sim::VertexTrackConst::eGeneratorFinal && vtxTrack.GetType() != evt::sim::VertexTrackConst::eDetector)
      continue;
    utl::Vector trackMom = vtxTrack.GetMomentum();
    const double pt = sqrt(trackMom.GetX()*trackMom.GetX() + trackMom.GetY()*trackMom.GetY());
    
    evt::sim::Vertex startVertex;
    if (simEvent.Has(vtxTrack.GetStartVertexIndex()))
      startVertex = simEvent.Get(vtxTrack.GetStartVertexIndex());
    if (vtxTrack.HasStopVertex()){
      const evt::sim::Vertex& stopVertex = simEvent.Get(vtxTrack.GetStopVertexIndex());
      if (stopVertex.GetPosition().GetZ() < psdPosition)
        continue;
    }
    if (trackMom.GetMag() < 1)
      continue;
    if (trackMom.GetZ() < 0.5)
      continue;
    if (pt > 2) 
      continue;
    if (vtxTrack.GetCharge() == -1 && trackMom.GetMag() > 120.)
      continue;
    //if (vtxTrack.GetType() == evt::sim::VertexTrackConst::eDetector && vtxTrack.GetParticleId() <= 22)
    //  continue;
    
    int isSpec = 0;
    
    if (vtxTrack.GetType() == evt::sim::VertexTrackConst::eGeneratorFinal){
      if (std::find(specList.begin(), specList.end(), vtxTrack.GetIndex())!=specList.end()){
        int A = 0;
        if (vtxTrack.GetParticleId() > 1000000000) {
          A = (vtxTrack.GetParticleId() % 10000) / 10;
        } else {
          A = 1;
        }
        isSpec += A;
      }
    }
    
    if (isSpec){
      double fermiMom = fRandGen.Gaus(0, 0.2/sqrt(isSpec));
      if (fermiMom < 0) fermiMom *= -1;
      double theta = fRandGen.Uniform(0, 3.1415);
      double phi   = fRandGen.Uniform(0, 2*3.1415);
      double oldPx = trackMom.GetX();
      double oldPy = trackMom.GetY();
      double newPx = oldPx + fermiMom*sin(theta)*sin(phi);
      double newPy = oldPy + fermiMom*sin(theta)*cos(phi);
      trackMom.Set(newPx, newPy, trackMom.GetZ());
    }

    utl::Vector stopMomentum;
    utl::Point stopPosition;
    
    utl::Point startPosition(startVertex.GetPosition().GetX(), startVertex.GetPosition().GetY(), startVertex.GetPosition().GetZ());
    
    bool trackSuccess = tracker.TrackToZ(psdPosition, vtxTrack.GetCharge(),
                                         startPosition, trackMom,
                                         stopPosition, stopMomentum);

    double hitX = stopPosition.GetX();
    double hitY = stopPosition.GetY();
    
    if (hitX < -40 + psdShift || hitX > 40 + psdShift || hitY < -60 - 1 || hitY > 40 - 1 || !trackSuccess)
      continue;
    
    //if (vtxTrack.GetCharge() == -1) neg->Fill(trackMom.GetMag(), pt);
    //if (vtxTrack.GetCharge() ==  0) neu->Fill(trackMom.GetMag(), pt);
    //if (vtxTrack.GetCharge() ==  1) pos->Fill(trackMom.GetMag(), pt);
    
    /*if (vtxTrack.GetType() == evt::sim::VertexTrackConst::eDetector && vtxTrack.GetParticleId() == 22){
      evt::sim::Vertex startVert = simEvent.Get(vtxTrack.GetStartVertexIndex());
      evt::sim::VertexTrack parTrack = simEvent.Get(startVert.GetFirstParentTrackIndex());
      startVert = simEvent.Get(parTrack.GetStartVertexIndex());
      parTrack = simEvent.Get(startVert.GetFirstParentTrackIndex());
      
      parEne->Fill(TString::Format("%d", parTrack.GetParticleId()), stopMomentum.GetMag());
    }*/
    
    double energy = sqrt(stopMomentum.GetMag2());
    //std::cout << energy << "\t";
    energy = fRandGen.Gaus(energy, (0.72/sqrt(energy) + 0.026)*energy);  //Values from T3 trigger study
    
    if (energy < 0) energy = 0;
    showerProfile.SetParameters(hitX, hitY, showerDecay, showerDecaySlow, fraction);
    
    for (int i = 1; i <= 44; ++i){
      double modSizeXlow  = -1 * detectorPSD.GetPositionX(i)/10 + psdShift - detectorPSD.GetWidth(i)/2;
      double modSizeXhigh = -1 * detectorPSD.GetPositionX(i)/10 + psdShift + detectorPSD.GetWidth(i)/2;
      double modSizeYlow  = detectorPSD.GetPositionY(i)/10 - 1  - detectorPSD.GetHeight(i)/2;
      double modSizeYhigh = detectorPSD.GetPositionY(i)/10 - 1  + detectorPSD.GetHeight(i)/2;
      
      double energyFraction = showerProfile.Integral(modSizeXlow, modSizeXhigh, modSizeYlow, modSizeYhigh)/energyNorm;
      //std::cout << energyFraction << "\t";
      moduleEnergy[i] += energy * energyFraction;
      //std::cout << moduleEnergy[i] << "\t";
    }
  }
  //std::cout << std::endl;
}