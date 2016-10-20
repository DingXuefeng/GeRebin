#ifndef IMask_H
#define IMask_H
namespace GeSpcAnalysis {
	 class IMask {
		  public:
				virtual const double CorrectMaskEffect(const int Nbins,double *binContent,double *binError) const = 0;
	 };
};
#endif
