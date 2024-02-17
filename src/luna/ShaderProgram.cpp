#include "ShaderProgram.hpp"

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

	ShaderProgram::ShaderProgram() :
		m_program(0),
		m_valid(false)
	{}

	ShaderProgram::ShaderProgram(const char* vertexSource, const char* fragmentSource) :
		ShaderProgram()
	{
		load(vertexSource, fragmentSource);
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept :
		m_program(other.m_program),
		m_valid(other.m_valid)
	{
		other.m_program = 0;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
		if (m_program != 0)
			glDeleteProgram(m_program);

		m_program = other.m_program;
		m_valid = other.m_valid;
		other.m_program = 0;

		return *this;
	}

	ShaderProgram::~ShaderProgram() {
		if (m_program != 0)
			glDeleteProgram(m_program);
	}

	void ShaderProgram::load(const char* vertexSource, const char* fragmentSource) {
		if (m_program != 0)
			glDeleteProgram(m_program);

		GLuint vertex = loadShader(vertexSource, GL_VERTEX_SHADER);
		GLuint fragment = loadShader(fragmentSource, GL_FRAGMENT_SHADER);
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

	bool ShaderProgram::isValid() const {
		return m_valid;
	}

	int ShaderProgram::uniformId(const char* name) const {
		return glGetUniformLocation(m_program, name);
	}

	void ShaderProgram::uniform(int id, int value) {
		bind();
		glUniform1i(id, value);
	}

	void ShaderProgram::uniform(int id, float value) {
		bind();
		glUniform1f(id, value);
	}

	void ShaderProgram::uniform(int id, glm::vec1 value) {
		bind();
		glUniform1f(id, value.x);
	}

	void ShaderProgram::uniform(int id, glm::vec2 value) {
		bind();
		glUniform2fv(id, 1, &value.x);
	}

	void ShaderProgram::uniform(int id, glm::vec3 value) {
		bind();
		glUniform3fv(id, 1, &value.x);
	}

	void ShaderProgram::uniform(int id, glm::vec4 value) {
		bind();
		glUniform4fv(id, 1, &value.x);
	}

	void ShaderProgram::uniform(int id, glm::mat3 value) {
		bind();
		glUniformMatrix3fv(id, 1, false, &value[0][0]);
	}

	void ShaderProgram::uniform(int id, glm::mat4 value) {
		bind();
		glUniformMatrix4fv(id, 1, false, &value[0][0]);
	}

	void ShaderProgram::uniform(int id, Color value) {
		bind();
		glUniform4fv(id, 1, &value.r);
	}

	void ShaderProgram::bind() const {
		static const ShaderProgram* boundShader = nullptr;
		if (boundShader != this) {
			glUseProgram(m_program);
			boundShader = this;
		}
	}
}