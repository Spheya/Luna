#include "Mesh.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>
#include <glad/glad.h>
#include <numeric>

#include "Logger.hpp"

namespace luna {

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

	Mesh::Mesh(Mesh&& other) noexcept :
		m_vao(other.m_vao),
		m_vbos{ other.m_vbos[0], other.m_vbos[1]},
		m_vertexCount(other.m_vertexCount),
		m_vertexBufferSize(other.m_vertexBufferSize),
		m_indexBufferSize(other.m_indexBufferSize)
	{
		other.m_vao = 0;
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept {
		Mesh::~Mesh();

		m_vao = other.m_vao;
		m_vbos[Vertices] = other.m_vbos[Vertices];
		m_vbos[Indices] = other.m_vbos[Indices];
		m_vertexCount = other.m_vertexCount;
		m_vertexBufferSize = other.m_vertexBufferSize;
		m_indexBufferSize = other.m_indexBufferSize;

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
		glVertexAttribPointer(0, 3,         GL_FLOAT, false, sizeof(Vertex), (void*)0);  // Position
		glVertexAttribPointer(1, 2,         GL_FLOAT, false, sizeof(Vertex), (void*)12); // Uv
		glVertexAttribPointer(2, 3,         GL_FLOAT, false, sizeof(Vertex), (void*)20); // Normal
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE,  true, sizeof(Vertex), (void*)32); // Color
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
	}

	void Mesh::setVertices(const Vertex* vertices, size_t size) {
		if (m_vao == 0)
			init();

		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[Vertices]);

		if (m_vertexBufferSize < size) {
			glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex), vertices, GL_STATIC_DRAW);
			m_vertexBufferSize = size;
		} else {
			glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(Vertex), vertices);
		}
	}

	void Mesh::setIndices(const unsigned int* indices, size_t size) {
		if (m_vao == 0)
			init();

		m_vertexCount = size;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbos[Indices]);

		if (m_indexBufferSize < size) {
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), indices, GL_STATIC_DRAW);
			m_indexBufferSize = size;
		} else {
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size * sizeof(unsigned int), indices);
		}
	}

	void Mesh::bind() const {
		glBindVertexArray(m_vao);
	}

	unsigned int Mesh::getInternalHandle() const {
		return m_vao;
	}

	size_t Mesh::vertexCount() const {
		return m_vertexCount;
	}

	Mesh Mesh::loadFromFile(const char* filepath) {
		std::string fn(filepath);
		std::string extension = fn.substr(fn.find_last_of('.') + 1);
		
		if (extension == "obj") {
			return loadObj(filepath);
		}

		log("." + extension + " model files are not supported by Luna", luna::MessageSeverity::Error);
		return Mesh();
	}

	Mesh Mesh::loadObj(const char* filepath) {
		tinyobj::ObjReaderConfig config;
		tinyobj::ObjReader reader;

		if (!reader.ParseFromFile(filepath, config)) {
			if (!reader.Error().empty()) {
				log("Error occured while loading model: " + reader.Error(), MessageSeverity::Error);
				return Mesh();
			}
		}

		if (!reader.Warning().empty())
			log("Warning from loading model: " + reader.Warning(), MessageSeverity::Warning);

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();

		std::vector<Vertex> vertices;
		vertices.reserve(shapes.size() * 3);

		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces(polygon)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					glm::vec3 pos = glm::vec3(
						attrib.vertices[size_t(index.vertex_index) * 3 + 0],
						attrib.vertices[size_t(index.vertex_index) * 3 + 1],
						attrib.vertices[size_t(index.vertex_index) * 3 + 2]
					);

					glm::vec2 uv(0.0f);
					if (index.texcoord_index >= 0) {
						uv = glm::vec2(
							attrib.texcoords[size_t(index.texcoord_index) * 2 + 0],
							attrib.texcoords[size_t(index.texcoord_index) * 2 + 1]
						);
					}

					glm::vec3 normal(0.0f);
					if (index.normal_index >= 0) {
						normal = glm::vec3(
							attrib.normals[size_t(index.normal_index) * 3 + 0],
							attrib.normals[size_t(index.normal_index) * 3 + 1],
							attrib.normals[size_t(index.normal_index) * 3 + 2]
						);
					}

					vertices.emplace_back(pos, uv, normal);
				}

				index_offset += fv;
			}
		}

		return Mesh(vertices.data(), vertices.size());
	}
}
