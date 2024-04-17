#include "Renderer.hpp"

#include <glad/glad.h>
#include <algorithm>

#include "Context.hpp"

namespace luna {

	void Renderer::beginFrame() {
		m_renderObjects.clear();
	}

	void Renderer::endFrame() {
		m_renderBatches.clear();

		if (!m_renderObjects.empty()) {
			std::sort(m_renderObjects.begin(), m_renderObjects.end(), [&](const RenderObject& a, const RenderObject& b) {
				if (a.material->getRenderQueue() != b.material->getRenderQueue()) return a.material->getRenderQueue() < b.material->getRenderQueue();
				if (&a.material->getShader()->getProgram() != &b.material->getShader()->getProgram()) return &a.material->getShader()->getProgram() < &b.material->getShader()->getProgram();
				return a.mesh < b.mesh;
			});

			RenderBatch currentBatch(m_renderObjects.begin());
			currentBatch.size = 0;

			for (auto it = m_renderObjects.begin(); it != m_renderObjects.end(); ++it) {

				if (it->material->getRenderQueue() != currentBatch.start->material->getRenderQueue() ||
					(currentBatch.start->material->getRenderQueue()!=RenderQueue::Transparent && &currentBatch.start->material->getShader()->getProgram() != &it->material->getShader()->getProgram())) {
					m_renderBatches.push_back(currentBatch);
					currentBatch = RenderBatch(it);
				} else {
					++currentBatch.size;
					currentBatch.max = glm::max(glm::vec3(it->matrix[3]), currentBatch.max);
					currentBatch.min = glm::min(glm::vec3(it->matrix[3]), currentBatch.min);
				}
			}
			m_renderBatches.push_back(currentBatch);
		}
	}

	void Renderer::push(const Mesh* mesh, glm::mat4 matrix, const Material* material) {
		m_renderObjects.emplace_back(mesh, matrix, material);
	}

	void Renderer::push(const Mesh* mesh, glm::mat4 matrix) {
		m_renderObjects.emplace_back(mesh, matrix, luna::getDefaultMaterial());
	}

	void Renderer::render(const Camera& camera) {
		if (camera.getTarget()) {
			camera.getTarget()->makeActiveTarget();
			RenderTarget::clear(camera.getBackgroundColor());
			luna::uploadCameraMatrices(camera.projection(), camera.getTransform().inverseMatrix());

			// Sort batches based on distance
			std::sort(m_renderBatches.begin(), m_renderBatches.end(), [&](const RenderBatch& a, const RenderBatch& b) {
				if (a.start->material->getRenderQueue() != b.start->material->getRenderQueue()) return a.start->material->getRenderQueue() < b.start->material->getRenderQueue();

				glm::vec3 aMax = a.max - camera.getTransform().position;
				glm::vec3 aMin = a.min - camera.getTransform().position;
				glm::vec3 bMax = b.max - camera.getTransform().position;
				glm::vec3 bMin = b.min - camera.getTransform().position;

				float distA = std::min(glm::dot(aMax, aMax), glm::dot(aMin, aMin));
				float distB = std::min(glm::dot(bMax, bMax), glm::dot(bMin, bMin));

				return distA > distB;
			});

			// Go over each batch
			for (auto& batch : m_renderBatches) {
				// Sort the content
				if (batch.start->material->getRenderQueue() == RenderQueue::Transparent) {
					std::sort(batch.start, batch.start + batch.size, [&](const RenderObject& a, const RenderObject& b) {
						return glm::dot(glm::vec3(a.matrix[3]), camera.getTransform().position) < glm::dot(glm::vec3(b.matrix[3]), camera.getTransform().position);
					});
				} else {
					std::sort(batch.start, batch.start + batch.size, [&](const RenderObject& a, const RenderObject& b) {
						return glm::dot(glm::vec3(a.matrix[3]), camera.getTransform().position) > glm::dot(glm::vec3(b.matrix[3]), camera.getTransform().position);
					});
				}

				// Render every object in the batch
				for (int i = 0; i < batch.size; ++i) {
					auto& object = *(batch.start + i);

					object.mesh->bind();
					object.material->bind();
					auto& shader = object.material->getShader()->getProgram();
					shader.uniform(shader.uniformId("ModelMatrix"), object.matrix);
					draw(object.mesh);
				}
			}
		}
	}

	void Renderer::draw(const Mesh* mesh) const {
		glDrawElements(GL_TRIANGLES, GLsizei(mesh->vertexCount()), GL_UNSIGNED_INT, nullptr);
	}

}
