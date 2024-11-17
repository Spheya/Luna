#pragma once

#include "../platform/mesh.hpp"
#include "../rendering/primitive.hpp"
#include "../rendering/material.hpp"

namespace luna {

	void initialize(const char* applicationName);
	void terminate();
	void update();

	float time();
	float deltatime();

	const Material& getDefaultUnlitMaterial();
	const Shader& getDefaultUnlitShader();
	const Mesh& getPrimitive(Primitive primitive);

	void loadDefaultAssets();

}