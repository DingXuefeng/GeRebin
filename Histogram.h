/***************************************************************************
 * Author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
 * Date: 2017.07.22
 * Copyrighted, all rights reserved
 * ***********************************************************************/
#ifndef Histogram_H
#define Histogram_H
#include <vector>
class TRandom;
class Histogram {
  public:
    Histogram(const std::vector<unsigned int> &raw_spc);
    Histogram(unsigned int Nbins,double L,double R);
    bool Fill(double x);
    bool Fill(double x,double y);
    unsigned int Integral() const { return N; }
    const unsigned int GetNbins() const { return Nbins; };
    double GetRandom() const;
    double GetBinContent(int bin) const;
    double GetBinError(int bin) const;
    unsigned int GetMaximum() const { return maximum; };
  private:
    unsigned int Find(double x) const;
    TRandom *gRandom;
    std::vector<double> biny;
    std::vector<double> binyerr;
    double L;
    double R;
    unsigned int Nbins;
    double dx;
    unsigned int N;
    unsigned int maximum;
    bool bin_ready;
};
#endif
