#pragma once

#include "ShaderProgram.hpp"

namespace luna {

	class Shader {
	public:
		Shader() = default;
		Shader(const char* vertexSource, const char* fragmentSource);

		void load(const char* vertexSource, const char* fragmentSource);

		ShaderProgram& getProgram();
		const ShaderProgram& getProgram() const;

		bool isValid() const;

	private:
		ShaderProgram m_program;
	};

}