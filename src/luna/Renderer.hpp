#pragma once

#include <vector>

#include "Camera.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

namespace luna {

	class Renderer {
	public:
		virtual ~Renderer() = default;

		void push(const Mesh* mesh, const glm::mat4 matrix, const Material* material);

		virtual void beginFrame();
		virtual void endFrame();
		virtual void render(const Camera& camera) const;

	protected:
		struct RenderObject {
			const Mesh* mesh;
			glm::mat4 matrix;
			const Material* material;
		};

		void draw(const RenderObject& mesh) const;

		std::vector<RenderObject> m_renderObjects;

	};

}