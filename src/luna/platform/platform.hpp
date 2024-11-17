#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "../core/logger.hpp"

// todo: let the shader specify its max instance count (requires preprocessing)
#define MAX_INSTANCE_COUNT (256)

namespace luna {
	namespace opengl {
		const char* getErrorString(GLenum error);
	}
}

#ifdef _DEBUG
#define LUNA_GLCALL(x) \
x;\
do {\
	GLenum error = glGetError();\
	if (error != GL_NO_ERROR) {\
			luna::error("OpenGL Error: {} ({}:{})",\
				luna::opengl::getErrorString(error),\
				std::max(strrchr(__FILE__, '\\'), strrchr(__FILE__, '/')) ? std::max(strrchr(__FILE__, '\\'), strrchr(__FILE__, '/')) + 1 : __FILE__,\
				__LINE__\
			);\
	}\
} while(0)
#else
#define LUNA_GLCALL(x) x
#endif