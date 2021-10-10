cmake_minimum_required(VERSION 3.10)

project(bisc)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL REQUIRED)
add_subdirectory(lib/glfw)

add_executable(bisc src/main.cpp)

target_link_libraries(bisc ${OPENGL_LIBRARIES} glfw)

target_include_directories( bisc PUBLIC
					${OPENGL_INCLUDE_DIRS} 
					${PROJECT_SOURCE_DIR}/glfw)

