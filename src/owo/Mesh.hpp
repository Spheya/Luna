#pragma once

#include "Vertex.hpp"

#include <initializer_list>

namespace owo {

	class Mesh {
	public:
		Mesh();
		Mesh(std::initializer_list<Vertex> vertices);
		Mesh(const Vertex* vertices, size_t vertexCount);
		Mesh(Mesh&) = delete;
		Mesh& operator=(Mesh&) = delete;
		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;
		~Mesh();

		void setVertices(const Vertex* vertices, size_t vertexCount);

		unsigned int handle() const;
		size_t vertexCount() const;

	private:
		unsigned int m_vao;
		unsigned int m_verticesVbo;

		size_t m_vertexCount;
	};

}