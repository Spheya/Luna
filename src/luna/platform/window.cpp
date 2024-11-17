#include "window.hpp"

#include "../core/core.hpp"
#include "../core/logger.hpp"
#include "graphics.hpp"

namespace luna {

	static GLFWwindow* createWindow(const char* title, glm::ivec2 size) {
		glfwInit();

		// create a hidden window as an off-screen graphics context
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		GLFWwindow* window = glfwCreateWindow(size.x, size.y, title, nullptr, nullptr);
		
		// load opengl functions
		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			error("An error occured while loading OpenGL");
			throw std::runtime_error("An error occured while loading OpenGL");
		}

		log("OpenGL context initialized");

		return window;
	}

	Window::Window(const char* title, glm::ivec2 size) :
		m_window(createWindow(title, size))
	{
		trace("Window created");
	}

	Window::Window(const char* title, int width, int height) : Window(title, glm::ivec2(width, height)) {}

	Window::Window(Window&& other) noexcept :
		m_window(other.m_window)
	{
		other.m_window = nullptr;
	}

	Window& Window::operator=(Window&& other) noexcept {
		close();

		m_window = other.m_window;
		other.m_window = nullptr;

		return *this;
	}

	Window::~Window() {
		close();
	}

	bool Window::isCloseRequested() const {
		return glfwWindowShouldClose(m_window);
	}

	void Window::close() {
		if (m_window) {
			glfwDestroyWindow(m_window);
			trace("Window closed");
		}
	}

	glm::uvec2 Window::size() const {
		int w, h;
		glfwGetWindowSize(m_window, &w, &h);
		return glm::uvec2(unsigned(w), unsigned(h));
	}

	unsigned Window::width() const {
		return size().x;
	}

	unsigned Window::height() const {
		return size().y;
	}

	void Window::update() {
		glfwSwapBuffers(m_window);
	}

	void Window::bindTarget() {
		LUNA_GLCALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		LUNA_GLCALL(glViewport(0, 0, framebufferWidth(), framebufferHeight()));

	}

	glm::uvec2 Window::framebufferSize() const {
		int w, h;
		glfwGetFramebufferSize(m_window, &w, &h);
		return glm::uvec2(unsigned(w), unsigned(h));
	}

}
