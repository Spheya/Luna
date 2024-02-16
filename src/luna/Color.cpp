#include "Color.hpp"

namespace luna {

	Color Color::Red(1.0f, 0.0f, 0.0f);
	Color Color::Green(0.0f, 1.0f, 0.0f);
	Color Color::Blue(0.0f, 0.0f, 1.0f);
	Color Color::Cyan(0.0f, 1.0f, 1.0f);
	Color Color::Yellow(1.0f, 1.0f, 0.0f);
	Color Color::Magenta(1.0f, 0.0f, 1.0f);
	Color Color::Black(0.0f, 0.0f, 0.0f);
	Color Color::Gray(0.5f, 0.5f, 0.5f);
	Color Color::White(1.0f, 1.0f, 1.0f);

	constexpr Color::Color() : Color(0.0f, 0.0f, 0.0f, 0.0f) {}

	constexpr Color::Color(float r, float g, float b, float a) :
		r(r), g(g), b(b), a(a)
	{}

	constexpr Color::Color(int r, int g, int b, int a) :
		Color(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f, float(a) / 255.0f)
	{}

	glm::vec3 Color::asVec3() const {
		return glm::vec3(r, g, b);
	}

	glm::vec4 Color::asVec4() const {
		return glm::vec4(r, g, b, a);
	}

}