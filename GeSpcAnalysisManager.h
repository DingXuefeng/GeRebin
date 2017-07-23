/***************************************************************************
 * Author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
 * Date: 2017.07.22
 * Copyrighted, all rights reserved
 * ***********************************************************************/
#ifndef GeSpcAnalysisManager_H
#define GeSpcAnalysisManager_H

#include "stdafx.h"

#include <string>
#include <vector>
#include <iostream>
using std::vector;
using std::string;
using std::istream;
#include "Config.h"
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
				//string &GetOutputName() { return m_output_f_v.at(m_current_file); };
				//bool HasNext() const { return m_current_file+1<m_input_f_v.size(); } 
                void ShowHelp() const;
				
		  private:
                void LoadConfig(const std::string &configname);
				bool FindFlag(istream &in,const string &flag);
				bool ParseRawDataFile(string raw_fname, vector<unsigned int> &raw_spc,vector<double> &res_par);
                void LoadTasks(const std::string &tasklists) {}
		  private:
			    unsigned int m_current_file;
				vector<string> inputname;
				vector<string> outputname;
                Config config;
				
		  private:
				vector<unsigned int> m_raw_spc;
				vector<double> m_res_par;
	 };
};
#endif
