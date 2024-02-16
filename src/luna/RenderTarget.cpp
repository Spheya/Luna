#include "RenderTarget.hpp"

#include <glad/glad.h>

namespace luna {

	void RenderTarget::clear(Color color) {
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

}