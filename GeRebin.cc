//#include <iostream>
//#include <cstdio>
//#include <stdexcept>
#include "Spectrum.h"
#include <stdexcept>
#include <iostream>
#include "GeSpcAnalysisManager.hh"
using GeSpcAnalysis::GeSpcAnalysisManager;
int main(const int argc,const char *argv[]) {

	 try {
		  GeSpcAnalysisManager::GetInstance()->ReadOptions(argc,argv);
		  GeSpcAnalysisManager::GetInstance()->LoadRawSpectrum();
	 } catch (std::exception &ex) {
		  std::cerr<<"Exception: "<<ex.what()<<std::endl;
		  exit(1);
	 }

	 Spectrum *spc = new Spectrum(1*Spectrum::keV);

	 spc->SetSpectrum(GeSpcAnalysisManager::GetInstance()->GetSpectrum());
	 spc->SetCalibration(GeSpcAnalysisManager::GetInstance()->GetCalibration());
	 spc->Convert("ConvertToOutN");
	 spc->Show();
	 spc->Write(GeSpcAnalysisManager::GetInstance()->GetOutputName());
	 spc->WriteROOT(GeSpcAnalysisManager::GetInstance()->GetOutputROOTName());

	 return 0;
}

