add_subdirectory(external/glad)
add_subdirectory(external/stb)

# Github libraries
CPMAddPackage("gh:g-truc/glm#1.0.1")
CPMAddPackage(
	Name glfw
	GITHUB_REPOSITORY glfw/glfw
	GIT_TAG 7b6aead9fb88b3623e3b3725ebb42670cbe4c579
	OPTIONS
		"GLFW_BUILD_DOCS OFF"
		"GLFW_BUILD_TESTS OFF"
		"GLFW_BUILD_EXAMPLES OFF"
)