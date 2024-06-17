#include "Context.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "Logger.hpp"
#include "Input.hpp"
#include "UniformBlock.hpp"

namespace luna {

	namespace {
		bool validContext = false;
		GLFWwindow* graphicsContext;

		float deltaTime = 0.0f;
		double time = 0.0f;

		std::unique_ptr<UniformBlock> cameraMatricesBlock;

		std::unique_ptr<Mesh> primitives[3];

		std::unique_ptr<ShaderProgram> blitShader;
		std::unique_ptr<Mesh> blitQuad;

		std::unique_ptr<Shader> defaultShader;
		std::unique_ptr<Shader> defaultUnlitShader;
		std::unique_ptr<Shader> defaultTranslucentShader;
		std::unique_ptr<Shader> defaultUnlitTranslucentShader;
		std::unique_ptr<Material> defaultMaterial;
		std::unique_ptr<Material> defaultUnlitMaterial;
		std::unique_ptr<Material> defaultTranslucentMaterial;
		std::unique_ptr<Material> defaultUnlitTranslucentMaterial;
		std::unique_ptr<Texture> defaultTexture;

		void loadPrimitives() {
			// Quad
			Vertex quadVertices[] = {
				Vertex(glm::vec3(-0.5f, +0.5f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
				Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
				Vertex(glm::vec3(+0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
				Vertex(glm::vec3(+0.5f, +0.5f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f))
			};
			unsigned int quadIndices[] = { 0,1,2, 2,3,0 };
			primitives[uint8_t(Primitive::Quad)] = std::make_unique<Mesh>(quadVertices, 4, quadIndices, 6);

			// Cube
			Vertex cubeVertices[] = {
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
			};
			unsigned int cubeIndices[] = {
				0,3,1, 3,2,1, 4,5,7, 7,5,6, 8,11,9, 11,10,9, 12,13,15, 15,13,14, 16,19,17, 19,18,17, 20,21,23, 23,21,22
			};
			primitives[uint8_t(Primitive::Cube)] = std::make_unique<Mesh>(cubeVertices, 24, cubeIndices, 36);

			// Teapot
			#include "Teapot.primitive"

			// Blitquad
			Vertex blitQuadVertices[] = {
				Vertex(glm::vec3(-1.0f, +1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
				Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
				Vertex(glm::vec3(+1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
				Vertex(glm::vec3(+1.0f, +1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
			};
			unsigned int blitQuadIndices[] = { 0,1,2, 2,3,0 };
			blitQuad = std::make_unique<Mesh>(blitQuadVertices, 4, blitQuadIndices, 6);
		}

		void loadShaders() {
			const char* vertSrc = "\
				#version 430 core\n\
				\n\
				layout(location = 0) in vec3 Position;\n\
				layout(location = 1) in vec2 UV;\n\
				layout(location = 2) in vec3 Normal;\n\
				layout(location = 3) in vec4 Color;\n\
				\n\
				out vec4 vertexColor;\n\
				out vec2 uv;\n\
				out vec3 normal;\n\
				\n\
				uniform vec4 MainColor;\n\
				uniform vec4 MainTexture_ST;\n\
				\n\
				layout(std140, binding = 0) uniform CameraMatrices { \n\
					mat4 ProjectionMatrix;\n\
					mat4 ViewMatrix;\n\
				};\n\
				uniform mat4 ModelMatrix;\n\
				\n\
				void main() {\n\
					gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);\n\
					vertexColor = MainColor * Color;\n\
					uv = UV * MainTexture_ST.xy + MainTexture_ST.zw;\n\
					normal = mat3(ModelMatrix) * Normal;\n\
				}\n";

			const char* fragSrc = "\
				#version 430 core\n\
				\n\
				in vec4 vertexColor;\n\
				in vec2 uv;\n\
				in vec3 normal;\n\
				\n\
				uniform sampler2D MainTexture;\n\
				\n\
				out vec4 fragColor;\n\
				\n\
				void main() {\n\
					fragColor = texture(MainTexture, uv) * vertexColor;\n\
					fragColor.rgb *= max(dot(normalize(normal), normalize(vec3(0.5, 1.0, -1.0))), 0.1) * 0.8 + (normal.y + 1.0) * 0.1;\n\
					if(fragColor.a == 0.0) discard;\n\
				}\n";

			const char* vertUnlitSrc = "\
				#version 430 core\n\
				\n\
				layout(location = 0) in vec3 Position;\n\
				layout(location = 1) in vec2 UV;\n\
				layout(location = 3) in vec4 Color;\n\
				\n\
				out vec4 vertexColor;\n\
				out vec2 uv;\n\
				\n\
				uniform vec4 MainColor;\n\
				uniform vec4 MainTexture_ST;\n\
				\n\
				layout(std140, binding = 0) uniform CameraMatrices { \n\
					mat4 ProjectionMatrix;\n\
					mat4 ViewMatrix;\n\
				};\n\
				uniform mat4 ModelMatrix;\n\
				\n\
				void main() {\n\
					gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);\n\
					vertexColor = MainColor * Color;\n\
					uv = UV * MainTexture_ST.xy + MainTexture_ST.zw;\n\
				}\n";

			const char* fragUnlitSrc = "\
				#version 430 core\n\
				\n\
				in vec4 vertexColor;\n\
				in vec2 uv;\n\
				\n\
				uniform sampler2D MainTexture;\n\
				\n\
				out vec4 fragColor;\n\
				\n\
				void main() {\n\
					fragColor = texture(MainTexture, uv) * vertexColor;\n\
					if(fragColor.a == 0.0) discard;\n\
				}\n";

			defaultShader = std::make_unique<Shader>(vertSrc, fragSrc);
			defaultTranslucentShader = std::make_unique<Shader>(vertSrc, fragSrc);
			defaultTranslucentShader->getProgram().setBlendMode(BlendMode::On);
			defaultTranslucentShader->getProgram().setRenderQueue(RenderQueue::Transparent);
			defaultUnlitShader = std::make_unique<Shader>(vertUnlitSrc, fragUnlitSrc);
			defaultUnlitTranslucentShader = std::make_unique<Shader>(vertUnlitSrc, fragUnlitSrc);
			defaultUnlitTranslucentShader->getProgram().setBlendMode(BlendMode::On);
			defaultUnlitTranslucentShader->getProgram().setRenderQueue(RenderQueue::Transparent);

			defaultTexture = std::make_unique<Texture>(Color::White);
			defaultMaterial = std::make_unique<Material>(defaultShader.get());
			defaultMaterial->setMainTexture(defaultTexture.get());
			defaultTranslucentMaterial = std::make_unique<Material>(defaultTranslucentShader.get());
			defaultTranslucentMaterial->setMainTexture(defaultTexture.get());
			defaultUnlitMaterial = std::make_unique<Material>(defaultUnlitShader.get());
			defaultUnlitMaterial->setMainTexture(defaultTexture.get());
			defaultUnlitTranslucentMaterial = std::make_unique<Material>(defaultUnlitTranslucentShader.get());
			defaultUnlitTranslucentMaterial->setMainTexture(defaultTexture.get());

			blitShader = std::make_unique<ShaderProgram>(
				"#version 430 core\nin vec3 Position;in vec2 UV;out vec2 i;void main(){gl_Position=vec4(Position,1);i=UV;}",
				"#version 430 core\nin vec2 i;uniform sampler2D t;out vec4 v;void main(){v=texture(t,i);}"
			);
			blitShader->uniform(blitShader->uniformId("t"), 0);
			blitShader->setDepthTestMode(DepthTestMode::Off);
			blitShader->setBlendMode(BlendMode::Off);
		}
		
		void loadUniformBlocks() {
			cameraMatricesBlock = std::make_unique<UniformBlock>();
			glm::mat4 initialData[] = { glm::mat4(1.0f), glm::mat4(1.0f) };
			cameraMatricesBlock->setContent(initialData, sizeof(initialData));
			cameraMatricesBlock->setBinding(0);
		}
	}

	void initialize() {
		// init glfw
		if (!glfwInit()) {
			log("An error occured while initializing GLFW", MessageSeverity::Error);
			return;
		}
		log("Initialized GLFW", MessageSeverity::Info);

		// glfw doesnt support contexts without a window, this is why we create a 1x1 invisible window
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		graphicsContext = glfwCreateWindow(1, 1, "Luna", nullptr, nullptr);
		Input::addWindow(graphicsContext);
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

		if (!graphicsContext) {
			log("An error occured while creating the graphics context", MessageSeverity::Error);
			return;
		}

		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(graphicsContext, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		// load opengl functions
		glfwMakeContextCurrent(graphicsContext);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			log("An error occured while loading OpenGL", MessageSeverity::Error);
			return;
		}
		log("Loaded OpenGL", MessageSeverity::Info);

		loadPrimitives();
		loadShaders();
		loadUniformBlocks();
		Input::initializeCursors();
		log("Loaded default assets", MessageSeverity::Info);

		log("Luna context created", MessageSeverity::Info);

		validContext = true;
	}

	void terminate() {
		for (int i = 0; i < sizeof(primitives) / sizeof(*primitives); i++)
			primitives[i].reset();

		defaultShader.reset();
		defaultTranslucentShader.reset();
		defaultUnlitShader.reset();
		defaultUnlitTranslucentShader.reset();
		blitShader.reset();

		defaultMaterial.reset();
		defaultTranslucentMaterial.reset();
		defaultUnlitMaterial.reset();
		defaultUnlitTranslucentMaterial.reset();

		defaultTexture.reset();

		cameraMatricesBlock.reset();
		log("Unloaded default assets", MessageSeverity::Info);

		glfwTerminate();
		log("Glfw context terminated", MessageSeverity::Info);

		log("Luna context terminated", MessageSeverity::Info);
	}

	bool isContextValid() {
		return validContext;
	}

	bool isCloseRequested() {
		return glfwWindowShouldClose(graphicsContext);
	}

	void* getGraphicsContext() {
		return graphicsContext;
	}

	void update() {
		Input::update();
		glfwPollEvents(); // todo: do this on separate thread

		double curTime = glfwGetTime();
		deltaTime = float(curTime - time);
		time = curTime;
	}

	float getDeltaTime() {
		return deltaTime;
	}

	float getTime() {
		return float(time);
	}

	const Mesh* getPrimitive(Primitive primitive) {
		return primitives[uint8_t(primitive)].get();
	}

	const Shader* getDefaultShader() {
		return defaultShader.get();
	}

	const Shader* getDefaultUnlitShader() {
		return defaultUnlitShader.get();
	}

	const Shader* getDefaultTranslucentShader() {
		return defaultTranslucentShader.get();
	}

	const Shader* getDefaultUnlitTranslucentShader() {
		return defaultUnlitTranslucentShader.get();
	}

	const Material* getDefaultMaterial() {
		return defaultMaterial.get();
	}

	const Material* getDefaultUnlitMaterial() {
		return defaultUnlitMaterial.get();
	}

	const Material* getDefaultTranslucentMaterial() {
		return defaultTranslucentMaterial.get();
	}

	const Material* getDefaultUnlitTranslucentMaterial() {
		return defaultUnlitTranslucentMaterial.get();
	}

	void blit(const Texture* source, RenderTarget* target) {
		target->makeActiveTarget();
		blitShader->bind();
		blitQuad->bind();
		source->bind(0);
		glDrawElements(GL_TRIANGLES, GLsizei(blitQuad->vertexCount()), GL_UNSIGNED_INT, nullptr);
	}

	void blit(const Texture* source, RenderTarget* target, const Material* material) {
		target->makeActiveTarget();
		material->bind();
		blitQuad->bind();
		source->bind(0);
		auto& program = material->getShader()->getProgram();
		program.uniform(program.uniformId("MainTexture"), 0);
		glDrawElements(GL_TRIANGLES, GLsizei(blitQuad->vertexCount()), GL_UNSIGNED_INT, nullptr);
	}

	void uploadCameraMatrices(const glm::mat4& projection, const glm::mat4& view) {
		glm::mat4 data[] = { projection, view };
		cameraMatricesBlock->setContent(data, sizeof(data));
	}
}
