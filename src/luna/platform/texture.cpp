#include "texture.hpp"

#include <stb_image.h>

namespace luna {

	static constexpr GLenum InternalTextureFormats[] = {
		GL_R8,
		GL_RGB8,
		GL_RGBA8,
		GL_R32F,
		GL_RGBA32F,
		GL_DEPTH24_STENCIL8,
		GL_DEPTH_COMPONENT
	};

	static constexpr GLenum TextureFormats[] = {
		GL_R,
		GL_RGB,
		GL_RGBA,
		GL_R,
		GL_RGBA,
		GL_DEPTH_STENCIL,
		GL_DEPTH_COMPONENT
	};

	static constexpr GLenum TextureFilters[] = {
		GL_NEAREST,
		GL_LINEAR
	};

	static constexpr GLenum MipmapTextureFilters[] = {
		GL_NEAREST_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_LINEAR
	};

	static constexpr GLenum TextureWrapModes[] = {
		GL_REPEAT,
		GL_CLAMP
	};

	Texture::Texture(TextureFormat format) :
		m_dimensions(glm::uvec2(0, 0)),
		m_format(format),
		m_minFilter(TextureFilter::Linear),
		m_magFilter(TextureFilter::Linear),
		m_horizontalWrap(TextureWrapMode::Clamp),
		m_verticalWrap(TextureWrapMode::Clamp)
	{
		LUNA_GLCALL(glGenTextures(1, & m_texture));
		setFilter(TextureFilter::Linear);
		setWrapMode(TextureWrapMode::Clamp);
		trace("Texture created (id={})", m_texture);
	}

	Texture::Texture(glm::uvec2 size, TextureFormat format) :
		Texture(format)
	{
		setSize(size);
	}

	Texture::Texture(const std::uint8_t* data, glm::uvec2 size, TextureFormat format) :
		Texture(format)
	{
		setTextureData(data, size);
	}

	Texture::Texture(const float* data, glm::uvec2 size, TextureFormat format) :
		Texture(format)
	{
		setTextureData(data, size);
	}

	Texture::Texture(const glm::vec4* data, glm::uvec2 size, TextureFormat format) :
		Texture(format)
	{
		setTextureData(data, size);
	}

	Texture::Texture(glm::vec4 color) :
		Texture(TextureFormat::Rgba)
	{
		setTextureData(&color, glm::uvec2(1,1));
	}

	Texture::Texture(Texture&& other) noexcept :
		m_texture(other.m_texture),
		m_dimensions(other.m_dimensions),
		m_format(other.m_format),
		m_minFilter(other.m_minFilter),
		m_magFilter(other.m_magFilter),
		m_horizontalWrap(other.m_horizontalWrap),
		m_verticalWrap(other.m_verticalWrap)
	{
		other.m_texture = 0;
	}

	Texture& Texture::operator=(Texture&& other) noexcept {
		if (m_texture) {
			LUNA_GLCALL(glDeleteTextures(1, &m_texture));
			trace("Texture destroyed (id={})", m_texture);
		}

		m_texture = other.m_texture;
		m_dimensions = other.m_dimensions;
		m_format = other.m_format;
		m_minFilter = other.m_minFilter;
		m_magFilter = other.m_magFilter;
		m_horizontalWrap = other.m_horizontalWrap;
		m_verticalWrap = other.m_verticalWrap;

		other.m_texture = 0;

		return *this;
	}

	Texture::~Texture() {
		if (m_texture) {
			LUNA_GLCALL(glDeleteTextures(1, &m_texture));
			trace("Texture destroyed (id={})", m_texture);
		}
	}

	void Texture::setTextureData(const std::uint8_t* data) {
		setTextureData(data, m_dimensions);
	}

