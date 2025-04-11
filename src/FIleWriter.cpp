#include "../include/FileWriter.h"
#include <fstream>
#include <iostream>

void FileWriter::Save(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (file) {
        file << content;
        std::cout << "Results saved to " << filename << std::endl;
    } else {
        throw std::runtime_error("Failed to open file: " + filename);
    }
}