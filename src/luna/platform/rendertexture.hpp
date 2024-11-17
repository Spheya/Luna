#pragma once

#include <span>

#include "texture.hpp"
#include "rendertarget.hpp"

namespace luna {

	enum class RenderTextureType {
		ReadableDepth,
		OptimizedDepth
	};

	class RenderTexture : public Texture, public RenderTarget {
	public:
		explicit RenderTexture(glm::uvec2 size, std::span<const TextureFormat> colorFormats, TextureFormat depthFormat = TextureFormat::Depth, RenderTextureType type = RenderTextureType::OptimizedDepth);
		explicit RenderTexture(glm::uvec2 size, std::initializer_list<TextureFormat> colorFormats, TextureFormat depthFormat = TextureFormat::Depth, RenderTextureType type = RenderTextureType::OptimizedDepth);
		explicit RenderTexture(glm::uvec2 size, TextureFormat format = TextureFormat::Rgb, TextureFormat depthFormat = TextureFormat::Depth, RenderTextureType type = RenderTextureType::OptimizedDepth);
		RenderTexture(RenderTexture&& other) noexcept;
		RenderTexture& operator=(RenderTexture&& other) noexcept;
		virtual ~RenderTexture();

		RenderTextureType type() const;

		const Sampler* colorBuffer(size_t index) const;
		const Sampler* depthBuffer() const;

		virtual void bindTarget() override;
		virtual glm::uvec2 framebufferSize() const override;

	private:
		GLuint m_fbo;
		GLuint m_rbo;

		std::unique_ptr<Texture> m_depthBufferTexture;
		std::vector<Texture> m_additionalColorAttachments;

		RenderTextureType m_type;
	};

}
