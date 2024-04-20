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
		other.m_valid = false;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
		ShaderProgram::~ShaderProgram();

		m_program = other.m_program;
		m_valid = other.m_valid;

		other.m_program = 0;
		other.m_valid = false;

		return *this;
	}

	ShaderProgram::~ShaderProgram() {
		if (m_program != 0)
			glDeleteProgram(m_program);
	}

	void ShaderProgram::load(const char* vertexSource, const char* fragmentSource) {
		ShaderProgram::~ShaderProgram();

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

	void ShaderProgram::uniform(int id, int value) const {
		glUseProgram(m_program);
		glUniform1i(id, value);
	}

	void ShaderProgram::uniform(int id, float value) const {
		glUseProgram(m_program);
		glUniform1f(id, value);
	}

	void ShaderProgram::uniform(int id, glm::vec1 value) const {
		glUseProgram(m_program);
		glUniform1f(id, value.x);
	}

	void ShaderProgram::uniform(int id, glm::vec2 value) const {
		glUseProgram(m_program);
		glUniform2fv(id, 1, &value.x);
	}

	void ShaderProgram::uniform(int id, glm::vec3 value) const {
		glUseProgram(m_program);
		glUniform3fv(id, 1, &value.x);
	}

	void ShaderProgram::uniform(int id, glm::vec4 value) const {
		glUseProgram(m_program);
		glUniform4fv(id, 1, &value.x);
	}

	void ShaderProgram::uniform(int id, glm::mat3 value) const {
		glUseProgram(m_program);
		glUniformMatrix3fv(id, 1, false, &value[0][0]);
	}

	void ShaderProgram::uniform(int id, glm::mat4 value) const {
		glUseProgram(m_program);
		glUniformMatrix4fv(id, 1, false, &value[0][0]);
	}

	void ShaderProgram::uniform(int id, Color value) const {
		glUseProgram(m_program);
		glUniform4fv(id, 1, &value.r);
	}

	void ShaderProgram::bind() const {
		glUseProgram(m_program);

		if (m_srcBlendFactor == BlendFactor::One && m_dstBlendFactor == BlendFactor::Zero) { glDisable(GL_BLEND); }
		else {
			glEnable(GL_BLEND);
			GLenum srcFactor, dstFactor;
			switch (m_srcBlendFactor) {
				case BlendFactor::Zero: srcFactor = GL_ZERO; break;
				case BlendFactor::One: srcFactor = GL_ONE; break;
				case BlendFactor::SrcAlpha: srcFactor = GL_SRC_ALPHA; break;
				case BlendFactor::DstAlpha: srcFactor = GL_DST_ALPHA; break;
				case BlendFactor::OneMinusSrcAlpha: srcFactor = GL_ONE_MINUS_SRC_ALPHA; break;
				case BlendFactor::OneMinusDstAlpha: srcFactor = GL_ONE_MINUS_DST_ALPHA; break;
				default: srcFactor = GL_ZERO;
			}

			switch (m_dstBlendFactor) {
				case BlendFactor::Zero: dstFactor = GL_ZERO; break;
				case BlendFactor::One: dstFactor = GL_ONE; break;
				case BlendFactor::SrcAlpha: dstFactor = GL_SRC_ALPHA; break;
				case BlendFactor::DstAlpha: dstFactor = GL_DST_ALPHA; break;
				case BlendFactor::OneMinusSrcAlpha: dstFactor = GL_ONE_MINUS_SRC_ALPHA; break;
				case BlendFactor::OneMinusDstAlpha: dstFactor = GL_ONE_MINUS_DST_ALPHA; break;
				default: dstFactor = GL_ZERO;
			}
			glBlendFunc(srcFactor, dstFactor);
		}

		if (m_cullMode == CullMode::Off) { glDisable(GL_CULL_FACE); }
		else {
			glEnable(GL_CULL_FACE);
			glCullFace(m_cullMode == CullMode::Front ? GL_FRONT : GL_BACK);
		}

		if (m_depthTestMode == DepthTestMode::Off) { glDisable(GL_DEPTH_TEST); }
		else { glEnable(GL_DEPTH_TEST); }
	}

	unsigned int ShaderProgram::getInternalHandle() const {
		return m_program;
	}

	void ShaderProgram::setRenderQueue(RenderQueue queue) {
		m_queue = queue;
	}

	RenderQueue ShaderProgram::getRenderQueue() const {
		return m_queue;
	}

	void ShaderProgram::setBlendMode(BlendFactor srcFactor, BlendFactor dstFactor) {
		m_srcBlendFactor = srcFactor;
		m_dstBlendFactor = dstFactor;
	}

	void ShaderProgram::setBlendMode(BlendMode blendMode) {
		if (blendMode == BlendMode::Off) {
			setBlendMode(BlendFactor::One, BlendFactor::Zero);
		} else {
			setBlendMode(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);
		}
	}

	BlendFactor ShaderProgram::getSrcBlendFactor() const {
		return m_srcBlendFactor;
	}

	BlendFactor ShaderProgram::getDstBlendFactor() const {
		return m_dstBlendFactor;
	}

	void ShaderProgram::setCullMode(CullMode cullMode) {
		m_cullMode = cullMode;
	}

	CullMode ShaderProgram::getCullMode() const {
		return m_cullMode;
	}

	void ShaderProgram::setDepthTestMode(DepthTestMode depthTest) {
		m_depthTestMode = depthTest;
	}

	DepthTestMode ShaderProgram::setDepthTestMode() const {
		return m_depthTestMode;
	}
}