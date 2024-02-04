cmake_minimum_required (VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("luna")

add_subdirectory ("external")

add_library (luna "src/luna/Window.cpp" "src/luna/Camera.cpp" "src/luna/Mesh.cpp" "src/luna/Renderer.cpp" "src/luna/Color.cpp")
set_property(TARGET luna PROPERTY CXX_STANDARD 20)

target_include_directories(luna PUBLIC "src")
target_include_directories(luna PUBLIC "external")

target_link_libraries(luna PUBLIC glm glfw glad)

install(TARGETS luna DESTINATION lib)
install(FILES LICENSE DESTINATION .)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")

add_subdirectory ("example")
