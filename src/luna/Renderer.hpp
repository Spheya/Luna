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
		void push(const Mesh* mesh, const glm::mat4 matrix);

		virtual void beginFrame();
		virtual void endFrame();
		virtual void render(const Camera& camera);

	protected:
		struct RenderObject {
			const Mesh* mesh;
			glm::mat4 matrix;
			const Material* material;
		};

		void draw(const Mesh* mesh) const;

	private:
		struct RenderBatch {
			RenderBatch(std::vector<RenderObject>::iterator start) : start(start), size(1), min(glm::vec3(start->matrix[3])), max(glm::vec3(start->matrix[3])) {}

			std::vector<RenderObject>::iterator start;
			int size;
			glm::vec3 min;
			glm::vec3 max;
		};

		std::vector<RenderObject> m_renderObjects;
		std::vector<RenderBatch> m_renderBatches;
	};

}