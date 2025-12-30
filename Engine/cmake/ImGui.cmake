# ImGui CMake Configuration

FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.5
)
FetchContent_MakeAvailable(ImGui)

# Options for backends
option(IMGUI_USE_SDL3_BINDING "Enable SDL3 binding" ON)
option(IMGUI_USE_OPENGL3_BINDING "Enable OpenGL3 binding" ON)

# Core ImGui sources
set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

# Add backend sources based on options
if(IMGUI_USE_SDL3_BINDING)
    list(APPEND IMGUI_SOURCES ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp)
endif()

if(IMGUI_USE_OPENGL3_BINDING)
    list(APPEND IMGUI_SOURCES ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
endif()

# Create the library
add_library(ImGui STATIC ${IMGUI_SOURCES})

# Include directories
target_include_directories(ImGui PUBLIC 
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
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