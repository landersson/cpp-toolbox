
#pragma once

#include <iomanip>
#include <sstream>
#include <ctype.h>
#include <cmath>

namespace toolbox {
namespace strutil {

template<typename T>
inline std::string to_string(const T& t, int width=0, int base=10, int precision=-1, bool fixed=false)
{
    // convert atomic type to std::string if possible
    std::ostringstream oss;
    if (precision != -1) oss << std::setprecision(precision);
    if (width != 0) oss << std::setw(width) << std::setfill('0');
    if (base != 10) oss << std::setbase(base);
    if (fixed) oss << std::fixed;
    oss << t;
    return oss.str();
}

// trim from left
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from right
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

inline std::string underscore_2_camel_case(const std::string & input)
{
    std::string camel;
    bool next_upper = false;
    for (auto c : input)
    {
        if (c == '_')
            next_upper=true;
        else
        {
            if (next_upper)
            {
                camel.push_back(toupper(c));
                next_upper = false;
            }
            else
                camel.push_back(c);
        }
    }
    return camel;
}

// join range of printable items
template <typename Iter>
std::string join(Iter begin, Iter end, std::string const& separator)
{
  std::ostringstream result;
  if (begin != end)
    result << *begin++;
  while (begin != end)
    result << separator << *begin++;
  return result.str();
}

template <typename T>
std::string join(const std::vector<T> & v, std::string const& separator = " ")
{
    return join(v.begin(), v.end(), separator);
}


// get filename extension
inline std::string extension(const std::string &fname)
{
    auto last_dot = fname.find_last_of('.');
    if (last_dot == std::string::npos)
        return "";
    return fname.substr(last_dot, std::string::npos);
}

// get filename extension
inline std::string without_extension(const std::string &fname)
{
    return fname.substr(0, fname.find_last_of('.'));
}

inline bool contains(const std::string & haystack, char needle)
{
    return haystack.find(needle) != std::string::npos;
}
inline bool is_valid_number(const std::string & str)\
{
    char const *str_begin = str.c_str();
    char *str_end;
    float x = strtof(str_begin, &str_end);

    return str_end != str_begin && !std::isnan(x);
}

}
}
