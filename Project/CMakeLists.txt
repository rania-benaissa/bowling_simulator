cmake_minimum_required(VERSION 3.22)

project(bowlingSim
        VERSION 1.0
        DESCRIPTION "bowling simulation"
        LANGUAGES CXX)

# create executables
add_executable(${PROJECT_NAME} main.cpp src/objParser.cpp src/shader.cpp src/solid.cpp)

# include headers 

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)


# look for required libraries

# OpenGL
find_package(OpenGL REQUIRED)

# GLuT
find_package(GLUT REQUIRED)

# GLEW
find_package(GLEW REQUIRED)


include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} )
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES})
