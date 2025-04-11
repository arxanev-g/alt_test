#pragma once
#include <string>


class FileWriter {
public:
    static void Save(const std::string& filename, const std::string& content);
};