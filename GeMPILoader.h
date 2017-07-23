/***************************************************************************
 * Author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
 * Date: 2017.07.22
 * Copyrighted, all rights reserved
 * ***********************************************************************/
#ifndef GeMPILoader_H
#define GeMPILoader_H
#include <string>
#include <vector>
#include "FileLoader.h"
class GeMPILoader : public FileLoader {
  public:
    void ParseRawDataFile(const std::string &fname);
    std::vector<unsigned int> &get_raw() { return raw_spc; }
    std::vector<double> &get_res_par() { return res_par; }
  private:
    std::vector<unsigned int> raw_spc;
    std::vector<double> res_par;
};
#endif
