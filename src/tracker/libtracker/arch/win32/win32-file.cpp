#include "arch/file.hpp"
#include "arch/os.hpp"

#include <commdlg.h>

namespace arch {

    std::vector<std::filesystem::path> OpenFileDialog(const std::string& title, bool allowMultiple) noexcept {
        char file[MAX_PATH] = { 0 };

        OPENFILENAMEA ofn{};
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = nullptr;
        ofn.lpstrFile = file;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = "All Files\0*.*";
        ofn.lpstrTitle = title.c_str();
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_EXPLORER;
        if (allowMultiple) {
            ofn.Flags |= OFN_ALLOWMULTISELECT;
        }

        if (GetOpenFileNameA(&ofn) == TRUE) {
            std::vector<std::filesystem::path> filePaths{};

            auto rawFileNames = ofn.lpstrFile;
            auto path = std::string(rawFileNames);
            rawFileNames += path.size() + 1;

            if (*rawFileNames == 0) {
                filePaths.emplace_back(std::move(path));
            } else {
                while (*rawFileNames != 0) {
                    std::string fileName = rawFileNames;
                    std::string result{};
                    result = result.append(path).append("\\").append(fileName);
                    filePaths.emplace_back(result);
                    rawFileNames += fileName.size() + 1;
                }
            }
            return filePaths;
        }
        return {};
    }

    std::filesystem::path SaveFileDialog(const std::string& title) noexcept {
        char file[MAX_PATH] = { 0 };

        OPENFILENAMEA ofn{};
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = nullptr;
        ofn.lpstrFile = file;
        ofn.lpstrTitle = title.c_str();
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrFilter = "All Files\0*.*";
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST;

        if (GetSaveFileNameA(&ofn) == 1) {
            return std::filesystem::path{ ofn.lpstrFile };
        }
        return {};
    }
}// namespace arch