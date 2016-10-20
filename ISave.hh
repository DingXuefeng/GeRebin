#ifndef ISave_H
#define ISave_H
namespace GeSpcAnalysis {
	 class ISave {
		  public:
				virtual const double Save(const int Nbins,double *binContent,double *binError,const char *outputname) const = 0;
	 };
};
#endif
