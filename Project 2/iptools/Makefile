# source files.
SRC =  image/image.cpp utility/utility.cpp

OBJ = $(SRC:.cpp=.o)

OUT = ../lib/libiptools.a

# include directories
INCLUDES = -I. -I/usr/local/include -Iimage/  -Iutility/ -I/usr/include/c++/4.2.1 

# C++ compiler flags (-g -O2 -Wall)
CCFLAGS = -g

# compiler
CCC = g++

#OPENCVLIBS = -L/apps/opencv/4.0.1/lib64 -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_objdetect -Wl,-rpath=/apps/opencv/4.0.1/lib64
#OPENCVINCLUDE = -I/apps/opencv/4.0.1/include/opencv4

# compile flags
LDFLAGS = -g

.SUFFIXES: .cpp

.cpp.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@
	

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)

clean:
	rm -f $(OBJ) $(OUT) Makefile.bak# DO NOT DELETE
