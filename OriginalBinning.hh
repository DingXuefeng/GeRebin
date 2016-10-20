#ifndef OriginalBinning_H
#define OriginalBinning_H
#include "IBinning.hh"
namespace GeSpcAnalysis {
	 class OriginalBinning : public IBinning {
		  bool SetBinEdge(int &bin_n, double *const bin_edge) const;
	 };
}
#endif
