#pragma once

#include "strutil.h"

#include <fstream>
#include <json/json.h>

namespace toolbox {
namespace json {

inline Json::Value loadFromFile(const std::string& fname)
{
    std::istream* input_file;

#if 0
    if (strutil::extension(fname) == ".gz")
    {
        input_file = new igzstream(fname.c_str());
    }
    else
#endif
    input_file = new std::ifstream(fname, std::ifstream::binary);

    Json::Value json;
    Json::CharReaderBuilder rbuilder;
    std::string errs;
    bool parsing_ok = Json::parseFromStream(rbuilder, *input_file, &json, &errs);
    if (!parsing_ok)
        fprintf(stderr,
                "Failed to json data from file '%s': %s",
                fname.c_str(),
                errs.c_str());

    delete input_file;
    return json;
};

inline void saveToFile(const std::string& fname, const Json::Value& root)
{
    Json::StreamWriterBuilder wbuilder;
    std::ofstream json_stream(fname);
    json_stream << Json::writeString(wbuilder, root);
    json_stream.close();
};

} // namespace json
} // namespace toolbox
