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
class FileLoader;
class Spectrum;
namespace GeSpcAnalysis {
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
      void SetLoader(FileLoader *loader_) { loader = loader_; }
      bool Next() { return (++m_current_task)!=inputname.size(); } 
      void ProcessCurrentTask();
      void ShowHelp() const;
      void Dump() const;

    private:
      void LoadConfig(const std::string &configname);
      bool FindFlag(istream &in,const string &flag);
      bool ParseRawDataFile(string raw_fname, vector<unsigned int> &raw_spc,vector<double> &res_par);
      void LoadTasks(const std::string &tasklists);
    private:
      unsigned int m_current_task;
      vector<string> inputname;
      vector<string> outputname;
      Config config;
      FileLoader *loader = nullptr;
      Spectrum *spc = nullptr;
  };
};
#endif
