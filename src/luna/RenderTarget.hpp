#pragma once

namespace luna {

	class RenderTarget {
	public:
		virtual ~RenderTarget() = default;

		virtual void clear() = 0;
	};

}