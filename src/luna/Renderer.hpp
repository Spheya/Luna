#pragma once

#include <vector>

#include "Camera.hpp"
#include "Mesh.hpp"

namespace luna {

	class Renderer {
	public:
		struct DrawCommand {
			const Mesh* mesh;
		};

		virtual ~Renderer() = default;

		void push(const Mesh* mesh);

		virtual void beginFrame();
		virtual void endFrame();
		virtual void render(const Camera& camera) const;

	protected:
		void draw(const Mesh* mesh) const;

	private:
		std::vector<DrawCommand> m_commandBuffer;

	};

}