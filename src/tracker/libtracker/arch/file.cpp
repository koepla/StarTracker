#include <fstream>

#include "file.hpp"

namespace arch {

    std::string ReadFile(const std::filesystem::path& filePath) noexcept {
        std::ifstream fileIn{ filePath };
        if (fileIn.is_open()) {
            return std::string{ std::istreambuf_iterator{ fileIn }, std::istreambuf_iterator<char>{} };
        }
        return {};
    }

    bool WriteFile(const std::filesystem::path& filePath, std::string_view data) noexcept {
        std::ofstream fileOut{ filePath };
        if (fileOut.is_open()) {
            fileOut.write(data.data(), static_cast<std::streamsize>(data.size()));
            return true;
        }
        return false;
    }
}// namespace arch
