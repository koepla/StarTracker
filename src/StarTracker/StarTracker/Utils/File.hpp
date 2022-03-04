#ifndef STARTRACKER_UTILS_FILE_H
#define STARTRACKER_UTILS_FILE_H

#include <filesystem>
#include <fstream>
#include <string>

namespace StarTracker::Utils {

	class File {

	public:
		[[nodiscard]] static std::string ReadFile(const std::filesystem::path& filePath) noexcept;
	};
}

#endif // STARTRACKER_UTILS_FILE_H