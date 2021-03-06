#include "PLASTIC_Calibrator.h"

using namespace std;

//---------------------------------------------------------------

PLASTIC_Calibrator::PLASTIC_Calibrator(bool ONLINE){

	this->ONLINE = ONLINE;


	am_bin = 1000;

	//only for ONLINE CALIBRATION
	if(ONLINE){
		fired = new bool*[100];
		for(int i = 0;i < 100;++i){
			fired[i] = new bool[100];
			for(int j = 0;j < 100;++j) fired[i][j] = false;
		}

		char tmp_name[1000];

		Fine_Hist = new TH1**[100];
		for(int i = 0;i < 100;++i){
			Fine_Hist[i] = new TH1*[100];
			for(int j = 0;j < 100;++j){
				sprintf(tmp_name,"PDF_%d_%d",i,j);
				Fine_Hist[i][j] = new TH1(tmp_name,tmp_name,nbins,min_val,max_val);
			}
		}
	}
	else{
		bins_x_arr = new double[nbins];

		fired = NULL;
		Fine_Hist = NULL;
		load_Calibration_Files();
	}
}

//---------------------------------------------------------------

PLASTIC_Calibrator::~PLASTIC_Calibrator(){
	if(ONLINE){
		for(int i = 0;i < 100;++i){
			for(int j = 0;j < 100;++j) delete Fine_Hist[i][j];
			delete[] fired[i];
			delete[] Fine_Hist[i];
		}
		delete[] Fine_Hist;
		delete[] fired;
	}
	else{
		for(int i = 0;i < iter;++i){
			for(int j = 0;j < 100;++j) if(Cal_arr[i][j]) delete[] Cal_arr[i][j];
			delete[] Cal_arr[i];
		}
		delete[] Cal_arr;
		delete[] bins_x_arr;
	}
}

//---------------------------------------------------------------

void PLASTIC_Calibrator::load_Calibration_Files(){
	
	//load calibration map for used ids
	ifstream map_file("Configuration_Files/Calibration_PLASTIC/MAP.dat");

	string line;
	
	if(map_file.fail()){
		cerr << "Could not find PLASTIC Calibration file MAP" << endl;
		exit(0);
	}
	
	const char* format_MAP = "%d %d";

	int tamex_id,ch_id;

	int used_ids[100][100];
	iter = 0;

	while(map_file.good()){
		getline(map_file,line,'\n');
		if(line[0] == '#') continue;
		sscanf(line.c_str(),format_MAP,&tamex_id,&ch_id);
		used_ids[iter][0] = tamex_id;
		used_ids[iter][1] = ch_id;

		wired_tamex_ch[tamex_id][ch_id] = true;

		iter++;
	}

	//Load all wired Calibration files specified by MAP
	char filename[1000];
	ifstream file;

	int b_iter = 0;
	double bin,val;

	Cal_arr = new double**[iter];

	bool first = true;

	for(int i = 0;i < iter;++i){

		Cal_arr[i] = new double*[100];

		b_iter = 0;

		tamex_id = used_ids[i][0];
		ch_id = used_ids[i][1];

		Cal_arr[tamex_id][ch_id] = new double[nbins];

		sprintf(filename,"Configuration_Files/Calibration_PLASTIC/Calib_%d_%d.dat",tamex_id,ch_id);
		file.open(filename);

		if(file.fail()){
			cerr << "Could not find PLASTIC Calibration file " << tamex_id << " " << ch_id << endl;
			exit(0);
		}
		while(file.good()){
			getline(file,line,'\n');
			if(line[0] == '#') continue;
			sscanf(line.c_str(),format,&bin,&val);
			if(first) bins_x_arr[b_iter] = bin;
			Cal_arr[tamex_id][ch_id][b_iter] = val;

			b_iter++;
		}

		first = false;

		file.close();
		file.clear();
	}

}

//---------------------------------------------------------------
//DO IT!
void PLASTIC_Calibrator::OFFLINE_CALIBRATION(ULong* fine_T,int tamex_id,int ch_id){

}

//---------------------------------------------------------------

void PLASTIC_Calibrator::get_data(ULong*** fine_T,int*** ch_id,int tamex_iter,int* iterator){
	//write into corresponding root histograms
	for(int i = 0;i < tamex_iter;++i){
		for(int j = 0;j < iterator[i];++j) Fine_Hist[i][ch_id[i][j][0]]->Fill(fine_T[i][j][0]);
		fired[i][ch_id[i][j][0]] = true;
	}
}

//---------------------------------------------------------------

void PLASTIC_Calibrator::ONLINE_CALIBRATION(){

	//Root file to check histograms
	TFile* ROOT_FILE = new TFile("Root_Trees/PLASTIC_TREE.root","RECREATE");

	//output file file stream
	ofstream cal_file;

	cout << "ONLINE CALIBRATION FOR PLASTIC INITIALIZED" << endl;
	
	//pdf and cdf for "perfect" binning
	//pdf: probability distribution funtion, cdf: cummulative distribution function
	double density_perfect = 1./((double) nbins);
	double perfect[nbins];
	for(int i = 0;i < nbins;++i) perfect[i] = (i+1)*density_perfect;

	char filename[1000];
	
	//create sum spectra
	double sum_arr[nbins];
	double full_sum = 0;
	cout << "Running... ";
	cout.flush();

	//loop over all tamex modules and their respective channels
	for(int i = 0;i < 100;++i){
		for(int j = 0;j < 100;++j){
			//only if channel fired
			if(fired[i][j]){

				//add pdf and cdf to root file
				ROOT_FILE->Add(Fine_Hist[i][j]);
				ROOT_FILE->Add(Fine_Hist[i][j]->GetCumulative());

				sprintf(filename,"Configuration_Files/Calibration_PLASTIC/Calib_%d_%d.dat",i,j);
				cal_file.open(filename);

				cal_file << "# Calibration file of tamex_id " << i << " @ channel " << j << endl;

				//calculating cdf for "real" case
				sum_arr[0] = Fine_Hist[i][j]->GetBinContent(1);
				for(int k = 1;k < nbins;++k) sum_arr[k] = Fine_Hist[i][j]->GetBinContent(k+1) + sum_arr[k-1];
				
				//normalize cdf to 1 and calculate difference to perfect scenario
				//write everything into calibration file (name of file = filename)
				full_sum = sum_arr[nbins-1];
				for(int k = 0;k < nbins;++k) cal_file << bins_x[k] << " " <<  sum_arr[k]/full_sum - perfect[k] << endl;
				
				cal_file.close();
				cal_file.clear();
			}
		}
		cout << "\r";
		cout << "Running... " << i << " % \t\t\t\t";
		cout.flush();
	}
	cout << "\r";
	cout << "Running... DONE" << endl;
	cout << "\n-> Exiting program after calibration <-\n";
	cout << "-> PLEASE SWITCH BACK TO OFFLINE CALIBRATION <-" << endl;

	ROOT_FILE->Write();
	ROOT_FILE->Close();
}
