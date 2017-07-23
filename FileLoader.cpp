/***************************************************************************
 * Author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
 * Date: 2017.07.22
 * Copyrighted, all rights reserved
 * ***********************************************************************/
#include "FileLoader.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
void FileLoader::FindFlag(std::istream &in,const std::string &flag) {
	// find $MCA_CAL flag
	while(true) {
      std::string tmp;
		getline(in,tmp);
		if(!tmp.compare(0, flag.length(), flag))
			break; // found
		if(in.fail()) {
          std::cerr<<__func__<<" ["<<flag<<"] flag not found."<<std::endl;
			throw std::runtime_error("Flag not found");
		}
	}
}
void FileLoader::Split(const std::string &input,std::vector<std::string> &results) {
  std::stringstream ss;
  ss<<input;
  while(true) {
    std::string tmp;
    ss>>tmp;
    if(ss.fail()) break;
    results.push_back(tmp);
  };
}
