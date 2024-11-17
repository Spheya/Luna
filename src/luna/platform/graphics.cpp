#include "graphics.hpp"

namespace luna {
	namespace graphics {

		void bindShader(const ShaderProgram& shader) {
			LUNA_GLCALL(glUseProgram(shader.internalHandle()));
		}

		void bindInstanceBuffer(const Buffer& buffer) {
			LUNA_GLCALL(glBindBuffer(GL_ARRAY_BUFFER, buffer.internalHandle()));
		}

		void setPropertyBlockData(PropertyBlockType target, const Buffer& buffer) {
			LUNA_GLCALL(glBindBufferBase(GL_UNIFORM_BUFFER, GLuint(target), buffer.internalHandle()));
		}

		void draw(const Mesh& mesh) {
			LUNA_GLCALL(glBindVertexArray(mesh.internalHandle()));
			LUNA_GLCALL(glDrawElements(GL_TRIANGLES, uint32_t(mesh.indexCount()), GL_UNSIGNED_INT, nullptr));
		}

		void drawInstanced(const Mesh& mesh, uint32_t instanceCount, uint32_t firstInstance) {
			// assume instancing buffer is already bound
			LUNA_GLCALL(glBindVertexArray(mesh.internalHandle()));
			LUNA_GLCALL(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 64, (void*)( 0 + 64 * size_t(firstInstance))));
			LUNA_GLCALL(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 64, (void*)(16 + 64 * size_t(firstInstance))));
			LUNA_GLCALL(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 64, (void*)(32 + 64 * size_t(firstInstance))));
			LUNA_GLCALL(glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 64, (void*)(48 + 64 * size_t(firstInstance))));
			LUNA_GLCALL(glDrawElementsInstanced(GL_TRIANGLES, uint32_t(mesh.indexCount()), GL_UNSIGNED_INT, nullptr, instanceCount));
		}

		void clear() {
			LUNA_GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		}

	}
}
