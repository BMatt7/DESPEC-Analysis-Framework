//---------------------------------------------------------------
//        Go4 Release Package v3.02-0 (build 30200) 
//                      21-July-2006
//---------------------------------------------------------------
//	 The GSI Online Offline Object Oriented (Go4) Project
//	 Experiment Data Processing at EE department, GSI
//---------------------------------------------------------------
//
//Copyright (C) 2000- Gesellschaft f. Schwerionenforschung, GSI
//                    Planckstr. 1, 64291 Darmstadt, Germany
//Contact:            http://go4.gsi.de
//----------------------------------------------------------------
//This software can be used under the license agreements as stated
//in Go4License.txt file which is part of the distribution.
//----------------------------------------------------------------
//
//  HS: 07-11-14
//  Simple Analysis Program for following CAEN modules:
//
//  subevent PID=20: V785 ADC, V775 TDC, V792 QDC
//
//----------------------------------------------------------------

#include "First_Test.h"

#include "Riostream.h"

#include "TH1.h"
#include "TH2.h"
#include "TCutG.h"
#include "snprintf.h"
#include "TTree.h"

#include "TGo4MbsEvent.h"
#include "TGo4WinCond.h"
#include "TGo4PolyCond.h"
#include "TGo4CondArray.h"
#include "TGo4Picture.h"

#include "Tx7_Param.h"

