#include "buffer.hpp"

#include <cassert>

namespace luna {
	
	static constexpr GLenum BufferTargets[]{
		GL_ARRAY_BUFFER,         // vertex buffer
		GL_ELEMENT_ARRAY_BUFFER, // index buffer
		GL_UNIFORM_BUFFER        // uniform buffer
	};

	static GLenum getInternalBufferUsage(BufferAccess access, BufferUsage usage) {
		if (access == BufferAccess::Read) {
			switch (usage) {
			case BufferUsage::Static:  return GL_STATIC_READ;
			case BufferUsage::Dynamic: return GL_DYNAMIC_READ;
			case BufferUsage::Stream:  return GL_STREAM_READ;
			default: warn("Invalid buffer usage");
			}
		} else {
			switch (usage) {
			case BufferUsage::Static:  return GL_STATIC_DRAW;
			case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
			case BufferUsage::Stream:  return GL_STREAM_DRAW;
			default: warn("Invalid buffer usage");
			}
		}

		return GL_STATIC_DRAW;
	}

	Buffer::Buffer(BufferType type, BufferAccess access, BufferUsage usage) :
		m_type(type),
		m_access(access),
		m_usage(usage),
		m_size(0),
		m_gpuHandle(0)
	{}

	Buffer::Buffer(BufferType type, BufferAccess access, BufferUsage usage, size_t size) :
		Buffer(type, access, usage)
	{
		LUNA_GLCALL(glGenBuffers(1, &m_gpuHandle));
		LUNA_GLCALL(glBindBuffer(BufferTargets[int(type)], m_gpuHandle));
		LUNA_GLCALL(glBufferData(BufferTargets[int(type)], size, nullptr, getInternalBufferUsage(m_access, m_usage)));
		m_size = size;

		trace("Buffer created (id={})", m_gpuHandle);
	}

	Buffer::Buffer(BufferType type, BufferAccess access, BufferUsage usage, const void* data, size_t size) :
		Buffer(type, access, usage)
	{
		LUNA_GLCALL(glGenBuffers(1, &m_gpuHandle));
		LUNA_GLCALL(glBindBuffer(BufferTargets[int(type)], m_gpuHandle));
		LUNA_GLCALL(glBufferData(BufferTargets[int(type)], size, data, getInternalBufferUsage(m_access, m_usage)));
		m_size = size;

		trace("Buffer created (id={})", m_gpuHandle);
	}

	Buffer::Buffer(Buffer&& other) noexcept :
		m_type(other.m_type),
		m_access(other.m_access),
		m_usage(other.m_usage),
		m_size(other.m_size),
		m_gpuHandle(other.m_gpuHandle)
	{
		other.m_gpuHandle = 0;
	}

	Buffer& Buffer::operator=(Buffer&& other) noexcept {
		clear();

		m_type = other.m_type;
		m_access = other.m_access;
		m_usage = other.m_usage;
		m_size = other.m_size;
		m_gpuHandle = other.m_gpuHandle;
		other.m_gpuHandle = 0;

		return *this;
	}

	Buffer::~Buffer() {
		clear();
	}

	void Buffer::setData(const void* data, size_t size) {
		if (size == 0)
			clear();

		if (!m_gpuHandle) {
			LUNA_GLCALL(glGenBuffers(1, &m_gpuHandle));
			trace("Buffer created (id={})", m_gpuHandle);
		}

		if (m_size != size) {
			LUNA_GLCALL(glBindBuffer(BufferTargets[int(m_type)], m_gpuHandle));
			LUNA_GLCALL(glBufferData(BufferTargets[int(m_type)], size, data, getInternalBufferUsage(m_access, m_usage)));
			trace("Buffer resized (id={})", m_gpuHandle);
		} else {
			setData(data, size, 0);
		}

		m_size = size;
	}

	void Buffer::setData(const void* data, size_t size, size_t offset) {
		assert(size + offset <= m_size);
		assert(data);

		if (size == 0) return;

		LUNA_GLCALL(glBindBuffer(BufferTargets[int(m_type)], m_gpuHandle));
		LUNA_GLCALL(glBufferSubData(BufferTargets[int(m_type)], 0, size, data));
	}

	void Buffer::readData(void* dst) const {
		LUNA_GLCALL(glGetBufferSubData(BufferTargets[int(m_type)], 0, m_size, dst));
	}

	void Buffer::resize(size_t size) {
		setData(nullptr, size);
	}

	void Buffer::clear() {
		m_size = 0;
		if (m_gpuHandle) {
			LUNA_GLCALL(glDeleteBuffers(1, &m_gpuHandle));
			trace("Buffer destroyed (id={})", m_gpuHandle);
		}
	}

	size_t Buffer::size() const {
		return m_size;
	}

	BufferType Buffer::type() const {
		return m_type;
	}

	BufferAccess Buffer::access() const {
		return m_access;
	}

	BufferUsage Buffer::usage() const {
		return m_usage;
	}

	GLuint Buffer::internalHandle() const {
		return m_gpuHandle;
	}

}