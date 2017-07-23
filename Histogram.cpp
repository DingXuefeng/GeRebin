/***************************************************************************
 * Author: Xuefeng Ding<xuefeng.ding.physics@gmail.com>
 * Date: 2017.07.22
 * Copyrighted, all rights reserved
 * ***********************************************************************/
#include "Histogram.h"
#include "TRandom.h"
#include <iostream>
#include <cmath>
Histogram::Histogram(const std::vector<unsigned int> &raw_spc) :
  gRandom(new TRandom),biny(raw_spc.size()+2,0),binyerr(raw_spc.size()+2,0),
  L(-0.5),R(raw_spc.size()-0.5),Nbins(raw_spc.size()),dx(1),N(0),maximum(0),bin_ready(true)
{
#define START_FROM_0
#ifdef START_FROM_0
  L = 0;
  R = Nbins;
#endif
  for(int i = 0;i<Nbins;++i) {
    Fill(i,raw_spc.at(i));
  }
}
Histogram::Histogram(unsigned int Nbins_in,double iL,double iR) :
  gRandom(new TRandom),biny(Nbins_in+2,0),binyerr(Nbins_in+2,0),
  L(iL),R(iR),Nbins(Nbins_in),dx((iR-iL)/Nbins_in),N(0),maximum(0),bin_ready(true)
{ }

double Histogram::GetBinContent(int bin) const { 
  try {
    return biny.at(bin);
  } catch(std::exception &ex) {
    std::cerr<<"trying to get bin "<<bin<<" while bin ["<<L<<","<<R<<","<<Nbins<<","<<N<<"]: "<<ex.what()<<std::endl;
    return -1;
  };
}
double Histogram::GetBinError(int bin) const { 
  try {
    return binyerr.at(bin);
  } catch(std::exception &ex) {
    std::cerr<<"trying to get bin "<<bin<<" while bin ["<<L<<","<<R<<","<<Nbins<<","<<N<<"]: "<<ex.what()<<std::endl;
    return -1;
  };
}

unsigned int Histogram::Find(double x) const {
  if((x>R)||(x<L)) 
    throw std::runtime_error("Fill out of range for histogram");
  return (x-L)/dx+1;
}

bool Histogram::Fill(double x) {
  return Fill(x,1);
}

bool Histogram::Fill(double x,double y) {
  if(!bin_ready)
    throw std::runtime_error("fill while bin not ready. set L,R,N,dx first.");
  const unsigned int bin = Find(x);
  biny.at(bin)+=y;
  N+=y;
  binyerr.at(bin) = sqrt(biny.at(bin));
  if(biny.at(bin)>maximum) maximum = biny.at(bin);
  return true;
}

double Histogram::GetRandom() const {
  unsigned int bin = 0;
  unsigned int Nsum = 0;
  double Nsample = gRandom->Rndm()*Integral();
  while(Nsum<Nsample)
    Nsum+=GetBinContent(++bin);
  return (bin-(Nsum-Nsample)/GetBinContent(bin))*dx;
}
