GRAPHICS_MAGICK_COMPILE_FLAGS = $(shell GraphicsMagick++-config --cppflags)
GRAPHICS_MAGICK_LINK_FLAGS = $(shell GraphicsMagick++-config --cxxflags --ldflags --libs)

main: main.o convert.o
	$(CXX) -o $@ $^ $(GRAPHICS_MAGICK_LINK_FLAGS)

main.o: main.cpp convert.hpp
	$(CXX) -c -o $@ main.cpp $(GRAPHICS_MAGICK_COMPILE_FLAGS)

convert.o: convert.cpp convert.hpp
	$(CXX) -c -o $@ convert.cpp $(GRAPHICS_MAGICK_COMPILE_FLAGS)

.PHONY clean:
	$(RM) *.o main
