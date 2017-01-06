# This makefile expects to be run in an environment like that which
# results from running the demo-setup script. All the environment
# variables that are used to find the headers and libraries must be
# defined for this makefile to work.


# CPPFLAGS should provide the information of where all headers are
# found.
CPPFLAGS=-I $(BOOST_INC) \
         -I $(CANVAS_INC) \
         -I $(CETLIB_INC) \
         -I $(FHICLCPP_INC) \
         -I $(GALLERY_INC) \
         -I $(LARCOREOBJ_INC) \
         -I $(LARDATAOBJ_INC) \
         -I $(NUSIMDATA_INC) \
         -I $(ROOT_INC)

# LDFLAGS provides the locations and names of all libraries against
# which we need to link.
LDFLAGS=$$(root-config --libs) \
        -L $(CANVAS_LIB) -l canvas_Utilities -l canvas_Persistency_Common -l canvas_Persistency_Provenance \
        -L $(CETLIB_LIB) -l cetlib \
        -L $(GALLERY_LIB) -l gallery \
        -L $(NUSIMDATA_LIB) -l nusimdata_SimulationBase \
        -L $(LARCOREOBJ_LIB) -l larcoreobj_SummaryData \
        -L $(LARDATAOBJ_LIB) -l lardataobj_RecoBase

# CXXFLAGS controls the language version, optimization level, and
# warning control. Make sure to use something compatible with the
# other libraries you are using!
CXXFLAGS=-std=c++14 -O3 -Wall -Werror -pedantic

# CXX determines the compiler we're using. Make sure to use one that
# is binary compatible with the one that compiled the other libraries
# you're using! Here, we rely on finding the one in the PATH, as
# established by the UPS products we've set up.
CXX=g++

all: demo

# Here's how we build the demo program.
demo: demo.cc analyze.o
	@echo Building $@
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $< analyze.o

analyze.o: analyze.cc analyze.hh
	@echo Compiling $@
	@$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

clean:
	rm -f *.o
	rm -f demo
