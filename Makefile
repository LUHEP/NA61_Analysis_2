# $Id: Makefile 8018 2012-10-12 09:32:58Z munger $

USER_SRCS := $(wildcard *.cpp)
EXE := $(patsubst %.cc,%, $(USER_SRCS))
.PHONY: all depend clean


ifdef SHINEOFFLINEROOT
  SHINEOFFLINECONFIG := $(SHINEOFFLINEROOT)/bin/shine-offline-config
else
  SHINEOFFLINECONFIG := shine-offline-config
endif

CPPFLAGS := $(shell $(SHINEOFFLINECONFIG) --cppflags) -g
CXXFLAGS := $(shell $(SHINEOFFLINECONFIG) --cxxflags) -g
LDFLAGS  := $(shell $(SHINEOFFLINECONFIG) --ldflags) -g
CONFIGFILES := $(shell $(SHINEOFFLINECONFIG) --config)
DBFILES := $(shell $(SHINEOFFLINECONFIG) --db-path)
TESTFILE := ../../../doc/SampleEvents/run-009917x054_shoe.root


all: $(EXE) NA61_LRC_2

%: %.in
	@echo -n "Generating $@ file..."
	@ sed -e 's!@''CONFIGDIR@!$(CONFIGFILES)!g;s!@''SHINEDBDIR@!$(DBFILES)!g' $< >$@
	@echo "done"

run: $(EXE)
	$(SHELL) runAll.sh $(TESTFILE) $@

clean:
	- rm -f run core $(EXE)

# part for LRC 2014-08-20

NA61_LRC_2: NA61_LRC_2.cpp 
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(USER_SRCS) -o $@

#NA61_LRC_2: NA61LRC.cpp HandlersList.o
#	$(CXX) $(CPPFLAGS) $(LDFLAGS) HandlersList.o NA61LRC.cpp -#o $@  

#HandlersList.o: NA61LRCHandlerList.cpp Handlers.o CutList.o #Cut.o NA61LRCHandlerList.h
#	$(CXX) $(CPPFLAGS) $(LDFLAGS) CutList.o Handlers.o Cut.o #NA61LRCHandlerList.cpp  -c -o $@

#CutList.o: NA61LRCCutList.cpp NA61LRCCutList.h Cut.o
#	$(CXX) $(CPPFLAGS) $(LDFLAGS) NA61LRCCutList.cpp  Cut.o  -#c -o $@

#Cut.o: NA61LRCcuts.cpp NA61LRCcuts.h NA61LRCconst.h
#	$(CXX) $(CPPFLAGS) $(LDFLAGS) NA61LRCcuts.cpp -c -o $@ 

#Handlers.o: NA61LRChandlers.cpp NA61LRChandlers.h #NA61LRCconst.h
#	$(CXX) $(CPPFLAGS) $(LDFLAGS) NA61LRChandlers.cpp -c -o $@ 

