#include "Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Logger.hpp"
#include "Context.hpp"
#include "Input.hpp"

namespace luna {

	void onWindowSizeChange(GLFWwindow* window, int width, int height) {
		Window* lunaWindow = (Window*)glfwGetWindowUserPointer(window);
		lunaWindow->m_contents->setSize(width, height);
	}

	Window::Window(const char* title, glm::ivec2 size) :
		m_isValid(false)
	{
		m_contents = std::make_unique<RenderTexture>(size.x, size.y);
		m_windowHandle = glfwCreateWindow(size.x, size.y, title, nullptr, (GLFWwindow*)luna::getGraphicsContext());
		Input::addWindow(m_windowHandle);
		glfwSetWindowUserPointer(m_windowHandle, this);
		glfwSetWindowSizeCallback(m_windowHandle, onWindowSizeChange);

		if (!m_windowHandle) {
			log("An error occured while creating a window", MessageSeverity::Error);
			return;
		}

		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(m_windowHandle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		glfwMakeContextCurrent(m_windowHandle);

		Vertex vertices[] = {
			Vertex(glm::vec3(-1.0f, +1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
			Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
			Vertex(glm::vec3(+1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
			Vertex(glm::vec3(+1.0f, +1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
		};

		unsigned int indices[] = { 0,1,2, 2,3,0 };

		m_blitQuad.setVertices(vertices, 4);
		m_blitQuad.setIndices(indices, 6);

		m_blitShader.load(
			"#version 430 core\nin vec3 Position;in vec2 UV;out vec2 i;void main(){gl_Position=vec4(Position,1);i=UV;}",
			"#version 430 core\nin vec2 i;uniform sampler2D t;out vec4 v;void main(){v=texture(t,i);}"
		);
		m_blitShader.uniform(m_blitShader.uniformId("t"), 0);

		m_contents->bind(0);
		m_blitShader.bind();
		m_blitQuad.bind();

		glfwMakeContextCurrent((GLFWwindow*)luna::getGraphicsContext());

		m_isValid = true;
		log("Window created", MessageSeverity::Info);
	}

	Window::Window(const char* title, int width, int height) :
		Window(title, glm::ivec2(width, height))
	{}

	Window::Window(Window&& other) noexcept :
		m_isValid(other.m_isValid),
		m_windowHandle(other.m_windowHandle),
		m_blitQuad(std::move(other.m_blitQuad))
	{
		glfwSetWindowUserPointer(m_windowHandle, this);
		other.m_windowHandle = nullptr;
	}

	Window& Window::operator=(Window&& other) noexcept {
		if (m_windowHandle) {
			Window::~Window();
		}

		m_isValid = other.m_isValid;
		m_windowHandle = other.m_windowHandle;
		m_blitQuad = std::move(other.m_blitQuad);
		glfwSetWindowUserPointer(m_windowHandle, this);

		other.m_windowHandle = nullptr;

		return *this;
	}

	Window::~Window() {
		if (m_windowHandle) {
			glfwDestroyWindow(m_windowHandle);
			log("Window closed", MessageSeverity::Info);
		}
	}

	bool Window::isValid() const {
		return m_isValid;
	}

	void Window::update() {
		glfwMakeContextCurrent(m_windowHandle);

		glViewport(0, 0, getWidth(), getHeight());
		glDrawElements(GL_TRIANGLES, GLsizei(m_blitQuad.vertexCount()), GL_UNSIGNED_INT, nullptr);
		glfwSwapBuffers(m_windowHandle);

		glfwMakeContextCurrent((GLFWwindow*)luna::getGraphicsContext());
	}

	bool Window::isCloseRequested() const {
		return glfwWindowShouldClose(m_windowHandle);
	}

	void Window::close() {
		glfwDestroyWindow(m_windowHandle);
		log("Window closed", MessageSeverity::Info);
	}

	void Window::lockCursor(bool lock) {
		glfwSetInputMode(m_windowHandle, GLFW_CURSOR, lock ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}

	void Window::makeActiveTarget() {
		m_contents->makeActiveTarget();
	}

	glm::ivec2 Window::getSize() const {
		return m_contents->getSize();
	}

	int Window::getWidth() const {
		return m_contents->getWidth();
	}

	int Window::getHeight() const {
		return m_contents->getHeight();
	}

	GLFWwindow* Window::getInternalWindowPointer() const {
		return m_windowHandle;
	}
}