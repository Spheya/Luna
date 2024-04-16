#include "Renderer.hpp"

#include <glad/glad.h>
#include <algorithm>

#include "Context.hpp"

namespace luna {

	void Renderer::beginFrame() {
		m_renderObjects.clear();
	}

	void Renderer::endFrame() {
	}

	void Renderer::push(const Mesh* mesh, glm::mat4 matrix, const Material* material) {
		m_renderObjects.emplace_back(mesh, matrix, material);
	}

	void Renderer::push(const Mesh* mesh, glm::mat4 matrix) {
		m_renderObjects.emplace_back(mesh, matrix, luna::getDefaultMaterial());
	}

	void Renderer::render(const Camera& camera) const {
		std::vector<RenderObject> renderBatch = m_renderObjects;

		std::sort(renderBatch.begin(), renderBatch.end(), [&](const RenderObject& a, const RenderObject& b) {
			return glm::dot(glm::vec3(a.matrix[3]), camera.getTransform().position) < glm::dot(glm::vec3(b.matrix[3]), camera.getTransform().position);
		});

		if (camera.getTarget()) {
			camera.getTarget()->makeActiveTarget();
			RenderTarget::clear(camera.getBackgroundColor());

			for (const auto& object : renderBatch) {
				object.mesh->bind();
				object.material->bind();
				auto& shader = object.material->getShader()->getProgram();
				shader.uniform(shader.uniformId("ModelMatrix"), object.matrix);
				shader.uniform(shader.uniformId("ViewMatrix"), camera.getTransform().inverseMatrix());
				shader.uniform(shader.uniformId("ProjectionMatrix"), camera.projection());
				draw(object.mesh);
			}
		}
	}

	void Renderer::draw(const Mesh* mesh) const {
		glDrawElements(GL_TRIANGLES, GLsizei(mesh->vertexCount()), GL_UNSIGNED_INT, nullptr);
	}

}
