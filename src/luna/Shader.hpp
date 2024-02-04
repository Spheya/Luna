#pragma once

#include "Math.hpp"

namespace luna {

	class Shader {
	public:
		Shader();
		Shader(const char* vertexSource, const char* fragmentSource);
		Shader(Shader&) = delete;
		Shader& operator=(Shader&) = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;
		~Shader();

		void load(const char* vertexSource, const char* fragmentSource);
		bool isValid() const;

		int uniformId(const char* name) const;
		void uniform(int id, float value);
		void uniform(int id, glm::vec1 value);
		void uniform(int id, glm::vec2 value);
		void uniform(int id, glm::vec3 value);
		void uniform(int id, glm::vec4 value);
		void uniform(int id, glm::mat3 value);
		void uniform(int id, glm::mat4 value);

		void bind() const;

	private:
		unsigned int m_program;
		bool m_valid;

	};

}