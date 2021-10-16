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

target_link_libraries(${PROJECT_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
					  
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
	

# install arguments - shaders, images
install(DIRECTORY shaders/ DESTINATION build/shaders/
		PATTERN shader/*
		PERMISSIONS OWNER_READ)
		
install(DIRECTORY res/ DESTINATION build/res/
		PATTERN res/*
		PERMISSIONS OWNER_READ)

install(TARGETS ${PROJECT_NAME} DESTINATION build/bin/)