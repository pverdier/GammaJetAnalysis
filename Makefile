CXXFLAGS = -g -Wall -std=c++0x -fPIC -Wno-deprecated -I. -I`root-config --incdir` #-std=c++0x dire à g++ d'utiliser le c++11 au moment de la compilation
LDFLAGS = -g -I.
CXX = c++
DEPS =  $(patsubst %.cc, %.d, $(wildcard *.cc)) 

all: run

run: My2DAna.o runMy2DAna.o GeneralPlots.o QGSyst.o alphaBinning.o ptBinning.o prescaleParser.o
	${CXX} ${LDFLAGS} -o runMy2DAna $^ `root-config --libs`

runMy2DAna.o: runMy2DAna.cpp
	${CXX} -c ${CXXFLAGS} `root-config --cflags` runMy2DAna.cpp

My2DAna.o: My2DAna.cpp My2DAna.h RootTupleDefs.h
	${CXX} -c ${CXXFLAGS} `root-config --cflags` My2DAna.cpp

GeneralPlots.o: GeneralPlots.cpp GeneralPlots.h
	${CXX} -c ${CXXFLAGS} `root-config --cflags` GeneralPlots.cpp

QGSyst.o: QGSyst.cpp QGSyst.h
	${CXX} -c ${CXXFLAGS} `root-config --cflags` QGSyst.cpp

alphaBinning.o: alphaBinning.cpp alphaBinning.h
	${CXX} -c ${CXXFLAGS} `root-config --cflags` alphaBinning.cpp

ptBinning.o: ptBinning.cpp ptBinning.h
	${CXX} -c ${CXXFLAGS} `root-config --cflags` ptBinning.cpp

prescaleParser.o: prescaleParser.cpp prescaleParser.h
	${CXX} -c ${CXXFLAGS} `root-config --cflags` prescaleParser.cpp

clean:
	rm -f *.o runMy2DAna core


# Make the dependencies 
# If one of the .h included in the different .cc is modified, it will be automatically recompiled
%.d: %.cc
	@echo "Generating dependencies for $<"
	@set -e; $(CXX) -M $(CXXFLAGS) $< \
	| sed 's%\($*\)\.o[ :]*%\1.o $@ : %g' > $@; \
	[ -s $@ ] || rm -f $@

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif