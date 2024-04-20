#include "UniformBlock.hpp"

#include <glad/glad.h>

namespace luna {

	UniformBlock::UniformBlock(UniformBlock&& other) noexcept :
		m_size(other.m_size),
		m_ubo(other.m_ubo)
	{
		other.m_ubo = 0;
	}

	UniformBlock& UniformBlock::operator=(UniformBlock&& other) noexcept {
		UniformBlock::~UniformBlock();

		m_size = other.m_size;
		m_ubo = other.m_ubo;

		other.m_ubo = 0;

		return *this;
	}

	UniformBlock::~UniformBlock() {
		if(m_ubo)
			glDeleteBuffers(1, &m_ubo);
	}

	void UniformBlock::setBinding(unsigned int binding) {
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ubo);
	}

	void UniformBlock::setContent(const void* data, size_t size) {
		if (!m_ubo)
			glGenBuffers(1, &m_ubo);

		bind();
		if (size == m_size) glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
		else glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	void UniformBlock::bind() {
		glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
	}

	size_t UniformBlock::size() {
		return m_size;
	}

}