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
		ImGuiContext();
		ImGuiContext(ImGuiContext&) = delete;
		ImGuiContext& operator=(ImGuiContext&) = delete;
		ImGuiContext(ImGuiContext&& other) noexcept;
		ImGuiContext& operator=(ImGuiContext&& other) noexcept;
		~ImGuiContext();

		void makeCurrentContext() const;

		void update(const Window* window);

	private:
		void render();
		
		ImGui::Context* m_context = nullptr;
	};

}

#endif