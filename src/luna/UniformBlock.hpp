#pragma once

namespace luna {

	class UniformBlock {
	public:
		UniformBlock() = default;
		UniformBlock(UniformBlock&) = delete;
		UniformBlock& operator=(UniformBlock&) = delete;
		UniformBlock(UniformBlock&& other) noexcept;
		UniformBlock& operator=(UniformBlock&& other) noexcept;
		~UniformBlock();
		
		void setBinding(unsigned int binding);
		void setContent(const void* data, size_t size);

		void bind();
		size_t size();

	private:
		size_t m_size = 0;
		unsigned int m_ubo = 0;

	};

}