//***********************************************************
First_Test::First_Test() : TGo4EventProcessor("Proc")
{
  cout << "**** First_Test: Create instance " << endl;
}
//***********************************************************
First_Test::~First_Test()
{
  cout << "**** First_Test: Delete instance " << endl;
}
//***********************************************************
// this one is used in standard factory
First_Test::First_Test(const char* name) : TGo4EventProcessor(name)
{
  cout << "**** First_Test: Create instance " << name << endl;

  Text_t chis[16];
  Text_t chead[64];
  Int_t i;

  //// init user analysis objects:

  // Creation of parameters (check if restored from auto save file):

  if(GetParameter("Par1")==0)
  {// no auto save, create new
      fParam = new Tx7_Param("Par1");
      AddParameter(fParam);
      cout << "**** First_Test: Created parameter" << endl;


//fdTime = new TH1I ("dTime","Gated histogram",10000,1,10000);
//AddHistogram(fdTime);
  
  }
  else // got them from autosave file, restore pointers
  {
      fParam=(Tx7_Param *)GetParameter("Par1");
      cout << "**** First_Test: Restored parameter from autosave" << endl;
  }

  // Creation of histograms (check if restored from auto save file):
  
  if(GetHistogram("Crate1/Cr1Ch01")==0)
  {// no auto save, create new
 #ifdef EXAMPLE_CODE
     for(i =0;i<8;i++)
      {
          snprintf(chis,15,"Cr1Ch%02d",i+1);  snprintf(chead,63,"Crate 1 channel %2d",i+1);
          fCr1Ch[i] = new TH1I (chis,chead,5000,1,5000);
          AddHistogram(fCr1Ch[i],"Crate1");
          snprintf(chis,15,"Cr2Ch%02d",i+1);  snprintf(chead,63, "Crate 2 channel %2d",i+1);
          fCr2Ch[i] = new TH1I (chis,chead,5000,1,5000);
          AddHistogram(fCr2Ch[i],"Crate2");
      }

      fCr1Ch1x2 = new TH2I("Cr1Ch1x2","Crate 1 channel 1x2",200,1,5000,200,1,5000);
      AddHistogram(fCr1Ch1x2);
      fHis1 = new TH1I ("His1","Condition histogram",5000,1,5000);
      AddHistogram(fHis1);
      fHis2 = new TH1I ("His2","Condition histogram",5000,1,5000);
      AddHistogram(fHis2);
      fHis1gate = new TH1I ("His1g","Gated histogram",5000,1,5000);
      AddHistogram(fHis1gate);
      fHis2gate = new TH1I ("His2g","Gated histogram",5000,1,5000);
      AddHistogram(fHis2gate);

//      AddTree(gg_tree)

      //*******   Li-Liz Tree ***********//
//      TTree *toy_tree = new TTree("toy_tree","toy_tree");
//      Float_t E1;
//      toy_tree->Branch("E1",&E1,"E1/F");
//	     toy_tree->Fill();
     //**********//

      cout << "**** First_Test: Created histograms" << endl;
#endif

//      TGo4Analysis::AddTree(gg_tree);
//     Float_t E1,E2,T1,T2;

//      gg_tree = new TTree("gg_tree","gg_tree");
//      gg_tree->Branch("E1",&E1,"E1/F");
//      gg_tree->Branch("E2",&E2,"E2/F");
//      gg_tree->Branch("T1",&T1,"T1/F");
//      gg_tree->Branch("T2",&T2,"T2/F");






//       AddTree(gg_tree,"Tree");

	  for(i =0;i<32;i++)   // QDC
      {
	          snprintf(chis,15,"E_QDC%02d",i+1);  
          snprintf(chead,63,"QDC channel %2d",i+1);
          fE_QDC[i] = new TH1I (chis,chead,4096,1,4096);
          AddHistogram(fE_QDC[i],"CAEN_QDC_V792");
      }

      for(i =0;i<32;i++)   // TDC
      {
          snprintf(chis,15,"Time%02d",i+1);  
          snprintf(chead,63,"TDC channel %2d",i+1);
          fTime[i] = new TH1I (chis,chead,4096,1,4096);
          AddHistogram(fTime[i],"CAEN_TDC_V775");
      }

      for(i =0;i<32;i++)   // ADC
      {
        
	      snprintf(chis,15,"E_ADC%02d",i+1);  
          snprintf(chead,63,"ADC channel %2d",i+1);
          fE_ADC[i] = new TH1I (chis,chead,4096,1,4096);
          AddHistogram(fE_ADC[i],"CAEN_ADC_V785");
      }
	//try for time resolution
//     if (fE_ADC[0]>200&&fE_ADC[1]>200)  AddHistogram(fTime[5],"CAEN_TDC_V775");
      for(i =0;i<32;i++)   // V1290 TDC
      {
          snprintf(chis,15,"Time2%02d",i+1);  
          snprintf(chead,63,"TDC channel %2d",i+1);
          fTime2[i] = new TH1I (chis,chead,4096,1,65536);
          AddHistogram(fTime2[i],"CAEN_TDC_V1290");
		
	  }


  }
  else // got them from autosave file, restore pointers
  {
#ifdef EXAMPLE_CODE
      for(i =0;i<8;i++)
      {
         snprintf(chis,15,"Crate1/Cr1Ch%02d",i+1); fCr1Ch[i] = (TH1I*)GetHistogram(chis);
         snprintf(chis,15,"Crate2/Cr2Ch%02d",i+1); fCr2Ch[i] = (TH1I*)GetHistogram(chis);
      }

      fCr1Ch1x2 = (TH2I*)GetHistogram("Cr1Ch1x2");
      fHis1     = (TH1I*)GetHistogram("His1");
      fHis2     = (TH1I*)GetHistogram("His2");
      fHis1gate = (TH1I*)GetHistogram("His1g");
      fHis2gate = (TH1I*)GetHistogram("His2g");
#endif
      cout << "**** First_Test: Restored histograms from autosave" << endl;
  }

#ifdef EXAMPLE_CODE
  // Creation of conditions (check if restored from auto save file):
  if(GetAnalysisCondition("cHis1")==0)
    {// no auto save, create new
      fconHis1= new TGo4WinCond("cHis1");
      fconHis2= new TGo4WinCond("cHis2");
      fconHis1->SetValues(100,2000);
      fconHis2->SetValues(100,2000);
      AddAnalysisCondition(fconHis1);
      AddAnalysisCondition(fconHis2);

      Double_t xvalues[4]={400,700,600,400};
      Double_t yvalues[4]={800,900,1100,800};
      TCutG* mycut= new TCutG("cut1",4,xvalues,yvalues);
      fPolyCon= new TGo4PolyCond("polycon");
      fPolyCon->SetValues(mycut); // copies mycat into fPolyCon
      fPolyCon->Disable(true);   // means: condition check always returns true
      delete mycut; // mycat has been copied into the conditions
      AddAnalysisCondition(fPolyCon);

      xvalues[0]=1000;xvalues[1]=2000;xvalues[2]=1500;xvalues[3]=1000;
      yvalues[0]=1000;yvalues[1]=1000;yvalues[2]=3000;yvalues[3]=1000;
      mycut= new TCutG("cut2",4,xvalues,yvalues);
      fConArr = new TGo4CondArray("polyconar",4,"TGo4PolyCond");
      fConArr->SetValues(mycut);
      fConArr->Disable(true);   // means: condition check always returns true
      delete mycut; // mycat has been copied into the conditions
      AddAnalysisCondition(fConArr);
      cout << "**** First_Test: Created conditions" << endl;
    }
  else // got them from autosave file, restore pointers
    {
      fPolyCon  = (TGo4PolyCond*) GetAnalysisCondition("polycon");
      fConArr   = (TGo4CondArray*)GetAnalysisCondition("polyconar");
      fconHis1  = (TGo4WinCond*)  GetAnalysisCondition("cHis1");
      fconHis2  = (TGo4WinCond*)  GetAnalysisCondition("cHis2");
      fconHis1->ResetCounts();
      fconHis2->ResetCounts();
      fPolyCon->ResetCounts();
      fConArr->ResetCounts();
      cout << "**** First_Test: Restored conditions from autosave" << endl;
    }
  // connect histograms to conditions. will be drawn when condition is edited.
  fconHis1->SetHistogram("His1");
  fconHis2->SetHistogram("His2");
  fconHis1->Enable();
  fconHis2->Enable();
  fPolyCon->Enable();
  ((*fConArr)[0])->Enable();
  ((*fConArr)[1])->Enable(); // 2 and 3 remain disabled

  if (GetPicture("Picture")==0)
    {// no auto save, create new
      // in the upper two pads, the condition limits can be set,
      // in the lower two pads, the resulting histograms are shown
      fcondSet = new TGo4Picture("condSet","Set conditions");
      fcondSet->SetLinesDivision(2,2,2);
      fcondSet->LPic(0,0)->AddObject(fHis1);
      fcondSet->LPic(0,1)->AddObject(fHis2);
      fcondSet->LPic(0,0)->AddCondition(fconHis1);
      fcondSet->LPic(0,1)->AddCondition(fconHis2);
      fcondSet->LPic(1,0)->AddObject(fHis1gate);
      fcondSet->LPic(1,1)->AddObject(fHis2gate);
      fcondSet->LPic(1,0)->SetFillAtt(4, 1001); // solid
      fcondSet->LPic(1,0)->SetLineAtt(4,1,1);
      fcondSet->LPic(1,1)->SetFillAtt(9, 1001); // solid
      fcondSet->LPic(1,1)->SetLineAtt(9,1,1);
      AddPicture(fcondSet);

      fPicture = new TGo4Picture("Picture","Picture example");
      fPicture->SetLinesDivision(3, 2,3,1);
      fPicture->LPic(0,0)->AddObject(fCr1Ch[0]);
      fPicture->LPic(0,0)->SetFillAtt(5, 3001); // pattern
      fPicture->LPic(0,0)->SetLineAtt(5,1,1);
      fPicture->LPic(0,1)->AddObject(fCr1Ch[1]);
      fPicture->LPic(0,1)->SetFillAtt(4, 3001); // pattern
      fPicture->LPic(0,1)->SetLineAtt(4,1,1);
      fPicture->LPic(1,0)->AddObject(fCr1Ch[2]);
      fPicture->LPic(1,0)->SetFillAtt(6, 1001); // solid
      fPicture->LPic(1,0)->SetLineAtt(6,1,1);
      fPicture->LPic(1,1)->AddObject(fCr1Ch[3]);
      fPicture->LPic(1,1)->SetFillAtt(7, 1001); // solid
      fPicture->LPic(1,1)->SetLineAtt(7,1,1);
      fPicture->LPic(1,2)->AddObject(fCr1Ch[4]);
      fPicture->LPic(3,0)->AddObject(fCr1Ch1x2);
      fPicture->LPic(3,0)->SetDrawOption("CONT");
      AddPicture(fPicture);
      cout << "**** First_Test: Created pictures" << endl;
    }
  else  // got them from autosave file, restore pointers
    {
      fPicture = GetPicture("Picture");
      fcondSet = GetPicture("condSet");
      cout << "**** First_Test: Restored pictures from autosave" << endl;
    }
#endif  // end EXAMPLE_CODE
}

