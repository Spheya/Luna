#pragma once

#include <string>
#include <string_view>
#include <filesystem>

#include "rendertype.hpp"

namespace luna {

	enum class ShaderStage {
		Vertex = 0,
		Fragment = 1
	};

	struct ShaderSource {
		std::string code;
		ShaderStage stage;
		RenderType renderType;

		static ShaderSource fromString(std::string_view source, std::string_view fileName, const std::filesystem::path& includePath, ShaderStage stage);
		static ShaderSource fromFile(const std::filesystem::path& filepath, ShaderStage stage);
	};

}