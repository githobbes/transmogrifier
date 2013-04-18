#include <boost/python/module.hpp>
#include <boost/python/def.hpp>

#include "transmogrify.hpp"

BOOST_PYTHON_MODULE(transmogripy)
{
    boost::python::def("init", transmogrifier::init);
   boost::python::def("named_file_to_named_file", transmogrifier::namedFileToNamedFile);
}
