#include "platform.hpp"

namespace luna {
	namespace opengl {

		const char* luna::opengl::getErrorString(GLenum error) {
			switch (error) {
			case GL_INVALID_ENUM:                  return "GL_INVALID_ENUM";
			case GL_INVALID_VALUE:                 return "GL_INVALID_VALUE";
			case GL_INVALID_OPERATION:             return "GL_INVALID_OPERATION";
			case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
			case GL_OUT_OF_MEMORY:                 return "GL_OUT_OF_MEMORY";
			case GL_STACK_UNDERFLOW:               return "GL_STACK_UNDERFLOW";
			case GL_STACK_OVERFLOW:                return "GL_STACK_OVERFLOW";
			}
			return "INVALID_ERROR_VALUE";
		}

	}
}