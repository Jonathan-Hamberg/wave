cmake_minimum_required ( VERSION 3.3 )
project( wave )

set (CMAKE_CXX_STANDARD 11)

include_directories ( ${CMAKE_SOURCE_DIR}/src/ )

set ( SOURCE_FILES
        src/main.cpp
        src/imgui.cpp
        src/imgui_draw.cpp
        src/imgui_impl_glfw_gl3.cpp
        src/imgui_demo.cpp
        src/glad/glad.c
        src/gui.cpp
        src/kiss_fft.c
	src/wav.cpp
        )

add_executable ( wave ${SOURCE_FILES} )

if(WIN32)
	file( COPY ${CMAKE_SOURCE_DIR}/lib/glfw3.dll DESTINATION ${CMAKE_SOURCE_DIR}/bin/ )
	target_link_libraries( wave ${CMAKE_SOURCE_DIR}/lib/glfw3dll.lib )
        target_link_libraries( wave ${CMAKE_SOURCE_DIR}/lib/nfd.lib )

else(WIN32)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
	find_package(glfw3 3.1 REQUIRED)
	target_link_libraries( wave ${GLFW3_LIBRARY} dl X11 Xrandr Xi pthread Xinerama Xcursor gtk-3)
	target_link_libraries( wave ${CMAKE_SOURCE_DIR}/lib/libnfd.a )
	target_link_libraries( wave ${GTK3_LIBRARIES})
endif(WIN32)	

include_directories( ${CMAKE_SOURCE_DIR}/scr/ )
