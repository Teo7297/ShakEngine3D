# ShakEngine3D

**ShakEngine3D** is a custom 3D game engine written in C++23 using OpenGL 4.6. It is the spiritual successor to the 2D "ShakEngine", designed to explore 3D rendering, entity-component systems, and modern engine architecture.

## Features

*   **Modern C++**: Built with C++23 standards.
*   **Rendering**:
    *   OpenGL 4.6 Core Profile.
    *   Programmable pipeline with GLSL shaders (compiled to SPIR-V).
    *   Skybox rendering.
    *   Basic Mesh and Material system.
*   **Architecture**:
    *   Entity-Component System (GameObject & Component based).
    *   Scene Graph with hierarchical Transforms (Parent/Child relationships).
*   **Input & Windowing**:
    *   Powered by **SDL3**.
    *   FPS-style Camera controller.
*   **GUI**:
    *   Integrated **ImGui** (Docking branch) for debug tools and editor interface.
*   **Dependencies**:
    *   Managed via **vcpkg**.
    *   GLM (Math), GLEW (OpenGL Loader), stb_image (Textures).

## Prerequisites

To build and run ShakEngine3D, you need:

*   **Visual Studio 2022** (or a C++23 compliant compiler).
*   **CMake** (3.25+ recommended).
*   **vcpkg** package manager.
*   **Vulkan SDK** (Required for `glslc` to compile shaders to SPIR-V).

## Build Instructions

### VS Code (Recommended)

This project is configured for VS Code with CMake Presets and custom Tasks.

1.  **Clone the repository**:
    ```bash
    git clone https://github.com/Teo7297/ShakEngine3D.git
    cd ShakEngine3D
    ```

2.  **Open in VS Code**.

3.  **Configure vcpkg**:
    Ensure the `VCPKG_ROOT` environment variable is set, or VS Code can find your vcpkg installation.

4.  **Build**:
    *   Press `Ctrl+Shift+B` to access build tasks.
    *   Select **Build Debug** or **Build Release**.
    *   *Note: The tasks are configured to automatically load the MSVC environment (`vcvarsall.bat`).*

### Manual CMake Build

```bash
# Configure
cmake --preset vcpkg-msvc-ninjamulticonf

# Build
cmake --build build --config Release
```

## Controls (Demo)

WIP

## Project Structure

*   `src/`: C++ source files (`.cpp`).
*   `include/`: Header files (`.h`).
*   `shaders/`: GLSL shader source files (`.vert`, `.frag`).
*   `assets/`: Game assets (textures, cubemaps).
*   `cmake/`: CMake helper scripts.
*   `vcpkg.json`: Dependency manifest.

## Roadmap

See [TODO.md](TODO.md) for the current development roadmap and planned features.

## Author
Matteo Cavagnino
