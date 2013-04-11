#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <ostream>
#include <string>

namespace transmogrifier {

extern std::string programName;

std::ostream&
log();

std::ostream&
error();
}

#endif /* LOGGING_HPP */
