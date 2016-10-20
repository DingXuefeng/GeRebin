CFLAGS:=$(shell root-config --cflags) -g
LIBS:=$(shell root-config --libs) -g

#GeRebin: UniformDithering.o EqualBinning.o GeSpcAnalysisManager.o GeRebin.o
GeRebin: GeSpcAnalysisManager.o GeRebin.o Spectrum.o
	g++ -o $@ $(LIBS) $^

GeSpcAnalysisManager.o: GeSpcAnalysisManager.cc GeSpcAnalysisManager.hh
	g++ -c $(CFLAGS) $<
#
Spectrum.o: Spectrum.cc Spectrum.h
	g++ -c $(CFLAGS) $<
#
GeRebin.o: GeRebin.cc GeSpcAnalysisManager.hh
	g++ -c $(CFLAGS) $<
#
##UniformDithering.o: UniformDithering.cc UniformDithering.hh IDithering.hh
#UniformDithering.o: UniformDithering.cc UniformDithering.hh
#	g++ -c $(CFLAGS) $<
#
#EqualBinning.o: EqualBinning.cc EqualBinning.hh IDithering.hh
#	g++ -c $(CFLAGS) $<

clean:
	-rm *~
	-rm .*.sw*
	-rm *.o
