#pragma once

#include <vector>

#include "shader.hpp"
#include "../platform/buffer.hpp"
#include "../platform/texture.hpp"

namespace luna {

	class Material {
	public:
		explicit Material(const Shader* shader);
		Material(const Material& other);
		Material& operator=(const Material& other);
		Material(Material&&) = default;
		Material& operator=(Material&&) = default;
		~Material() = default;

		void setShader(const Shader* shader);
		const Shader* shader() const;

		void setMaterialBlockData(const void* data);
		const void* getMaterialBlockdata() const;
		const Buffer& materialBlock() const;
		size_t materialBlockSize() const;

		size_t getSamplerIndex(const std::string& name);
		void setSampler(size_t index, const Sampler* value);
		void setSampler(const std::string& name, const Sampler* value);
		const Sampler* getSampler(size_t index) const;

	private:
		const Shader* m_shader;

		Buffer m_materialBlock;
		std::vector<uint8_t> m_materialBlockData;
		std::vector<const Sampler*> m_samplers;
	};

}