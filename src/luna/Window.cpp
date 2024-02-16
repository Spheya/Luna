#include "Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Logger.hpp"

namespace luna {

	Window::Window(const Context& context, const char* title, glm::ivec2 size) :
		m_isValid(false)
	{
		m_windowHandle = glfwCreateWindow(size.x, size.y, title, nullptr, context.getGraphicsContext());

		if (!m_windowHandle) {
			log("An error occured while creating a window", MessageSeverity::Error);
			return;
		}

		makeActiveTarget();

		m_isValid = true;
		log("Created window", MessageSeverity::Info);
	}

	Window::Window(const Context& context, const char* title, int width, int height) :
		Window(context, title, glm::ivec2(width, height))
	{}

	Window::~Window() {
		glfwDestroyWindow(m_windowHandle);
		log("Closed window", MessageSeverity::Info);
	}

	bool Window::isValid() const {
		return m_isValid;
	}

	void Window::update() {
		glfwSwapBuffers(m_windowHandle);
		glfwPollEvents();
	}

	bool Window::isCloseRequested() const {
		return glfwWindowShouldClose(m_windowHandle);
	}

	void Window::makeActiveTarget() {
		glfwMakeContextCurrent(m_windowHandle);
	}
}