#include "Context.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "Logger.hpp"

namespace luna {

	namespace {
		bool validContext = false;
		GLFWwindow* graphicsContext;

		float deltatime = 0.0f;
		double time = 0.0f;

		std::unique_ptr<Mesh> primitives[2];

		std::unique_ptr<ShaderProgram> blitShader;
		std::unique_ptr<Mesh> blitQuad;

		std::unique_ptr<Shader> defaultShader;
		std::unique_ptr<Material> defaultMaterial;

		void loadPrimitives() {
			// Quad
			Vertex quadVertices[] = {
				Vertex(glm::vec3(-1.0f, +1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
				Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
				Vertex(glm::vec3(+1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
				Vertex(glm::vec3(+1.0f, +1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f))
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

			// Blitquad
			Vertex vertices[] = {
				Vertex(glm::vec3(-1.0f, +1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
				Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
				Vertex(glm::vec3(+1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
				Vertex(glm::vec3(+1.0f, +1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
			};
			unsigned int indices[] = { 0,1,2, 2,3,0 };
			blitQuad = std::make_unique<Mesh>(vertices, 4, indices, 6);
		}

		void loadShaders() {
			defaultShader = std::make_unique<Shader>(
				"\
				#version 330 core\n\
				\n\
				layout(location = 0) in vec3 Position;\n\
				layout(location = 1) in vec2 UV;\n\
				layout(location = 2) in vec3 Normal;\n\
				\n\
				out vec4 vertexColor;\n\
				\n\
				uniform vec4 MainColor;\n\
				\n\
				uniform mat4 ProjectionMatrix;\n\
				uniform mat4 ViewMatrix;\n\
				uniform mat4 ModelMatrix;\n\
				\n\
				void main() {\n\
					gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);\n\
					vertexColor = vec4(Normal * 0.5 + 0.5, 0.0);\n\
				}\n",

				"\
				#version 330 core\n\
				\n\
				in vec4 vertexColor;\n\
				\n\
				out vec4 FragColor;\n\
				\n\
				void main() {\n\
					FragColor = vertexColor;\n\
				}\n"
			);
			defaultMaterial = std::make_unique<Material>(defaultShader.get());

			blitShader = std::make_unique<ShaderProgram>(
				"#version 330 core\nin vec3 Position;in vec2 UV;out vec2 i;void main(){gl_Position=vec4(Position,1);i=UV;}",
				"#version 330 core\nin vec2 i;uniform sampler2D t;out vec4 v;void main(){v=texture(t,i);}"
			);
			blitShader->uniform(blitShader->uniformId("t"), 0);
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
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		graphicsContext = glfwCreateWindow(1, 1, "Luna", nullptr, nullptr);
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

		if (!graphicsContext) {
			log("An error occured while creating the graphics context", MessageSeverity::Error);
			return;
		}

		// load opengl functions
		glfwMakeContextCurrent(graphicsContext);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			log("An error occured while loading OpenGL", MessageSeverity::Error);
			return;
		}
		log("Loaded OpenGL", MessageSeverity::Info);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		loadPrimitives();
		loadShaders();

		log("Luna context created", MessageSeverity::Info);

		validContext = true;
	}

	void terminate() {
		for (int i = 0; i < sizeof(primitives) / sizeof(*primitives); i++)
			primitives[i].release();
		defaultShader.release();
		defaultMaterial.release();
		blitShader.release();

		glfwTerminate();
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
		glfwPollEvents(); // todo: do this on separate thread
		double curTime = glfwGetTime();
		deltatime = float(curTime - time);
		time = curTime;
	}

	float getDeltatime() {
		return deltatime;
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

	const Material* getDefaultMaterial() {
		return defaultMaterial.get();
	}

	void blit(Texture* source, RenderTarget* target) {
		target->makeActiveTarget();
		source->bind(0);
		blitShader->bind();
		blitQuad->bind();
		glDrawElements(GL_TRIANGLES, GLsizei(blitQuad->vertexCount()), GL_UNSIGNED_INT, nullptr);
	}
}
