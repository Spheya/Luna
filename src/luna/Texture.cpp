#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>

#include "Logger.hpp"

namespace luna {

	Texture::Texture(TextureFormat format) :
		m_format(format)
	{}

	Texture::Texture(const std::uint8_t* data, int width, int height, TextureFormat format) :
		m_format(format),
		m_dimensions(width, height)
	{
		setTextureData(data);
	}

	Texture::Texture(const std::uint8_t* data, glm::ivec2 size, TextureFormat format) :
		Texture(data, size.x, size.y, format)
	{}

	Texture::Texture(const Color* data, int width, int height, TextureFormat format) :
		m_format(format),
		m_dimensions(width, height)
	{
		setTextureData(data);
	}

	Texture::Texture(const Color* data, glm::ivec2 size, TextureFormat format) :
		Texture(data, size.x, size.y, format) {
	}

	Texture::Texture(const float* data, int width, int height, TextureFormat format) :
		m_format(format),
		m_dimensions(width, height) {
		setTextureData(data);
	}

	Texture::Texture(const float* data, glm::ivec2 size, TextureFormat format) :
		Texture(data, size.x, size.y, format) {
	}

	Texture::Texture(Color color, TextureFormat format) :
		Texture(&color, 1, 1, format)
	{}

	Texture::Texture(int width, int height, TextureFormat format) :
		m_format(format)
	{
		setSize(width, height);
	}

	Texture::Texture(glm::ivec2 size, TextureFormat format) :
		Texture(size.x, size.y, format)
	{}

	Texture::Texture(Texture&& other) noexcept :
		m_texture(other.m_texture),
		m_dimensions(other.m_dimensions),
		m_minFilter(other.m_minFilter),
		m_magFilter(other.m_magFilter),
		m_horizontalWrap(other.m_horizontalWrap),
		m_verticalWrap(other.m_verticalWrap)
	{
		other.m_texture = 0;
	}

	Texture& Texture::operator=(Texture&& other) noexcept {
		Texture::~Texture();

		m_texture = other.m_texture;
		m_dimensions = other.m_dimensions;
		m_minFilter = other.m_minFilter;
		m_magFilter = other.m_magFilter;
		m_horizontalWrap = other.m_horizontalWrap;
		m_verticalWrap = other.m_verticalWrap;

		other.m_texture = 0;

		return *this;
	}

	Texture::~Texture() {
		if(m_texture)
			glDeleteTextures(1, &m_texture);
	}

	void Texture::setTextureDataInternal(void* data, int width, int height, int format, int type) {
		GLenum internalFormat = GL_RGBA8;
		switch (m_format) {
			case TextureFormat::Rgb: internalFormat = GL_RGB8; break;
			case TextureFormat::Rgba: internalFormat = GL_RGBA8; break;
			case TextureFormat::Float: internalFormat = GL_R16F; break;
			case TextureFormat::FloatRgba: internalFormat = GL_RGBA16F; break;
		}

		if(m_texture == 0) {
			if (width < 1 || height < 1) {
				log("Trying to use a texture with a size smaller than 1x1", MessageSeverity::Error);
				return;
			}

			glGenTextures(1, &m_texture);
			bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_horizontalWrap == TextureWrapMode::Repeat ? GL_REPEAT : GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_verticalWrap == TextureWrapMode::Repeat ? GL_REPEAT : GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter == TextureFilter::Nearest ? GL_NEAREST : GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter == TextureFilter::Nearest ? GL_NEAREST : GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
		} else {
			bind();
			if (m_dimensions == glm::ivec2(width, height)) {
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, data);
			} else {
				if (width < 1 || height < 1) {
					log("Trying to use a texture with a size smaller than 1x1", MessageSeverity::Error);
					return;
				}

				onSizeChange(width, height);
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
			}
		}

