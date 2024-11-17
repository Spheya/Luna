#pragma once

#include <string>
#include <span>
#include <glad/glad.h>

#include "platform.hpp"
#include "../rendering/shaderpreprocessor.hpp"

namespace luna {

	class ShaderProgram {
	public:
		ShaderProgram(std::span<const std::string> samplers, std::span<const ShaderSource> modules);
		ShaderProgram(std::initializer_list<std::string> samplers, std::initializer_list<const ShaderSource> modules);
		ShaderProgram(ShaderProgram&& other) noexcept;
		ShaderProgram& operator=(ShaderProgram&& other) noexcept;
		~ShaderProgram();

		bool isValid() const;

		GLuint internalHandle() const;

	private:
		GLuint m_gpuHandle;
	};

}