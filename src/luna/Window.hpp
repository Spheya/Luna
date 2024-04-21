#pragma once

#include <memory>

#include "Context.hpp"
#include "RenderTexture.hpp"
#include "ShaderProgram.hpp"
#include "Mesh.hpp"

struct GLFWwindow;

namespace luna {
	class ImGuiContext;

	class Window : public RenderTarget {
		friend void onWindowSizeChange(GLFWwindow*, int, int);
	public:
		Window(const char* title = "Luna", glm::ivec2 size = glm::ivec2(960, 640));
		Window(const char* title, int width, int height);
		Window(Window&) = delete;
		Window& operator=(Window&) = delete;
		Window(Window&& other) noexcept;
		Window& operator=(Window&& other) noexcept;
		~Window();

		bool isValid() const;
		void update();
		bool isCloseRequested() const;
		void close();

		void lockCursor(bool lock = true);
		void enableVSync(bool enable = true);

		void makeActiveTarget() override;
		glm::ivec2 getSize() const override;
		int getWidth() const override;
		int getHeight() const override;

		GLFWwindow* getInternalWindowPointer() const;

#ifndef IMGUI_DISABLE
		void makeActiveImGuiContext() const;
#endif

	private:
		bool m_isValid;

		GLFWwindow* m_windowHandle;

		Mesh m_blitQuad;
		ShaderProgram m_blitShader;
		std::unique_ptr<RenderTexture> m_contents;

#ifndef IMGUI_DISABLE
		std::unique_ptr<ImGuiContext> m_imguiContext;
#endif
	};

}