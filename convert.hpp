#ifndef CONVERT_HPP
#define CONVERT_HPP

#include <string>
#include <ostream>
#include <istream>

namespace transmogrifier {
void
streamToPixelMap(const std::istream& inputStream, std::ostream& ppmStream);

void
namedFileToPixelMap(const std::string& inputImgName, std::ostream& ppmStream);

void
pixelMapToStream(const std::stringstream& ppmStream, std::ostream& outStream, const std::string& outputFormat);

void
pixelMapToNamedFile(const std::stringstream& ppmStream, const std::string& pngImgName);
}
#endif /* CONVERT_HPP */
