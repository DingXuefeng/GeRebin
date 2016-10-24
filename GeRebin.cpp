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
int main(const int argc,const char *argv[]) {

	 try {
		  GeSpcAnalysisManager::GetInstance()->ReadOptions(argc,argv);
		  
	 } catch (std::exception &ex) {
		  std::cerr<<"Exception: "<<ex.what()<<std::endl;
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
		 spc->Show();
		 spc->Write(GeSpcAnalysisManager::GetInstance()->GetOutputName());
		 }
	 } catch (std::exception &ex) {
		 std::cerr<<"Exception: "<<ex.what()<<std::endl;
		 std::cin.get();
		 return -1;
	 }

	 return 0;
}

