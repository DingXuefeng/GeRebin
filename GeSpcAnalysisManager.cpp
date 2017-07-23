/***************************************************************************
 * Author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
 * Date: 2017.07.22
 * Copyrighted, all rights reserved
 * ***********************************************************************/
#include "stdafx.h"

#include "GeSpcAnalysisManager.h"
#include "FileLoader.h"
#include "Spectrum.h"

#include <iostream>
using GeSpcAnalysis::GeSpcAnalysisManager;

GeSpcAnalysisManager *GeSpcAnalysisManager::fGeSpcAnalysisManager = 0L;

GeSpcAnalysisManager::GeSpcAnalysisManager() : 
  m_current_task(-1) , inputname(), outputname(), config ( 1.,0.5,3000.5 )
{ }

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
  std::cout<<"Program for rebinning of Germanium detector spectrum"<<std::endl;
  std::cout<<"Usage 1: ./process -s <input file> [outputfile] [configfile]"<<std::endl;
  std::cout<<"If the output file not specified, it will be <inputfilename>_rebin_[E_bin]_[E_start]_[E_end].dat."<<std::endl;
  std::cout<<"If the config file is specified, it should contain three numbers: E_bin, E_start and E_end;"<<std::endl;
  std::cout<<"     Otherwise, the output histogram will be from 0.5 keV to 3000.5 keV, and 1 keV per bin."<<std::endl;
  std::cout<<"Usage 2: ./process -m <file> [configfile]"<<std::endl;
  std::cout<<"             each line of the file should contain <input file> [output file]"<<std::endl;
  std::cout<<"Usage 3: ./process -h"<<std::endl;
  std::abort();
}
bool GeSpcAnalysisManager::ReadOptions(const int argc, const char *argv[]) {
  if(argc>1 && std::string(argv[1])=="-s") {
    if(argc<3) ShowHelp();
    inputname.push_back(argv[2]);
    if(argc>3) outputname.push_back(argv[3]); else outputname.push_back("");
    if(argc>4) LoadConfig(argv[4]);
  } else if(argc>1 && std::string(argv[1])=="-m") {
    if(argc<3) ShowHelp();
    LoadTasks(argv[2]);
    if(argc>3) LoadConfig(argv[3]);
  } else {
    ShowHelp();
  }
  spc = new Spectrum(config.E_bin);
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
void GeSpcAnalysisManager::LoadTasks(const std::string &tasklists) {
  std::ifstream task_f;
  task_f.open(tasklists);
  if(!task_f) {
    std::cerr<<"Cannot open task lists ["<<tasklists<<"]"<<std::endl;
    throw std::runtime_error("Cannot open tasklists");
  }
  while(true) {
    std::string tmp;
    getline(task_f,tmp);
    if(task_f.fail()) break;
    std::vector<std::string> keys;
    FileLoader::Split(tmp,keys);
    if(keys.size()!=1 && keys.size()!=2) { std::cerr<<"Wrong format in task list. Put <input file> [output file] please"<<std::endl; throw std::runtime_error("Wrong format"); }
    inputname.push_back(keys[0]);
    if(keys.size()==2) outputname.push_back(keys[1]); else outputname.push_back("");
  }
}
void GeSpcAnalysisManager::Dump() const {
  std::cout<<"GeSpcAnalysisManager"<<std::endl;
  std::cout<<"Config (keV): bin ["<<config.E_bin<<"] start ["<<config.E_start<<"] end ["<<config.E_end<<"]"<<std::endl;
  std::cout<<"Tasks:"<<std::endl;
  for(int i = 0;i<inputname.size();++i) {
    std::cout<<"["<<inputname[i]<<"] => ["<<outputname[i]<<"]"<<std::endl;
  }
}

void GeSpcAnalysisManager::ProcessCurrentTask() {
  std::cout<<"processing ["<<inputname[m_current_task]<<"] => ["<<outputname[m_current_task]<<"]"<<std::endl;
  spc->Reset();
  spc->SetLoader(loader);
  spc->LoadRawSpectrum(inputname[m_current_task]);
  // "ConvertToOutN" or "FillOutN"
  spc->Convert("FillOutN");
  spc->Write(outputname[m_current_task]);
}
