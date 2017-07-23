/***************************************************************************
 * Author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
 * Date: 2017.07.22
 * Copyrighted, all rights reserved
 * ***********************************************************************/
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
#include "GeMPILoader.h"
using GeSpcAnalysis::GeSpcAnalysisManager;
#ifdef _WIN32
#define __PAUSE__ system("PAUSE");
#else
#define __PAUSE__ ;
#endif
int main(const int argc,const char *argv[]) {

  try {
    GeSpcAnalysisManager::GetInstance()->ReadOptions(argc,argv);
    GeSpcAnalysisManager::GetInstance()->SetLoader(new GeMPILoader);
  } catch (std::exception &ex) {
    std::cerr<<"Exception: "<<ex.what()<<std::endl;
    __PAUSE__;
    exit(1);
  }


  GeSpcAnalysisManager::GetInstance()->Dump();

  try {
    while(GeSpcAnalysisManager::GetInstance()->Next()) {
      GeSpcAnalysisManager::GetInstance()->ProcessCurrentTask();
    }
  } catch (std::exception &ex) {
    std::cerr<<"Exception: "<<ex.what()<<std::endl;
    __PAUSE__;
    exit(1);
  }
  __PAUSE__;

  return 0;
}

