/***************************************************************************
 * Author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
 * Date: 2017.07.22
 * Copyrighted, all rights reserved
 * ***********************************************************************/
#ifndef Config_H
#define Config_H
struct Config {
  double E_bin;
  double E_start;
  double E_end;
  Config(double bin,double start,double end) : E_bin(bin),E_start(start),E_end(end) {}
};
#endif
