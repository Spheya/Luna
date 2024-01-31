#pragma once

namespace owo {

	class RenderTarget {
	public:
		virtual ~RenderTarget() = default;

		virtual void clear() = 0;
	};

}