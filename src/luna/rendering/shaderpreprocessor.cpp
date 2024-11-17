#include "shaderpreprocessor.hpp"

#include "../core/logger.hpp"
#include "../core/utils.hpp"

namespace luna {

	constexpr const char* RequiredLunaHeader = 
		"#version 330\n"
		"#define PropertyBlock(x) layout(std140) uniform Luna ## x\n";

	constexpr const char* LunaCoreVertexFile =
		"layout(location = 0) in vec3 VertexPosition;\n"
		"layout(location = 1) in vec2 VertexUv;\n"
		"layout(location = 2) in vec3 VertexNormal;\n"
		"layout(location = 3) in vec4 VertexColor;\n"
		"layout(location = 4) in mat4 _ModelMat;\n"
		"PropertyBlock(Camera) {\n"
		"  mat4 _ProjMat;\n"
		"  mat4 _InvProjMat;\n"
		"  mat4 _ViewMat;\n"
		"  mat4 _InvViewMat;\n"
		"  mat4 _ProjViewMat;\n"
		"  uvec2 _TargetSize;\n"
		"  vec3 _CameraPosition;\n"
		"};\n";

	constexpr const char* LunaCoreFile =
		"PropertyBlock(Camera) {\n"
		"  mat4 _ProjMat;\n"
		"  mat4 _InvProjMat;\n"
		"  mat4 _ViewMat;\n"
		"  mat4 _InvViewMat;\n"
		"  mat4 _ProjViewMat;\n"
		"  uvec2 _TargetSize;\n"
		"  vec3 _CameraPosition;\n"
		"};\n";

	constexpr const char* LunaForwardFile =
		"struct Light { vec3 color; vec4 position; };\n"
		"PropertyBlock(RenderType) {\n"
		"  Light[16] _LightSources;\n"
		"  int _NumLights;\n"
		"};\n";

	static const char* getVirtualLunaShaderFile(std::string_view path, ShaderStage stage) {
		if (path == "luna/core.glsl") 
			return stage == ShaderStage::Vertex ? LunaCoreVertexFile : LunaCoreFile;

		if (path == "luna/forward.glsl")
			return LunaForwardFile;

		return nullptr;
	}

	static std::string handleIncludes (std::string_view source, std::string_view filename, const std::filesystem::path& includePath, ShaderStage stage);

	static bool handleIncludeDirective(std::string_view directive, std::string_view filename, int line, const std::filesystem::path& includePath, std::stringstream& stream, ShaderStage stage) {
		directive.remove_prefix(sizeof("#include"));

		auto includePathStart = directive.find('<');
		if (includePathStart == std::string_view::npos) {
			luna::error("Shader preprocessing error: {}({}) : expected '<' after #include", filename, line);
			return false;
		}

		auto includePathEnd = directive.find('>', includePathStart);
		if (includePathEnd == std::string_view::npos) {
			luna::error("Shader preprocessing error: {}({}) : expected '>' after '<'", filename, line);
			return false;
		}

		auto filePath = directive.substr(includePathStart + 1, includePathEnd - includePathStart - 1);
		if (filePath.starts_with("luna/")) {
			const char* fileContent = getVirtualLunaShaderFile(filePath, stage);
			if (!fileContent) {
				luna::error("Shader preprocessing error: {}({}) : Invalid file include '{}', paths starting with 'luna/' are reserved for the Luna shader library", filename, line, filePath);
				return false;
			}
			stream << "#line 0 \"" << filePath << "\"\n" << fileContent << "\n#line " << line << " \"" << filename << "\"\n";
		} else {
			auto file = includePath / filePath;
			auto content = handleIncludes(readFileToString(file.c_str()), file.filename().string(), file.parent_path(), stage);
			stream << content << "\n#line " << line << " \"" << filename << "\"\n";
		}

		return true;
	}

	static std::string handleIncludes(std::string_view source, std::string_view filename, const std::filesystem::path& includePath, ShaderStage stage) {
		std::stringstream stream;
		stream << "#line -1 \"" << filename << "\"\n";
		std::string_view::size_type lineStart = 0;
		if (source.empty())
			lineStart = std::string_view::npos;

		int lineIdx = 0;
		while (lineStart < source.size()) {
			auto lineEnd = source.find('\n', lineStart);
			lineEnd = (lineEnd == std::string_view::npos) ? source.size() : lineEnd + 1;
			auto line = source.substr(lineStart, lineEnd - lineStart);
			auto directiveStart = line.find_first_not_of(" \t");
			auto directive = (directiveStart == std::string_view::npos) ? line : line.substr(directiveStart);

			if (directive.starts_with("#include")) {
				if (!handleIncludeDirective(directive, filename, lineIdx, includePath, stream, stage))
					stream << line;
			} else {
				stream << line;
			}

			lineStart = lineEnd;
			++lineIdx;
		}
		return stream.str();
	}

	ShaderSource ShaderSource::fromString(std::string_view source, std::string_view fileName, const std::filesystem::path& includePath, ShaderStage stage) {
		ShaderSource result{};
		result.stage = stage;
		result.renderType = NullRenderType;

		std::stringstream stream;
		stream << RequiredLunaHeader << handleIncludes(source, fileName, includePath, stage) << std::endl;
		result.code = stream.str();
		return result;
	}

	ShaderSource ShaderSource::fromFile(const std::filesystem::path& filepath, ShaderStage stage) {
		return fromString(readFileToString(filepath.c_str()), filepath.filename().string(), filepath.parent_path(), stage);
	}

}