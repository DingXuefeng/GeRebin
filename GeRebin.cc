//#include <iostream>
//#include <cstdio>
//#include <stdexcept>
//#include "Spectrum.h"
#include <stdexcept>
#include <iostream>
#include "GeSpcAnalysisManager.hh"
using GeSpcAnalysis::GeSpcAnalysisManager;
int main(const int argc,const char *argv[]) {

	 try {
		  GeSpcAnalysisManager::GetInstance()->ReadOptions(argc,argv);
	 } catch (std::exception &ex) {
		  std::cerr<<"Exception: "<<ex.what()<<std::endl;
		  exit(1);
	 }

	 /*
		 Spectrum *spc = new Spectrum;

		 spc->LoadSpectrum(input_f); 
		 spc->LoadCalibration(input_f); // model: "[0]+[1]*x+[2]*x^2"
		 spc->SetOutput(atoi(argv[3]));
		 spc->Convert();
		 spc->Show();
		 spc->Write(output_f);
		 */

	 return 0;
}

void ParseRawDataFile(const char *raw_file, unsigned int const Nhit[], double const calibration_par[3]) {
//	 FILE *in = ::fopen(raw_file,"read");
//	 if(!in) {
//		  std::cout<<"File ["<<raw_file<<"] not found."<<std::endl;
//		  throw std::run_timeerror("File not found.");
//	 }
//	 /****************************************************************************
//	  * The typical structure of input file xxx.spe:
//	  * $SPEC_ID:
//	  * GeMPI4, bg, NOV-2012, w shield, w sample support plate
//	  * $SPEC_REM:
//	  * DET# 27
//	  * DETDESC# GeMPI4 Detector
//	  * AP# GammaVision Version 6.08
//	  * $DATE_MEA:
//	  * 11/05/2012 12:09:33
//	  * $MEAS_TIM:
//	  * 2806052 2806052
//	  * $DATA:
//	  * 0 8191
//	  *        0
//	  *        0
//	  *        ...
//	  *        0
//	  * $ROI:
//	  * 0
//	  * $PRESETS:
//	  * None
//	  * 0
//	  * 0
//	  * $ENER_FIT:
//	  * -15.214184 0.471975
//	  * $MCA_CAL:
//	  * 3
//	  * -1.5214184E+001 4.719750E-001 0.000000E+000 keV
//	  * $SHAPE_CAL:
//	  * 3
//	  * 1.158641E+001 -2.875630E-003 0.000000E+000
//	  ****************************************************************************/
//	 // get entry of in_N
//	 int in_N_begin,in_N_end; {
//		  // skip the head description part
//		  while(true) {
//				char buff[255];
//				fscanf(in,"%s",buff);
//				in.getline(buff,255);
//				TString buff_tstr(buff);
//				if(buff_tstr.Contains("DATA")) break;
//		  }
//		  in.getline(buff,255);
//		  stringstream buff_s;
//		  buff_s<<buff;
//		  buff_s>>in_N_begin>>in_N_end; 
//	 }
//	 // get the entries for each channel
//	 /* Nhit */ for(int i = 1;i<= in_N_end; ++i) {
//		  char buff[255];
//		  in.getline(buff,255);
//		  stringstream buff_s;
//		  int n_i;
//		  buff_s<<buff;
//		  buff_s>>n_i;
//		  in_N->SetBinContent(i,n_i);
//	 }
//	 ::fclose(in);
}
