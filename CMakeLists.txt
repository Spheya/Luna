cmake_minimum_required (VERSION 3.14)

if (NOT CPM_SOURCE_CACHE)
    set(CPM_SOURCE_CACHE "${CMAKE_SRC_DIR}/out/build/cpm-cache" CACHE STRING "Set default CPM cache path for external dependencies")
endif()
include(external/CPM.cmake)

project ("luna")

# Create Luna library
set (SOURCES
	"src/luna/core/core.cpp"
	"src/luna/core/logger.cpp"
	"src/luna/core/utils.cpp"

	"src/luna/platform/buffer.cpp"
	"src/luna/platform/graphics.cpp"
	"src/luna/platform/mesh.cpp"
	"src/luna/platform/platform.cpp"
	"src/luna/platform/shaderprogram.cpp"
	"src/luna/platform/window.cpp"

	"src/luna/rendering/camera.cpp"
	"src/luna/rendering/forwardrenderer.cpp"
	"src/luna/rendering/material.cpp"
	"src/luna/rendering/renderer.cpp"
	"src/luna/rendering/rendertype.cpp"
	"src/luna/rendering/shader.cpp"
	"src/luna/rendering/shaderpreprocessor.cpp"
)
set (HEADERS
	"src/luna/core/core.hpp"
	"src/luna/core/logger.hpp"
	"src/luna/core/math.hpp"
	"src/luna/core/utils.hpp"

	"src/luna/platform/buffer.hpp"
	"src/luna/platform/graphics.hpp"
	"src/luna/platform/mesh.hpp"
	"src/luna/platform/platform.hpp"
	"src/luna/platform/shaderprogram.hpp"
	"src/luna/platform/window.hpp"

	"src/luna/rendering/camera.hpp"
	"src/luna/rendering/drawable.hpp"
	"src/luna/rendering/material.hpp"
	"src/luna/rendering/propertyblock.hpp"
	"src/luna/rendering/renderer.hpp"
	"src/luna/rendering/rendertype.hpp"
	"src/luna/rendering/shader.hpp"
	"src/luna/rendering/shaderpreprocessor.hpp"
	"src/luna/rendering/vertex.hpp"
)

add_library (luna ${SOURCES} ${HEADERS} "external/stb/include/stb_image.h" "src/luna/platform/texture.cpp" "src/luna/platform/rendertexture.cpp" )
set_property(TARGET luna PROPERTY CXX_STANDARD 20)
target_include_directories(luna PUBLIC "src")

# Dependencies
include(external/external.cmake)
target_link_libraries(luna PUBLIC
	glfw
	glm
	stb
)

# Backend specific dependencies
target_link_libraries(luna PUBLIC glad)
add_compile_definitions(LUNA_OPENGL)

# Only build example project if luna is the main project
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	add_subdirectory("example")
endif()