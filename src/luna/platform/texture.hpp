#pragma once

#include "platform.hpp"
#include "sampler.hpp"
#include "../core/math.hpp"

namespace luna {

	enum class TextureFormat {
		R, Rgb, Rgba, Float, FloatRgba, DepthStencil, Depth
	};

	enum class TextureFilter {
		Nearest, Linear
	};

	enum class TextureWrapMode {
		Repeat, Clamp
	};

	class Texture : public Sampler {
	public:
		explicit Texture(TextureFormat format = TextureFormat::Rgba);
		Texture(glm::uvec2 size, TextureFormat format = TextureFormat::Rgba);
		Texture(const std::uint8_t* data, glm::uvec2 size, TextureFormat format = TextureFormat::Rgba);
		Texture(const float* data, glm::uvec2 size, TextureFormat format = TextureFormat::Rgba);
		Texture(const glm::vec4* data, glm::uvec2 size, TextureFormat format = TextureFormat::Rgba);
		Texture(glm::vec4 color);
		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;
		virtual ~Texture();

		void setTextureData(const std::uint8_t* data);
		void setTextureData(const std::uint8_t* data, glm::uvec2 size);

		void setTextureData(const float* data);
		void setTextureData(const float* data, glm::uvec2 size);

		void setTextureData(const glm::vec4* data);
		void setTextureData(const glm::vec4* data, glm::uvec2 size);

		TextureFormat format() const;

		void setFilter(TextureFilter min, TextureFilter mag);
		void setFilter(TextureFilter filter);
		TextureFilter minFilter() const;
		TextureFilter magFilter() const;

		void setWrapMode(TextureWrapMode horizontal, TextureWrapMode vertical);
		void setWrapMode(TextureWrapMode wrapmode);
		TextureWrapMode horizontalWrapMode() const;
		TextureWrapMode verticalWrapMode() const;

		void generateMipmap();

		unsigned width() const;
		unsigned height() const;
		glm::uvec2 size() const;
		void setSize(glm::uvec2 size);

		virtual void bindSampler(int slot) const override;
		GLuint internalHandle() const;

		static Texture fromFile(const char* filepath);

	protected:
		virtual void onSizeChange() {}

	private:
		unsigned int m_texture = 0;

		glm::uvec2 m_dimensions;

		TextureFormat m_format;
		TextureFilter m_minFilter;
		TextureFilter m_magFilter;
		TextureWrapMode m_horizontalWrap;
		TextureWrapMode m_verticalWrap;

		bool m_hasMipmap = false;
	};

}
