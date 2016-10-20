#include "OriginalBinning.hh"
#include "GeSpcAnalysisGlobal.hh"
using GeSpcAnalysis::OriginalBinning;
bool OriginalBinning::SetBinEdge(int &bin_n, double *const bin_edge) const {
	 // user should allocate memory for bin_edge himself
	 const int f_bin_n = GeSpcAnalysisManager::GetInstance()->GetNbins();
	 static const double *f_bin_edge(nullptr);
	 if(!f_bin_edge) {
		  f_bin_edge = new double[f_bin_n+1];
		  for(int i = 0;i<f_bin_n+1; ++i) {
				const double binEdge_channel = i+0.5; // i=0, the first bin edge, should be channel of 0.5
				((double*)f_bin_edge)[i] = GeSpcAnalysisManager::GetInstance()->ChannelToEnergy(binEdge_channel);
		  }
	 }
	 bin_n = f_bin_n;
	 for(int i = 0;i<bin_n+1; ++i)
		  bin_edge[i] = f_bin_edge[i];
	 return true;
}
