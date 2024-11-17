#pragma once

#include "mesh.hpp"
#include "../core/math.hpp"
#include "../rendering/propertyblock.hpp"
#include "../rendering/material.hpp"

namespace luna {

	// todo: commandbuffer
	namespace graphics {
		void bindShader(const ShaderProgram& shader);
		void bindInstanceBuffer(const Buffer& buffer);

		void setPropertyBlockData(PropertyBlockType target, const Buffer& buffer);

		void draw(const Mesh& mesh);
		void drawInstanced(const Mesh& mesh, uint32_t instanceCount, uint32_t firstInstance);

		void clear();
	}

}