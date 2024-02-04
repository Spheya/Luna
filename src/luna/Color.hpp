#pragma once

namespace luna {

	struct Color {
		static Color red;
		static Color green;
		static Color blue;
		static Color cyan;
		static Color yellow;
		static Color magenta;
		static Color black;
		static Color gray;
		static Color white;

		Color();
		Color(float r, float g, float b, float a = 1.0f);
		Color(int r, int g, int b, int a = 255);

		float r, g, b, a;
	};

}