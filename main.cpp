#include <iostream>

void PrintUsage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " <branch1> <branch2>\n"
              << "Example: " << program_name << " Sisyphus p10\n";
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        PrintUsage(argv[0]);
        return 1;
    }

    const std::string branch1 = argv[1];
    const std::string branch2 = argv[2];
    const std::string output_filename = "result.json";
}