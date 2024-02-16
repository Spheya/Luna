#include "Renderer.hpp"

#include <glad/glad.h>

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

	void Renderer::render(const Camera& camera) const {
		camera.getTarget()->makeActiveTarget();
		RenderTarget::clear(camera.getBackgroundColor());

		for (const auto& object : m_renderObjects) {
			object.material->bind();
			object.mesh->bind();
			draw(object.mesh);
		}
	}

	void Renderer::draw(const Mesh* mesh) const {
		glDrawElements(GL_TRIANGLES, GLsizei(mesh->vertexCount()), GL_UNSIGNED_INT, nullptr);
	}

}
