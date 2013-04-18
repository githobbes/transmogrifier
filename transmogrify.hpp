#ifndef TRANSMOGRIFY_HPP
#define TRANSMOGRIFY_HPP

#include <string>

namespace transmogrifier {

void
init(char *argv[]);

void
streamToStream(const std::istream& inputSteam, std::ostream& outStream, const std::string& outputFormat, unsigned iterations);

void
streamToNamedFile(const std::istream& inputStream, const std::string& outputFileName, unsigned iterations);

void
namedFileToStream(const std::string& inputFileName, std::ostream& outStream, const std::string& outputFormat, unsigned iterations);

void
namedFileToNamedFile(const std::string& inputFileName, const std::string& outputFileName, unsigned iterations);

}
#endif /* TRANSMOGRIFY_HPP */
