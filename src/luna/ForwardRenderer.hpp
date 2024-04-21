#pragma once

#include "Renderer.hpp"

namespace luna {

	class ForwardRenderer : public Renderer {
	public:
		void push(const Mesh* mesh, const glm::mat4& matrix, const Material* material);
		void push(const Mesh* mesh, const glm::mat4& matrix);

		void beginFrame() override;
		void endFrame() override;
		void render(const luna::Camera& camera) override;

	private:
		struct RenderObject {
			const Mesh* mesh;
			glm::mat4 matrix;
			const Material* material;
		};

		struct RenderBatch {
			RenderBatch(std::vector<RenderObject>::iterator start) : start(start), size(1), min(glm::vec3(start->matrix[3])), max(glm::vec3(start->matrix[3])) {}

			std::vector<RenderObject>::iterator start;
			int size;
			glm::vec3 min;
			glm::vec3 max;
		};

		std::vector<RenderBatch> m_renderBatches;
		std::vector<RenderObject> m_renderObjects;

	};

}