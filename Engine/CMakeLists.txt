cmake_minimum_required(VERSION 4.2.0)

project(ShakEngine3D VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)
# find_package(assimp CONFIG REQUIRED)

include(FetchContent)

# Include and configure ImGui
include(cmake/ImGui.cmake)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} 
PUBLIC
    include
PRIVATE
    ${Stb_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    SDL3::SDL3
    glm::glm-header-only
    $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
    SDL3_ttf::SDL3_ttf
    OpenGL::GL
    GLEW::GLEW
    ImGui
    # assimp::assimp
)

# Simple script for shaders compilation to spirv, will populate "SHADERS_TARGET" target to use as dependency
include(cmake/CompileShadersSpirv.cmake)
add_dependencies(${PROJECT_NAME} ${SHADERS_TARGET})
