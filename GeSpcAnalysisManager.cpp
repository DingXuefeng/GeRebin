/***************************************************************************
 * Author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
 * Date: 2017.07.22
 * Copyrighted, all rights reserved
 * ***********************************************************************/
#include "stdafx.h"

#include "GeSpcAnalysisManager.h"

#include <iostream>
using GeSpcAnalysis::GeSpcAnalysisManager;
using std::cout;
using std::endl;

GeSpcAnalysisManager *GeSpcAnalysisManager::fGeSpcAnalysisManager = 0L;

GeSpcAnalysisManager::GeSpcAnalysisManager() : m_current_file(-1) { }

GeSpcAnalysisManager *GeSpcAnalysisManager::GetInstance() {
  if(!fGeSpcAnalysisManager) fGeSpcAnalysisManager = new GeSpcAnalysisManager;
  return fGeSpcAnalysisManager;
}

#include <exception>
using std::runtime_error;
#include <fstream>
using std::ifstream;
using std::getline;
void GeSpcAnalysisManager::ShowHelp() const {
  cout<<"Program for rebinning of Germanium detector spectrum"<<endl;
  cout<<"Usage 1: ./process -s <input file> [config] [outputfile]"<<endl;
  cout<<"If the config file not specified, the output histogram will be from 0.5 keV to 3000.5 keV, and 1 keV per bin."<<endl;
  cout<<"If the config file is specified, it should contain three numbers: E_bin, E_start and E_end."<<endl;
  cout<<"If the output file not specified, it will be <inputfilename>_rebin_[E_bin]_[E_start]_[E_end].dat"<<endl;
  cout<<"Usage 2: ./process -m <file> [configfile]"<<endl;
  cout<<"             each line of the file should contain <input file> [outputfile]"<<endl;
  cout<<"Usage 3: ./process -h"<<endl;
  std::abort();
}
bool GeSpcAnalysisManager::ReadOptions(const int argc, const char *argv[]) {
  if(std::string(argv[1])=="-s") {
    if(argc<3) ShowHelp();
    inputname.push_back(argv[2]);
    if(argc>3) LoadConfig(argv[3]);
    if(argc>4) outputname.push_back(argv[3]);
  } else if(std::string(argv[1])=="-m") {
    if(argc<3) ShowHelp();
    LoadTasks(argv[2]);
    if(argc>3) LoadConfig(argv[3]);
  } else if(std::string(argv[1])=="-h") {
    ShowHelp();
  }
  return true;
}

void GeSpcAnalysisManager::LoadConfig(const std::string &configname) {
  std::ifstream config_f;
  config_f.open(configname);
  if(!config_f.is_open()) { std::cerr<<"Cannot open config file: <"<<configname<<">"<<std::endl; }
  config_f >> config.E_bin >> config.E_start >> config.E_end ;
  if(config_f.fail())  { 
    std::cerr<<"Illegal format in config file! You should put three numbers (unit in keV):\nE_bin E_start E_end\nin <"<<configname<<">"<<std::endl; 
    throw std::runtime_error("Wrong format");
  }
  config_f.close();
}
bool GeSpcAnalysisManager::LoadRawSpectrum() {
  return true;
//	if(m_current_file==m_input_f_v.size()) 
//		throw runtime_error("All spectrums are loaded.");
//	return ParseRawDataFile(m_input_f_v.at(++m_current_file),m_raw_spc,m_res_par);
}

