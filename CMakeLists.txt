cmake_minimum_required ( VERSION 3.3 )
project( wave )

find_package(glfw3 3.1 REQUIRED)

add_compile_options(-std=c++11)

include_directories ( ${CMAKE_SOURCE_DIR}/src/ )

set ( SOURCE_FILES
        src/main.cpp
        src/imgui.cpp
        src/imgui_draw.cpp
        src/imgui_impl_glfw_gl3.cpp
        src/imgui_demo.cpp
        src/glad/glad.c
        )

add_executable ( wave ${SOURCE_FILES} )

if(WIN32)
	link_directories ( ${CMAKE_SOURCE_DIR}/lib/ )
	file( COPY ${CMAKE_SOURCE_DIR}/lib/glfw3.dll DESTINATION ${CMAKE_SOURCE_DIR}/bin/ )
	target_link_libraries( wave glfw3dll )

else(WIN32)
	target_link_libraries( wave ${GLFW3_LIBRARY} dl X11 Xrandr Xi pthread Xinerama Xcursor )
endif(WIN32)	

include_directories( ${CMAKE_SOURCE_DIR}/scr/ )



set_target_properties ( wave PROPERTIES
                                RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/bin/
                                RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/
                                )
