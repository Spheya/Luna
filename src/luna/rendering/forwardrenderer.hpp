#pragma once

#include "renderer.hpp"
#include "lightsource.hpp"
#include "../platform/rendertexture.hpp"

namespace luna {

	class ForwardRenderer : public Renderer {
	public:
		ForwardRenderer();

		void submit(const Mesh* mesh, const Material* material, const glm::mat4& matrix);
		virtual void render(const Camera& camera, bool clear = true) override;

		void clear();

	private:
		struct LunaForwardData {
			glm::vec4 lightSources[32];
			int lightSourceCount;
		};
		LunaForwardData m_lunaForwardData;
		DirectionalLight m_mainLight;
		bool m_hasMainLight;

		std::vector<Drawable> m_drawables;
		Buffer m_renderTypeBuffer;

		RenderType m_lunaForwardRenderType;

	public:
		RenderTexture m_shadowMap;
	};

}