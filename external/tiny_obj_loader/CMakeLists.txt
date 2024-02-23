project(tiny_obj_loader)

add_library(tiny_obj_loader INTERFACE)

target_include_directories(tiny_obj_loader INTERFACE include "${CMAKE_CURRENT_SOURCE_DIR}/")

install(FILES LICENSE DESTINATION include/tiny_obj_loader)
install(DIRECTORY tiny_obj_loader DESTINATION include/tiny_obj_loader)