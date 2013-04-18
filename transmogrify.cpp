#include "transmogrify.hpp"

#include "algorithms.hpp"
#include "convert.hpp"

#include <Magick++.h>

#include <sstream>

void
transmogrifier::init(char *argv[]) {
    Magick::InitializeMagick(*argv);
}

void
transmogrifier::namedFileToNamedFile(const std::string& inputFileName, const std::string& outputFileName, unsigned iterations) {

std::stringstream ppmStream;
transmogrifier::namedFileToPixelMap(inputFileName, ppmStream);

std::stringstream outputImg;
transmogrifier::penroseChuck(ppmStream, outputImg, iterations);

transmogrifier::pixelMapToNamedFile(outputImg, outputFileName);

}

