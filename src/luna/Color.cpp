#include "Color.hpp"

namespace luna {

	Color Color::red(1.0f, 0.0f, 0.0f);
	Color Color::green(0.0f, 1.0f, 0.0f);
	Color Color::blue(0.0f, 0.0f, 1.0f);
	Color Color::cyan(0.0f, 1.0f, 1.0f);
	Color Color::yellow(1.0f, 1.0f, 0.0f);
	Color Color::magenta(1.0f, 0.0f, 1.0f);
	Color Color::black(0.0f, 0.0f, 0.0f);
	Color Color::gray(0.5f, 0.5f, 0.5f);
	Color Color::white(1.0f, 1.0f, 1.0f);

	Color::Color() : Color(0.0f, 0.0f, 0.0f, 0.0f) {}

	Color::Color(float r, float g, float b, float a) :
		r(r), g(g), b(b), a(a) {
	}

	Color::Color(int r, int g, int b, int a) : Color(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f, float(a) / 255.0f) {}

}