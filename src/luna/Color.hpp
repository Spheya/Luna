#pragma once

#include "Math.hpp"

namespace luna {

	struct Color {
		static Color Red;
		static Color Green;
		static Color Blue;
		static Color Cyan;
		static Color Yellow;
		static Color Magenta;
		static Color Black;
		static Color Gray;
		static Color White;

		constexpr Color();
		constexpr Color(float r, float g, float b, float a = 1.0f);
		constexpr Color(int r, int g, int b, int a = 255);
		GLM_CONSTEXPR Color(glm::vec4 color) : Color(color.r, color.g, color.b, color.a) {}
		GLM_CONSTEXPR Color(glm::vec3 color) : Color(color.r, color.g, color.b) {}

		glm::vec3 asVec3() const;
		glm::vec4 asVec4() const;

		float r, g, b, a;
	};

}