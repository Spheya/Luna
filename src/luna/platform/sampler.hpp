#pragma once

namespace luna {

	class Sampler {
	public:
		virtual ~Sampler() = default;
		virtual void bindSampler(int slot) const = 0;
	};

}