#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <GL/glew.h>
#include <SDL3/SDL_opengl.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <glm/glm.hpp>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_GLContext gl_context;
} AppState;

static const struct {
    const char* key;
    const char* value;
} extended_metadata[] = {
    { SDL_PROP_APP_METADATA_URL_STRING, "https://teo7297.github.io/" },
    { SDL_PROP_APP_METADATA_CREATOR_STRING, "Matteo Cavagnino" },
    { SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "Placed in the public domain" },
    { SDL_PROP_APP_METADATA_TYPE_STRING, "Game Engine" }
};

/* OpenGL objects */
static GLuint VAO, VBO, EBO;
static GLuint shaderProgram;

/* Cube vertices with colors (position XYZ, color RGB) */
static float vertices[] = {
    // Front face (red)
    -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
    // Back face (green)
    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
     1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
     // Top face (blue)
     -1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,
     -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
      1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
      1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,
      // Bottom face (yellow)
      -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
       1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
       1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
      -1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
      // Right face (magenta)
       1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
       1.0f,  1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
       1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
       1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
       // Left face (cyan)
       -1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 1.0f,
       -1.0f, -1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
       -1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 1.0f,
       -1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 1.0f
};

/* Indices for drawing the cube */
static unsigned int indices[] = {
    0,  1,  2,  2,  3,  0,   // Front
    4,  5,  6,  6,  7,  4,   // Back
    8,  9, 10, 10, 11,  8,   // Top
   12, 13, 14, 14, 15, 12,   // Bottom
   16, 17, 18, 18, 19, 16,   // Right
   20, 21, 22, 22, 23, 20    // Left
};

/* Helper function to read SPIR-V binary file */
static std::vector<uint32_t> readSpirvFile(const char* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if(!file.is_open()) {
        SDL_Log("Failed to open shader file: %s", filename);
        return {};
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

    file.seekg(0);
    file.read((char*)buffer.data(), fileSize);
    file.close();

    return buffer;
}

/* Helper function to compile shaders */
static GLuint loadSpirvShader(GLenum type, const char* filename) {
    auto spirv = readSpirvFile(filename);
    if(spirv.empty()) {
        return 0;
    }

    GLuint shader = glCreateShader(type);
    glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
    glSpecializeShader(shader, "main", 0, nullptr, nullptr);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        SDL_Log("SPIR-V shader specialization failed (%s): %s", filename, infoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("OpenGL 4.6 Rotating Cube", "1.0", "com.example.opengl46-cube");

    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    for(int i = 0; i < SDL_arraysize(extended_metadata); i++)
        if(!SDL_SetAppMetadataProperty(extended_metadata[i].key, extended_metadata[i].value))
            return SDL_APP_FAILURE;

    AppState* as = (AppState*)SDL_calloc(1, sizeof(AppState));
    if(!as)
        return SDL_APP_FAILURE;
    else
        *appstate = as;

    /* Set OpenGL 4.6 Core Profile attributes */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    as->window = SDL_CreateWindow("OpenGL 4.6 Rotating Cube", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(!as->window)
    {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    as->gl_context = SDL_GL_CreateContext(as->window);
    if(!as->gl_context)
    {
        SDL_Log("Couldn't create OpenGL context: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_GL_SetSwapInterval(1); /* Enable VSync */

    /* Initialize GLEW */
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if(glewError != GLEW_OK) {
        SDL_Log("Failed to initialize GLEW: %s", glewGetErrorString(glewError));
        return SDL_APP_FAILURE;
    }

    /* Log OpenGL version */
    SDL_Log("OpenGL Version: %s", glGetString(GL_VERSION));
    SDL_Log("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    /* Check for SPIR-V support */
    if(!GLEW_ARB_gl_spirv) {
        SDL_Log("GL_ARB_gl_spirv extension not supported. Cannot load SPIR-V shaders.");
        return SDL_APP_FAILURE;
    }

    /* Load and compile SPIR-V shaders */
    // const char* basePath = SDL_GetBasePath();
    // char vertShaderPath[512];
    // char fragShaderPath[512];
    // SDL_snprintf(vertShaderPath, sizeof(vertShaderPath), "%s../shaders/test.vert.spv", basePath);
    // SDL_snprintf(fragShaderPath, sizeof(fragShaderPath), "%s../shaders/test.frag.spv", basePath);
    // SDL_free((void*)basePath);

    // SDL_Log("Loading vertex shader from: %s", vertShaderPath);
    // SDL_Log("Loading fragment shader from: %s", fragShaderPath);

    // GLuint vertexShader = loadSpirvShader(GL_VERTEX_SHADER, vertShaderPath);
    // GLuint fragmentShader = loadSpirvShader(GL_FRAGMENT_SHADER, fragShaderPath);

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if(event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    AppState* as = (AppState*)appstate;

    glClearColor(0.2, 0.5, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap buffers */
    SDL_GL_SwapWindow(as->window);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    AppState* as = (AppState*)appstate;

    if(as->gl_context) {
        SDL_GL_DestroyContext(as->gl_context);
    }
    if(as->window) {
        SDL_DestroyWindow(as->window);
    }
}