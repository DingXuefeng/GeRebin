#ifndef IDithering_H
#define IDithering_H
namespace GeSpcAnalysis {
	class Pdf;
	 class IDithering {
		  public:
				virtual const double Fill(const Pdf &pdf,const int bin,const double x_left,const double x_right,double &binContent,double &binError) const = 0;
	 };
};
#endif
