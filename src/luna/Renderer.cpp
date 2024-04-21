#include "Renderer.hpp"

#include <glad/glad.h>

namespace luna {

	void Renderer::draw(const Mesh* mesh) const {
		glDrawElements(GL_TRIANGLES, GLsizei(mesh->vertexCount()), GL_UNSIGNED_INT, nullptr);
	}

}
