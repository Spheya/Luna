#include "utils.hpp"

#include <fstream>

namespace luna {

	std::string readFileToString(const char* filePath) {
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);
		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}
		size_t fileSize = (size_t)file.tellg();
		std::string buffer(fileSize, ' ');
		file.seekg(0);
		file.read(&buffer[0], fileSize);
		return buffer;
	}


	std::string readFileToString(const wchar_t* filePath) {
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);
		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}
		size_t fileSize = (size_t)file.tellg();
		std::string buffer(fileSize, ' ');
		file.seekg(0);
		file.read(&buffer[0], fileSize);
		return buffer;
	}

	std::vector<char> readFileToBytes(const char* filePath) {
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);
		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize, ' ');
		file.seekg(0);
		file.read(&buffer[0], fileSize);
		return buffer;
	}

	std::vector<char> readFileToBytes(const wchar_t* filePath) {
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);
		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize, ' ');
		file.seekg(0);
		file.read(&buffer[0], fileSize);
		return buffer;
	}

}