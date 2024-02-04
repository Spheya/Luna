#include "Shader.hpp"

#include <string>
#include <glad/glad.h>
#include "Logger.hpp"

namespace luna {

	namespace {
		GLuint loadShader(const char* source, GLenum type) {
			GLuint shader = glCreateShader(type);
			GLint len = GLint(std::strlen(source));
			glShaderSource(shader, 1, &source, &len);

			glCompileShader(shader);
			int status;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE) {
				char infoLog[512];
				glGetShaderInfoLog(shader, 512, nullptr, infoLog);
				log(std::string("Shader compilation failed:\n")+ infoLog, MessageSeverity::Error);
				return 0;
			}

			return shader;
		}
	}

	Shader::Shader() :
		m_program(0),
		m_valid(false)
	{}

	Shader::Shader(const char* vertexSource, const char* fragmentSource) :
		Shader()
	{
		load(vertexSource, fragmentSource);
	}

	Shader::Shader(Shader&& other) noexcept :
		m_program(other.m_program),
		m_valid(other.m_valid)
	{
		other.m_program = 0;
	}

	Shader& Shader::operator=(Shader&& other) noexcept {
		if (m_program != 0)
			glDeleteProgram(m_program);

		m_program = other.m_program;
		m_valid = other.m_valid;
		other.m_program = 0;

		return *this;
	}

	Shader::~Shader() {
		if (m_program != 0)
			glDeleteProgram(m_program);
	}

	void Shader::load(const char* vertexSource, const char* fragmentSource) {
		if (m_program != 0)
			glDeleteProgram(m_program);

		GLuint vertex = loadShader(vertexSource, GL_VERTEX_SHADER);
		GLuint fragment = loadShader(vertexSource, GL_FRAGMENT_SHADER);
		if (vertex && fragment) {
			m_program = glCreateProgram();
			glAttachShader(m_program, vertex);
			glAttachShader(m_program, fragment);

			glLinkProgram(m_program);
			int status;
			glGetProgramiv(m_program, GL_LINK_STATUS, &status);
			if (status == GL_FALSE) {
				char infoLog[512];
				glGetProgramInfoLog(m_program, 512, nullptr, infoLog);
				log(std::string("Shader linking failed:\n") + infoLog, MessageSeverity::Error);

			} else {

				glValidateProgram(m_program);
				int status;
				glGetProgramiv(m_program, GL_VALIDATE_STATUS, &status);
				if (status == GL_FALSE) {
					char infoLog[512];
					glGetProgramInfoLog(m_program, 512, nullptr, infoLog);
					log(std::string("Shader validation failed:\n") + infoLog, MessageSeverity::Error);
				} else {
					m_valid = true;
				}
			}
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	bool Shader::isValid() const {
		return m_valid;
	}

	int Shader::uniformId(const char* name) const {
		return glGetUniformLocation(m_program, name);
	}

	void uniform(int id, float value) {
		glUniform1f(id, value);
	}

	void uniform(int id, glm::vec1 value) {
		glUniform1f(id, value.x);
	}

	void uniform(int id, glm::vec2 value) {
		glUniform2fv(id, 2, &value.x);
	}

	void uniform(int id, glm::vec3 value) {
		glUniform3fv(id, 3, &value.x);
	}

	void uniform(int id, glm::vec4 value) {
		glUniform4fv(id, 4, &value.x);
	}

	void uniform(int id, glm::mat3 value) {
		glUniformMatrix3fv(id, 9, false, &value[0][0]);
	}

	void uniform(int id, glm::mat4 value) {
		glUniformMatrix4fv(id, 16, false, &value[0][0]);
	}

	void Shader::bind() const {
		static const Shader* boundShader = nullptr;
		if (boundShader != this) {
			glUseProgram(m_program);
			boundShader = this;
		}
	}
}