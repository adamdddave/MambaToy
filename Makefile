ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

CXX           = g++
CXXFLAGS      = -g -std=gnu++11 -Wall -fPIC -Wno-deprecated -fpermissive

NGLIBS         = $(ROOTGLIBS) 
#NGLIBS        += -lMinuit
NGLIBS        += -lRooFit
NGLIBS        += -lRooFitCore
CXXFLAGS      += $(ROOTCFLAGS)
CXX           += -I./	
LIBS           = $(ROOTLIBS) 

GLIBS          = $(filter-out -lNew, $(NGLIBS))

CXX	      += -I./lib/ 
OUTLIB	      = ./lib/
.SUFFIXES: .C
.PREFIXES: ./lib/

#----------------------------------------------------#

all:  MilanoDaqToys

MilanoDaqToys: runner.cpp 
	$(CXX) $(CXXFLAGS) ToyDataGenerator.cpp  FileWriter.cpp -o MilanoDAQToys  $(GLIBS) $<

clean:
	rm -f MilanoDAQToys
