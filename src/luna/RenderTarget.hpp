#pragma once

#include "Color.hpp"

namespace luna {

	class RenderTarget {
	public:
		virtual ~RenderTarget() = default;

		virtual void clear(Color color) = 0;
	};

}