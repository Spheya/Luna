#pragma once

#include "Math.hpp"
#include "Color.hpp"

namespace luna {
	enum class TextureFormat {
		Rgb, Rgba, Float, FloatRgba
	};

	enum class TextureFilter {
		Nearest, Linear
	};

	enum class TextureWrapMode {
		Repeat, Clamp
	};

	class Texture {
	public:
		explicit Texture(TextureFormat format = TextureFormat::Rgba);
		Texture(const std::uint8_t* data, int width, int height, TextureFormat format = TextureFormat::Rgba);
		Texture(const std::uint8_t* data, glm::ivec2 size, TextureFormat format = TextureFormat::Rgba);
		Texture(const Color* data, int width, int height, TextureFormat format = TextureFormat::Rgba);
		Texture(const Color* data, glm::ivec2 size, TextureFormat format = TextureFormat::Rgba);
		Texture(const float* data, int width, int height, TextureFormat format = TextureFormat::Rgba);
		Texture(const float* data, glm::ivec2 size, TextureFormat format = TextureFormat::Rgba);
		Texture(Color color, TextureFormat format = TextureFormat::Rgba);
		Texture(int width, int height, TextureFormat format = TextureFormat::Rgba);
		Texture(glm::ivec2 size, TextureFormat format = TextureFormat::Rgba);
		Texture(Texture&) = delete;
		Texture& operator=(Texture&) = delete;
		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;
		virtual ~Texture();

		void setTextureData(const std::uint8_t* data);
		void setTextureData(const std::uint8_t* data, int width, int height);
		void setTextureData(const std::uint8_t* data, glm::ivec2 size);

		void setTextureData(const float* data);
		void setTextureData(const float* data, int width, int height);
		void setTextureData(const float* data, glm::ivec2 size);

		void setTextureData(Color data);
		void setTextureData(const Color* data);
		void setTextureData(const Color* data, int width, int height);
		void setTextureData(const Color* data, glm::ivec2 size);

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

		static Texture loadFromFile(const char* filepath);

	protected:
		void setTextureDataInternal(void* data, int width, int height, int format, int type);

		unsigned int getTextureHandle() const;
		virtual void onSizeChange(int width, int height) {};

		void bind() const;

	private:
		unsigned int m_texture = 0;

		glm::ivec2 m_dimensions = glm::ivec2(0);

		TextureFormat m_format = TextureFormat::Rgba;
		TextureFilter m_minFilter = TextureFilter::Linear;
		TextureFilter m_magFilter = TextureFilter::Nearest;
		TextureWrapMode m_horizontalWrap = TextureWrapMode::Clamp;
		TextureWrapMode m_verticalWrap = TextureWrapMode::Clamp;
	};

}