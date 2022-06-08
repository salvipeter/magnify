all: magnify

FLAGS=-std=c++17 -Wall -pedantic -g
INCLUDES=`Magick++-config --cppflags`
LIBS=`Magick++-config --libs` -lGL -lGLU -lglut

magnify: magnify.cc
	$(CXX) -o $@ $< $(FLAGS) $(INCLUDES) $(LIBS)
