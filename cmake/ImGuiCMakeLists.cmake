# ImGui CMake Configuration
# Assumes imgui submodule is at ${CMAKE_SOURCE_DIR}/imgui

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)

# Options for backends
option(IMGUI_USE_SDL3_BINDING "Enable SDL3 binding" ON)
option(IMGUI_USE_OPENGL3_BINDING "Enable OpenGL3 binding" ON)

# Core ImGui sources
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

# Add backend sources based on options
if(IMGUI_USE_SDL3_BINDING)
    list(APPEND IMGUI_SOURCES ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp)
endif()

if(IMGUI_USE_OPENGL3_BINDING)
    list(APPEND IMGUI_SOURCES ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)
endif()

# Create the library
add_library(ImGui STATIC ${IMGUI_SOURCES})

# Include directories
target_include_directories(ImGui PUBLIC 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Link dependencies for backends
if(IMGUI_USE_SDL3_BINDING)
    target_link_libraries(ImGui PUBLIC SDL3::SDL3)
endif()

if(IMGUI_USE_OPENGL3_BINDING)
    # If using GLEW, we need to tell ImGui to use it
    target_compile_definitions(ImGui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
    target_link_libraries(ImGui PUBLIC GLEW::GLEW OpenGL::GL)
endif()