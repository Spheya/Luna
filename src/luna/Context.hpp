#pragma once

#include <vector>
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "RenderTarget.hpp"
#include "Texture.hpp"

namespace luna {

	enum class Primitive : uint8_t {
		Quad, Cube, Teapot
	};

	void initialize();
	void terminate();

	bool isContextValid();
	void* getGraphicsContext();

	void update();

	float getDeltaTime();
	float getTime();

	const Mesh* getPrimitive(Primitive primitive);
	const Shader* getDefaultShader();
	const Shader* getDefaultUnlitShader();
	const Shader* getDefaultTranslucentShader();
	const Shader* getDefaultUnlitTranslucentShader();
	const Material* getDefaultMaterial();
	const Material* getDefaultUnlitMaterial();
	const Material* getDefaultTranslucentMaterial();
	const Material* getDefaultUnlitTranslucentMaterial();

	void blit(const Texture* source, RenderTarget* target);
	void blit(const Texture* source, RenderTarget* target, const Material* material);

	bool isCloseRequested();

	void uploadCameraMatrices(const glm::mat4& projection, const glm::mat4& view);

}