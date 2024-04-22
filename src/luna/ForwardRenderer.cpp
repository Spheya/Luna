#include "ForwardRenderer.hpp"

#include <algorithm>

#include "Context.hpp"

namespace luna {

	void ForwardRenderer::beginFrame() {
		m_renderObjects.clear();
	}

	void ForwardRenderer::endFrame() {
		m_renderBatches.clear();

		if (!m_renderObjects.empty()) {
			std::sort(m_renderObjects.begin(), m_renderObjects.end(), [&](const RenderObject& a, const RenderObject& b) {
				const ShaderProgram* aShader = &a.material->getShader()->getProgram();
				const ShaderProgram* bShader = &b.material->getShader()->getProgram();

				if (aShader->getRenderQueue() != bShader->getRenderQueue()) return aShader->getRenderQueue() < bShader->getRenderQueue();
				if (aShader != bShader) return aShader < bShader;
				return a.mesh < b.mesh;
			});

			RenderBatch currentBatch(m_renderObjects.begin());
			currentBatch.size = 0;

			for (auto it = m_renderObjects.begin(); it != m_renderObjects.end(); ++it) {
				const ShaderProgram& batchShader = currentBatch.start->material->getShader()->getProgram();
				const ShaderProgram& shader = it->material->getShader()->getProgram();
				if (shader.getRenderQueue() != batchShader.getRenderQueue() ||
					(batchShader.getRenderQueue() != RenderQueue::Transparent && &batchShader != &shader)) {
					m_renderBatches.push_back(currentBatch);
					currentBatch = RenderBatch(it);
				} else {
					++currentBatch.size;
				}
			}
			m_renderBatches.push_back(currentBatch);
		}
	}

	void ForwardRenderer::push(const Mesh* mesh, const glm::mat4& matrix, const Material* material) {
		m_renderObjects.emplace_back(mesh, matrix, material);
	}

	void ForwardRenderer::push(const Mesh* mesh, const glm::mat4& matrix) {
		m_renderObjects.emplace_back(mesh, matrix, luna::getDefaultMaterial());
	}

	void ForwardRenderer::render(const Camera& camera) {
		if (camera.getTarget()) {
			camera.getTarget()->makeActiveTarget();
			RenderTarget::clear(camera.getBackgroundColor());
			luna::uploadCameraMatrices(camera.projection(), camera.getTransform().inverseMatrix());

			// Cache distance to camera
			for (auto& object : m_renderObjects) {
				object.distToCameraSq = glm::dot(glm::vec3(object.matrix[3]), camera.getTransform().position);
			}

			for (auto& batch : m_renderBatches) {
				batch.distToCameraSq = 1e32;
				for (auto it = batch.start; it != batch.start + batch.size; ++it)
					batch.distToCameraSq = std::min(it->distToCameraSq, batch.distToCameraSq);
			}

			// Sort batches based on distance
			std::sort(m_renderBatches.begin(), m_renderBatches.end());

			// Go over each batch
			for (auto& batch : m_renderBatches) {
				// Sort batch contents
				std::sort(batch.start, batch.start + batch.size);

				// Render batch contents
				if (batch.start->material->getShader()->getProgram().getRenderQueue() == RenderQueue::Transparent) {
					for (int i = 0; i < batch.size; ++i) {
						auto& object = *(batch.start + i);

						object.mesh->bind();
						object.material->bind();
						auto& shader = object.material->getShader()->getProgram();
						shader.uniform(shader.uniformId("ModelMatrix"), object.matrix);
						draw(object.mesh);
					}
				} else {
					batch.start->mesh->bind();
					batch.start->material->bind();
					auto& shader = batch.start->material->getShader()->getProgram();
					int modelMatrixId = shader.uniformId("ModelMatrix");

					for (int i = 0; i < batch.size; ++i) {
						auto& object = *(batch.start + i);
						shader.uniform(modelMatrixId, object.matrix);
						draw(object.mesh);
					}
				}
			}
		}
	}

	bool ForwardRenderer::RenderBatch::operator<(const RenderBatch& other) {
		const ShaderProgram& aShader = start->material->getShader()->getProgram();
		const ShaderProgram& bShader = other.start->material->getShader()->getProgram();
		if (aShader.getRenderQueue() != bShader.getRenderQueue()) return aShader.getRenderQueue() < bShader.getRenderQueue();
		return distToCameraSq > other.distToCameraSq;
	}

	bool ForwardRenderer::RenderObject::operator<(const RenderObject& other) {
		if (material->getShader()->getProgram().getRenderQueue() == RenderQueue::Transparent) return distToCameraSq < other.distToCameraSq;
		return distToCameraSq > other.distToCameraSq;
	}
}