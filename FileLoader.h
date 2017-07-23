/***************************************************************************
 * Author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
 * Date: 2017.07.22
 * Copyrighted, all rights reserved
 * ***********************************************************************/
#ifndef FileLoader_H
#define FileLoader_H
#include <string>
#include <vector>
class FileLoader {
  public:
    static void FindFlag(std::istream &in,const std::string &flag) ;
    static void Split(const std::string &input,std::vector<std::string> &results) ;
    virtual void ParseRawDataFile(const std::string &fname) = 0;
    virtual std::vector<unsigned int> &get_raw() = 0;
    virtual std::vector<double> &get_res_par() = 0;
};
#endif
