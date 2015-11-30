#ifndef HANDLERS
#define HANDLERS
#include "NA61_LRC_2_Handler.h"
#endif



#ifndef HANDLERLIST
#define HANDLERLIST
#include "NA61_LRC_2_Handler_List.h"
#endif

#ifndef CUTLIST
#define CUTLIST
#include "NA61_LRC_2_Cut_List.h"
#endif

#ifndef CUT
#define CUT
#include "NA61_LRC_Cut.h"
#endif

#ifndef CONST
#define CONST
#include "NA61_LRC_2_Const.h"
#endif

HandlerList::HandlerList()
{
	hasSimHandler = false;
	hasRecHandler = false;
	hasRawHandler = false;
    myNext=0;
    myValue=0;
	init=false;
}

HandlerList::HandlerList(BaseHandler* handler)
{
	hasSimHandler = false;
	hasRecHandler = false;
	hasRawHandler = false;
    myNext=0;
    myValue=handler;
	init=false;
}

HandlerList::~HandlerList()
{
    HandlerList *current = this;
    BaseHandler *currentHandler = 0;
    int length = this->GetLength();
    cout<<"length:  "<<length<<endl;
    for (int i=0; i<length;i++)
    {
        delete this->GetValue(i);
    }
	delete []pGoodEvent;
}


void HandlerList::AddHandler(BaseHandler *handler)
{
	if ((handler->IsItRaw() == true) && (handler->GetSim() == true)){
		cout<<"Can't add sim handler. Change the value of bool realData in Const.h to false"<<endl;
		return;
	} 
	if (handler->GetSim() == true) hasSimHandler = true;
	else 	hasRecHandler = true;
	if (handler->IsItRaw() == true) hasRawHandler = true;
    HandlerList *current = this;
    if (current ->myValue == 0)             myValue=handler;
    else
    {
        while(current -> myNext != 0)
            current = current -> myNext;
        current -> myNext = new HandlerList(handler);
    }
}

int HandlerList::GetLength()
{
    if (myValue == 0) return 0;
    HandlerList *current = this;
    int length = 1; 
    while (current -> myNext !=0)
    {
        current=current -> myNext;
        length++;
    }
    return length;
}


BaseHandler* HandlerList::GetValue(int index)
{
    int length = this->GetLength();
    if (length<=index) return 0;
    HandlerList *current = this;
    for (int i=0; i<index; i++)
        current = current->myNext;
    return current->myValue;
}

void HandlerList::EndOfEvent(Event& ev)
{
    int length = this->GetLength();
    if (length == 0) return;
    for (int i=0; i<length;i++)
    	if (pGoodEvent[i]==true)
        	this->GetValue(i)->EndOfEvent(ev);
}
int goodEventCounter = 0;

