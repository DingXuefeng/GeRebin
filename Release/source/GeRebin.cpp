// GeRebin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <iostream>
//#include <cstdio>
//#include <stdexcept>
#include "Spectrum.h"
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include "GeSpcAnalysisManager.h"
using GeSpcAnalysis::GeSpcAnalysisManager;
#ifdef _WIN32
#define __PAUSE__ system("PAUSE");
#else
#define __PAUSE__ ;
#endif
int main(const int argc,const char *argv[]) {

	 try {
		  GeSpcAnalysisManager::GetInstance()->ReadOptions(argc,argv);
	 } catch (std::exception &ex) {
		  std::cerr<<"Exception: "<<ex.what()<<std::endl;
		  __PAUSE__;
		  exit(1);
	 }

	 try {
		 Spectrum *spc = new Spectrum(1*Spectrum::keV);
		 while(GeSpcAnalysisManager::GetInstance()->HasNext()) {
			 GeSpcAnalysisManager::GetInstance()->LoadRawSpectrum();
			 spc->SetSpectrum(GeSpcAnalysisManager::GetInstance()->GetSpectrum());
			 spc->SetCalibration(GeSpcAnalysisManager::GetInstance()->GetCalibration());
			 // "ConvertToOutN" or "FillOutN"
			 spc->Convert("FillOutN");
			 spc->Write(GeSpcAnalysisManager::GetInstance()->GetOutputName());
		 }
	 } catch (std::exception &ex) {
		 std::cerr<<"Exception: "<<ex.what()<<std::endl;
		 __PAUSE__;
		 exit(1);
	 }
	 __PAUSE__;

	 return 0;
}

