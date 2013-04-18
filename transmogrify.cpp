#include "transmogrify.hpp"

#include "algorithms.hpp"
#include "convert.hpp"

#include <Magick++.h>

#include <sstream>

void
transmogrifier::init() {
    Magick::InitializeMagick(NULL);
}

void
transmogrifier::streamToStream(const std::istream& inputStream, std::ostream& outStream, const std::string& outputFormat, unsigned iterations) {

    std::stringstream ppmStream;
    transmogrifier::streamToPixelMap(inputStream, ppmStream);

    std::stringstream outputImg;
    transmogrifier::penroseChuck(ppmStream, outputImg, iterations);

    transmogrifier::pixelMapToStream(ppmStream, outStream, outputFormat);

}

void
transmogrifier::streamToNamedFile(const std::istream& inputStream, const std::string& outputFileName, unsigned iterations) {

    std::stringstream ppmStream;
    transmogrifier::streamToPixelMap(inputStream, ppmStream);

    std::stringstream outputImg;
    transmogrifier::penroseChuck(ppmStream, outputImg, iterations);

    transmogrifier::pixelMapToNamedFile(ppmStream, outputFileName);


}

void
transmogrifier::namedFileToStream(const std::string& inputFileName, std::ostream& outStream, const std::string& outputFormat, unsigned iterations) {

    std::stringstream ppmStream;
    transmogrifier::namedFileToPixelMap(inputFileName, ppmStream);

    std::stringstream outputImg;
    transmogrifier::penroseChuck(ppmStream, outputImg, iterations);

    transmogrifier::pixelMapToStream(ppmStream, outStream, outputFormat);

}


void
transmogrifier::namedFileToNamedFile(const std::string& inputFileName, const std::string& outputFileName, unsigned iterations) {

std::stringstream ppmStream;
transmogrifier::namedFileToPixelMap(inputFileName, ppmStream);

std::stringstream outputImg;
transmogrifier::penroseChuck(ppmStream, outputImg, iterations);

transmogrifier::pixelMapToNamedFile(outputImg, outputFileName);

}

