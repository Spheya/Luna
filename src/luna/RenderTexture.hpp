#pragma once

#include "Texture.hpp"
#include "RenderTarget.hpp"

namespace luna {

	class RenderTexture : public Texture, public RenderTarget {
	public:
		RenderTexture(int width, int height, TextureFormat format = TextureFormat::Rgba);
		RenderTexture(glm::ivec2 size, TextureFormat format = TextureFormat::Rgba);
		RenderTexture(RenderTexture&) = delete;
		RenderTexture& operator=(RenderTexture&) = delete;
		RenderTexture(RenderTexture&& other) noexcept;
		RenderTexture& operator=(RenderTexture&& other) noexcept;
		virtual ~RenderTexture();

		void makeActiveTarget() override;

		int getWidth() const override { return Texture::getWidth(); }
		int getHeight() const override { return Texture::getHeight(); }
		glm::ivec2 getSize() const override { return Texture::getSize(); }
		
		using Texture::getWidth;
		using Texture::getHeight;
		using Texture::getSize;

	private:		
		unsigned int m_fbo = 0;
		unsigned int m_rbo = 0;

		void bindFbo() const;
		void onSizeChange(int width, int height) override;
	};

}