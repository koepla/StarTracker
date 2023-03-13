#ifndef LIBTRACKER_ARCH_FILE_H
#define LIBTRACKER_ARCH_FILE_H

#include <filesystem>
#include <string>

#include "core/application.hpp"
#include "utility/types.hpp"

namespace arch {

    /**
     * @brief Reads the content of the specified file and returns it as a std::string
     * @param filePath path of the file
     * @return content of the file
     */
    std::string ReadFile(const std::filesystem::path& filePath) noexcept;

    /**
     * @brief Reads the content of the specified file and returns it as a std::string
     * @param filePath path of the file
     * @return content of the file
     */
    bool WriteFile(const std::filesystem::path& filePath, std::string_view data) noexcept;

    /**
     * @brief Opens an Open-File-Dialog and returns the paths for the selected files
     *        Note that if the parameter allowMultiple is set to true, only one path
     *        is in the resulting vector
     * @param title title of the dialog-window
     * @param allowMultiple controls if multiple files can be selected or only one
     * @return std::vector of the selected paths
     */
    std::vector<std::filesystem::path> OpenFileDialog(const std::string& title, bool allowMultiple) noexcept;

    /**
     * @brief Opens an Save-File-Dialog and returns the paths of the selected file
     * @param title title of the dialog-window
     * @return path of the selected file
     */
    std::filesystem::path SaveFileDialog(const std::string& title) noexcept;
}// namespace arch

#endif// LIBTRACKER_ARCH_FILE_H