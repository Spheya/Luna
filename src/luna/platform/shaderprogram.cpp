#include "shaderprogram.hpp"

#include "../rendering/propertyblock.hpp"

namespace luna {

	static constexpr GLenum shaderTypes[] = {
		GL_VERTEX_SHADER,
		GL_FRAGMENT_SHADER
	};

	static constexpr const char* shaderNames[] = {
		"Vertex",
		"Fragment"
	};

	static std::string getProgramInfoLog(GLuint program) {
		GLint infoLogLength;
		LUNA_GLCALL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength));
		std::string infoLog(infoLogLength, ' ');
		LUNA_GLCALL(glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog.data()));
		return infoLog;
	}

	static std::string getShaderInfoLog(GLuint shader) {
		GLint infoLogLength;
		LUNA_GLCALL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength));
		std::string infoLog(infoLogLength, ' ');
		LUNA_GLCALL(glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog.data()));
		return infoLog;
	}

	static GLuint compileModule(const ShaderSource& shaderModule) {
		auto shader = LUNA_GLCALL(glCreateShader(shaderTypes[int(shaderModule.stage)]));
		const char* source = shaderModule.code.c_str();
		LUNA_GLCALL(glShaderSource(shader, 1, &source, nullptr));

		GLint status;
		LUNA_GLCALL(glCompileShader(shader));
		LUNA_GLCALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));
		if (status == GL_FALSE) {
			error("{} shader compilation failed:\n{}", shaderNames[int(shaderModule.stage)], getShaderInfoLog(shader));
			LUNA_GLCALL(glDeleteShader(shader));
			return 0;
		}

		return shader;
	}

	static void bindUniformBlock(GLuint program, const char* name, GLuint binding) {
		GLuint idx = LUNA_GLCALL(glGetUniformBlockIndex(program, name));
		if (idx != GL_INVALID_INDEX)
			LUNA_GLCALL(glUniformBlockBinding(program, idx, binding));
	}

	ShaderProgram::ShaderProgram(std::span<const std::string> samplers, std::span<const ShaderSource> modules) :
		m_gpuHandle(glCreateProgram())
	{
		std::vector<GLuint> compiledModules;

		// Compile all individual modules
		for (auto shaderModule : modules) {
			auto compiledShader = compileModule(shaderModule);
			if (compiledShader == 0) {
				// Compilation failed, delete all modules
				for (auto compileModule : compiledModules)
					LUNA_GLCALL(glDeleteShader(compileModule));

				LUNA_GLCALL(glDeleteProgram(m_gpuHandle));
				m_gpuHandle = 0;
				return;
			}

			// Attach shader to program
			LUNA_GLCALL(glAttachShader(m_gpuHandle, compiledShader));
			compiledModules.push_back(compiledShader);
		}

		// Link and validate the program
		GLint status;
		LUNA_GLCALL(glLinkProgram(m_gpuHandle));
		LUNA_GLCALL(glGetProgramiv(m_gpuHandle, GL_LINK_STATUS, &status));
		if (status == GL_FALSE) {
			error("Shader program linking failed:\n{}", getProgramInfoLog(m_gpuHandle));
			LUNA_GLCALL(glDeleteProgram(m_gpuHandle));
			m_gpuHandle = 0;
		} else {
			LUNA_GLCALL(glValidateProgram(m_gpuHandle));
			LUNA_GLCALL(glGetProgramiv(m_gpuHandle, GL_VALIDATE_STATUS, &status));
			if (status == GL_FALSE) {
				error("Shader program validation failed:\n{}", getProgramInfoLog(m_gpuHandle));
				LUNA_GLCALL(glDeleteProgram(m_gpuHandle));
				m_gpuHandle = 0;
			}
		}

		// Deallocate the modules, they are no longer needed after the program is created
		for (auto compileModule : compiledModules)
			LUNA_GLCALL(glDeleteShader(compileModule));

		// Bind the uniform blocks
		bindUniformBlock(m_gpuHandle, "LunaCamera",     GLuint(PropertyBlockType::Camera));
		bindUniformBlock(m_gpuHandle, "LunaRenderType", GLuint(PropertyBlockType::RenderType));
		bindUniformBlock(m_gpuHandle, "LunaMaterial",   GLuint(PropertyBlockType::Material));
		bindUniformBlock(m_gpuHandle, "LunaInstance",   GLuint(PropertyBlockType::Instance));
		bindUniformBlock(m_gpuHandle, "LunaTransform",  4);

		// Bind the samplers
		glUseProgram(m_gpuHandle);
		for (size_t i = 0; i < samplers.size(); ++i)
			glUniform1i(glGetUniformLocation(m_gpuHandle, samplers[i].c_str()), i);

		if(m_gpuHandle)
			trace("Shader program created (id={})", m_gpuHandle);
	}

	ShaderProgram::ShaderProgram(std::initializer_list<std::string> samplers, std::initializer_list<const ShaderSource> modules) :
		ShaderProgram(std::span(samplers.begin(), samplers.size()), std::span(modules.begin(), modules.size()))
	{}

	ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept :
		m_gpuHandle(other.m_gpuHandle)
	{
		other.m_gpuHandle = 0;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
		if(m_gpuHandle)
			LUNA_GLCALL(glDeleteProgram(m_gpuHandle));
		m_gpuHandle = other.m_gpuHandle;
		other.m_gpuHandle = 0;
		return *this;
	}

	ShaderProgram::~ShaderProgram() {
		if (m_gpuHandle) {
			LUNA_GLCALL(glDeleteProgram(m_gpuHandle));
			trace("ShaderProgram destroyed (id={})", m_gpuHandle);
		}
	}

	bool ShaderProgram::isValid() const {
		return bool(m_gpuHandle);
	}

	GLuint ShaderProgram::internalHandle() const {
		return m_gpuHandle;
	}

}