#ifndef GeSpcAnalysisManager_H
#define GeSpcAnalysisManager_H
#include <string>
#include <vector>
using std::vector;
namespace GeSpcAnalysis {
	 class IDithering;
	 class IBinning;
	 class IMask;
	 class ISave;
	 class IPdf;
	 class GeSpcAnalysisManager {
		  public:
				static GeSpcAnalysisManager* GetInstance();
		  private:
				//! Singlton, new are forbiden.
				GeSpcAnalysisManager();
				static GeSpcAnalysisManager* fGeSpcAnalysisManager;

		  public:
				bool ReadOptions(const int argc, const char *argv[]);
				bool Fill();
				bool Save();

		  private:
				bool ParseOptions();
				bool MakePdf();
		  private:
				std::string m_input_f;
				std::string m_output_f;
				std::string m_config_f;
				std::string m_outputName;
				IDithering *m_dithering_generator;
				IBinning *m_binning_manager;
//				IMask *m_mask_manager;
//				ISave *m_save_manager;
				bool m_mask_corr;
				bool m_dithering;
		  private:
//				IPdf *m_pdf;
				int m_Nbins;
				vector<double> m_binEdge;
				vector<double> m_binContent;
				vector<double> m_binError; // error of content
	 };
};
#endif
