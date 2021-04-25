#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

namespace util {

	std::string read_file(const std::string& path) {

		std::ifstream fin = std::ifstream(path, std::ios::in);
		std::string content;

		if (fin) {
			fin.seekg(0, std::ios::end);
			content.resize((size_t)fin.tellg());
			fin.seekg(0, std::ios::beg);
			fin.read(&content[0], content.size());
		}

		return content;
	}

	std::vector<std::string> split_str(const std::string& str, char delimiter) {

		std::vector<std::string> result;
		size_t last = 0;
		size_t next = 0;
		while ((next = str.find(delimiter, last)) != std::string::npos) {

			result.push_back(str.substr(last, next - last));
			last = next + +1;
		}
		result.push_back(str.substr(last));

		return result;
	}
}