#pragma once

#include <vector>
#include <string>
#include <filesystem>

namespace luna {

	std::string readFileToString(const char* filePath);
	std::string readFileToString(const wchar_t* filePath);

	std::vector<char> readFileToBytes(const char* filePath);
	std::vector<char> readFileToBytes(const wchar_t* filePath);

}