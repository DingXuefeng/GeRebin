/***************************************************************************
 * Author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
 * Date: 2017.07.22
 * Copyrighted, all rights reserved
 * ***********************************************************************/
#include "GeMPILoader.h"
#include <fstream>
#include <iostream>
void GeMPILoader::ParseRawDataFile(const std::string &raw_fname) {
  // currently put this method in the GeSpcAnalysisManager
  raw_spc.clear();
  res_par.clear();
  std::ifstream rawspc_f;
  rawspc_f.open(raw_fname.c_str());
  if(!rawspc_f.is_open()) {
    std::cerr<<"file ["<<raw_fname<<"] not found"<<std::endl;
    throw std::runtime_error("File not found");
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
  // get entry of in_N
  int in_N_begin,in_N_end; {
    // skip the head description part
    FindFlag(rawspc_f,"$DATA:");
    rawspc_f>>in_N_begin>>in_N_end;
  }
  // get the entries for each channel
  for(int i = in_N_begin;i<= in_N_end; ++i) {
    unsigned int n;
    rawspc_f>>n;
    raw_spc.push_back(n);
  }
  if(rawspc_f.fail()) {
    std::cerr<<"In correct format in ["<<raw_fname<<"]"<<std::endl;
    throw std::runtime_error("Wrong format");
  }
  // find [$MCA_CAL:] flag
  FindFlag(rawspc_f,"$MCA_CAL:");
  int npar;
  rawspc_f>>npar;
  for(int j = 0;j<npar;++j) {
    double par;
    rawspc_f>>par;
    res_par.push_back(par);
  }
  if(rawspc_f.fail()) {
    std::cerr<<"In correct format in ["<<raw_fname<<"]"<<std::endl;
    throw std::runtime_error("Wrong format");
  }
  rawspc_f.close();
}
