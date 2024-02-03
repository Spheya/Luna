#include "Renderer.hpp"

#include <glad/glad.h>

namespace luna {

	void Renderer::beginFrame() {
		m_commandBuffer.clear();
	}

	void Renderer::endFrame() {
		// TODO : possibly do some optimization, like grouping draw command together
	}

	void Renderer::push(const Mesh* mesh) {
		m_commandBuffer.emplace_back(mesh);
	}

	void Renderer::render(const Camera& camera) const {
		for (const auto& command : m_commandBuffer) {
			draw(command.mesh);
		}
	}

	void Renderer::draw(const Mesh* mesh) const {
		glBindVertexArray(mesh->handle());
		glDrawArrays(GL_TRIANGLES, 0, GLsizei(mesh->vertexCount()));
	}

}
