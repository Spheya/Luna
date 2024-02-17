#pragma once

#include "Math.hpp"
#include "Color.hpp"

namespace luna {
	enum class TextureFilter {
		Nearest, Linear
	};

	enum class TextureWrapMode {
		Repeat, Clamp
	};

	class Texture {
	public:
		Texture() = default;
		Texture(uint8_t* data, int width, int height);
		Texture(Color* data, int width, int height);
		Texture(Color color);
		Texture(int width, int height);
		Texture(Texture&) = delete;
		Texture& operator=(Texture&) = delete;
		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;
		virtual ~Texture() = default;

		void setTextureData(uint8_t* data);
		void setTextureData(uint8_t* data, int width, int height);

		void setTextureData(Color data);
		void setTextureData(Color* data);
		void setTextureData(Color* data, int width, int height);

		void setFilter(TextureFilter filter);
		void setMinFilter(TextureFilter filter);
		void setMagFilter(TextureFilter filter);
		TextureFilter getMinFilter() const;
		TextureFilter getMagFilter() const;

		void setWrapMode(TextureWrapMode wrapmode);
		void setHorizontalWrapMode(TextureWrapMode wrapmode);
		void setVerticalWrapMode(TextureWrapMode wrapmode);
		TextureWrapMode getHorizontalWrapMode() const;
		TextureWrapMode getVerticalWrapMode() const;

		int getWidth() const;
		int getHeight() const;
		glm::ivec2 getSize() const;
		void setSize(int width, int height);
		void setSize(glm::ivec2 size);

		void bind(int textureSlot) const;

	private:
		unsigned int m_texture = 0;

		glm::ivec2 m_dimensions = glm::ivec2(0);

		TextureFilter m_minFilter = TextureFilter::Linear;
		TextureFilter m_magFilter = TextureFilter::Nearest;
		TextureWrapMode m_horizontalWrap = TextureWrapMode::Repeat;
		TextureWrapMode m_verticalWrap = TextureWrapMode::Repeat;

		void bind() const;
	};

}