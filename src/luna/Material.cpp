#include "Material.hpp"

namespace luna {

	Material::Material() {
		setMainColor(Color::White);
		setMainTextureScaleTranslation(glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
	}

	Material::Material(const Shader* shader) :
		m_shader(shader)
	{
		setMainColor(Color::White);
		setMainTextureScaleTranslation(glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
	}

	void Material::setShader(const Shader* shader) {
		m_shader = shader;
	}
	
	const Shader* Material::getShader() const {
		return m_shader;
	}

	size_t Material::getTextureCount() const {
		return m_textureParams.size();
	}

	void Material::bind() const {
		const ShaderProgram* program = &m_shader->getProgram();
		program->bind();

		for (const auto& param : m_floatParams) program->uniform(program->uniformId(param.name.c_str()), param.value);
		for (const auto& param : m_vec2Params)  program->uniform(program->uniformId(param.name.c_str()), param.value);
		for (const auto& param : m_vec3Params)  program->uniform(program->uniformId(param.name.c_str()), param.value);
		for (const auto& param : m_vec4Params)  program->uniform(program->uniformId(param.name.c_str()), param.value);
		for (const auto& param : m_mat3Params)  program->uniform(program->uniformId(param.name.c_str()), param.value);
		for (const auto& param : m_mat4Params)  program->uniform(program->uniformId(param.name.c_str()), param.value);

		for (int i = 0; i < m_textureParams.size(); i++) {
			program->uniform(program->uniformId(m_textureParams[i].name.c_str()), i);
			m_textureParams[i].value->bind(i);
		}
	}

	void Material::setMainColor(Color color) {
		setValue("MainColor", color);
	}

	void Material::setMainTexture(const Texture* value) {
		setValue("MainTexture", value);
	}

	void Material::setMainTextureScaleTranslation(glm::vec4 value) {
		setValue("MainTexture_ST", value);
	}

	void Material::setMainTextureScaleTranslation(glm::vec2 scale, glm::vec2 translation) {
		setValue("MainTexture_ST", glm::vec4(scale, translation));
	}

	void Material::setValue(const char* name, float value) {
		setParam(m_floatParams, name, value);
	}

	void Material::setValue(const char* name, glm::vec1 value) {
		setParam(m_floatParams, name, value.r);
	}

	void Material::setValue(const char* name, glm::vec2 value) {
		setParam(m_vec2Params, name, value);
	}

	void Material::setValue(const char* name, glm::vec3 value) {
		setParam(m_vec3Params, name, value);
	}

	void Material::setValue(const char* name, glm::vec4 value) {
		setParam(m_vec4Params, name, value);
	}

	void Material::setValue(const char* name, glm::mat3& value) {
		setParam(m_mat3Params, name, value);
	}

	void Material::setValue(const char* name, glm::mat4& value) {
		setParam(m_mat4Params, name, value);
	}

	void Material::setValue(const char* name, Color value) {
		setParam(m_vec4Params, name, value.vec4());
	}

	void Material::setValue(const char* name, const Texture* value) {
		setParam(m_textureParams, name, value);
	}
}