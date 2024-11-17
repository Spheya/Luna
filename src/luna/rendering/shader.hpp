#pragma once

#include <unordered_map>

#include "../platform/shaderprogram.hpp"
#include "shaderpreprocessor.hpp"
#include "rendertype.hpp"

namespace luna {

	class Shader {
	public:
		Shader(size_t materialBlockSize);
		Shader(size_t materialBlockSize, std::span<const std::string> samplers, std::span<const ShaderSource> modules);
		Shader(size_t materialBlockSize, std::initializer_list<std::string> samplers, std::initializer_list<ShaderSource> modules);

		void addPass(std::span<const std::string> samplers, std::span<const ShaderSource> modules);
		void addPass(std::initializer_list<std::string> samplers, std::initializer_list<ShaderSource> modules);
		bool hasPass(RenderType renderType) const;

		const ShaderProgram& shaderProgram(RenderType renderType) const;
		std::span<const size_t> samplerIndices(RenderType renderType) const;
		size_t getSamplerIndex(const std::string& samplerName) const;
		size_t samplerCount() const;

		size_t materialBlockSize() const;

	private:
		struct RenderTypeShaderData {
			ShaderProgram program;
			std::vector<size_t> samplerIndices;
		};

	private:
		std::vector<std::unique_ptr<RenderTypeShaderData>> m_renderTypeData;
		size_t m_materialBlockSize;
		size_t m_samplerCount;
		std::unordered_map<std::string, size_t> m_samplers;
	};

}