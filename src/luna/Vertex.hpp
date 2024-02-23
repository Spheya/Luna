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

inline GLM_CONSTEXPR bool operator==(const luna::Vertex& v1, const luna::Vertex& v2) {
	return v1.position == v2.position && v1.uv == v2.uv && v1.normal == v2.normal;
}

inline GLM_CONSTEXPR bool operator!=(const luna::Vertex& v1, const luna::Vertex& v2) {
	return !(v1 == v2);
}