#include "convert.hpp"
#include <Magick++.h>
#include <iostream>
#include <sstream>

int
main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);

	// Parse command-line arguments
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " INPUT_IMAGE OUTPUT_IMAGE" << std::endl;
		return 1;
	}
	std::string inputImgName(argv[1]);
	std::string outputImgName(argv[2]);

	// Convert image to PPM/PGM
	std::stringstream ppmStream;
	std::stringstream pgmStream;
	toPixelMap(inputImgName, ppmStream, pgmStream);

	std::cout << ppmStream.str() << std::endl;

	// Run algorithm on PPM/PGM input to produce PPM

	// Convert PPM to PNG

	return 0;
}
