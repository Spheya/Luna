project(glad)

add_library(glad OBJECT "src/glad.c")
target_include_directories(glad PUBLIC include)

install(DIRECTORY include DESTINATION .)