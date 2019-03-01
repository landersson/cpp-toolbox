#pragma once

#include "strutil.h"

#include <json/json.h>
#include <fstream>

namespace toolbox {
namespace json {

inline Json::Value loadFromFile(const std::string& fname)
{
    std::istream *input_file;

#if 0
    if (strutil::extension(fname) == ".gz")
    {
        input_file = new igzstream(fname.c_str());
    }
    else
#endif
    input_file = new std::ifstream(fname, std::ifstream::binary);

    Json::Value json;
    if (!Json::Reader().parse(*input_file, json, false))
        fprintf(stderr, "Failed to json data from file '%s'", fname.c_str());
//        LOGE << "Failed to load " << fname;

    delete input_file;
    return json;
};


inline void saveToFile(const std::string& fname, const Json::Value& root)
{
    std::ofstream json_stream(fname);
    json_stream << Json::StyledWriter().write(root);
    json_stream.close();
};


}
}
