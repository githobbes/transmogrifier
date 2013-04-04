#include "logging.hpp"
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

std::string transmogrifier::programName = "transmogrifier";

std::ostream&
transmogrifier::log() {
	return std::clog << "[" << boost::posix_time::second_clock::local_time() << "] " << transmogrifier::programName << ": ";
}
