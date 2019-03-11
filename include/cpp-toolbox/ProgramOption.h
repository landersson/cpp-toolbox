#pragma once

#include <string>
#include <tuple>

namespace boost {
namespace program_options {
class value_semantic;
}
}

namespace toolbox {
typedef std::
    tuple<std::string, std::string, boost::program_options::value_semantic*>
        ProgramOption;

}
