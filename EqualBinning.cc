#include "EqualBinning.hh"
#include "GeSpcAnalysisGlobal.hh"
using GeSpcAnalysis::EqualBinning;
bool EqualBinning::SetBinEdge(int &bin_n, double *const bin_edge) const {
	 const int f_bin_n = 3000;
	 static const double *f_bin_edge(nullptr);
	 if(!f_bin_edge) {
		  f_bin_edge = new double[f_bin_n+1];
		  for(int i = 0;i<f_bin_n+1; ++i)
				((double*)f_bin_edge)[i] = (i-0.5)*GeSpcAnalysis::EnergyUnit::keV;
	 }
	 bin_n = f_bin_n;
	 for(int i = 0;i<bin_n+1; ++i)
		  bin_edge[i] = f_bin_edge[i];
	 return true;
}
