CFLAGS:=-g
LIBS:=-g

GeRebin: GeSpcAnalysisManager.o GeRebin.o Spectrum.o TRandom.o
	g++ -o $@ $(LIBS) $^

TRandom.o: TRandom.cxx TRandom.h
	g++ -c $(CFLAGS) $<

GeSpcAnalysisManager.o: GeSpcAnalysisManager.cc GeSpcAnalysisManager.hh
	g++ -c $(CFLAGS) $<
#
Spectrum.o: Spectrum.cc Spectrum.h TRandom.h
	g++ -c $(CFLAGS) $< 
#
GeRebin.o: GeRebin.cc GeSpcAnalysisManager.hh
	g++ -c $(CFLAGS) $<

clean:
	-rm *~
	-rm .*.sw*
	-rm *.o
	-rm *.so
