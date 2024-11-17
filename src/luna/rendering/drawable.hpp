#pragma once

#include <vector>

#include "material.hpp"
#include "../platform/mesh.hpp"
#include "../core/math.hpp"

namespace luna {

	struct Drawable {
		const Mesh* mesh;
		const Material* material;
		glm::mat4 transform;
	};

}