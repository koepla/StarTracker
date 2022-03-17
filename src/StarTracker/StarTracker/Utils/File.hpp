#ifndef STARTRACKER_UTILS_FILE_H
#define STARTRACKER_UTILS_FILE_H

#include <StarTracker/Core/Application.hpp>
#include <StarTracker/Core/Core.hpp>

#include <commdlg.h>

#include <filesystem>
#include <fstream>
#include <string>

namespace StarTracker::Utils {

	class File {

	public:
		[[nodiscard]] static std::string ReadFile(const std::filesystem::path& filePath) noexcept;
        [[nodiscard]] static std::vector<std::filesystem::path> OpenFileDialog(const std::string& title, bool allowMultiple) noexcept;
        [[nodiscard]] static std::filesystem::path SaveFileDialog(const std::string& title) noexcept;
	};
}

#endif // STARTRACKER_UTILS_FILE_H