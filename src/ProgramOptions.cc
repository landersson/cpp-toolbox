
//#include "common/core/Log.h"
#include <cpp-toolbox/ProgramOptions.h>
#include <cpp-toolbox/strutil.h>
#include <cpp-toolbox/tokenize.h>

#include <boost/algorithm/string.hpp>

#include <fstream>

namespace toolbox {

ProgramOptions::ProgramOptions() {}

void
ProgramOptions::addOptions(const std::string& name, ProgramOption* options)
{
    if (options != nullptr) {
        boost::program_options::options_description description(name);
        while (std::get<2>(*options) != nullptr) {
            description.add_options()(std::get<0>(*options).c_str(),
                                      std::get<2>(*options),
                                      std::get<1>(*options).c_str());
            options++;
        }
        _description.add(description);
    }
}

std::vector<std::string>
ProgramOptions::getStrings(const std::string& option_name,
                           const std::string& separator)
{
    std::vector<std::string> empty;
    //    return empty;
    return strutil::split(this->operator[](option_name).as<std::string>(),
                          separator);
}

void
ProgramOptions::parseCommandLine(int argc, char** argv)
{
    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, _description),
        *this);
    // dont notify here, since any options specified as required will terminate
    // the program with an error message before we even have a chance to print
    // help.
    //    self->notify();
}

std::string
ProgramOptions::toString()
{
    return strutil::to_string(_description);
}

}
