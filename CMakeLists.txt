cmake_minimum_required (VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("luna")
option(LUNA_BUILD_EXAMPLE "Build the Luna example program" ON)
option(LUNA_BUILD_SANDBOX "Build the Luna sandbox program" ON)
option(LUNA_ENABLE_IMGUI "Enable ImGui" ON)

add_subdirectory ("external")

add_library (luna $<TARGET_OBJECTS:glad> $<TARGET_OBJECTS:glfw> $<TARGET_OBJECTS:imgui> "src/luna/Window.cpp" "src/luna/Camera.cpp" "src/luna/Mesh.cpp" "src/luna/Renderer.cpp" "src/luna/Color.cpp" "src/luna/ShaderProgram.cpp" "src/luna/Logger.cpp" "src/luna/Shader.cpp" "src/luna/Context.cpp" "src/luna/RenderTarget.cpp" "src/luna/Material.cpp" "src/luna/Texture.cpp" "src/luna/RenderTexture.cpp" "src/luna/Input.cpp" "src/luna/UniformBlock.cpp" "src/luna/ImGuiContext.cpp" "src/luna/imgui/imgui_impl_opengl3.cpp" "src/luna/ForwardRenderer.cpp" "src/luna/TempResources.cpp")
set_property(TARGET luna PROPERTY CXX_STANDARD 20)

target_include_directories(luna PUBLIC "src")

target_link_libraries(luna PUBLIC glad glfw glm imgui stb tiny_obj_loader)

install(TARGETS luna DESTINATION lib)
install(FILES LICENSE DESTINATION .)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")

if(LUNA_BUILD_EXAMPLE)
	add_subdirectory ("example")
endif()

if(LUNA_BUILD_SANDBOX)
	add_subdirectory ("sandbox")
endif()