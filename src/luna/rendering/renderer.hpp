#pragma once

#include "drawable.hpp"
#include "camera.hpp"
#include "rendertype.hpp"

namespace luna {

	class Renderer {
	public:
		struct CullResults {
			std::vector<Drawable> drawables;
		};

	public:
		Renderer();

		virtual void render(const Camera& camera, bool clear = true) = 0;

	protected:
		// Cull AND sort the drawables
		CullResults cullDrawables(const Camera& camera, std::span<const Drawable> drawables);

		void renderDrawables(const CullResults& cullResults, RenderType renderType);

		void bindInstancingData(const CullResults& cullResults);
		void bindCameraData(const CameraPropertyBlock& cameraproperties);

	private:
		Buffer m_cameraBuffer;
		Buffer m_instancingBuffer;
	};

}