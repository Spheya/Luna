#include "rendertexture.hpp"

namespace luna {

	RenderTexture::RenderTexture(glm::uvec2 size, std::span<const TextureFormat> colorFormats, TextureFormat depthFormat, RenderTextureType type) :
		Texture(size, colorFormats.empty() ? depthFormat : colorFormats[0]),
		m_fbo(0),
		m_rbo(0),
		m_type(type)
	{
		LUNA_GLCALL(glGenFramebuffers(1, &m_fbo));
		LUNA_GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

		if (colorFormats.empty()) {
			if (type != RenderTextureType::OptimizedDepth) {
				LUNA_GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, depthFormat == TextureFormat::DepthStencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Texture::internalHandle(), 0));
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
			}
		} else {
			LUNA_GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture::internalHandle(), 0));

			if (colorFormats.size() > 1) {
				m_additionalColorAttachments.reserve(colorFormats.size() - 1);
				for (size_t i = 1; i < colorFormats.size(); ++i) {
					m_additionalColorAttachments.emplace_back(size, colorFormats[i]);
					LUNA_GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_additionalColorAttachments[i].internalHandle(), 0));
				}
			}

			if (type != RenderTextureType::OptimizedDepth) {
				m_depthBufferTexture = std::make_unique<Texture>(size, depthFormat);
				LUNA_GLCALL(glFramebufferTexture2D(GL_FRAMEBUFFER, depthFormat == TextureFormat::DepthStencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBufferTexture->internalHandle(), 0));
			}
		}

		if (type == RenderTextureType::OptimizedDepth) {
			LUNA_GLCALL(glGenRenderbuffers(1, &m_rbo));
			LUNA_GLCALL(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
			LUNA_GLCALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GLsizei(size.x), GLsizei(size.y)));
		}

		trace("RenderTexture created (id={}, size={})", m_fbo, size);
	}

	RenderTexture::RenderTexture(glm::uvec2 size, std::initializer_list<TextureFormat> colorFormats, TextureFormat depthFormat, RenderTextureType type) :
		RenderTexture(size, std::span(colorFormats.begin(), colorFormats.size()), depthFormat, type)
	{}

	RenderTexture::RenderTexture(glm::uvec2 size, TextureFormat format, TextureFormat depthFormat, RenderTextureType type) :
		RenderTexture(size, {format}, depthFormat, type)
	{}

	RenderTexture::RenderTexture(RenderTexture && other) noexcept :
		Texture(std::move(other)),
		m_fbo(other.m_fbo),
		m_rbo(other.m_rbo),
		m_depthBufferTexture(std::move(other.m_depthBufferTexture)),
		m_additionalColorAttachments(std::move(other.m_additionalColorAttachments)),
		m_type(other.m_type)
	{
		other.m_fbo = 0;
		other.m_rbo = 0;
	}

	RenderTexture& RenderTexture::operator=(RenderTexture&& other) noexcept {
		Texture::operator=(std::move(other));
		if (m_rbo) {
			glDeleteRenderbuffers(1, &m_rbo);
		}
		if (m_fbo) {
			glDeleteFramebuffers(1, &m_fbo);
			trace("RenderTexture destroyed (id={})", m_fbo);
		}

		m_fbo = other.m_fbo;
		m_rbo = other.m_rbo;
		m_depthBufferTexture = std::move(other.m_depthBufferTexture);
		m_additionalColorAttachments = std::move(other.m_additionalColorAttachments);
		m_type = other.m_type;
		other.m_fbo = 0;
		other.m_rbo = 0;
		return *this;
	}

	RenderTexture::~RenderTexture() {
		if (m_rbo) {
			glDeleteRenderbuffers(1, &m_rbo);
		}
		if (m_fbo) {
			glDeleteFramebuffers(1, &m_fbo);
			trace("RenderTexture destroyed (id={})", m_fbo);
		}
	}

	RenderTextureType RenderTexture::type() const {
		return m_type;
	}

	const Sampler* RenderTexture::colorBuffer(size_t index) const {
		if (!m_depthBufferTexture)
			return nullptr;

		if (index == 0)
			return this;
		if(index - 1 < m_additionalColorAttachments.size())
			return &m_additionalColorAttachments[index];
		return nullptr;
	}

	const Sampler* RenderTexture::depthBuffer() const {
		switch (m_type) {
		case RenderTextureType::ReadableDepth:
			return m_depthBufferTexture ? m_depthBufferTexture.get() : this;
		case RenderTextureType::OptimizedDepth:
			warn("Cannot get the depth buffer from a RenderTexture with RenderTextureType::OptimizedDepth");
			return nullptr;
		}
	}

	void RenderTexture::bindTarget() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glViewport(0, 0, framebufferWidth(), framebufferHeight());
	}

	glm::uvec2 RenderTexture::framebufferSize() const {
		return size();
	}

}
