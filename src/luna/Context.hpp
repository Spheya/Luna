#pragma once

#include <vector>

struct GLFWwindow;

namespace luna {

	class Context {
	public:
		Context();
		Context(Context&) = delete;
		Context& operator=(Context&) = delete;
		Context(Context&&) = delete;
		Context& operator=(Context&&) = delete;
		~Context();

		bool isValid() const;
		bool isCloseRequested() const;
		GLFWwindow* getGraphicsContext() const;

	private:
		bool m_valid;
		GLFWwindow* m_graphicsContext;
	};

}