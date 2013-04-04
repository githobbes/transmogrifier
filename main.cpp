#include "convert.hpp"
#include "algorithms.hpp"
#include "logging.hpp"
#include <Magick++.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

std::string
generateUsage(const char *progName, const po::options_description &options) {
	std::ostringstream ss;
	ss << "Usage: " << progName << " [options] INPUT_IMAGE OUTPUT_IMAGE\n" << options << std::endl;
	return ss.str();
}

int
main (int argc, char *argv[])
{
	Magick::InitializeMagick(*argv);

	transmogrifier::programName = argv[0];

	po::options_description genericOptions("Allowed options");
	genericOptions.add_options()
	("version,V", "print program version")
	("help,h", "print usage")
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

	if (vm.count("help")) {
		std::cout << generateUsage(argv[0], genericOptions);
		return 0;
	}

	if (vm.count("version")) {
		std::cout << "Transmogrify 0.1" << std::endl;
		return 0;
	}

	try {
		po::notify(vm);
	}

	catch(po::error& e) {
		std::cerr << e.what() << "\n\n" << generateUsage(argv[0], genericOptions);
		return 1;
	}

	std::string inputImgName = vm["input-image"].as<std::string>();
	std::string outputImgName = vm["output-image"].as<std::string>();

	// Convert image to PPM/PGM
	std::stringstream ppmStream;
	std::stringstream pgmStream;
	if (inputImgName == "-")
		transmogrifier::streamToPixelMap(std::cin, ppmStream, pgmStream);
	else
		transmogrifier::namedFileToPixelMap(inputImgName, ppmStream, pgmStream);

	// Create output file
	std::stringstream outputImg;

	// Run algorithm on PPM input to produce PPM
	transmogrifier::penroseChuck(ppmStream, outputImg, vm["iterations"].as<unsigned>());

	// Convert PPM to PNG
	transmogrifier::writeImage(outputImg, outputImgName);

	return 0;
}

