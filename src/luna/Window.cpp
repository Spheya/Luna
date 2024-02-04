#include "Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>

namespace luna {
	
	std::weak_ptr<GlfwContext> GlfwContext::s_instance;

	GlfwContext::GlfwContext() {
		glfwInit();
	}

	GlfwContext::~GlfwContext() {
		glfwTerminate();
	}

	std::shared_ptr<GlfwContext> GlfwContext::getReference() {
		if (s_instance.expired()) {
			std::shared_ptr<GlfwContext> context(new GlfwContext());
			s_instance = context;
			return context;
		} else {
			return s_instance.lock();
		}
	}

	Window::Window(const char* title, glm::ivec2 size) :
		m_isValid(false)
	{
		assert(size.x > 0);
		assert(size.y > 0);

		m_contextReference = GlfwContext::getReference();

		m_windowHandle = glfwCreateWindow(size.x, size.y, title, nullptr, nullptr);
		makeCurrentContext();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			return;
		}

		m_isValid = true;
	}

	Window::Window(const char* title, int width, int height, bool fullscreen) :
		Window(title, glm::ivec2(width, height))
	{}

	Window::~Window() {
		glfwDestroyWindow(m_windowHandle);
	}

	bool Window::isValid() const {
		return m_isValid;
	}

	void Window::update() {
		glfwSwapBuffers(m_windowHandle);
		glfwPollEvents();
	}

	void Window::makeCurrentContext() {
		if(m_contextReference->m_currentGlContext != m_windowHandle)
			glfwMakeContextCurrent(m_windowHandle);
	}

	bool Window::isCloseRequested() const {
		return glfwWindowShouldClose(m_windowHandle);
	}

	void Window::clear(Color color) {
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
}