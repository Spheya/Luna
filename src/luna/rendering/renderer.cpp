#include "renderer.hpp"

#include <algorithm>

#include "../platform/platform.hpp"
#include "../platform/graphics.hpp"

namespace luna {

	static void bindMaterial(const Material& material, std::span<const size_t> samplerIndices) {
		graphics::setPropertyBlockData(luna::PropertyBlockType::Material, material.materialBlock());
		for (int i = 0; i < int(samplerIndices.size()); ++i) {
			auto* sampler = material.getSampler(samplerIndices[i]);
			if (sampler)
				sampler->bindSampler(i);
		}
			
	}

	Renderer::Renderer() :
		m_cameraBuffer(BufferType::UniformBuffer, BufferAccess::Write, BufferUsage::Dynamic),
		m_instancingBuffer(BufferType::VertexBuffer, BufferAccess::Write, BufferUsage::Dynamic)
	{}

	Renderer::CullResults Renderer::cullDrawables(const Camera& camera, std::span<const Drawable> drawables) {
		// todo: actually cull them
		CullResults result{
			std::vector<Drawable>(drawables.begin(), drawables.end())
		};

		// todo: sort by distance
		std::sort(result.drawables.begin(), result.drawables.end(), [](const Drawable& a, const Drawable& b) {
			if (a.material->shader() != b.material->shader()) return a.material->shader() < b.material->shader();
			if (a.material != b.material) return a.material < b.material;
			if (a.mesh != b.mesh) return a.mesh < b.mesh;
			return false;
		});

		return result;
	}

	void Renderer::bindInstancingData(const CullResults& drawables) {
		std::vector<glm::mat4> matrixData(drawables.drawables.size());
		for (size_t i = 0; i < drawables.drawables.size(); ++i)
			matrixData[i] = drawables.drawables[i].transform;

		if (matrixData.size() * sizeof(glm::mat4) > m_instancingBuffer.size()) {
			m_instancingBuffer.setData(matrixData.data(), sizeof(glm::mat4) * matrixData.size());
		} else {
			m_instancingBuffer.setData(matrixData.data(), sizeof(glm::mat4) * matrixData.size(), 0);
		}
	}

	void Renderer::renderDrawables(const CullResults& cullResults, RenderType renderType) {
		if (cullResults.drawables.empty())
			return;

		size_t batchStart = size_t(-1);
		for (size_t i = 0; i < cullResults.drawables.size(); ++i) {
			if (cullResults.drawables[i].material->shader()->hasPass(renderType)) {
				batchStart = i;
				break;
			}
		}
		if (batchStart == size_t(-1))
			return;

		bool supportsRenderType = true;
		const Material* boundMaterial = cullResults.drawables[batchStart].material;
		const Shader* boundShader = cullResults.drawables[batchStart].material->shader();
		const Mesh* boundMesh = cullResults.drawables[batchStart].mesh;
		graphics::bindShader(boundShader->shaderProgram(renderType));
		bindMaterial(*boundMaterial, boundShader->samplerIndices(renderType));
		
		for (size_t i = batchStart; i < cullResults.drawables.size(); ++i) {
			auto& drawable = cullResults.drawables[i];
			const Material* material = drawable.material;
			const Shader* shader = drawable.material->shader();
			const Mesh* mesh = drawable.mesh;
			bool drawableSupportsRenderType = shader->hasPass(renderType);

			if (!drawableSupportsRenderType && !supportsRenderType)
				continue;

			if ((!drawableSupportsRenderType && supportsRenderType) || mesh != boundMesh || material != boundMaterial || shader != boundShader) {
				graphics::drawInstanced(*boundMesh, i - batchStart, batchStart);
				batchStart = i;
			}

			if (drawableSupportsRenderType && !supportsRenderType) {
				batchStart = i;
			}

			if (shader != boundShader) {
				boundShader = shader;
				graphics::bindShader(boundShader->shaderProgram(renderType));
			}

			if (material != boundMaterial) {
				boundMaterial = material;
				bindMaterial(*boundMaterial, boundShader->samplerIndices(renderType));
			}

			if (mesh != boundMesh)
				boundMesh = mesh;

			supportsRenderType = drawableSupportsRenderType;
		}

		if(supportsRenderType)
			graphics::drawInstanced(*boundMesh, cullResults.drawables.size() - batchStart, batchStart);
	}

	void Renderer::bindCameraData(const CameraPropertyBlock& cameraproperties) {
		m_cameraBuffer.setData(&cameraproperties, sizeof(CameraPropertyBlock));
		graphics::setPropertyBlockData(PropertyBlockType::Camera, m_cameraBuffer);
	}

}