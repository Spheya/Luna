#include "Renderer.hpp"

#include <glad/glad.h>

#include "Context.hpp"

namespace luna {

	void Renderer::beginFrame() {
		m_renderObjects.clear();
	}

	void Renderer::endFrame() {
		// TODO : possibly do some optimization, like grouping draw commands together
	}

	void Renderer::push(const Mesh* mesh, glm::mat4 matrix, const Material* material) {
		m_renderObjects.emplace_back(mesh, matrix, material);
	}

	void Renderer::push(const Mesh* mesh, glm::mat4 matrix) {
		m_renderObjects.emplace_back(mesh, matrix, luna::getDefaultMaterial());
	}

	void Renderer::render(const Camera& camera) const {
		if (camera.getTarget()) {
			camera.getTarget()->makeActiveTarget();
			RenderTarget::clear(camera.getBackgroundColor());

			for (const auto& object : m_renderObjects) {
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