		m_dimensions = glm::ivec2(width, height);
	}

	void Texture::setTextureData(const std::uint8_t* data) {
		setTextureData(data, m_dimensions.x, m_dimensions.y);
	}

	void Texture::setTextureData(const std::uint8_t* data, int width, int height) {
		if (data) {
			m_textureType = TextureType::Solid;
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					std::uint8_t alpha = data[(x + y * width) * 4 + 3];
					if (m_textureType == TextureType::Solid && alpha == 0) m_textureType = TextureType::Cutout;
					else if (alpha != 255 && alpha != 0) { m_textureType = TextureType::Transparent; break; }
				}
				if (m_textureType == TextureType::Transparent) break;
			}
		}

		setTextureDataInternal((void*)data, width, height, GL_RGBA, GL_UNSIGNED_BYTE);
	}

	void Texture::setTextureData(const std::uint8_t* data, glm::ivec2 size) {
		setTextureData(data, size.x, size.y);
	}

	void Texture::setTextureData(const float* data) {
		setTextureData(data, m_dimensions.x, m_dimensions.y);
	}

	void Texture::setTextureData(const float* data, int width, int height) {
		m_textureType = TextureType::Solid;
		setTextureDataInternal((void*)data, width, height, GL_R, GL_FLOAT);
	}

	void Texture::setTextureData(const float* data, glm::ivec2 size) {
		setTextureData(data, size.x, size.y);
	}

	void Texture::setTextureData(Color data) {
		setTextureData(&data, 1, 1);
	}

	void Texture::setTextureData(const Color* data) {
		setTextureData(data, m_dimensions.x, m_dimensions.y);
	}

	void Texture::setTextureData(const Color* data, int width, int height) {
		if (data) {
			m_textureType = TextureType::Solid;
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					float alpha = data[x + y * width].a;
					if (m_textureType == TextureType::Solid && alpha == 0.0f) m_textureType = TextureType::Cutout;
					else if (alpha != 1.0f && alpha != 0.0f) { m_textureType = TextureType::Transparent; break; }
				}
				if (m_textureType == TextureType::Transparent) break;
			}
		}

		setTextureDataInternal((void*)data, width, height, GL_RGBA, GL_FLOAT);
	}

	void Texture::setTextureData(const Color* data, glm::ivec2 size) {
		setTextureData(data, size.x, size.y);
	}

	void Texture::setTextureType(TextureType type) {
		m_textureType = type;
	}

	TextureType Texture::getTextureType() const {
		return m_textureType;
	}

	void Texture::setFilter(TextureFilter filter) {
		m_minFilter = filter;
		m_magFilter = filter;

		if (m_texture != 0) {
			bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter == TextureFilter::Nearest ? GL_NEAREST : GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter == TextureFilter::Nearest ? GL_NEAREST : GL_LINEAR);
		}
	}

	void Texture::setMinFilter(TextureFilter filter) {
		m_minFilter = filter;
		
		if (m_texture != 0) {
			bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter == TextureFilter::Nearest ? GL_NEAREST : GL_LINEAR);
		}
	}

	void Texture::setMagFilter(TextureFilter filter) {
		m_magFilter = filter;

		if (m_texture != 0) {
			bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter == TextureFilter::Nearest ? GL_NEAREST : GL_LINEAR);
		}
	}

	TextureFilter Texture::getMinFilter() const {
		return m_minFilter;
	}

	TextureFilter Texture::getMagFilter() const {
		return m_magFilter;
	}

	void Texture::setWrapMode(TextureWrapMode wrapmode) {
		m_horizontalWrap = wrapmode;
		m_verticalWrap = wrapmode;

		if (m_texture != 0) {
			bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_horizontalWrap == TextureWrapMode::Repeat ? GL_REPEAT : GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_verticalWrap   == TextureWrapMode::Repeat ? GL_REPEAT : GL_CLAMP);
		}
	}

	void Texture::setHorizontalWrapMode(TextureWrapMode wrapmode) {
		m_horizontalWrap = wrapmode;
		
		if (m_texture != 0) {
			bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_horizontalWrap == TextureWrapMode::Repeat ? GL_REPEAT : GL_CLAMP);
		}
	}

	void Texture::setVerticalWrapMode(TextureWrapMode wrapmode) {
		m_verticalWrap = wrapmode;

		if (m_texture != 0) {
			bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_verticalWrap == TextureWrapMode::Repeat ? GL_REPEAT : GL_CLAMP);
		}
	}

	TextureWrapMode Texture::getHorizontalWrapMode() const {
		return m_horizontalWrap;
	}

	TextureWrapMode Texture::getVerticalWrapMode() const {
		return m_verticalWrap;
	}

	int Texture::getWidth() const {
		return m_dimensions.x;
	}

	int Texture::getHeight() const {
		return m_dimensions.y;
	}

	glm::ivec2 Texture::getSize() const {
		return m_dimensions;
	}

	void Texture::setSize(int width, int height) {
		if (m_dimensions != glm::ivec2(width, height))
			setTextureData((std::uint8_t*)nullptr, width, height);
	}

	void Texture::setSize(glm::ivec2 size) {
		setSize(size.x, size.y);
	}

	void Texture::bind(int textureSlot) const {
		if (textureSlot > GL_TEXTURE31) {
			log("Luna only supports up to 32 texture slots!", MessageSeverity::Error);
			return;
		}

		glActiveTexture(GL_TEXTURE0 + textureSlot);
		bind();
	}

	unsigned int Texture::getTextureHandle() const {
		return m_texture;
	}

	void Texture::bind() const {
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}

	Texture Texture::loadFromFile(const char* filepath) {
		int width, height, nrChannels;

		stbi_set_flip_vertically_on_load(true);
		std::uint8_t* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

		Texture result;

		if (nrChannels == 3) {
			result = Texture(data, width, height, TextureFormat::Rgb);
		}else if (nrChannels == 4) {
			result = Texture(data, width, height, TextureFormat::Rgba);
		} else {
			log("Luna only supports textures with 3 or 4 colour channels!", MessageSeverity::Error);
		}

		stbi_image_free(data);
		return result;
	}
}