void HandlerList::EatEvent(Event &event)
{
	if (init == false) this->Init();

	time_t     now;
	struct tm  *ts;
	char       buf[80];

	int length = this->GetLength();
    if (length == 0) return;
	bool test = false;
	for (int i=0; i<length; i++){
//		cout<<"handler["<<i<<"] event test"<<endl;
		if (this->GetValue(i)->EventTest(event)){
			pGoodEvent[i] = true;
			//cout << "goodEvent" << endl;
			test = true;
		} else {
			pGoodEvent[i] = false;
		}
	}
//	cout<<"event has tested..."<<test<<endl;
	if (test == false) return;
	goodEventCounter++;
	cout << "GOOD " << goodEventCounter << endl;

	
	// --- working status output
	if(goodEventCounter%1000==0){
		// Get the current time
		now = time(NULL);          
		// Format and print the time, "ddd yyyy-mm-dd hh:mm:ss zzz" 
		ts = localtime(&now);
		strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", ts);
		printf("running GOOD event %5d time: %s\n",(int) goodEventCounter,buf);
	} 

	
	if (hasRecHandler==true){

		RecEvent& recEvent = event.GetRecEvent();
		
		if (hasRawHandler == false){
			Vertex& mainVertex = recEvent.GetMainVertex();
			//new 
			for (VertexTrackIndexIterator vtxTrackIter = mainVertex.DaughterTracksBegin();
				vtxTrackIter != mainVertex.DaughterTracksEnd(); ++vtxTrackIter){
			//for (list<evt::rec::VertexTrack>::const_iterator iTrack = recEvent.Begin<evt::rec::VertexTrack>();
			//	iTrack != recEvent.End<evt::rec::VertexTrack>(); ++iTrack) {

				CutList::trackCutChecking.Reset();

       			//const evt::rec::VertexTrack& vtxTrack = *iTrack;
				//Vertex& mainVertex = recEvent.GetMainVertex();
				VertexTrack& vtxTrack = recEvent.Get(*vtxTrackIter);
				if (vtxTrack.GetStartVertexIndex() != mainVertex.GetIndex())
					continue;        
				for (int i=0; i<length; i++){
					if (pGoodEvent[i]){
						if (this->GetValue(i)->GetSim()==false){
							this->GetValue(i)->PutTrack(vtxTrack, event);
						}
					}
				}
			}		
		}else{
			Vertex& mainVertex = recEvent.GetMainVertex();
			//new 
			for (VertexTrackIndexIterator vtxTrackIter = mainVertex.DaughterTracksBegin();
				 vtxTrackIter != mainVertex.DaughterTracksEnd(); ++vtxTrackIter){		
			//old
//			for (list<evt::rec::VertexTrack>::const_iterator iTrack = recEvent.Begin<evt::rec::VertexTrack>();
//				iTrack != recEvent.End<evt::rec::VertexTrack>(); ++iTrack) {
				CutList::trackCutChecking.Reset();

				VertexTrack& vtxTrack = recEvent.Get(*vtxTrackIter);

				for (int i=0; i<length; i++){
					if (pGoodEvent[i]){
						this->GetValue(i)->PutTrack(vtxTrack, event);
					}
				}
			}


				//const evt::rec::VertexTrack& vtxTrack = *iTrack;
			}
		}
	//}
	if (hasSimHandler==true){
//		cout<<"SIM"<<endl;
		evt::SimEvent& simEvent = event.GetSimEvent();
		RecEvent& recEvent = event.GetRecEvent();
		for (list<evt::sim::VertexTrack>::const_iterator iTrack = simEvent.Begin<evt::sim::VertexTrack>();
			iTrack != simEvent.End<evt::sim::VertexTrack>(); ++iTrack){
			if (iTrack->GetType() == sim::VertexTrackConst::eGeneratorFinal) {
//				evt::sim::VertexTrack& vtxTrack = simEvent.Get(*iTrack);
        		const evt::sim::VertexTrack& vtxTrack = *iTrack;
				if (iTrack->GetType() == sim::VertexTrackConst::eGeneratorFinal) {
					CutList::trackCutChecking.Reset();

					for (int i = 0; i < length; i++){
						if (pGoodEvent[i]){
							if (this->GetValue(i)->GetSim() == true)
								this->GetValue(i)->PutTrack(vtxTrack, event);
						}
					}
				}
			}
		}
/*		const sim::Vertex &  mainVertex = simEvent.GetMainVertex();
		for (evt::sim::VertexTrackIndexIterator vtxTrackIter = mainVertex.DaughterTracksBegin();
			vtxTrackIter != mainVertex.DaughterTracksEnd(); ++vtxTrackIter){
			
			const evt::sim::VertexTrack& vtxTrack = simEvent.Get(*vtxTrackIter);

			CutList::trackCutChecking.Reset();

			for (int i = 0; i<length; i++){
				if (pGoodEvent[i]){
					if (this->GetValue(i)->GetSim() == true)
						this->GetValue(i)->PutTrack(vtxTrack, event);
				}
			}

		}*/
	}
//	cout<<"endOfEvent"<<endl;
    this->EndOfEvent(event);
	for (int i=0; i<length; i++)
		pGoodEvent[i]=false;
}


void HandlerList::Init()
{
	init=true;
	int N = this->GetLength();
	pGoodEvent = new bool[N];
	for (int i=0; i<N; i++) pGoodEvent[i] = false;
}
