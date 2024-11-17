#include "shader.hpp"

#include "../core/logger.hpp"

namespace luna {

	static RenderType validateRenderType(std::span<const ShaderSource> modules) {
		RenderType renderType = NullRenderType;
		ShaderStage definingStage = ShaderStage::Vertex;
		for (auto shaderModule : modules) {
			if (shaderModule.renderType != renderType && shaderModule.renderType != NullRenderType) {
				if (renderType == NullRenderType) {
					renderType = shaderModule.renderType;
					definingStage = shaderModule.stage;
					continue;
				} else {
					error(
						"Shader error: RenderType mismatch, '{}' does not match '{}'",
						getRenderTypeName(shaderModule.renderType),
						getRenderTypeName(renderType)
					);
					return NullRenderType;
				}
			}
		}
		return renderType;
	}

	Shader::Shader(size_t materialBlockSize) :
		m_materialBlockSize(materialBlockSize)
	{}

	Shader::Shader(size_t materialBlockSize, std::span<const std::string> samplers, std::span<const ShaderSource> modules) :
		Shader(materialBlockSize)
	{
		addPass(samplers, modules);
	}

	Shader::Shader(size_t materialBlockSize, std::initializer_list<std::string> samplers, std::initializer_list<ShaderSource> modules) :
		Shader(materialBlockSize) 
	{
		addPass(samplers, modules);
	}

	void Shader::addPass(std::span<const std::string> samplers, std::span<const ShaderSource> modules) {
		// todo: multi compilation
		
		RenderType type = validateRenderType(modules);
		if (m_renderTypeData.size() >= type)
			m_renderTypeData.resize(type + 1);
		if (m_renderTypeData[type]) {
			error("Shader error: Shader already contains a pass for RenderType '{}'", getRenderTypeName(type));
			return;
		}

		m_renderTypeData[type] = std::make_unique<RenderTypeShaderData>(ShaderProgram(samplers, modules));
		auto& renderTypeData = m_renderTypeData[type];

		// Add samplers
		for (size_t i = 0; i < samplers.size(); ++i) {
			auto it = m_samplers.find(samplers[i]);
			if (it == m_samplers.end()) {
				renderTypeData->samplerIndices.push_back(m_samplers.size());
				m_samplers.emplace(samplers[i], m_samplers.size());
			} else {
				renderTypeData->samplerIndices.push_back(it->second);
			}
		}
	}

	void Shader::addPass(std::initializer_list<std::string> samplers, std::initializer_list<ShaderSource> modules) {
		addPass(std::span(samplers.begin(), samplers.size()), std::span(modules.begin(), modules.size()));
	}

	bool Shader::hasPass(RenderType renderType) const {
		if (renderType > m_renderTypeData.size()) return false;
		return m_renderTypeData[renderType] != nullptr;
	}

	const ShaderProgram& Shader::shaderProgram(RenderType renderType) const {
		return m_renderTypeData[renderType]->program;
	}

	std::span<const size_t> Shader::samplerIndices(RenderType renderType) const {
		return m_renderTypeData[renderType]->samplerIndices;
	}

	size_t Shader::getSamplerIndex(const std::string& samplerName) const {
		auto it = m_samplers.find(samplerName);
		if (it == m_samplers.end())
			return size_t(-1);
		return it->second;
	}

	size_t Shader::samplerCount() const {
		return m_samplers.size();
	}

	size_t Shader::materialBlockSize() const {
		return m_materialBlockSize;
	}

}