#ifndef CONVERT_HPP
#define CONVERT_HPP

#include <string>
#include <ostream>
#include <istream>

void
toPixelMap(const std::string& inputImgName, std::ostream& ppmStream, std::ostream& pgmStream);

void
toPNG(const istream& ppmStream, const string& pngImgName);

#endif /* CONVERT_HPP */
