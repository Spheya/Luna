#pragma once
#include <imgui.h>
#ifndef IMGUI_DISABLE

namespace ImGui {
	using Context = ImGuiContext;
}

namespace luna {

	class Window;

	class ImGuiContext {
	public:
		ImGuiContext(const Window* window);
		ImGuiContext(ImGuiContext&) = delete;
		ImGuiContext& operator=(ImGuiContext&) = delete;
		ImGuiContext(ImGuiContext&& other) noexcept;
		ImGuiContext& operator=(ImGuiContext&& other) noexcept;
		~ImGuiContext();

		void makeCurrentContext();

		void update();

	private:
		void render();

		const Window* m_window = nullptr;
		ImGui::Context* m_context = nullptr;
	};

}

#endif