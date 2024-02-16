#include "Material.hpp"

namespace luna {

	Material::Material(Shader* shader) :
		m_shader(shader)
	{}

	void Material::setShader(Shader* shader) {
		m_shader = shader;
	}
	
	Shader* Material::getShader() const {
		return m_shader;
	}

	void Material::bind() const {
		m_shader->getProgram().bind();
	}

}