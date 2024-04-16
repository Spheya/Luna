set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory("glad")
add_subdirectory("GLFW")
add_subdirectory("glm")
add_subdirectory("stb")
add_subdirectory("tiny_obj_loader")

install(TARGETS glfw DESTINATION lib)
install(FILES GLFW/LICENSE.md DESTINATION include/GLFW)
install(DIRECTORY GLFW/include DESTINATION .)