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
        ofn.Flags = OFN_PATHMUSTEXIST;

        if(allowMultiple) {

            ofn.Flags |= OFN_ALLOWMULTISELECT;
        }

        if (GetOpenFileNameA(&ofn) == TRUE) {

            std::vector<std::filesystem::path> result{};
            const auto paths = splitString(ofn.lpstrFile, " ");

            if(paths.empty()) {

                return std::vector<std::filesystem::path>{};
            }

            const auto directory = std::filesystem::path{ paths.at(0) };
            for(auto i = std::size_t{ 1 }; i < paths.size(); i++) {

                result.emplace_back(directory / std::filesystem::path{ paths.at(i) });
            }

            return result;
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