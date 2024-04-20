#pragma once

#include "Math.hpp"
#include "Color.hpp"

namespace luna {

	enum class BlendMode : uint8_t {
		Off, On
	};

	enum class BlendFactor : uint8_t {
		Zero, One, SrcAlpha, DstAlpha, OneMinusSrcAlpha, OneMinusDstAlpha
	};

	enum class CullMode : uint8_t {
		Off, Back, Front
	};

	enum class DepthTestMode : uint8_t {
		Off, On
	};

	enum class RenderQueue {
		Solid = 1000,
		Transparent = 2000
	};

	class ShaderProgram {
	public:
		ShaderProgram();
		ShaderProgram(const char* vertexSource, const char* fragmentSource);
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&& other) noexcept;
		ShaderProgram& operator=(ShaderProgram&& other) noexcept;
		~ShaderProgram();

		void load(const char* vertexSource, const char* fragmentSource);
		bool isValid() const;

		int uniformId(const char* name) const;
		void uniform(int id, int value) const;
		void uniform(int id, float value) const;
		void uniform(int id, glm::vec1 value) const;
		void uniform(int id, glm::vec2 value) const;
		void uniform(int id, glm::vec3 value) const;
		void uniform(int id, glm::vec4 value) const;
		void uniform(int id, glm::mat3 value) const;
		void uniform(int id, glm::mat4 value) const;
		void uniform(int id, Color value) const;

		void bind() const;
		unsigned int getInternalHandle() const;

		void setRenderQueue(RenderQueue queue);
		RenderQueue getRenderQueue() const;

		void setBlendMode(BlendFactor srcFactor, BlendFactor dstFactor);
		void setBlendMode(BlendMode blendMode);
		BlendFactor getSrcBlendFactor() const;
		BlendFactor getDstBlendFactor() const;

		void setCullMode(CullMode cullMode);
		CullMode getCullMode() const;

		void setDepthTestMode(DepthTestMode depthTest);
		DepthTestMode setDepthTestMode() const;

	private:
		unsigned int m_program;
		bool m_valid;

		RenderQueue m_queue = RenderQueue::Solid;
		BlendFactor m_srcBlendFactor = BlendFactor::SrcAlpha;
		BlendFactor m_dstBlendFactor = BlendFactor::OneMinusSrcAlpha;
		CullMode m_cullMode = CullMode::Back;
		DepthTestMode m_depthTestMode = DepthTestMode::On;
	};

}