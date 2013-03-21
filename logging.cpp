#include "logging.hpp"
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

std::string programName = "transmogrifier";

std::ostream&
log() {

	return std::clog << "[" << boost::posix_time::second_clock::local_time() << "] " << programName << ": ";
}
