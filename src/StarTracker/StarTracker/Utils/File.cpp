#include "File.hpp"

namespace StarTracker::Utils {

    static std::vector<std::string> splitString(const std::string& string, const std::string& delimiter) noexcept {

        std::size_t posStart = 0;
        std::size_t posEnd = 0;
        std::size_t delimiterLength = delimiter.length();

        std::string token{};
        std::vector<std::string> result{};

        while ((posEnd = string.find (delimiter, posStart)) != std::string::npos) {

            token = string.substr (posStart, posEnd - posStart);
            posStart = posEnd + delimiterLength;
            result.emplace_back(token);
        }

        result.emplace_back(string.substr (posStart));

        return result;
    }

	std::string File::ReadFile(const std::filesystem::path& filePath) noexcept {

		std::ifstream fileIn{ filePath };

		if (fileIn.is_open()) {

			return std::string{ std::istreambuf_iterator<char>{ fileIn }, std::istreambuf_iterator<char>{} };
		}
		else {

			return std::string{};
		}
	}

    std::vector<std::filesystem::path> File::OpenFileDialog(const std::string& title, bool allowMultiple) noexcept {

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

            char *p = ofn.lpstrFile;
            std::string path = p;
            p += path.size() + 1;

            if (*p == 0) {

                filePaths.emplace_back(path.c_str());
            }
            else {

                while (*p != 0) {

                    std::string fileName = p;
                    std::string result{};
                    result = result.append(path).append("\\").append(fileName);
                    filePaths.emplace_back(result);
                    p += fileName.size() + 1;
                }
            }

            return filePaths;
        }
        else {

            return std::vector<std::filesystem::path>{};
        }
    }

    std::filesystem::path File::SaveFileDialog(const std::string& title) noexcept {

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
        else {

            return std::filesystem::path{};
        }
    }
}