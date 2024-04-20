project(stb)

add_library(stb INTERFACE)

target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/")

install(FILES LICENSE DESTINATION include/stb)
install(DIRECTORY stb DESTINATION include)