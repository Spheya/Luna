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
		std::vector<Drawable> m_drawables;
		RenderType m_lunaForwardRenderType;
	};

}