#pragma once

#include "platform.hpp"
#include "rendertarget.hpp"
#include "../core/math.hpp"

struct GLFWwindow;

namespace luna {

	class Window : public RenderTarget {
	public:
		Window(const char* title = "Luna", glm::ivec2 size = glm::ivec2(960, 640));
		Window(const char* title, int width, int height);
		Window(Window&& other) noexcept;
		Window& operator=(Window&& other) noexcept;
		~Window();

		bool isCloseRequested() const;
		void close();

		glm::uvec2 size() const;
		unsigned width() const;
		unsigned height() const;

		void update();

		virtual void bindTarget() override;
		virtual glm::uvec2 framebufferSize() const override;

	private:
		GLFWwindow* m_window;
	};

}