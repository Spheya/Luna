#pragma once

#include "../core/math.hpp"

namespace luna {

	class RenderTarget {
	public:
		virtual ~RenderTarget() = default;

		virtual void bindTarget() = 0;
		virtual glm::uvec2 framebufferSize() const = 0;
		unsigned framebufferWidth() const { return framebufferSize().x; }
		unsigned framebufferHeight() const { return framebufferSize().y; }
	};

}