cmake_minimum_required(VERSION 3.0)

project(point_to_pix)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(point_to_pix point_to_pix.cpp)
target_link_libraries(point_to_pix X11 ${OPENGL_LIBRARIES}  ${GLEW_LIBRARIES} glfw3  dl pthread X11 Xrandr Xi Xxf86vm Xcursor Xinerama)
message(OPENGL_INCLUDE_DIR ${GLEW_INCLUDE_DIRS})
 
target_include_directories(point_to_pix PRIVATE ${GLEW_INCLUDE_DIRS})
