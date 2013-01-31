GRAPHICS_MAGICK_COMPILE_FLAGS = $(shell GraphicsMagick++-config --cppflags)
GRAPHICS_MAGICK_LINK_FLAGS = $(shell GraphicsMagick++-config --cxxflags --ldflags --libs)

ShellClose: ShellClose.cpp
	$(CXX) $^ -o $@

convert.o: convert.cpp
	$(CXX) -c $(GRAPHICS_MAGICK_COMPILE_FLAGS) $^ -o $@

convert: convert.o
	$(CXX) $(GRAPHICS_MAGICK_LINK_FLAGS) $^ -o $@

.PHONY clean:
	$(RM) ShellClose
