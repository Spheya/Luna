#pragma once

#include "Vertex.hpp"

#include <initializer_list>

namespace luna {

	class Mesh {
	public:
		Mesh() = default;
		Mesh(std::initializer_list<Vertex> vertices);
		Mesh(const Vertex* vertices, size_t size);
		Mesh(const Vertex* vertices, size_t vertexCount, const unsigned int* indices, size_t indexCount);
		Mesh(Mesh&) = delete;
		Mesh& operator=(Mesh&) = delete;
		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;
		~Mesh();

		void setVertices(const Vertex* vertices, size_t size);
		void setIndices(const unsigned int* indices, size_t size);

		void bind() const;
		unsigned int getInternalHandle() const;
		size_t vertexCount() const;

		static Mesh loadFromFile(const char* filepath);

	private:
		void init();

		static Mesh loadObj(const char* filepath);

	private:
		enum BufferIndex {
			Vertices = 0,
			Indices = 1
		};

		unsigned int m_vao = 0;
		unsigned int m_vbos[2] = {0,0};

		size_t m_vertexCount = 0;
		size_t m_vertexBufferSize = 0;
		size_t m_indexBufferSize = 0;
	};

}
