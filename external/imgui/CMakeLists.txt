project(imgui)

add_library(imgui OBJECT "imgui.cpp" "imgui_demo.cpp" "imgui_draw.cpp" "imgui_tables.cpp" "imgui_widgets.cpp" )

target_include_directories(imgui INTERFACE include "${CMAKE_CURRENT_SOURCE_DIR}/")

install(FILES LICENSE.txt DESTINATION include/imgui)
install(FILES imgui.h DESTINATION include)

if(LUNA_ENABLE_IMGUI)
	target_include_directories(imgui PUBLIC include "${CMAKE_CURRENT_SOURCE_DIR}/config-enabled")
	install(FILES config-enabled/imconfig.h DESTINATION include)
else()
	target_include_directories(imgui PUBLIC include "${CMAKE_CURRENT_SOURCE_DIR}/config-disabled")
	install(FILES config-enabled/imconfig.h DESTINATION include)
endif()