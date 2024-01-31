#include "Mesh.hpp"

#include <glad/glad.h>

namespace owo {
	Mesh::Mesh() :
		m_vao(0),
		m_verticesVbo(0),
		m_vertexCount(0)
	{}

	Mesh::Mesh(std::initializer_list<Vertex> vertices) :
		Mesh(vertices.begin(), vertices.size())
	{}

	Mesh::Mesh(const Vertex* vertices, size_t vertexCount) :
		Mesh()
	{
		setVertices(vertices, vertexCount);
	}

	Mesh::Mesh(Mesh&& other) noexcept {
		m_vao = other.m_vao;
		m_verticesVbo = other.m_verticesVbo;
		m_vertexCount = other.m_vertexCount;

		other.m_vao = 0;
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept {
		m_vao = other.m_vao;
		m_verticesVbo = other.m_verticesVbo;
		m_vertexCount = other.m_vertexCount;

		other.m_vao = 0;

		return *this;
	}

	Mesh::~Mesh() {
		if (m_vao != 0) {
			glDeleteVertexArrays(1, &m_vao);
			glDeleteBuffers(1, &m_verticesVbo);
		}
	}

	void Mesh::setVertices(const Vertex* vertices, size_t vertexCount) {
		if (m_vao == 0) {
			glGenVertexArrays(1, &m_vao);
			glBindVertexArray(m_vao);

			glGenBuffers(1, &m_verticesVbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_verticesVbo);
			glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);  // Position
			glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (void*)12); // Uv
			glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (void*)20); // Normal
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
		}

		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
		m_vertexCount = vertexCount;
	}

	unsigned Mesh::handle() const {
		return m_vao;
	}

	size_t Mesh::vertexCount() const {
		return m_vertexCount;
	}
}