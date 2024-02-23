#pragma once

#include "Vertex.hpp"

#include <initializer_list>

namespace luna {

	class Mesh {
	public:
		Mesh();
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
		size_t vertexCount() const;

		static Mesh loadFromFile(const char* filepath);

	private:
		enum BufferIndex {
			Vertices = 0,
			Indices = 1
		};

		unsigned int m_vao;
		unsigned int m_vbos[2];

		size_t m_vertexCount;

		void init();
	};

}