	void Texture::setTextureData(const std::uint8_t* data, glm::uvec2 size) {
		LUNA_GLCALL(glBindTexture(GL_TEXTURE_2D, m_texture));
		LUNA_GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, InternalTextureFormats[int(m_format)], GLsizei(size.x), GLsizei(size.y), 0, TextureFormats[int(m_format)], GL_UNSIGNED_BYTE, data));

		if (size != m_dimensions) {
			m_dimensions = size;
			onSizeChange();
		}
	}

	void Texture::setTextureData(const float* data) {
		setTextureData(data, m_dimensions);
	}

	void Texture::setTextureData(const float* data, glm::uvec2 size) {
		LUNA_GLCALL(glBindTexture(GL_TEXTURE_2D, m_texture));
		LUNA_GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, InternalTextureFormats[int(m_format)], GLsizei(size.x), GLsizei(size.y), 0, TextureFormats[int(m_format)], GL_FLOAT, data));

		if (size != m_dimensions) {
			m_dimensions = size;
			onSizeChange();
		}
	}

	void Texture::setTextureData(const glm::vec4* data) {
		setTextureData(data, m_dimensions);
	}

	void Texture::setTextureData(const glm::vec4* data, glm::uvec2 size) {
		LUNA_GLCALL(glBindTexture(GL_TEXTURE_2D, m_texture));
		LUNA_GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, InternalTextureFormats[int(m_format)], GLsizei(size.x), GLsizei(size.y), 0, GL_RGBA, GL_FLOAT, data));

		if (size != m_dimensions) {
			m_dimensions = size;
			onSizeChange();
		}
	}
	TextureFormat Texture::format() const {
		return m_format;
	}

	void Texture::setFilter(TextureFilter min, TextureFilter mag) {
		m_minFilter = min;
		m_magFilter = mag;

		LUNA_GLCALL(glBindTexture(GL_TEXTURE_2D, m_texture));
		LUNA_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_hasMipmap ? MipmapTextureFilters[int(m_minFilter)] : TextureFilters[int(m_minFilter)]));
		LUNA_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFilters[int(m_magFilter)]));
	}

	void Texture::setFilter(TextureFilter filter) {
		setFilter(filter, filter);
	}

	TextureFilter Texture::minFilter() const {
		return m_minFilter;
	}

	TextureFilter Texture::magFilter() const {
		return m_magFilter;
	}
	
	void Texture::setWrapMode(TextureWrapMode horizontal, TextureWrapMode vertical) {
		m_horizontalWrap = horizontal;
		m_verticalWrap = vertical;

		LUNA_GLCALL(glBindTexture(GL_TEXTURE_2D, m_texture));
		LUNA_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapModes[int(m_horizontalWrap)]));
		LUNA_GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapModes[int(m_verticalWrap)]));
	}

	void Texture::setWrapMode(TextureWrapMode wrapmode) {
		setWrapMode(wrapmode, wrapmode);
	}

	TextureWrapMode Texture::horizontalWrapMode() const {
		return m_horizontalWrap;
	}

	TextureWrapMode Texture::verticalWrapMode() const {
		return m_verticalWrap;
	}

	void Texture::generateMipmap() {
		m_hasMipmap = true;
		setFilter(m_minFilter, m_magFilter);
		LUNA_GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
	}

	unsigned Texture::width() const {
		return m_dimensions.x;
	}

	unsigned Texture::height() const {
		return m_dimensions.y;
	}

	glm::uvec2 Texture::size() const {
		return m_dimensions;
	}

	void Texture::setSize(glm::uvec2 size) {
		if (m_dimensions != size)
			onSizeChange();

		m_dimensions = size;

		LUNA_GLCALL(glBindTexture(GL_TEXTURE_2D, m_texture));
		LUNA_GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, InternalTextureFormats[int(m_format)], GLsizei(size.x), GLsizei(size.y), 0, TextureFormats[int(m_format)], GL_UNSIGNED_BYTE, nullptr));
	}

	void Texture::bindSampler(int slot) const {
		LUNA_GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
		LUNA_GLCALL(glBindTexture(GL_TEXTURE_2D, m_texture));
	}

	GLuint Texture::internalHandle() const {
		return m_texture;
	}

	Texture Texture::fromFile(const char* filepath) {
		int width, height, nrChannels;

		stbi_set_flip_vertically_on_load(true);
		std::uint8_t* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

		TextureFormat format = TextureFormat::Rgba;
		switch (nrChannels) {
		case 1: format = TextureFormat::R; break;
		case 3: format = TextureFormat::Rgb; break;
		case 4: format = TextureFormat::Rgba; break;
		default:
			error("Texture at '{}' contains {} colour channels, Luna only supports images with 3 or 4 colour channels!", filepath, nrChannels);
			return Texture();
		}

		Texture result(data, glm::uvec2(unsigned(width), unsigned(height)), format);
		stbi_image_free(data);
		return result;
	}
}