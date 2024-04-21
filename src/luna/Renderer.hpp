#pragma once

#include <vector>

#include "Camera.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

namespace luna {

	class Renderer {
	public:
		virtual ~Renderer() = default;

		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;
		virtual void render(const Camera& camera) = 0;

	protected:
		void draw(const Mesh* mesh) const;

	};

}