//-----------------------------------------------------------
// event function
Bool_t First_Test::BuildEvent(TGo4EventElement* target)
{  // called by framework. We dont fill any output event here at all

  TGo4MbsSubEvent* psubevt;
  //  Int_t index=0;
  //  Float_t value1=0;
  //  Float_t value2=0;
  //  Float_t value=0;

  Int_t i,lwords;
  Int_t *pdata;
  Int_t *pl_data,
        diff;  
  char exit_flg=0;

// CAEN variables

unsigned long  caen_header,
               geo,                  /* geograph. address */
               type;                 /* [2,6] */

 long           //g1=0,
               d_cnt;                /* number of data words */
unsigned long  data,
               chan;

double t1=0;
double t2=0;


/* some masks for the CAEN data words */

#define  OV_MASK 0x00001000  /* overflow bit */
#define  UN_MASK 0x00002000  /* underthreshold bit */
#define   V_MASK 0x00004000  /* data valid bit */
#define FCH_MASK 0x00003F00  /* number of fired channels mask */
#define  CH_MASK 0x003F0000  /* channel mask */
#define  DA_MASK 0x00000FFF  /* data mask */
#define  TY_MASK 0x07000000  /* type mask */
#define GEO_MASK 0xF8000000  /* geo address mask */

#define CH_MASK2 0x03E00000  // channel mask for V1290
#define DA_MASK2 0x001FFFFF  // data mask for V1290

  fInput = (TGo4MbsEvent* ) GetInputEvent();
  if(fInput == 0)
  {
    cout << "AnlProc: no input event !"<< endl;
    return kFALSE;
  }

#ifdef EXAMPLE_CODE
  if(fInput->GetTrigger() > 11)
  {
      cout << "**** First_Test: Skip trigger event"<<endl;
      return kFALSE;
  }

  // first we fill the arrays fCrate1,2 with data from MBS source
  // we have up to two subevents, crate 1 and 2
  // Note that one has to loop over all subevents and select them by
  // crate number:   psubevt->GetSubcrate(),
  // procid:         psubevt->GetProcid(),
  // and/or control: psubevt->GetControl()
  // here we use only crate number
#endif

    // g1=fParam->caen_group;   // read CAEN input group (=8channels) number [0..3] from parameter 'caen_group'

    //    if (g1<0 || g1>3)
    //  g1=0;

  // fParam->group_mon=g1;

  //  g1=1;

  //------------------------------------

  fInput->ResetIterator();

  //****************************************************************
  // here userspecific unpacking and histogram filling, HS 30-01-08
  //****************************************************************


  while((psubevt = fInput->NextSubEvent()) != 0) // loop over subevents
  {

      pdata=psubevt->GetDataField();
      lwords= psubevt->GetIntLen();

      std::cout<< "ProcId" << psubevt-> GetProcid() <<"	IntLen"<< psubevt->GetIntLen() << std::endl;

      if (psubevt->GetProcid()==10 && psubevt->GetIntLen()>2)   // V792 subevent, PID=20 and not empty
      {
        pl_data = pdata;

        //***********************************
        //***  CAEN V785 or V775 or V792  ***
        //***********************************

        // module loop

        for(;;)
	{
	  //printf("GEO: %d\n",geo);  
          diff = pl_data - pdata;

          if ((diff >= lwords) || exit_flg)    // end of subevent
            break;

          caen_header=*pl_data++;              // read header word

          type = (caen_header&TY_MASK)>>24;    // get type [2,6] 

          if (type==2)   // card with data, else =6 
          {
            geo  = (caen_header&GEO_MASK)>>27;      // get geographical address 
            
	    //return kTRUE;

            d_cnt= (caen_header&FCH_MASK)>>8;       // get number of fired channels 


	    

            /*
            printf("         GEO: %d (0x%x)\n",geo,geo);
            printf("        Type: %d (0x%x)\n",type,type);
            printf("       D_CNT: %d (0x%x)\n\n",d_cnt,d_cnt);
            */

unsigned long  data1=0, data2=0, data3;
            for(i=0;i<d_cnt;i++)
            {
                data=*pl_data++;
                chan=(data & CH_MASK)>>16;     // [0..31] 
                data=data & DA_MASK;
             
                //***********
                // V785 ADC
                //***********

                if (geo == 16)   
			{
                  if (chan >=0 && chan <=31)
                    fE_ADC[chan]->Fill(data); 

			}

                //***********
                // V775 TDC
                //***********
                if (geo == 13)  
			{
                if (chan >=0 && chan <=31)
                    fTime[chan]->Fill(data);
		  		if (chan == 1) data1=data;
		  		if (chan == 2) data2=data;
            }

                //***********
                // V792 QDC
                //***********

                if (geo == 6) 
			{
                  if (chan >=0 && chan <=31)
                    fE_QDC[chan]->Fill(data);

			if (chan==0) {
                         E1=data;
                         }
            if (chan==1) {
                         E2=data;
                         }

                }

            } // end for(i)

 if (d_cnt== 2&&data1!=0&&data2!=0&&geo==13)
                        {
                    data3=data1-data2;
                    fTime[5]->Fill(data3+1000);
                        }

            pl_data++;   // skip EOB word 

          }   // end if(type)
      
        }   // end module loop for(;;)

      }   // end subevent PID 20

      //------------------------------------------

      if (psubevt->GetProcid()==20 && psubevt->GetIntLen()>2)   // V1290 TDC subevent, PID=50 and not empty
      {
        //************************
        //***  CAEN V1290 TDC  ***
        //************************

        caen_header=*pdata++;     // read header word 

        type = (caen_header >> 27) & 0x1F;   // get global header ID 

//cout<< "type " << type <<endl;

//        if (type==8)   // header ID (%01000) 
        if (type==20)   // header ID (%01000) 
        {
	    geo  = caen_header & 0x1F;            // get geographical address 
 //            printf("GEO[%d]: %d\n",i_tdc,geo); 
//if(geo!=9 && geo!=14)             
//cout << "geo address" << geo<< endl;

int LeadingOrTrailing=0;
if(psubevt->GetIntLen()==13)
{
            for(;;)   // scan data until global Trailer
            {
                data=*pdata++;

                if ((data & 0xF8000000) == 0x88000000)  // trigger time tag
                  data=*pdata++;

                if ((data & 0xF8000000) == 0x80000000)  // global trailer
		{
                  if (data & 0x07000000)
		    printf("V1290 Global Trailer contains Status Info: 0x%08x\n",(unsigned int)data);

		      break;  // exit loop
                }

                chan=(data & CH_MASK2)>>21;     // [0..31] 
                data=data & DA_MASK2;

    if(chan==0 && psubevt->GetIntLen()==13){ LeadingOrTrailing++;}

                //---------------------------------
                // simple histogram filling
                // no special multi hit processing
 

	cout << "channel address" << chan<< endl;

                if (chan >=0 && LeadingOrTrailing==1) {fTime2[0]->Fill(data);}
                if (chan >=0 && LeadingOrTrailing==2) {fTime2[1]->Fill(data);LeadingOrTrailing=0;}
                
		  
/*		

		         if (chan==0) {
                         T1=data;
                         }
                   if (chan==1) {
                         T2=data;
                         }

*/




 
#ifdef OLD_STUFF
                
                switch(chan)
                { 
                    case 0: // HISACC1I(CH1_T,data,1);
                            fTime2[0]->Fill(data);    
                            break;
                    case 1: // HISACC1I(CH2_T,data,1);
                            fTime2[1]->Fill(data); 
                            break;
                    case 2: // HISACC1I(CH3_T,data,1);
                            fTime2[2]->Fill(data); 
                            break;
                    case 3: // HISACC1I(CH4_T,data,1);
                            fTime2[3]->Fill(data);  
                            break;
                    case 4: // HISACC1I(CH5_T,data,1);
                            fTime2[4]->Fill(data); 
                            break;
                    case 5: // HISACC1I(CH6_T,data,1);
                            fTime2[5]->Fill(data); 
                            break;
                    case 6: // HISACC1I(CH7_T,data,1);
                            fTime2[6]->Fill(data); 
                            break;
                    case 7: // HISACC1I(CH8_T,data,1);
                            fTime2[7]->Fill(data); 
                            break;
                    default: break;
                }
 #endif

//	gg_tree->Fill();
            } // end for(;;)



}// end geo

		}  // end_if(type)



	}  // end subevent PID 50



  }     // end subevent while loop



#ifdef EXAMPLE_CODE
  // now we fill histograms from the arrays
  for(i = 0; i<8; i++)
    {
      fCr1Ch[i]->Fill(fCrate1[i]);
      fCr2Ch[i]->Fill(fCrate2[i]);
    }
  value1=fCrate1[0];
  value2=fCrate1[1];
  fHis1->Fill(value1); //fill histograms without gate
  fHis2->Fill(value2);
  if(fconHis1->Test(value1))fHis1gate->Fill(value1); //fill histograms with gate
  if(fconHis2->Test(value2))fHis2gate->Fill(value2);
  // fill Cr1Ch1x2 for three polygons:
  if(fPolyCon->Test(value1,value2))        fCr1Ch1x2->Fill(value1,value2);
  if(((*fConArr)[0])->Test(value1,value2)) fCr1Ch1x2->Fill(value1,value2);
  if(((*fConArr)[1])->Test(value1,value2)) fCr1Ch1x2->Fill(value1,value2);
#endif

  return kTRUE;
}

//----------------------------END OF GO4 SOURCE FILE ---------------------