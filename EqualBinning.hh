#ifndef EqualBinning_H
#define EqualBinning_H
#include "IBinning.hh"
namespace GeSpcAnalysis {
	 class EqualBinning: public IBinning {
		  bool SetBinEdge(int &bin_n, double *const bin_edge) const;
	 };
}
#endif
