#include <filesystem>
#include <sstream>

#include <fmt/format.h>

#include "arch/file.hpp"

namespace arch {

    namespace {

        std::string RunAppleScript(const std::string& script) {
            std::string command = "osascript -e '" + script + "'";
            std::array<char, 512> buffer;
            std::string result;

            FILE* pipe = popen(command.c_str(), "r");
            if (!pipe)
                return "";

            while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
                result += buffer.data();
            }

            pclose(pipe);

            // Remove trailing newline
            if (!result.empty() && result.back() == '\n')
                result.pop_back();

            return result;
        }

    }// namespace

    std::vector<std::filesystem::path> OpenFileDialog(const std::string& title, bool allowMultiple) noexcept {
        std::vector<std::filesystem::path> paths;

        std::ostringstream script;
        script << "set chosenFiles to ";
        if (allowMultiple)
            script << "choose file with prompt \"" << title << "\" with multiple selections allowed";
        else
            script << "choose file with prompt \"" << title << "\"";

        script << "\nset filePaths to {}\nrepeat with f in chosenFiles\n"
               << "set end of filePaths to POSIX path of f\nend repeat\n"
               << "return filePaths as string";

        std::string output = RunAppleScript(script.str());

        // Split result by ", " if multiple files were selected
        std::stringstream ss(output);
        std::string item;

        while (std::getline(ss, item, ',')) {
            // Remove leading space from AppleScript list entries
            if (!item.empty() && item[0] == ' ')
                item.erase(0, 1);

            paths.emplace_back(item);
        }

        return paths;
    }

    std::filesystem::path SaveFileDialog(const std::string& title) noexcept {
        std::ostringstream script;
        script << "set filePath to POSIX path of (choose file name with prompt \"" << title << "\")\n"
               << "return filePath";

        std::string output = RunAppleScript(script.str());

        return std::filesystem::path(output);
    }
}// namespace arch
