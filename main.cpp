#include "convert.hpp"
#include <Magick++.h>
#include <iostream>
#include <sstream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int
main (int argc, char *argv[])
{

  po::options_description genericOptions("Allowed options");
  genericOptions.add_options()
    ("version,V", "print program version")
    ("input-image", po::value<std::string>()->required(), "image to be transmogrified")
    ("output-image", po::value<std::string>()->required(), "location of transmogrified image")
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

    return -1;
  }

  std::cout << vm["input-image"].as<std::string>() << std::endl;
  std::cout << vm["output-image"].as<std::string>() << std::endl;
  return 0;

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
