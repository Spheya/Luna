#include "Context.hpp"

#include "Logger.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace luna {

	Context::Context() : 
		m_graphicsContext(nullptr),
		m_valid(false)
	{
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
		m_graphicsContext = glfwCreateWindow(1, 1, "Luna", nullptr, nullptr);
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

		if (!m_graphicsContext) {
			log("An error occured while creating a window", MessageSeverity::Error);
			return;
		}

		// load opengl functions
		glfwMakeContextCurrent(m_graphicsContext);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			log("An error occured while loading OpenGL", MessageSeverity::Error);
			return;
		}
		log("Loaded OpenGL", MessageSeverity::Info);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		loadPrimitives();

		log("Luna context created", MessageSeverity::Info);

		m_valid = true;
	}

	Context::~Context() {
		glfwTerminate();
	}

	bool Context::isValid() const {
		return m_valid;
	}

	bool Context::isCloseRequested() const {
		glfwPollEvents();
		return glfwWindowShouldClose(m_graphicsContext);
	}

	GLFWwindow* Context::getGraphicsContext() const {
		return m_graphicsContext;
	}

	void Context::update() {
		double curTime = glfwGetTime();
		m_deltatime = float(curTime - m_prevTime);
		m_prevTime = curTime;
	}

	float Context::getDeltatime() const {
		return m_deltatime;
	}

	float Context::getTime() const {
		return float(m_prevTime);
	}

	const Mesh* Context::getPrimitive(Primitive primitive) const {
		return &m_primitives[uint8_t(primitive)];
	}

	void Context::loadPrimitives() {
		// Quad
		Vertex quadVertices[] = {
			Vertex(glm::vec3(-1.0f, +1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
			Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
			Vertex(glm::vec3(+1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
			Vertex(glm::vec3(+1.0f, +1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f))
		};
		unsigned int quadIndices[] = { 0,1,2, 2,3,0 };
		m_primitives[uint8_t(Primitive::Quad)].setVertices(quadVertices, 4);
		m_primitives[uint8_t(Primitive::Quad)].setIndices(quadIndices, 6);

		// Cube
		Vertex cubeVertices[] = {
			Vertex(glm::vec3(-0.5f, +0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f,  0.0f, -1.0f)),
			Vertex(glm::vec3(+0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f,  0.0f, -1.0f)),
			Vertex(glm::vec3(+0.5f, +0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f)),

			Vertex(glm::vec3(-0.5f, +0.5f, +0.5f), glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, +1.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f,  0.0f, +1.0f)),
			Vertex(glm::vec3(+0.5f, -0.5f, +0.5f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f,  0.0f, +1.0f)),
			Vertex(glm::vec3(+0.5f, +0.5f, +0.5f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f,  0.0f, +1.0f)),
			
			Vertex(glm::vec3(+0.5f, +0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(+1.0f,  0.0f,  0.0f)),
			Vertex(glm::vec3(+0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(+1.0f,  0.0f,  0.0f)),
			Vertex(glm::vec3(+0.5f, -0.5f, +0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(+1.0f,  0.0f,  0.0f)),
			Vertex(glm::vec3(+0.5f, +0.5f, +0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(+1.0f,  0.0f,  0.0f)),
			
			Vertex(glm::vec3(-0.5f, +0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
			Vertex(glm::vec3(-0.5f, +0.5f, +0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f)),
			
			Vertex(glm::vec3(-0.5f, +0.5f, +0.5f), glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f, +1.0f,  0.0f)),
			Vertex(glm::vec3(-0.5f, +0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f, +1.0f,  0.0f)),
			Vertex(glm::vec3(+0.5f, +0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f, +1.0f,  0.0f)),
			Vertex(glm::vec3(+0.5f, +0.5f, +0.5f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f, +1.0f,  0.0f)),
			
			Vertex(glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f)),
			Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f, -1.0f,  0.0f)),
			Vertex(glm::vec3(+0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f, -1.0f,  0.0f)),
			Vertex(glm::vec3(+0.5f, -0.5f, +0.5f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f))
		};
		unsigned int cubeIndices[] = { 
			0,3,1, 3,2,1, 4,5,7, 7,5,6, 8,11,9, 11,10,9, 12,13,15, 15,13,14, 16,19,17, 19,18,17, 20,21,23, 23,21,22
		};
		m_primitives[uint8_t(Primitive::Cube)].setVertices(cubeVertices, 24);
		m_primitives[uint8_t(Primitive::Cube)].setIndices(cubeIndices, 36);
	}
}
