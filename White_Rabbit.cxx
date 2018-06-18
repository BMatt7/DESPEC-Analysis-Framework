#include "White_Rabbit.h"

using namespace std;

//---------------------------------------------------------------

White_Rabbit::White_Rabbit(){
    load_config_file();
}

//---------------------------------------------------------------

White_Rabbit::~White_Rabbit(){}

//---------------------------------------------------------------


void White_Rabbit::load_config_file(){
    
    ifstream config_file("Configuration_Files/White_Rabbit_Map.dat");
    if(config_file.fail()){
        cerr << "Could not find White_rabbit map!" << endl;
        //exit(0);
    }

    //load file (and skip header)
    string line;
    while(config_file.good()){
        getline(config_file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format,&DETECTORS[0],&DETECTORS[1]
                           ,&DETECTORS[2],&DETECTORS[3],&DETECTORS[4]);
    }
}


//---------------------------------------------------------------

void White_Rabbit::set_triggered_detector(int WR_d){

    //check for id of detector
    for(int i = 0;i < 5;++i){
        if(WR_d == DETECTORS[i]){
            detector_id = i;
            return;
        }
    }
    //unknown detector id in white rabbit header
    cerr << "White Rabbit ID of detector " << WR_d << " not known!" << endl;
    exit(0);
}

//---------------------------------------------------------------

void White_Rabbit::process_White_Rabbit(int* pdata){

    //check for White Rabbit header in pdata 
    WR_Header *wrh = (WR_Header*) pdata;
    int WR_d = wrh->check_wr;
    set_triggered_detector(WR_d);

    //calculate White Rabbit Time stamp
    double white_rabbit[4] = {0,0,0,0};
    for(int i = 0;i < 4;++i){
        pdata++;
        WR_Data* wrd = (WR_Data*) pdata;
        white_rabbit[i] = wrd->timestamp;
    }

    WR_Time = ((ULong64_t) white_rabbit[0])
            + (((ULong64_t) white_rabbit[1]) << 16)
            + (((ULong64_t) white_rabbit[2]) << 32)
            + (((ULong64_t) white_rabbit[3]) << 48);
}

//---------------------------------------------------------------

ULong64_t White_Rabbit::get_White_Rabbit(int* pdata){
    process_White_Rabbit(pdata);
    return WR_Time;
}

//---------------------------------------------------------------

int White_Rabbit::get_Detector_id(){return detector_id;}

//---------------------------------------------------------------
