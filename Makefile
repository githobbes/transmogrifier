GRAPHICS_MAGICK_COMPILE_FLAGS = $(shell GraphicsMagick++-config --cppflags)
GRAPHICS_MAGICK_LINK_FLAGS = $(shell GraphicsMagick++-config --cxxflags --ldflags --libs)

ShellClose: ShellClose.cpp
	$(CXX) $^ -o $@

convert.o: convert.cpp
	$(CXX) -c -o $@ $^ $(GRAPHICS_MAGICK_COMPILE_FLAGS)

convert: convert.o
	$(CXX) -o $@ $^ $(GRAPHICS_MAGICK_LINK_FLAGS)

.PHONY clean:
	$(RM) ShellClose
