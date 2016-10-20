#include <iostream>

#include "GeSpcAnalysisManager.hh"
#include "EqualBinning.hh"
#include "UniformDithering.hh"

using GeSpcAnalysis::GeSpcAnalysisManager;

GeSpcAnalysisManager *GeSpcAnalysisManager::fGeSpcAnalysisManager = nullptr;

GeSpcAnalysisManager::GeSpcAnalysisManager() :
	 m_mask_corr(false), m_dithering(false),
	 m_dithering_generator(nullptr), m_binning_manager(nullptr) {
}

GeSpcAnalysisManager *GeSpcAnalysisManager::GetInstance() {
	 if(!fGeSpcAnalysisManager) fGeSpcAnalysisManager = new GeSpcAnalysisManager;
	 return fGeSpcAnalysisManager;
}

bool GeSpcAnalysisManager::ReadOptions(const int argc, const char *argv[]) {
	 if((argc>1)&&(argc<5)) {
		  m_input_f = argv[1];
		  if(argc>2) {
				m_output_f = argv[2]; // otherwise 
				if(argc>3)
					 m_config_f = argv[3]; // otherwise use internal config
		  }
	 } else {
		  std::cout<<"Program for rebinning of Germanium detector spectrum"<<std::endl;
		  std::cout<<"Usage: ./process <inputfilename> [outputfilename] [bin.cfg]"<<std::endl;
		  std::cout<<"If the output file not specified, it will be <inputfilename>_rebin_[bin_n]_[start_bin]_[end_bin].dat"<<std::endl;
		  std::cout<<"If the config file not specified, the output histogram will be from 0.5 keV to 3000.5 keV, and 1 keV per bin."<<std::endl;
		  std::cout<<"Check the example configfile for all configs."<<std::endl;
		  return false;
	 }
	 return ParseOptions();
}

//bool GeSpcAnalysisManager::Fill() {
//	 MakePdf();
//	 m_binning_manager->SetBinEdge(m_Nbins,m_binEdge);
//	 for(int bin = 1; bin<= m_Nbins; ++bin)
//		  m_dithering_manager->Fill(bin,m_binEdge[bin-1],m_binEdge[bin],m_binContent[bin],m_binError[bin]);
//	 m_mask_manager->CorrectMaskEffect(m_Nbins,m_binContent,m_binError);
//}
//
bool GeSpcAnalysisManager::ParseOptions() {
	 if(m_config_f=="") { // default options
		  /*
		  m_binning_manager = new GeSpcAnalysis::EqualBinning;
		  m_dithering_manager = new GeSpcAnalysis::NoDithering;
		  m_mask_manager = new GeSpcAnalysis::NoMask;
		  */
//		  m_pdf_manager = new GeSpcAnalysis::UniformInsideBinPdf;
		  MakePdf();
//		  m_binning_manager = new GeSpcAnalysis::OriginalBinning;
//		  m_dithering_generator = new GeSpcAnalysis::UniformDithering;
//		  m_mask_manager = new GeSpcAnalysis::PoissonMask;
//		  m_save_manager = new GeSpcAnalysis::AscIISaver;
		  m_outputName = "output.root";
	 } else {
		  exit(1);
	 }
	 return true;
}
//
//bool GeSpcAnalysisManager::Save() {
//	 m_save_manager->Save(m_Nbins,m_binContent,m_binError,m_outputName);
//}
//
bool GeSpcAnalysisManager::MakePdf() {
	return true;
}
