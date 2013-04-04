#ifndef CONVERT_HPP
#define CONVERT_HPP

#include <string>
#include <ostream>
#include <istream>

namespace transmogrifier {
void
streamToPixelMap(const std::istream& inputStream, std::ostream& ppmStream, std::ostream& pgmStream);

void
namedFileToPixelMap(const std::string& inputImgName, std::ostream& ppmStream, std::ostream& pgmStream);

void
writeImage(const std::stringstream& ppmStream, const std::string& pngImgName);
}
#endif /* CONVERT_HPP */
