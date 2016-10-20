#ifndef IBinning_H
#define IBinning_H
namespace GeSpcAnalysis {
	 class IBinning {
		  public:
				virtual bool SetBinEdge(int &bin_n, double *const bin_edge) const = 0;
	 };
};
#endif
