#include "mesh.hpp"

#include <numeric>

namespace luna {

	static void bindAttributes(GLuint vao) {
		LUNA_GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, position)));
		LUNA_GLCALL(glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, uv)));
		LUNA_GLCALL(glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, normal)));
		LUNA_GLCALL(glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, color)));

		LUNA_GLCALL(glEnableVertexAttribArray(0));
		LUNA_GLCALL(glEnableVertexAttribArray(1));
		LUNA_GLCALL(glEnableVertexAttribArray(2));
		LUNA_GLCALL(glEnableVertexAttribArray(3));

		// instancing setup
		LUNA_GLCALL(glEnableVertexAttribArray(4));
		LUNA_GLCALL(glEnableVertexAttribArray(5));
		LUNA_GLCALL(glEnableVertexAttribArray(6));
		LUNA_GLCALL(glEnableVertexAttribArray(7));

		LUNA_GLCALL(glVertexAttribDivisor(4, 1));
		LUNA_GLCALL(glVertexAttribDivisor(5, 1));
		LUNA_GLCALL(glVertexAttribDivisor(6, 1));
		LUNA_GLCALL(glVertexAttribDivisor(7, 1));
	}

	Mesh::Mesh(std::span<const Vertex> vertices, BufferUsage usage) :
		m_vertexBuffer(BufferType::VertexBuffer, BufferAccess::Write, usage),
		m_indexBuffer(BufferType::VertexBuffer, BufferAccess::Write, usage)
	{
		std::vector<unsigned> indices(vertices.size());
		std::iota(indices.begin(), indices.end(), 0);

		LUNA_GLCALL(glGenVertexArrays(1, &m_gpuHandle));
		LUNA_GLCALL(glBindVertexArray(m_gpuHandle));
		m_indexBuffer.setData(indices.data(), indices.size() * sizeof(unsigned));
		m_vertexBuffer.setData(vertices.data(), vertices.size() * sizeof(Vertex));
		bindAttributes(m_gpuHandle);

		trace("Mesh created (id={})", m_gpuHandle);
	}

	Mesh::Mesh(std::span<const Vertex> vertices, std::span<const unsigned> indices, BufferUsage usage) :
		m_vertexBuffer(BufferType::VertexBuffer, BufferAccess::Write, usage),
		m_indexBuffer(BufferType::IndexBuffer, BufferAccess::Write, usage)
	{
		LUNA_GLCALL(glGenVertexArrays(1, &m_gpuHandle));
		LUNA_GLCALL(glBindVertexArray(m_gpuHandle));
		m_indexBuffer.setData(indices.data(), indices.size() * sizeof(unsigned));
		m_vertexBuffer.setData(vertices.data(), vertices.size() * sizeof(Vertex));
		bindAttributes(m_gpuHandle);

		trace("Mesh created (id={})", m_gpuHandle);
	}

	Mesh::Mesh(std::initializer_list<const Vertex> vertices, BufferUsage usage) :
		Mesh(std::span<const Vertex>(vertices.begin(), vertices.size()), usage)
	{}

	Mesh::Mesh(std::initializer_list<const Vertex> vertices, std::initializer_list<const unsigned> indices, BufferUsage usage) :
		Mesh(std::span<const Vertex>(vertices.begin(), vertices.size()), std::span<const unsigned>(indices.begin(), indices.size()), usage)
	{}

	Mesh::Mesh(Mesh&& other) noexcept :
		m_vertexBuffer(std::move(other.m_vertexBuffer)),
		m_indexBuffer(std::move(other.m_indexBuffer)),
		m_gpuHandle(other.m_gpuHandle)
	{
		other.m_gpuHandle = 0;
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept {
		if (m_gpuHandle) {
			LUNA_GLCALL(glDeleteVertexArrays(1, &m_gpuHandle));
			trace("Mesh destroyed (id={})", m_gpuHandle);
		}

		m_vertexBuffer = std::move(other.m_vertexBuffer);
		m_indexBuffer = std::move(other.m_indexBuffer);
		m_gpuHandle = other.m_gpuHandle;
		other.m_gpuHandle = 0;

		return *this;
	}

	Mesh::~Mesh() {
		if (m_gpuHandle) {
			LUNA_GLCALL(glDeleteVertexArrays(1, &m_gpuHandle));
			trace("Mesh destroyed (id={})", m_gpuHandle);
		}
	}

	void Mesh::setVertices(std::span<const Vertex> vertices) {
		m_vertexBuffer.setData(vertices.data(), vertices.size() * sizeof(Vertex));
	}

	void Mesh::setIndices(std::span<const unsigned> indices) {
		m_indexBuffer.setData(indices.data(), indices.size() * sizeof(unsigned));
	}
	
	size_t Mesh::vertexCount() const {
		return m_vertexBuffer.size() / sizeof(Vertex);
	}

	size_t Mesh::indexCount() const {
		return m_vertexBuffer.size() / sizeof(unsigned);
	}

	GLuint Mesh::internalHandle() const {
		return m_gpuHandle;
	}

}