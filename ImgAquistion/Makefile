# Makefile for Basler Pylon sample program
.PHONY			: all clean

PYLON_ROOT := /opt/pylon5
#GENICAM_ROOT_V1_1 := $(PYLON_ROOT)/genicam
GENICAM_ROOT_V1_1 := $(PYLON_ROOT)/

# the program to build
NAME			:= AcqExtTrig

# Build tools and flags
CXX			:= /usr/bin/g++
LD			:= /usr/bin/g++
CPPFLAGS		:= -I$(GENICAM_ROOT_V1_1)/include -I$(GENICAM_ROOT_V1_1)/include/genicam -I$(PYLON_ROOT)/include -I/usr/include/opencv/ -DUSE_GIGE -I/opt/pylon/genicam/library/CPP/include
CXXFLAGS		:=    			#e.g., CXXFLAGS=-g -O0 for debugging

#LDFLAGS			:= -L$(PYLON_ROOT)/lib64 -L$(GENICAM_ROOT_V1_1)/bin/Linux32_i86 -Wl,-E
LDFLAGS			:= -L$(PYLON_ROOT)/lib64 -Wl,-E


# librt is needed only for the timer_create stuff used for demonstration purposes
LIBS			:= -lGenApi_gcc_v3_1_Basler_pylon -lGCBase_gcc_v3_1_Basler_pylon -lMathParser_gcc_v3_1_Basler_pylon -lXmlParser_gcc_v3_1_Basler_pylon -lLog_gcc_v3_1_Basler_pylon -lNodeMapData_gcc_v3_1_Basler_pylon  -lpylonbase -lrt -ljpeg -lopencv_core -lopencv_highgui -lGCBase_gcc_v3_1_Basler_pylon -lpylonutility 
#LIBS			:= -lpylonbase -lrt -ljpeg -lopencv_core -lopencv_highgui -lGCBase_gcc40_v2_1 -lpylonutility

# -lcxcore -lpylonutility
#LIBS			:= -lpylonbase -lrt -lgenicam_base -ljpeg -lcv -lhighgui -lcxcore -lpylonutility

all			: $(NAME)

$(NAME)			: $(NAME).o
	$(LD) $(LDFLAGS) -o $@ $< $(LIBS)

$(NAME).o		: $(NAME).cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean			:
	$(RM) $(NAME).o $(NAME)
