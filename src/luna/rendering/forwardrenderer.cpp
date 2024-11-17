#include "forwardrenderer.hpp"
#include "../platform/graphics.hpp"

namespace luna {
	ForwardRenderer::ForwardRenderer() :
		Renderer(),
		m_lunaForwardRenderType(getRenderTypeId("LunaForward")),
		m_renderTypeBuffer(BufferType::UniformBuffer, BufferAccess::Write, BufferUsage::Dynamic)
	{}

	void ForwardRenderer::render(const Camera& camera, bool clear) {
		if (!camera.renderTarget())
			return;

		if (m_drawables.empty())
			return;

		camera.renderTarget()->bindTarget();
		if (clear)
			graphics::clear();

		bindCameraData(camera.propertyBlock());

		CullResults cullResults = cullDrawables(camera, m_drawables);
		bindInstancingData(cullResults);
		renderDrawables(cullResults, m_lunaForwardRenderType);
		renderDrawables(cullResults, NullRenderType);
	}

	void ForwardRenderer::clear() {
		m_drawables.clear();
	}

}