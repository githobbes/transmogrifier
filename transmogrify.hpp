#ifndef TRANSMOGRIFY_HPP
#define TRANSMOGRIFY_HPP

#include <string>

namespace transmogrifier {

void
init(char *argv[]);

void
namedFileToNamedFile(const std::string& inputFileName, const std::string& outputFileName, unsigned iterations);

}
#endif /* TRANSMOGRIFY_HPP */
