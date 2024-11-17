project(stb)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE include)

install(DIRECTORY include DESTINATION .)