// $Id: TSCNUnpackProc.h 755 2011-05-20 08:04:11Z linev $
//-----------------------------------------------------------------------
//       The GSI Online Offline Object Oriented (Go4) Project
//         Experiment Data Processing at EE department, GSI
//-----------------------------------------------------------------------
// Copyright (C) 2000- GSI Helmholtzzentrum für Schwerionenforschung GmbH
//                     Planckstr. 1, 64291 Darmstadt, Germany
// Contact:            http://go4.gsi.de
//-----------------------------------------------------------------------
// This software can be used under the license agreements as stated
// in Go4License.txt file which is part of the distribution.
//-----------------------------------------------------------------------

#ifndef FIRST_TEST_H
#define FIRST_TEST_H

#include "Riostream.h"

// Root Includes //
#include "TROOT.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TCutG.h"
#include "TArc.h"
#include "TTree.h"

#include <time.h>
#include <math.h>
#include <iomanip>

// Go4 Includes //
#include "TGo4UserException.h"
#include "TGo4Picture.h"
#include "TGo4MbsEvent.h"

// General Includes //
#include <fstream>
#include <vector>
#include <time.h>
#include <math.h>
#include <iomanip>




#include "TSCNUnpackEvent.h"

#include "Detector_System.cxx"
#include "FATIMA_Detector_System.h"
#include "PLASTIC_Detector_System.h"
#include "Data_Stream.cxx"
#include "White_Rabbit.h"

#include "PLASTIC_Data_Stream.h"
#include "FATIMA_Data_Stream.h"

#include <string>


using namespace std;

#include "TGo4EventProcessor.h"

	class TSCNUnpackEvent;

	#include "TSCNUnpackEvent.h"

	class TSCNUnpackProc : public TGo4EventProcessor
	{
		public:
			TSCNUnpackProc();
			TSCNUnpackProc(const char* name);
			virtual ~TSCNUnpackProc();

			virtual Bool_t BuildEvent(TGo4EventElement* dest);

		protected:
		
	
			TH1* FAT_E;
			TH2* FAT_MAT;
			TH2* hit_mat;

			
			//for the SIS modules

		private:
			Bool_t ffill;
			Int_t fshift;
			ULong64_t White_Rabbbit_old;

			Int_t PrcID_Array[10];
			bool Used_Systems[10];

			Detector_System** Detector_Systems;
			Data_Stream** data_stream;
			White_Rabbit* WR;

			Int_t get_Conversion(Int_t);
			void get_used_Systems();

			void load_PrcID_File();

			int count;

			ClassDef(TSCNUnpackProc,1)
	};

#endif //Analysis_v5_H



