#pragma once

#include "Math.hpp"

namespace luna {
	struct Vertex {
		GLM_CONSTEXPR explicit Vertex(glm::vec3 position, glm::vec2 uv = glm::vec2(0.0f), glm::vec3 normal = glm::vec3(0.0f), uint32_t color = 0xffffffff) :
			position(position),
			uv(uv),
			normal(normal),
			color(color)
		{}

		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
		uint32_t color;
	};
}

inline GLM_CONSTEXPR bool operator==(const luna::Vertex& v1, const luna::Vertex& v2) {
	return v1.position == v2.position && v1.uv == v2.uv && v1.normal == v2.normal && v1.color != v2.color;
}

inline GLM_CONSTEXPR bool operator!=(const luna::Vertex& v1, const luna::Vertex& v2) {
	return !(v1 == v2);
}