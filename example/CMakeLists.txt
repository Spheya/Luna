add_executable (luna_example "example.cpp")
set_property(TARGET luna_example PROPERTY CXX_STANDARD 20)
target_link_libraries(luna_example luna)

add_custom_target (
	luna_example_assets ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${PROJECT_SOURCE_DIR}/example/assets
	${PROJECT_BINARY_DIR}/example/assets
)
add_dependencies(luna_example luna_example_assets)