#pragma once

#include <memory>

#include "Math.hpp"
#include "RenderTarget.hpp"
#include "Context.hpp"

struct GLFWwindow;

namespace luna {

	class Window : public RenderTarget {
	public:
		Window(const Context& context, const char* title, glm::ivec2 size = glm::ivec2(960, 640));
		Window(const Context& context, const char* title, int width, int height);
		~Window();

		bool isValid() const;
		void update();
		bool isCloseRequested() const;
		void close();

		void makeActiveTarget() override;

	private:
		bool m_isValid;

		GLFWwindow* m_windowHandle;
	};

}