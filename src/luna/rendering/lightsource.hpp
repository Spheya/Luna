#pragma once

#include "../core/math.hpp"

namespace luna {

	struct PointLight {
		glm::vec4 color;
		glm::vec3 position;
	};

	struct DirectionalLight {
		glm::vec4 color;
		glm::vec3 direction;
	};

}