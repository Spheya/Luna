#include "Texture.hpp"

#include <glad/glad.h>

#include "Logger.hpp"

namespace luna {

	Texture::Texture(uint8_t* data, int width, int height) :
		m_dimensions(width, height)
	{
		setTextureData(data);
	}

	Texture::Texture(Color* data, int width, int height) :
		m_dimensions(width, height)
	{
		setTextureData(data);
	}

	Texture::Texture(Color color) :
		Texture(&color, 1, 1)
	{}

	Texture::Texture(int width, int height) :
		m_dimensions(width, height)
	{
		setSize(width, height);
	}

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
		m_texture = other.m_texture;
		m_dimensions = other.m_dimensions;
		m_minFilter = other.m_minFilter;
		m_magFilter = other.m_magFilter;
		m_horizontalWrap = other.m_horizontalWrap;
		m_verticalWrap = other.m_verticalWrap;

		other.m_texture = 0;

		return *this;
	}

	void Texture::setTextureData(uint8_t* data) {
		setTextureData(data, m_dimensions.x, m_dimensions.y);
	}

	void Texture::setTextureData(uint8_t* data, int width, int height) {
		if (m_texture == 0) {
			if (width < 1 || height < 1) {
				log("Trying to use a texture with a size smaller than 1x1", MessageSeverity::Error);
				return;
			}

			glGenTextures(1, &m_texture);
			bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_horizontalWrap == TextureWrapMode::Repeat ? GL_REPEAT : GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_verticalWrap   == TextureWrapMode::Repeat ? GL_REPEAT : GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter == TextureFilter::Nearest ? GL_NEAREST : GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter == TextureFilter::Nearest ? GL_NEAREST : GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		} else {
			bind();
			if (m_dimensions == glm::ivec2(width, height)) {
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
			} else {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
		}

		m_dimensions = glm::ivec2(width, height);
	}

	void Texture::setTextureData(Color data) {
		setTextureData(&data, 1, 1);
	}

	void Texture::setTextureData(Color* data) {
		setTextureData(data, m_dimensions.x, m_dimensions.y);
	}

	void Texture::setTextureData(Color* data, int width, int height) {
		if (width < 1 || height < 1) {
			log("Trying to use a texture with a size smaller than 1x1", MessageSeverity::Error);
			return;
		}

		int* buffer = new int[width * height];

		for (int y = 0; y < height; y++)
			for (int x = 0; x < width; x++)
				buffer[x + y * width] = data[x + y * width].asInt();

		setTextureData((uint8_t*)buffer, width, height);

		delete[] buffer;
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
		if (m_dimensions != glm::ivec2(width, height)) {
			bind();

			uint8_t* buffer = new uint8_t[width * height];
			setTextureData(buffer, width, height);
			delete[] buffer;
		}
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

	void Texture::bind() const {
		static const Texture* boundTexture = nullptr;
		if (boundTexture != this) {
			glBindTexture(GL_TEXTURE_2D, m_texture);
			boundTexture = this;
		}
		
	}

}