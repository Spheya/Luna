add_executable (sandbox "src/sandbox.cpp")
set_property(TARGET sandbox PROPERTY CXX_STANDARD 20)
target_link_libraries(sandbox PUBLIC luna)