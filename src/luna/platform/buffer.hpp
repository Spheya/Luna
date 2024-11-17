#pragma once

#include <cstdint>

#include "platform.hpp"

namespace luna {

	enum class BufferType {
		VertexBuffer =  0,
		IndexBuffer  =  1,
		UniformBuffer = 2
	};

	enum class BufferAccess {
		Read,
		Write,
		ReadWrite,
	};

	enum class BufferUsage {
		Static,  // Buffer contents will be set once
		Dynamic, // Buffer contents will be set occasionally
		Stream   // Buffer contents will be set every frame
	};

	class Buffer {
	public:
		Buffer(BufferType type, BufferAccess access, BufferUsage usage);
		Buffer(BufferType type, BufferAccess access, BufferUsage usage, size_t size);
		Buffer(BufferType type, BufferAccess access, BufferUsage usage, const void* data, size_t size);
		Buffer(Buffer&& other) noexcept;
		Buffer& operator=(Buffer&& other) noexcept;
		~Buffer();

		void setData(const void* data, size_t size);
		void setData(const void* data, size_t size, size_t offset);
		void readData(void* dst) const;

		void resize(size_t size);
		void clear();

		size_t size() const;
		BufferType type() const;
		BufferAccess access() const;
		BufferUsage usage() const;

		GLuint internalHandle() const;

	private:
		BufferType m_type;
		BufferAccess m_access;
		BufferUsage m_usage;
		size_t m_size;

		GLuint m_gpuHandle;
	};

}