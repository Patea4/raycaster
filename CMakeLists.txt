cmake_minimum_required(VERSION 3.25)
project(Raycaster)

set(CMAKE_CXX_STANDARD 14)

add_executable(Raycaster raycaster.cpp)
target_link_libraries(Raycaster -lGL -lGLU -lglut)

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
#target_link_libraries(Raycaster PRIVATE GLUT::GLUT OpenGL::GL)
