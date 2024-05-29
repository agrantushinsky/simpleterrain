cmake_minimum_required(VERSION 3.20.0)
project(
    simpleterrain 
    VERSION 1.0 
    LANGUAGES C)

# Source files
file(GLOB_RECURSE SOURCE_FILES 
    ${CMAKE_SOURCE_DIR}/src/game.c
    ${CMAKE_SOURCE_DIR}/src/input.c
    ${CMAKE_SOURCE_DIR}/src/shader.c
    ${CMAKE_SOURCE_DIR}/src/camera.c
    ${CMAKE_SOURCE_DIR}/src/main.c
)
	
# Header files
file(GLOB_RECURSE HEADER_FILES 
    ${CMAKE_SOURCE_DIR}/src/game.h
    ${CMAKE_SOURCE_DIR}/src/input.h
    ${CMAKE_SOURCE_DIR}/src/shader.h
    ${CMAKE_SOURCE_DIR}/src/camera.h
    ${CMAKE_SOURCE_DIR}/src/shared.h
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 
    ${CMAKE_BINARY_DIR}/bin)

set(PRJ_LIBRARIES
    glad::glad
    glfw
    cglm::cglm)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(cglm CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${PRJ_LIBRARIES} PUBLIC ${OPENGL_LIBRARY})

if(MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES 
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
endif()

