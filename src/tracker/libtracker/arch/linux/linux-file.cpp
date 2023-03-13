#include <filesystem>

#include <fmt/format.h>

#include "arch/file.hpp"
#include "utility/conversion.hpp"

namespace arch {

    namespace {

        std::string OpenZenity(std::string_view title, bool multiple) {
            const auto command = fmt::format(R"(zenity --file-selection --modal --separator=";" --title="{}"{})", title,
                                             multiple ? " --multiple" : "");

            const auto process = popen(command.c_str(), "r");
            if (!process) {
                return {};
            }

            // Allocate some memory so that we can store a larger number of paths
            char buffer[4096];
            while (fgets(buffer, sizeof buffer, process) != nullptr) { }
            pclose(process);

            auto paths = std::string{ buffer };
            if (!paths.empty() && paths.at(paths.size() - 1) == '\n') {
                return std::string{ paths.begin(), paths.end() - 1 };
            }
            return paths;
        }
    }// namespace

    std::vector<std::filesystem::path> OpenFileDialog(const std::string& title, bool allowMultiple) noexcept {
        const auto paths = OpenZenity(title, allowMultiple);

        std::vector<std::filesystem::path> target;
        utility::Split<decltype(target), std::string_view>(target, paths, ";");
        return target;
    }

    std::filesystem::path SaveFileDialog(const std::string& title) noexcept {
        return OpenZenity(title, false);
    }
}// namespace arch
