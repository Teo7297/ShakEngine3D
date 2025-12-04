#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <GL/glew.h>
#include <SDL3/SDL_opengl.h>
#include <glm/glm.hpp>
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>

void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
        abort();
    }
}

#ifdef _DEBUG
#define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif

typedef struct {
    SDL_Window* window;
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

static float vertices[] =
{
    -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
};

static unsigned int indices[] =
{
    0,1,2,0,3,1
};

static const char* frag = R"(
#version 460 core

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(fragColor, 1.0);
}
)";

static const char* vert = R"(
#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

layout(location = 0) out vec3 fragColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    fragColor = aColor;
}
)";

void createShaderProgram()
{
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vert, NULL); // vertex_shader_source is a GLchar* containing glsl shader source code
    glCompileShader(vshader);

    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &frag, NULL); // fragment_shader_source is a GLchar* containing glsl shader source code
    glCompileShader(fshader);

    shaderProgram = glCreateProgram();

    glBindAttribLocation(shaderProgram, 0, "aPos");

    glAttachShader(shaderProgram, vshader);
    glAttachShader(shaderProgram, fshader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vshader);
    glDeleteShader(fshader);
}

std::vector<char> readBinary(const char* filename)
{
    std::ifstream is(filename, std::ifstream::binary);
    std::vector<char> buffer;
    if(is)
    {
        is.seekg(0, is.end);
        int length = is.tellg();
        is.seekg(0, is.beg);

        buffer.reserve(length);
        buffer.resize(length);

        is.read(buffer.data(), length);
        is.close();
    }

    return buffer;
}

void createShaderFromSpirv()
{
    auto vert = readBinary("../shaders/test.vert.spv");
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderBinary(1, &vshader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, vert.data(), vert.size());
    glSpecializeShader(vshader, "main", 0, NULL, NULL);
    int vcompiled = 0;
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &vcompiled);

    auto frag = readBinary("../shaders/test.frag.spv");
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderBinary(1, &fshader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, frag.data(), frag.size());
    glSpecializeShader(fshader, "main", 0, NULL, NULL);
    int fcompiled = 0;
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &fcompiled);

    shaderProgram = glCreateProgram();

    glBindAttribLocation(shaderProgram, 0, "aPos");

    if(vcompiled)
        glAttachShader(shaderProgram, vshader);
    else
        std::cout << "v error\n";

    if(fcompiled)
        glAttachShader(shaderProgram, fshader);
    else
        std::cout << "f error\n";

    glLinkProgram(shaderProgram);

    glDeleteShader(vshader);
    glDeleteShader(fshader);
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
    const GLubyte* glVersion, * glslVersion;
    GL_CHECK(glVersion = glGetString(GL_VERSION));
    GL_CHECK(glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION));
    SDL_Log("OpenGL Version: %s", glVersion);
    SDL_Log("GLSL Version: %s", glslVersion);

    /* Check for SPIR-V support */
    if(!GLEW_ARB_gl_spirv) {
        SDL_Log("GL_ARB_gl_spirv extension not supported. Cannot load SPIR-V shaders.");
        return SDL_APP_FAILURE;
    }

    // createShaderProgram();
    createShaderFromSpirv();

    // Create needed buffers for vertices rendering
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // VAO is our magic buffer. It will store the state of the next buffers!
    glBindVertexArray(VAO);

    // VBO, pass in the vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Dynamic for animated verts

    // Position attribute
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // EBO, pass in the indices data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

    GL_CHECK(glClearColor(0.0, 0.3, 0.5, 1.0));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

    // Use shader program
    GL_CHECK(glUseProgram(shaderProgram));

    // Bind VAO and draw using indices
    GL_CHECK(glBindVertexArray(VAO));
    GL_CHECK(glDrawElements(GL_TRIANGLES, SDL_arraysize(indices), GL_UNSIGNED_INT, 0)); // last parameter is an offset when EBO is bound through VAO
    GL_CHECK(glBindVertexArray(0));

    /* Swap buffers */
    if(!SDL_GL_SwapWindow(as->window))
    {
        SDL_Log("%s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    AppState* as = (AppState*)appstate;

    // Clean up OpenGL resources before destroying context
    if(VAO) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if(VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if(EBO) {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
    if(shaderProgram) {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }

    if(as->gl_context) {
        SDL_GL_DestroyContext(as->gl_context);
    }
    if(as->window) {
        SDL_DestroyWindow(as->window);
    }

    SDL_free(as);
}