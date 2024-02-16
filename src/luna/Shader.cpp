#include "Shader.hpp"

namespace luna {

	Shader::Shader(const char* vertexSource, const char* fragmentSource) :
		m_program(vertexSource, fragmentSource) {
	}

	void Shader::load(const char* vertexSource, const char* fragmentSource) {
		m_program.load(vertexSource, fragmentSource);
	}

	ShaderProgram& Shader::getProgram() {
		return m_program;
	}

	const ShaderProgram& Shader::getProgram() const {
		return m_program;
	}

	bool Shader::isValid() const {
		return getProgram().isValid();
	}
}