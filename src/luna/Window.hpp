#pragma once

#include <memory>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Math.hpp"
#include "RenderTarget.hpp"

namespace luna {

	class GlfwContext {
		friend class Window;

	public:
		~GlfwContext();

	private:
		GlfwContext();

		static std::shared_ptr<GlfwContext> getReference();
		static std::weak_ptr<GlfwContext> s_instance;

		GLFWwindow* m_currentGlContext = nullptr;
	};

	class Window : public RenderTarget {
	public:
		explicit Window(const char* title, glm::ivec2 size = glm::ivec2(960, 640));
		Window(const char* title, int width, int height, bool fullscreen = false);
		~Window();

		bool isValid() const;
		void update();
		void makeCurrentContext();
		bool isCloseRequested() const;
		void close();

		virtual void clear();

	private:
		bool m_isValid;

		GLFWwindow* m_windowHandle;
		std::shared_ptr<GlfwContext> m_contextReference;
	};

}