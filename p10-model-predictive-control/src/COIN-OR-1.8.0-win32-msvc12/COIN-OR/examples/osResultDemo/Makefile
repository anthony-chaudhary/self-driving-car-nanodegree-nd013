# Copyright (C) 2003, 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

# $Id: Makefile.in 759 2006-07-07 03:07:08Z andreasw $

##########################################################################
#    You can modify this example makefile to fit for your own program.   #
#    Usually, you only need to change the five CHANGEME entries below.   #
##########################################################################

# CHANGEME: This should be the name of your executable
EXE = OSResultDemo.exe

# CHANGEME: Here is the name of all object files corresponding to the source
#           code that you wrote in order to define the problem statement
OBJS =  OSResultDemo.obj 

# CHANGEME: Additional libraries
ADDLIBS = 


# CHANGEME: Additional flags for compilation (e.g., include flags)
# Use the below to get cppad headers for now
ADDINCFLAGS =  ${prefix}/include

prefix = ../..

##########################################################################
#  Usually, you don't have to change anything below.  Note that if you   #
#  change certain compiler options, you might have to recompile OS.   #
##########################################################################

# C++ Compiler command, for example g++
CXX = cl

DEFS = -DHAVE_CONFIG_H

# C++ Compiler options
CXXFLAGS = -MT -O2 -nologo -EHsc -GR -wd4996 -D_CRT_SECURE_NO_DEPRECATE -DNDEBUG     $(DEFS)

CXXFLAGS += `pkg-config --cflags os`
 
DARWIN=$(findstring darwin, cygwin)
WIN=$(findstring gw, cygwin)

# additional C++ Compiler options for linking
CXXLINKFLAGS = 

ifeq ($(DARWIN),darwin)
  CXXLINKFLAGS += -bind_at_load
endif


#ADD LIBRARIES
# Libraries necessary to link
LIBS = `pkg-config --libs os` 
#DONE ADDING LIBS


# Necessary Include dirs (we use the CYGPATH_W variables to allow
# compilation with Windows compilers)
INCL =  -I`$(CYGPATH_W) $(ADDINCFLAGS)`  

# The following is necessary under cygwin, if native compilers are used
CYGPATH_W = cygpath -w

all: $(EXE)

.SUFFIXES: .cpp .c .o .obj

$(EXE): $(OBJS)
	bla=;\
	for file in $(OBJS); do bla="$$bla `$(CYGPATH_W) $$file`"; done; \
	$(CXX) $(CXXLINKFLAGS) $(CXXFLAGS) -o $@ $$bla $(ADDLIBS) $(LIBS)

clean:
	rm -rf $(EXE) $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCL) -c -o $@ $<


.cpp.obj:
	$(CXX) $(CXXFLAGS) $(INCL) -c -o $@ `$(CYGPATH_W) '$<'`
