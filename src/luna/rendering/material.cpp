#include "material.hpp"

namespace luna {

	Material::Material(const Shader* shader) :
		m_shader(shader),
		m_materialBlock(BufferType::UniformBuffer, BufferAccess::Write, BufferUsage::Dynamic, shader->materialBlockSize()),
		m_materialBlockData(shader->materialBlockSize()),
		m_samplers(shader->samplerCount())
	{}

	Material::Material(const Material& other) :
		m_shader(other.m_shader),
		m_materialBlock(BufferType::UniformBuffer, BufferAccess::Write, BufferUsage::Dynamic, other.m_materialBlockData.data(), other.m_materialBlock.size()),
		m_materialBlockData(other.m_materialBlockData),
		m_samplers(other.m_samplers)
	{}

	Material& Material::operator=(const Material& other) {
		m_shader = other.m_shader;
		m_materialBlock = Buffer(BufferType::UniformBuffer, BufferAccess::Write, BufferUsage::Dynamic, other.m_materialBlockData.data(), other.m_materialBlock.size());
		m_materialBlockData = other.m_materialBlockData;
		m_samplers = other.m_samplers;
		return *this;
	}

	void Material::setShader(const Shader* shader) {
		m_shader = shader;
		m_materialBlock.resize(shader->materialBlockSize());
		m_materialBlockData.resize(shader->materialBlockSize());
		m_samplers.resize(shader->samplerCount());
	}

	const Shader* Material::shader() const {
		return m_shader;
	}

	void Material::setMaterialBlockData(const void* data) {
		memcpy(m_materialBlockData.data(), data, m_materialBlockData.size());
		m_materialBlock.setData(data, m_materialBlockData.size(), 0);
	}

	const void* Material::getMaterialBlockdata() const {
		return m_materialBlockData.data();
	}

	const Buffer& Material::materialBlock() const {
		return m_materialBlock;
	}

	size_t Material::materialBlockSize() const {
		return m_shader->materialBlockSize();
	}

	size_t Material::getSamplerIndex(const std::string& name) {
		return m_shader->getSamplerIndex(name);
	}

	void Material::setSampler(size_t index, const Sampler* value) {
		if (index == size_t(-1)) {
			warn("Attempting to set a material's sampler at an invalid index");
			return;
		}
		m_samplers[index] = value;
	}

	void Material::setSampler(const std::string& name, const Sampler* value) {
		setSampler(getSamplerIndex(name), value);
	}

	const Sampler* Material::getSampler(size_t index) const {
		return m_samplers[index];
	}

}