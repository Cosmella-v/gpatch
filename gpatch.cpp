#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    const char* geode_env = std::getenv("GEODE_SDK");
    if (!geode_env) {
        std::cerr << "GEODE_SDK environment variable is not set????.\n";
        return 1;
    }

    fs::path sdk_path = geode_env;
    fs::path file = sdk_path / "cmake" / "GeodeFile.cmake";

    if (!fs::exists(file)) {
        std::cerr << "Could not find GeodeFile.cmake at: " << file << " 3: \n";
        return 1;
    }

    std::cout << "Patching: " << file << "\n";

    std::ifstream in(file);
    if (!in) {
        std::cerr << "Failed to open file 3:\n";
        return 1;
    }
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string content = buffer.str();
    in.close();

    std::string from = "message(FATAL_ERROR\n            \"Mod";
    std::string to   = "message(WARNING\n            \"Mod";

    size_t pos = content.find(from);
    if (pos == std::string::npos) {
        std::cout << "prob already patched or something fucked up happened 3:";
        return 0;
    }

    content.replace(pos, from.size(), to);

    std::ofstream out(file, std::ios::trunc);
    if (!out) {
        std::cerr << "Failed to open file 3:";
        return 1;
    }
    out << content;
    out.close();

    std::cout << "Patched specific FATAL_ERROR to WARNING block.\n";
    return 0;
}
