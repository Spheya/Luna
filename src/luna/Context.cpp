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
		log("Luna context created!", MessageSeverity::Info);

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

}
