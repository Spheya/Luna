#include "ImGuiContext.hpp"
#ifndef IMGUI_DISABLE
#include "imgui/imgui_impl_opengl3.h"
#include "Logger.hpp"
#include "Window.hpp"
#include "Input.hpp"

namespace luna {

	float getDeltaTime();

	namespace {
		struct BackendPlatformUserData {
			const Window* window;
		};

		BackendPlatformUserData* getBackendPlatformUserData() {
			BackendPlatformUserData* data = (BackendPlatformUserData*)ImGui::GetIO().BackendPlatformUserData;
			if (!data) {
				log("Attempting to retreive the ImGui backend platform user data, but it does not exist", MessageSeverity::Error);
				std::terminate();
			}
			return data;
		}
	}

	ImGuiContext::ImGuiContext(const Window* window) :
		m_window(window)
	{
		// Create the ImGui context
		ImGui::Context* prevContext = ImGui::GetCurrentContext();
		m_context = ImGui::CreateContext();
		ImGui::SetCurrentContext(m_context);

		ImGui_ImplOpenGL3_Init();

		ImGuiIO& io = ImGui::GetIO();

		// Setup backend platform
		BackendPlatformUserData* data = new BackendPlatformUserData();
		io.BackendPlatformUserData = data;
		io.BackendPlatformName = "Luna";
		data->window = window;

		// Start first frame
		update();

		// Set the context back to what it originally was, if there was any
		if (prevContext)
			ImGui::SetCurrentContext(prevContext);

		log("ImGui context created", MessageSeverity::Info);
	}

	ImGuiContext::ImGuiContext(ImGuiContext&& other) noexcept :
		m_window(other.m_window),
		m_context(other.m_context)
	{
		other.m_window = nullptr;
		other.m_context = nullptr;
	}

	ImGuiContext& ImGuiContext::operator=(ImGuiContext&& other) noexcept {
		ImGuiContext::~ImGuiContext();

		m_window = other.m_window;
		m_context = other.m_context;

		other.m_window = nullptr;
		other.m_context = nullptr;

		return *this;
	}

	ImGuiContext::~ImGuiContext() {
		if (m_context) {
			ImGui::Context* prevContext = ImGui::GetCurrentContext();
			ImGui::SetCurrentContext(m_context);

			ImGuiIO& io = ImGui::GetIO();
			delete io.BackendPlatformUserData;
			io.BackendPlatformUserData = nullptr;
			io.BackendPlatformName = nullptr;

			ImGui_ImplOpenGL3_Shutdown();
			ImGui::DestroyContext(m_context);

			ImGui::SetCurrentContext(prevContext);

			log("ImGui context destroyed", MessageSeverity::Info);
		}
	}

	void ImGuiContext::makeCurrentContext() {
		ImGui::SetCurrentContext(m_context);
	}

	void ImGuiContext::update() {
		ImGui::Context* prevContext = ImGui::GetCurrentContext();
		ImGui::SetCurrentContext(m_context);

		// Render if a frame is running
		if (ImGui::GetFrameCount() != 0) {
			ImGui::Render();
			render();
		}

		// Update backend platform
		ImGuiIO& io = ImGui::GetIO();
		glm::vec2 mousePos = Input::getMousePos(*m_window);

		io.DisplaySize = ImVec2(float(m_window->getWidth()), float(m_window->getHeight()));
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		io.DeltaTime = getDeltaTime();
		io.MousePos = ImVec2(mousePos.x, mousePos.y);
		io.MouseDown[0] = Input::isMouseButtonDown(luna::MouseButton::Mouse1);
		io.MouseDown[1] = Input::isMouseButtonDown(luna::MouseButton::Mouse2);
		io.MouseDown[2] = Input::isMouseButtonDown(luna::MouseButton::Mouse3);
		io.MouseDown[3] = Input::isMouseButtonDown(luna::MouseButton::Mouse4);
		io.MouseDown[4] = Input::isMouseButtonDown(luna::MouseButton::Mouse5);

		// Start a new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::SetCurrentContext(prevContext);
	}

	void ImGuiContext::render() {
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}

#endif