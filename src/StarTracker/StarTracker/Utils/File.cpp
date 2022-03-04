#include "File.hpp"

namespace StarTracker::Utils {

	std::string File::ReadFile(const std::filesystem::path& filePath) noexcept {

		std::ifstream fileIn{ filePath };

		if (fileIn.is_open()) {

			return std::string{ std::istreambuf_iterator<char>{ fileIn }, std::istreambuf_iterator<char>{} };
		}
		else {

			return std::string{};
		}
	}
}