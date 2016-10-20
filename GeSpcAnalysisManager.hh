#ifndef GeSpcAnalysisManager_H
#define GeSpcAnalysisManager_H
#include <string>
#include <vector>
#include <iostream>
using std::vector;
using std::string;
using std::istream;
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
				bool LoadRawSpectrum();
				vector<unsigned int> &GetSpectrum() { return m_raw_spc; };
				vector<double> &GetCalibration() { return m_res_par; };

		  private:
				bool FindFlag(istream &in,const string &flag);
				bool ParseRawDataFile(string raw_fname, vector<unsigned int> &raw_spc,vector<double> &res_par);
		  private:
				std::string m_input_f;
		  private:
				vector<unsigned int> m_raw_spc;
				vector<double> m_res_par;
	 };
};
#endif
