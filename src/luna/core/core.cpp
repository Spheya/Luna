#include "core.hpp"

#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../rendering/rendertype.hpp"
#include "teapot.hpp"

namespace luna {
	struct DefaultAssets {
		Shader defaultUnlitShader;
		Material defaultUnlitMaterial;

		Mesh primitives[3];

		DefaultAssets(Shader unlitShader, Mesh quad, Mesh cube, Mesh teapot) :
			defaultUnlitShader(std::move(unlitShader)),
			defaultUnlitMaterial(&defaultUnlitShader),
			primitives{ std::move(quad), std::move(cube), std::move(teapot) }
		{}
	};

	static std::unique_ptr<DefaultAssets> defaultAssets;
	static float deltaTime;
	static double currentTime;

	void initialize(const char* applicationName) {
		currentTime = glfwGetTime();
		registerRenderType("ShadowCaster");
		registerRenderType("LunaForward");
		loadDefaultAssets();

		LUNA_GLCALL(glEnable(GL_DEPTH_TEST));
		log("Luna initialized");
	}

	void terminate() {
		defaultAssets.reset();
		log("Luna terminated");
	}

	void update() {
		glfwPollEvents();

		double t = glfwGetTime();
		deltaTime = float(t - currentTime);
		currentTime = t;
	}

	float time() {
		return float(currentTime);
	}

	float deltatime() {
		return deltaTime;
	}

	const Material& getDefaultUnlitMaterial() {
		return defaultAssets->defaultUnlitMaterial;
	}

	const Shader& getDefaultUnlitShader() {
		return defaultAssets->defaultUnlitShader;
	}

	const Mesh& getPrimitive(Primitive primitive) {
		return defaultAssets->primitives[int(primitive)];
	}

	void loadDefaultAssets() {
		Shader defaultUnlitShader(16, {}, {
			ShaderSource::fromString(
				"#include <luna/core.glsl>\n"
				"PropertyBlock (Material) { vec4 _Color; };\n"
				"out vec4 color;\n"
				"void main() {\n"
				"  color = _Color;\n"
				"  gl_Position = _ProjViewMat * _ModelMat * vec4(VertexPosition, 1.0);\n"
				"}\n"
			, "defaultUnlit.vsh", "", ShaderStage::Vertex
			),
			ShaderSource::fromString(
				"in vec4 color;\n"
				"out vec4 outColor;\n"
				"void main() {\n"
				"  outColor = color;\n"
				"}\n"
			, "defaultUnlit.fsh", "", ShaderStage::Fragment
			)
		});

		Mesh quad(
			{
				luna::Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)),
				luna::Vertex(glm::vec3(+0.5f, -0.5f, 0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)),
				luna::Vertex(glm::vec3(+0.5f, +0.5f, 0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)),
				luna::Vertex(glm::vec3(-0.5f, +0.5f, 0.0f), glm::vec2(0.0f), glm::vec3(0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
			},
			{ 0, 2, 1, 2, 3, 0 }
		);

		Mesh cube(
			{
				Vertex(glm::vec3(-0.5f, +0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
				Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
				Vertex(glm::vec3(+0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
				Vertex(glm::vec3(+0.5f, +0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),

				Vertex(glm::vec3(-0.5f, +0.5f, +0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  0.0f, +1.0f)),
				Vertex(glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  0.0f, +1.0f)),
				Vertex(glm::vec3(+0.5f, -0.5f, +0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  0.0f, +1.0f)),
				Vertex(glm::vec3(+0.5f, +0.5f, +0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  0.0f, +1.0f)),

				Vertex(glm::vec3(+0.5f, +0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(+1.0f,  0.0f,  0.0f)),
				Vertex(glm::vec3(+0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(+1.0f,  0.0f,  0.0f)),
				Vertex(glm::vec3(+0.5f, -0.5f, +0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(+1.0f,  0.0f,  0.0f)),
				Vertex(glm::vec3(+0.5f, +0.5f, +0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(+1.0f,  0.0f,  0.0f)),

				Vertex(glm::vec3(-0.5f, +0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
				Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
				Vertex(glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
				Vertex(glm::vec3(-0.5f, +0.5f, +0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),

				Vertex(glm::vec3(-0.5f, +0.5f, +0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, +1.0f,  0.0f)),
				Vertex(glm::vec3(-0.5f, +0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, +1.0f,  0.0f)),
				Vertex(glm::vec3(+0.5f, +0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, +1.0f,  0.0f)),
				Vertex(glm::vec3(+0.5f, +0.5f, +0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, +1.0f,  0.0f)),

				Vertex(glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
				Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
				Vertex(glm::vec3(+0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
				Vertex(glm::vec3(+0.5f, -0.5f, +0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f))
			},
			{ 0,3,1, 3,2,1, 4,5,7, 7,5,6, 8,11,9, 11,10,9, 12,13,15, 15,13,14, 16,19,17, 19,18,17, 20,21,23, 23,21,22 }
		);

		Mesh teapot = loadTeapot();


		defaultAssets = std::make_unique<DefaultAssets>(std::move(defaultUnlitShader), std::move(quad), std::move(cube), std::move(teapot));

		glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
		defaultAssets->defaultUnlitMaterial.setMaterialBlockData(&white);

		log("Default assets loaded");
	}
}
