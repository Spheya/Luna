#pragma once

#include <vector>
#include <string>

#include "Shader.hpp"
#include "Texture.hpp"

namespace luna {

	class Material {
	public:
		Material();
		explicit Material(Shader* shader);

		void setShader(Shader* shader);
		Shader* getShader() const;

		void bind() const;

		void setMainColor(Color value);
		void setMainTexture(const Texture* value);
		void setMainTextureScaleTranslation(const glm::vec4 value);
		void setValue(const char* name, float value);
		void setValue(const char* name, glm::vec1 value);
		void setValue(const char* name, glm::vec2 value);
		void setValue(const char* name, glm::vec3 value);
		void setValue(const char* name, glm::vec4 value);
		void setValue(const char* name, glm::mat3& value);
		void setValue(const char* name, glm::mat4& value);
		void setValue(const char* name, Color value);
		void setValue(const char* name, const Texture* value);

	private:
		template<typename T>
		struct Parameter {
			std::size_t hash;
			std::string name;
			T value;
		};

		template<typename T>
		void setParam(std::vector<Material::Parameter<T>>& vector, const std::string& name, T value) {
			size_t hash = std::hash<std::string>{}(name);
			for (auto& param : vector) {
				if (param.hash == hash) {
					param.value = value;
					return;
				}
			}

			vector.emplace_back(hash, name, value);
		}

		std::vector<Parameter<const Texture*>> m_textureParams;
		std::vector<Parameter<float>> m_floatParams;
		std::vector<Parameter<glm::vec2>> m_vec2Params;
		std::vector<Parameter<glm::vec3>> m_vec3Params;
		std::vector<Parameter<glm::vec4>> m_vec4Params;
		std::vector<Parameter<glm::mat3>> m_mat3Params;
		std::vector<Parameter<glm::mat4>> m_mat4Params;
		Shader* m_shader = nullptr;
	};

}