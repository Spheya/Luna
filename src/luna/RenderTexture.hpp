#pragma once

#include "Texture.hpp"
#include "RenderTarget.hpp"

namespace luna {

	class RenderTexture : public Texture, public RenderTarget {
	public:
		RenderTexture(int width, int height);
		RenderTexture(glm::ivec2 size);
		RenderTexture(RenderTexture&) = delete;
		RenderTexture& operator=(RenderTexture&) = delete;
		RenderTexture(RenderTexture&& other) noexcept;
		RenderTexture& operator=(RenderTexture&& other) noexcept;
		virtual ~RenderTexture();

		void makeActiveTarget() override;

	private:		
		unsigned int m_fbo = 0;
		unsigned int m_rbo = 0;

		void bindFbo() const;
		void onSizeChange(int width, int height) override;
	};

}