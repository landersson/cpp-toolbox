
#pragma once

#include <cstdio>
#include <string>
#include <vector>

namespace toolbox {

template<typename T>
std::vector<T>
load_vector_from_file(const std::string& filename)
{
    std::vector<T> data;

    FILE* fp = std::fopen(filename.c_str(), "rb");
    if (fp == nullptr)
        return data;

    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    data.resize((file_size + sizeof(T) - 1) / sizeof(T));

    size_t bytes_read = fread(data.data(), 1, file_size, fp);
    fclose(fp);

    if (bytes_read != file_size)
        data.resize(0);

    return data;
}
}
