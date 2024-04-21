add_executable (example "example.cpp")
set_property(TARGET example PROPERTY CXX_STANDARD 20)
target_link_libraries(example PUBLIC luna)