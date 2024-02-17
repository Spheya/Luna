#pragma once

#include "Color.hpp"

namespace luna {

	class RenderTarget {
	public:
		virtual ~RenderTarget() = default;

		static void clear(Color color);
		virtual void makeActiveTarget() = 0;
		virtual glm::ivec2 getSize() const = 0;
		virtual int getWidth() const = 0;
		virtual int getHeight() const = 0;
	};

}