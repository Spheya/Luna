#include "Mesh.hpp"

#include <glad/glad.h>
#include <numeric>

namespace luna {
	Mesh::Mesh() :
		m_vao(0),
		m_vbos{ 0,0 },
		m_vertexCount(0)
	{}

	Mesh::Mesh(std::initializer_list<Vertex> vertices) :
		Mesh(vertices.begin(), vertices.size())
	{}

	Mesh::Mesh(const Vertex* vertices, size_t size) :
		Mesh()
	{
		setVertices(vertices, size);
		unsigned int* indices = new unsigned int[size];
		std::iota(indices, indices + size, 0);
		setIndices(indices, size);
		delete[] indices;
	}


	Mesh::Mesh(const Vertex* vertices, size_t vertexCount, const unsigned int* indices, size_t indexCount) :
		Mesh()
	{
		setVertices(vertices, vertexCount);
		setIndices(indices, indexCount);
	}

	Mesh::Mesh(Mesh&& other) noexcept {
		m_vao = other.m_vao;
		m_vbos[Vertices] = other.m_vbos[Vertices];
		m_vbos[Indices] = other.m_vbos[Indices];
		m_vertexCount = other.m_vertexCount;

		other.m_vao = 0;
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept {
		m_vao = other.m_vao;
		m_vbos[Vertices] = other.m_vbos[Vertices];
		m_vbos[Indices] = other.m_vbos[Indices];
		m_vertexCount = other.m_vertexCount;

		other.m_vao = 0;

		return *this;
	}

	Mesh::~Mesh() {
		if (m_vao != 0) {
			glDeleteVertexArrays(1, &m_vao);
			glDeleteBuffers(2, m_vbos);
		}
	}

	void Mesh::init() {
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(2, m_vbos);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[Vertices]);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);  // Position
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (void*)12); // Uv
		glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (void*)20); // Normal
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

	}

	void Mesh::setVertices(const Vertex* vertices, size_t size) {
		if (m_vao == 0)
			init();

		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[Vertices]);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	}

	void Mesh::setIndices(const unsigned int* indices, size_t size) {
		if (m_vao == 0)
			init();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbos[Indices]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), indices, GL_STATIC_DRAW);
		m_vertexCount = size;
	}

	unsigned Mesh::handle() const {
		return m_vao;
	}

	size_t Mesh::vertexCount() const {
		return m_vertexCount;
	}
}