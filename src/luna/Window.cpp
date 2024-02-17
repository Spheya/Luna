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
			"#version 330 core\nin vec3 Position;in vec2 UV;out vec2 i;void main(){gl_Position=vec4(Position,1);i=UV;}",
			"#version 330 core\nin vec2 i;uniform sampler2D t;out vec4 v;void main(){v=texture(t,i);}"
		);
		m_blitShader.uniform(m_blitShader.uniformId("t"), 0);

		glfwMakeContextCurrent(context.getGraphicsContext());
		
		m_contents = std::make_unique<RenderTexture>(size.x, size.y);

		m_isValid = true;
		log("Created window", MessageSeverity::Info);
	}

	Window::Window(const Context& context, const char* title, int width, int height) :
		Window(context, title, glm::ivec2(width, height))
	{}

	Window::Window(Window&& other) noexcept :
		m_isValid(other.m_isValid),
		m_windowHandle(other.m_windowHandle),
		m_blitQuad(std::move(other.m_blitQuad))
	{
		other.m_windowHandle = nullptr;
	}

	Window& Window::operator=(Window&& other) noexcept {
		if (m_windowHandle) {
			Window::~Window();
		}

		m_isValid = other.m_isValid;
		m_windowHandle = other.m_windowHandle;
		m_blitQuad = std::move(other.m_blitQuad);

		other.m_windowHandle = nullptr;

		return *this;
	}

	Window::~Window() {
		if (m_windowHandle) {
			glfwDestroyWindow(m_windowHandle);
			log("Closed window", MessageSeverity::Info);
		}
	}

	bool Window::isValid() const {
		return m_isValid;
	}

	void Window::update() {
		auto* currentContext = glfwGetCurrentContext();
		glfwMakeContextCurrent(m_windowHandle);
		m_contents->bind(0);
		m_blitShader.bind();
		m_blitQuad.bind();
		glDrawElements(GL_TRIANGLES, GLsizei(m_blitQuad.vertexCount()), GL_UNSIGNED_INT, nullptr);
		glfwMakeContextCurrent(currentContext);

		glfwSwapBuffers(m_windowHandle);
		glfwPollEvents();
	}

	bool Window::isCloseRequested() const {
		return glfwWindowShouldClose(m_windowHandle);
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
}