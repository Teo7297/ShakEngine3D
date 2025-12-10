cmake_minimum_required(VERSION 4.2.0)

project(EngineLearning VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)


# Include ImGui configuration
include(cmake/ImGuiCMakeLists.cmake)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ${Stb_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
    SDL3_ttf::SDL3_ttf
    OpenGL::GL
    GLEW::GLEW
    glm::glm-header-only
    ImGui
)

# Simple script for shaders compilation to spirv, will populate "SHADERS_TARGET" target to use as dependency
include(${CMAKE_SOURCE_DIR}/cmake/CompileShadersSpirv.cmake)
add_dependencies(${PROJECT_NAME} ${SHADERS_TARGET})