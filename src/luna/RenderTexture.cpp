#include "RenderTexture.hpp"

#include <glad/glad.h>

#include "Logger.hpp"

namespace luna {

	RenderTexture::RenderTexture(int width, int height, TextureFormat format) :
		Texture(width, height, format)
	{
		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, getTextureHandle(), 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
	}

	RenderTexture::RenderTexture(glm::ivec2 size, TextureFormat format) :
		RenderTexture(size.x, size.y, format)
	{}

	RenderTexture::RenderTexture(RenderTexture&& other) noexcept :
		Texture(std::move(other)),
		m_fbo(other.m_fbo),
		m_rbo(other.m_rbo)
	{
		other.m_fbo = 0;
	}

	RenderTexture& RenderTexture::operator=(RenderTexture&& other) noexcept
	{
		Texture::operator=(std::move(other));

		m_fbo = other.m_fbo;
		other.m_fbo = 0;

		return *this;
	}

	RenderTexture::~RenderTexture() {
		if (m_fbo) {
			glDeleteFramebuffers(1, &m_fbo);
			glDeleteRenderbuffers(1, &m_rbo);
		}
	}

	void RenderTexture::makeActiveTarget() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glViewport(0, 0, getWidth(), getHeight());
	}

	void RenderTexture::onSizeChange(int width, int height) {
		if (m_fbo) {
			glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		}
	}
}