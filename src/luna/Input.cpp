#include "Input.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "Logger.hpp"

namespace luna {
	namespace {
		std::function<void(glm::vec2 delta)> mouseMoveCallback;
		std::function<void(MouseButton button, bool pressed)> mouseButtonCallback;
		std::function<void(Key key, bool pressed)> keyCallback;
		std::function<void(unsigned int codepoint)> charCallback;

		bool mouseButtonsDown[8];
		bool prevMouseButtonsDown[8];

		bool keysDown[349];
		bool prevKeysDown[349];

		glm::vec2 mousePos = glm::vec2(0.0f);
		glm::vec2 mouseDelta = glm::vec2(0.0f);

		void cursorPositionCallbackGlfw(GLFWwindow* window, double xpos, double ypos) {
			int windowX, windowY;
			glfwGetWindowPos(window, &windowX, &windowY);
			glm::vec2 newMousePos(xpos - windowX, ypos - windowY);
			mouseDelta = mousePos - newMousePos;
			mousePos = newMousePos;

			if (mouseMoveCallback)
				mouseMoveCallback(mouseDelta);
		}

		void mouseButtonCallbackGlfw(GLFWwindow* window, int button, int action, int mods) {
			switch (action) {
			case GLFW_PRESS:
				mouseButtonsDown[button] = true;
				if (mouseButtonCallback)
					mouseButtonCallback(MouseButton(button), true);
				break;

			case GLFW_RELEASE:
				mouseButtonsDown[button] = false;
				if (mouseButtonCallback)
					mouseButtonCallback(MouseButton(button), false);
				break;
			}
		}

		void keyCallbackGlfw(GLFWwindow* window, int key, int scancode, int action, int mods) {
			switch (action) {
			case GLFW_PRESS:
				keysDown[key] = true;
				if (keyCallback)
					keyCallback(Key(key), true);
				break;

			case GLFW_RELEASE:
				keysDown[key] = false;
				if (keyCallback)
					keyCallback(Key(key), false);
				break;
			}
		}

		void charCallbackGlfw(GLFWwindow* window, unsigned int codepoint) {
			if (charCallback)
				charCallback(codepoint);
		}
	}

	void Input::setMouseMoveCallback(std::function<void(glm::vec2 delta)> callback) {
		mouseMoveCallback = callback;
	}

	void Input::setMouseButtonCallback(std::function<void(MouseButton button, bool pressed)> callback) {
		mouseButtonCallback = callback;
	}

	void Input::setKeyCallback(std::function<void(Key key, bool pressed)> callback) {
		keyCallback = callback;
	}

	void Input::setCharCallback(std::function<void(unsigned int codepoint)> callback) {
		charCallback = callback;
	}

	bool Input::isKeyDown(Key key) {
		return keysDown[unsigned(key)];
	}

	bool Input::isKeyPressed(Key key) {
		return keysDown[unsigned(key)] && !prevKeysDown[unsigned(key)];
	}

	bool Input::isKeyReleased(Key key) {
		return !keysDown[unsigned(key)] && prevKeysDown[unsigned(key)];
	}

	bool Input::isMouseButtonDown(MouseButton button) {
		return mouseButtonsDown[unsigned(button)];
	}

	bool Input::isMouseButtonPressed(MouseButton button) {
		return mouseButtonsDown[unsigned(button)] && !mouseButtonsDown[unsigned(button)];
	}

	bool Input::isMouseButtonReleased(MouseButton button) {
		return !mouseButtonsDown[unsigned(button)] && mouseButtonsDown[unsigned(button)];
	}

	void Input::addWindow(GLFWwindow* window) {
		glfwSetCursorPosCallback(window, cursorPositionCallbackGlfw);
		glfwSetMouseButtonCallback(window, mouseButtonCallbackGlfw);
		glfwSetKeyCallback(window, keyCallbackGlfw);
		glfwSetCharCallback(window, charCallbackGlfw);

		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	glm::vec2 Input::getMousePos(const Window* window) {
		double x, y;
		glfwGetCursorPos(window->getInternalWindowPointer(), &x, &y);
		return glm::vec2(float(x), float(y));
	}

	glm::vec2 luna::Input::getMouseDelta() {
		return mouseDelta;
	}

	void Input::update() {
		mouseDelta = glm::vec2(0.0f);
		memcpy(prevMouseButtonsDown, mouseButtonsDown, sizeof(prevMouseButtonsDown));
		memcpy(prevKeysDown, keysDown, sizeof(prevKeysDown));
	}

}