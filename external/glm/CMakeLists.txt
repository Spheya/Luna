project(glm)

add_library(glm INTERFACE)

target_include_directories(glm INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/")

install(FILES copying.txt DESTINATION include/glm)
install(DIRECTORY glm DESTINATION include/)