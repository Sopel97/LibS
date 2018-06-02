#pragma once

#include <string>
#include <fstream>

inline std::string readFile(const char* path)
{
    std::fstream file(path, std::ios::in);
    if (!file.is_open())
    {
        return "";
    }

    std::string contents;
    file.seekg(0, std::ios::end);
    contents.resize(static_cast<size_t>(file.tellg()));
    file.seekg(0, std::ios::beg);
    file.read(contents.data(), contents.size());
    file.close();

    return contents;
}
