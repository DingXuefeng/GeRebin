#include <iostream>

#include "GeSpcAnalysisManager.hh"
#include "GeSpcAnalysisManager.hh"
using GeSpcAnalysis::GeSpcAnalysisManager;
using std::cout;
using std::endl;

GeSpcAnalysisManager *GeSpcAnalysisManager::fGeSpcAnalysisManager = nullptr;

GeSpcAnalysisManager::GeSpcAnalysisManager() {
}

GeSpcAnalysisManager *GeSpcAnalysisManager::GetInstance() {
	if(!fGeSpcAnalysisManager) fGeSpcAnalysisManager = new GeSpcAnalysisManager;
	return fGeSpcAnalysisManager;
}

#include <exception>
using std::runtime_error;
bool GeSpcAnalysisManager::ReadOptions(const int argc, const char *argv[]) {
	if(argc==3) {
		m_input_f = argv[1];
		m_output_f = argv[2];
	} else {
		cout<<"Program for rebinning of Germanium detector spectrum"<<endl;
		cout<<"Usage: ./process <inputfilename> <outputfilename>"<<endl;
		cout<<"If the output file not specified, it will be <inputfilename>_rebin_[bin_n]_[start_bin]_[end_bin].dat"<<endl;
		cout<<"If the config file not specified, the output histogram will be from 0.5 keV to 3000.5 keV, and 1 keV per bin."<<endl;
		cout<<"Check the example configfile for all configs."<<endl;
		throw runtime_error("not good arguments");
	}
	return true;
}

bool GeSpcAnalysisManager::LoadRawSpectrum() {
	return ParseRawDataFile(m_input_f,m_raw_spc,m_res_par);
}

#include <fstream>
#include <sstream>
using std::ifstream;
using std::stringstream;
bool GeSpcAnalysisManager::FindFlag(istream &in,const string &flag) {
	// find $MCA_CAL flag
	while(true) {
		string tmp;
		getline(in,tmp);
		if(!tmp.compare(0, flag.length(), flag))
			break; // found
		if(in.fail()) {
			stringstream fatal;
			fatal<<"["<<flag<<"] flag not found."<<endl;
			throw runtime_error(fatal.str());
		}
	}
	return true;
}

bool GeSpcAnalysisManager::ParseRawDataFile(string raw_fname, vector<unsigned int> &raw_spc,vector<double> &res_par) {
	// currently put this method in the GeSpcAnalysisManager
	ifstream rawspc_f;
	rawspc_f.open(raw_fname.c_str());
	if(!rawspc_f.is_open()) {
		stringstream fatal;
		fatal<<"file ["<<raw_fname<<"] not found"<<endl;
		throw runtime_error(fatal.str());
	}
	/****************************************************************************
	 * The typical structure of input file xxx.spe:
	 * $SPEC_ID:
	 * GeMPI4, bg, NOV-2012, w shield, w sample support plate
	 * $SPEC_REM:
	 * DET# 27
	 * DETDESC# GeMPI4 Detector
	 * AP# GammaVision Version 6.08
	 * $DATE_MEA:
	 * 11/05/2012 12:09:33
	 * $MEAS_TIM:
	 * 2806052 2806052
	 * $DATA:
	 * 0 8191
	 *        0
	 *        0
	 *        ...
	 *        0
	 * $ROI:
	 * 0
	 * $PRESETS:
	 * None
	 * 0
	 * 0
	 * $ENER_FIT:
	 * -15.214184 0.471975
	 * $MCA_CAL:
	 * 3
	 * -1.5214184E+001 4.719750E-001 0.000000E+000 keV
	 * $SHAPE_CAL:
	 * 3
	 * 1.158641E+001 -2.875630E-003 0.000000E+000
	 ****************************************************************************/
	{
		// get entry of in_N
		int in_N_begin,in_N_end; {
			// skip the head description part
			FindFlag(rawspc_f,"$DATA:");
			rawspc_f>>in_N_begin>>in_N_end;
		}
		// get the entries for each channel
		for(int i = in_N_begin;i< in_N_end; ++i) {
			unsigned int n;
			rawspc_f>>n;
			raw_spc.push_back(n);
		}
		if(rawspc_f.fail()) {
			stringstream fatal;
			fatal<<"In correct format in ["<<raw_fname<<"]"<<endl;
			throw runtime_error(fatal.str());
		}
	}
	{
		// find [$MCA_CAL:] flag
		FindFlag(rawspc_f,"$MCA_CAL:");
		int npar;
		rawspc_f>>npar;
		for(int i = 0;i<npar;++i) {
			double par;
			rawspc_f>>par;
			res_par.push_back(par);
		}
		if(rawspc_f.fail()) {
			stringstream fatal;
			fatal<<"In correct format in ["<<raw_fname<<"]"<<endl;
			throw runtime_error(fatal.str());
		}
		rawspc_f.close();
		return true;
	}
}
//string GeSpcAnalysisManager::GetOutputROOTName() {
//	stringstream outputRootName;
//	outputRootName<<m_output_f<<".root";
//	return outputRootName.str();
//}
