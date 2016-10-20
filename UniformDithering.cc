#include <random>
#include "UniformDithering.hh"
using GeSpcAnalysis::UniformDithering;
const double UniformDithering::Dither() const {
	 static std::uniform_real_distribution<double> uni_d(0.0,1.0);
	 static std::default_random_engine re;
	 return uni_d(re)-0.5;
}
