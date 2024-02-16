#pragma once

#include "Shader.hpp"

namespace luna {

	class Material {
	public:
		Material() = default;
		explicit Material(Shader* shader);

		void setShader(Shader* shader);
		Shader* getShader() const;

		void bind() const;

	private:
		Shader* m_shader;

	};

}