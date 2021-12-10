cmake_minimum_required(VERSION 3.0)

project(bisc)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(lib/glfw)

# option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(lib/assimp)

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
					lib/assimp/include/assimp/
					lib/glm/
					/)

file(GLOB LIBRARY_SOURCES lib/glad/src/glad.c)
file(GLOB PROJECT_HEADERS headers/*.h)
file(GLOB PROJECT_SOURCES src/*.cpp)
file(GLOB PROJECT_SHADERS shaders/*)

source_group("lib" 		FILES ${LIBRARY_SOURCES})
source_group("headers" 	FILES ${PROJECT_HEADERS})
source_group("src" 		FILES ${PROJECT_SOURCES})
source_group("shaders" 	FILES ${PROJECT_SHADERS})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${PROJECT_SHADERS} ${LIBRARY_SOURCES}) 

target_link_libraries(${PROJECT_NAME} glfw assimp
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
					  
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
	

# install arguments - shaders, images
#-install(DIRECTORY shaders/ DESTINATION build/shaders/
#-		PATTERN shader/*
#-		PERMISSIONS OWNER_READ)

file(INSTALL res/models/sphere_63488.obj DESTINATION res/models/)

file(INSTALL res/textures/sky_sphere.png DESTINATION res/textures/)

install(TARGETS ${PROJECT_NAME} DESTINATION build/bin/)
