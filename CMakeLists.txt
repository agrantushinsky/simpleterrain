cmake_minimum_required(VERSION 3.20.0)
project(
    simpleterrain 
    VERSION 1.0 
    LANGUAGES C)

# Source files
file(GLOB_RECURSE SOURCE_FILES 
    ${CMAKE_SOURCE_DIR}/src/worldgen.c
    ${CMAKE_SOURCE_DIR}/src/texture.c
    ${CMAKE_SOURCE_DIR}/src/world.c
    ${CMAKE_SOURCE_DIR}/src/chunk.c
    ${CMAKE_SOURCE_DIR}/src/block.c
    ${CMAKE_SOURCE_DIR}/src/game.c
    ${CMAKE_SOURCE_DIR}/src/input.c
    ${CMAKE_SOURCE_DIR}/src/shader.c
    ${CMAKE_SOURCE_DIR}/src/camera.c
    ${CMAKE_SOURCE_DIR}/src/main.c
)
	
# Header files
file(GLOB_RECURSE HEADER_FILES 
    ${CMAKE_SOURCE_DIR}/src/worldgen.h
    ${CMAKE_SOURCE_DIR}/src/texture.h
    ${CMAKE_SOURCE_DIR}/src/world.h
    ${CMAKE_SOURCE_DIR}/src/chunk.h
    ${CMAKE_SOURCE_DIR}/src/block.h
    ${CMAKE_SOURCE_DIR}/src/game.h
    ${CMAKE_SOURCE_DIR}/src/input.h
    ${CMAKE_SOURCE_DIR}/src/shader.h
    ${CMAKE_SOURCE_DIR}/src/camera.h
    ${CMAKE_SOURCE_DIR}/src/shared.h

    ${CMAKE_SOURCE_DIR}/lib/stb/stb_image.h
    ${CMAKE_SOURCE_DIR}/lib/stb/stb_ds.h
    ${CMAKE_SOURCE_DIR}/lib/stb/stb_perlin.h
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 
    ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/lib)

set(PRJ_LIBRARIES
    glad::glad
    glfw
    cglm::cglm)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(cglm CONFIG REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${PRJ_LIBRARIES} PUBLIC ${OPENGL_LIBRARY})

set_target_properties(${PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

