project(glad)

add_library(glad "src/glad.c")
target_include_directories(glad PUBLIC include)

install(TARGETS glad DESTINATION lib)
install(DIRECTORY include DESTINATION .)