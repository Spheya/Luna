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
			float distToCameraSq;

			bool operator<(const RenderObject& other) const;
		};

		struct RenderBatch {
			RenderBatch(std::vector<RenderObject>::iterator start) : start(start), size(1), distToCameraSq(0.0f) {}

			std::vector<RenderObject>::iterator start;
			size_t size;
			float distToCameraSq;

			bool operator<(const RenderBatch& other) const;
		};

		std::vector<RenderBatch> m_renderBatches;
		std::vector<RenderObject> m_renderObjects;

	};

}