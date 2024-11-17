#pragma once

#include <span>

#include "buffer.hpp"
#include "platform.hpp"
#include "../rendering/vertex.hpp"

namespace luna {

	class Mesh {
	public:
		explicit Mesh(std::span<const Vertex> vertices, BufferUsage usage = BufferUsage::Static);
		Mesh(std::span<const Vertex> vertices, std::span<const unsigned> indices, BufferUsage usage = BufferUsage::Static);
		Mesh(std::initializer_list<const Vertex> vertices, BufferUsage usage = BufferUsage::Static);
		Mesh(std::initializer_list<const Vertex> vertices, std::initializer_list<const unsigned> indices, BufferUsage usage = BufferUsage::Static);
		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;
		~Mesh();

		void setVertices(std::span<const Vertex> vertices);
		void setIndices(std::span<const unsigned> indices);

		size_t vertexCount() const;
		size_t indexCount() const;

		GLuint internalHandle() const;

	private:
		Buffer m_vertexBuffer;
		Buffer m_indexBuffer;
		GLuint m_gpuHandle;
	};

}