#pragma once

#include "Math.hpp"
#include "Color.hpp"

namespace luna {

	class ShaderProgram {
	public:
		ShaderProgram();
		ShaderProgram(const char* vertexSource, const char* fragmentSource);
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&& other) noexcept;
		ShaderProgram& operator=(ShaderProgram&& other) noexcept;
		~ShaderProgram();

		void load(const char* vertexSource, const char* fragmentSource);
		bool isValid() const;

		int uniformId(const char* name) const;
		void uniform(int id, int value) const;
		void uniform(int id, float value) const;
		void uniform(int id, glm::vec1 value) const;
		void uniform(int id, glm::vec2 value) const;
		void uniform(int id, glm::vec3 value) const;
		void uniform(int id, glm::vec4 value) const;
		void uniform(int id, glm::mat3 value) const;
		void uniform(int id, glm::mat4 value) const;
		void uniform(int id, Color value) const;

		void bind() const;

	private:
		unsigned int m_program;
		bool m_valid;

	};

}