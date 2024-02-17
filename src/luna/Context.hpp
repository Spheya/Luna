#pragma once

#include <vector>
#include "Mesh.hpp"

struct GLFWwindow;

namespace luna {

	enum class Primitive : uint8_t {
		Quad, Cube
	};

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

		void update();
		float getDeltatime() const;
		float getTime() const;

		const Mesh* getPrimitive(Primitive primitive) const;

	private:
		bool m_valid;
		GLFWwindow* m_graphicsContext;
		
		float m_deltatime;
		double m_prevTime;

		Mesh m_primitives[2];
		void loadPrimitives();
	};

}