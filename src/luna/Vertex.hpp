#pragma once

#include "Math.hpp"

namespace luna {
	struct Vertex {
		GLM_CONSTEXPR explicit Vertex(glm::vec3 position, glm::vec2 uv = glm::vec2(0.0f), glm::vec3 normal = glm::vec3(0.0f)) :
			position(position),
			uv(uv),
			normal(normal)
		{}

		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
	};
}