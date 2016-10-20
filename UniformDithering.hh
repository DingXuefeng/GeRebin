#ifndef UniformDithering_H
#define UniformDithering_H
#include "IDithering.hh"
namespace GeSpcAnalysis {
	 class UniformDithering : public IDithering {
		  const double Dither() const;
	 };
}
#endif
