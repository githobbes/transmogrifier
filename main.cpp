#include "logging.hpp"
#include "transmogrify.hpp"

#include <Magick++.h>
#include <boost/program_options.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

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
    transmogrifier::init(argv);

	transmogrifier::programName = argv[0];

	po::options_description genericOptions("Allowed options");
	genericOptions.add_options()
	("version,V", "print program version")
	("help,h", "print usage")
	("input-image", po::value<std::string>()->required(), "image to be transmogrified")
	("output-image", po::value<std::string>()->required(), "location of transmogrified image")
	("output-format,f", po::value<std::string>(), "format for output image ONLY if writing to stdout")
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
	unsigned iterations = vm["iterations"].as<unsigned>();

	if (outputImgName == "-" && vm.count("output-format") == 0) {
		transmogrifier::error() << "Must pass `--output-format' when output is sent to stdout (`--output-image' is \"-\")" << std::endl;
		return 1;
	}

	if (outputImgName != "-" && vm.count("output-format") > 0) {
		transmogrifier::error() << "Must pass `--output-format' ONLY when output is sent to stdout (`--output-image' is \"-\")" << std::endl;
		return 1;
	}
       
	if (inputImgName == "-") {
	    if (outputImgName == "-")
		transmogrifier::streamToStream(std::cin, std::cout, vm["output-format"].as<std::string>(), iterations);
	    else
		transmogrifier::streamToNamedFile(std::cin, outputImgName, iterations);
	}
	else {
	    if (outputImgName == "-")
		transmogrifier::namedFileToStream(inputImgName, std::cout, vm["output-format"].as<std::string>(), iterations);
	    else
		transmogrifier::namedFileToNamedFile(inputImgName, outputImgName, iterations);
	}

	return 0;
}

