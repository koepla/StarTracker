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
		/**
		* @brief Reads the content of the specified file and returns it as a std::string
		*
		* @param filePath path of the file
		*
		* @return content of the file
		* 
		*/
		[[nodiscard]] static std::string ReadFile(const std::filesystem::path& filePath) noexcept;

		/**
		* @brief Opens an Open-File-Dialog and returns the paths for the selected files
		*        Note that if the parameter allowMultiple is set to true, only one path 
		*        is in the resulting vector
		*
		* @param title title of the dialog-window
		*
		* @param allowMultiple controls if multiple files can be selected or only one
		*
		* @return std::vector of the selected paths
		* 
		*/
        [[nodiscard]] static std::vector<std::filesystem::path> OpenFileDialog(const std::string& title, bool allowMultiple) noexcept;

		/**
		* @brief Opens an Save-File-Dialog and returns the paths of the selected file
		*
		* @param title title of the dialog-window
		*
		* @return path of the selected file
		* 
		*/
        [[nodiscard]] static std::filesystem::path SaveFileDialog(const std::string& title) noexcept;
	};
}

#endif // STARTRACKER_UTILS_FILE_H