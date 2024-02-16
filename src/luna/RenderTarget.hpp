#pragma once

#include "Color.hpp"

namespace luna {

	class RenderTarget {
	public:
		virtual ~RenderTarget() = default;

		static void clear(Color color);
		virtual void makeActiveTarget() = 0;
	};

}