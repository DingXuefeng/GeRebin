CFLAGS:=-g
LIBS:=-g

GeRebin: GeSpcAnalysisManager.o GeRebin.o Spectrum.o TRandom.o
	g++ -o $@ $(LIBS) $^

TRandom.o: TRandom.cpp TRandom.h
	g++ -c $(CFLAGS) $<

GeSpcAnalysisManager.o: GeSpcAnalysisManager.cpp GeSpcAnalysisManager.h
	g++ -c $(CFLAGS) $<
#
Spectrum.o: Spectrum.cpp Spectrum.h TRandom.h
	g++ -c $(CFLAGS) $< 
#
GeRebin.o: GeRebin.cpp GeSpcAnalysisManager.h
	g++ -c $(CFLAGS) $<

clean:
	-rm *~
	-rm .*.sw*
	-rm *.o
	-rm *.so
