#include "convert.hpp"
#include "algorithms.hpp"
#include "logging.hpp"
#include <Magick++.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int
main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);

	programName = argv[0];

	po::options_description genericOptions("Allowed options");
	genericOptions.add_options()
	("version,V", "print program version")
	("input-image", po::value<std::string>()->required(), "image to be transmogrified")
	("output-image", po::value<std::string>()->required(), "location of transmogrified image")
	("iterations,i", po::value<unsigned>()->default_value(8), "number of iterations")
	;

	po::positional_options_description positionalOptions;
	positionalOptions.add("input-image", 1);
	positionalOptions.add("output-image", 2);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv)
	          .options(genericOptions)
	          .positional(positionalOptions)
	          .run(), vm);

	if (vm.count("version")) {
		std::cout << "Transmogrify 0.1" << std::endl;
		return 0;
	}

	try {
		po::notify(vm);
	}

	catch(po::error& e) {
		std::cerr << e.what() << "\n\n" << "Usage: " << argv[0] << " [options] INPUT_IMAGE OUTPUT_IMAGE\n" << genericOptions << std::endl;

		return 1;
	}

	std::string inputImgName = vm["input-image"].as<std::string>();
	std::string outputImgName = vm["output-image"].as<std::string>();

	// Convert image to PPM/PGM
	std::stringstream ppmStream;
	std::stringstream pgmStream;
	toPixelMap(inputImgName, ppmStream, pgmStream);

	// Create output file
	std::stringstream outputImg;

	// Run algorithm on PPM input to produce PPM
	penroseChuck(ppmStream, outputImg, vm["iterations"].as<unsigned>());

	// Convert PPM to PNG
	writeImage(outputImg, outputImgName);

	return 0;
}
