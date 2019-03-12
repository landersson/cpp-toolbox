
#pragma once

#include "ProgramOption.h"

#include <boost/program_options.hpp>

#include <tuple>
#include <vector>

namespace toolbox {

class ProgramOptions : public boost::program_options::variables_map
{
public:
    ProgramOptions();
    void addOptions(const std::string& name, ProgramOption* options);
    void parseCommandLine(int argc, char** argv);
    void parseConfigFile(const std::string& config_fname, const char* app_name);
    std::string toString();

    // returns true if option was excplicitly specified with a non-empty value
    bool specified(const std::string& option_name)
    {
        return !(this->operator[](option_name).defaulted() ||
                 this->operator[](option_name).empty());
    }

    // shortcut for parsing option value as an int
    int getInt(const std::string& option_name)
    {
        return this->operator[](option_name).as<int>();
    }

    // shortcut for parsing option value as an int
    float getFloat(const std::string& option_name)
    {
        return this->operator[](option_name).as<float>();
    }

    // shortcut for parsing option value as a bool
    bool getBool(const std::string& option_name)
    {
        return this->operator[](option_name).as<bool>();
    }

    // shortcut for parsing option value as a string
    std::string getString(const std::string& option_name) const
    {
        return this->operator[](option_name).as<std::string>();
    }
    // shortcut for parsing option value as a string
    const char* getCString(const std::string& option_name) const
    {
        return this->operator[](option_name).as<std::string>().c_str();
    }

    // shortcut for parsing option value as a list of strings, separated by
    // 'separator'
    std::vector<std::string> getStrings(const std::string& option_name,
                                        const std::string& separator = ",");

protected:
    boost::program_options::options_description _description;
};

namespace opt {

// namespaced option factory functions

enum Opt
{
    Required
};

// optional option with default value. if no default value is specified, the
// default value will be initialized using the default contructor of the given
// type.
template<typename T>
inline ProgramOption def(const char* name,
                         const char* description,
                         const T& default_value = T())
{
    return std::make_tuple(
        name,
        description,
        boost::program_options::value<T>()->default_value(default_value));
}

// optional option with default value. if no default value is specified, the
// default value will be initialized using the default contructor of the given
// type.
template<typename T>
inline ProgramOption implicit(const char* name,
                              const char* description,
                              const T& default_value = T())
{
    return std::make_tuple(
        name,
        description,
        boost::program_options::value<T>()->implicit_value(default_value));
}

// required, no default value version of above
template<typename T>
inline ProgramOption def(const char* name, const char* description, Opt r)
{
    auto val = boost::program_options::value<T>();
    val = val->required();
    return std::make_tuple(name, description, val);
}

// bool specialization, no arguments allowed, set to true if specified w.o
// argument
template<>
inline ProgramOption def(const char* name,
                         const char* description,
                         const bool& default_value)
{
    return std::make_tuple(name,
                           description,
                           boost::program_options::value<bool>()
                               ->default_value(default_value)
                               ->implicit_value(true)
                               ->zero_tokens());
}

/** multitoken option

 Code:
     lc::opt::def::multitoken<std::vector<int>>("multi", "help")

     if (!_options["multi"].empty())
         for (int i : _options["multi"].as<std::vector<int>>()) { .. }

 Usage:
   --multi 1 2 3
   or
   --multi 1 --multi 2 3
 */

template<typename T>
inline ProgramOption multitoken(const char* name, const char* description)
{
    return std::make_tuple(
        name, description, boost::program_options::value<T>()->multitoken());
}

#include "multichoice_impl.h"

/**
 multi choice (enum) option

 Code:
     lc::opt::choice<std::string>("tool", "required option", {"hammer", "saw"})
 or lc::opt::choice<std::string>("tool", "required option", {"hammer", "saw"},
 lc::opt::def::Required) */

// optional, with default value
template<typename T>
inline ProgramOption choice(const char* name,
                            const char* description,
                            const std::vector<T>& choices,
                            T default_value = T())
{
    assert(choices.size() > 0);

    if (default_value != T())
    {
        if (std::find(choices.begin(), choices.end(), default_value) == choices.end())
            assert(0);
    }
    auto val = new multichoice_value<T>(choices);
    val->value_name(std::string("<") + val->formatChoices() + ">");
    val->default_value(default_value);

    return std::make_tuple(name, description, val);
}

// required, with no default value
template<typename T>
inline ProgramOption choice(const char* name,
                            const char* description,
                            const std::vector<T>& choices,
                            Opt r)
{
    auto val = new multichoice_value<T>(choices);
    val->value_name(std::string("<") + val->formatChoices() + ">");
    val = val->required();

    return std::make_tuple(name, description, val);
}

// option definition list terminator
inline ProgramOption end()
{
    return std::make_tuple("", "", nullptr);
}

} // namespace opt

} // namespace toolbox
