cmake_minimum_required(VERSION 3.0)

project(bisc)

add_subdirectory(lib/glfw)

# specify the C++ standard
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(headers/
                    lib/glad/include/
                    lib/glfw/include/
					/)

file(GLOB LIBRARY_SOURCES lib/glad/include/src/glad.c)
file(GLOB PROJECT_HEADERS headers/*)
file(GLOB PROJECT_SOURCES src/*.cpp)
file(GLOB PROJECT_SHADERS Shaders/*)

source_group("lib" 		FILES ${LIBRARY_SOURCES})
source_group("headers" 	FILES ${PROJECT_HEADERS})
source_group("src" 		FILES ${PROJECT_SOURCES})
source_group("Shaders" FILES ${PROJECT_SHADERS})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_SHADERS} ${LIBRARY_SOURCES}) 
target_link_libraries(${PROJECT_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
	
	































if(FALSE) #fake comment
find_package(OpenGL REQUIRED)
add_subdirectory(lib/glfw)

set(SOURCES src/main.cpp src/glad.c)
add_executable(bisc ${SOURCES})

target_link_libraries(bisc ${OPENGL_LIBRARIES} glfw)

target_include_directories(bisc PUBLIC 
						${OPENGL_INCLUDE_DIRS} 
						${CMAKE_CURRENT_SOURCE_DIR}/src/include 
						${CMAKE_CURRENT_SOURCE_DIR}/glfw)


find_package(OpenGL REQUIRED)

add_subdirectory(includes/glfw)
# add_subdirectory(includes/assimp)

set(GLAD ${PROJECT_SOURCE_DIR}/src/include glad.h
         ${PROJECT_SOURCE_DIR}/src/glad.c)

add_executable(bisc 
			src/main.cpp
			src/glad.c		)

target_link_libraries(bisc ${OPENGL_LIBRARIES} glfw)

target_include_directories( bisc PUBLIC
					${OPENGL_INCLUDE_DIRS}
					${PROJECT_SOURCE_DIR}/glfw
					${PROJECT_SOURCE_DIR}/src/include/KHR
					${PROJECT_SOURCE_DIR}/own)
					

endif()