
#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <GL/glew.h>
#include <SDL3/SDL_opengl.h>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>


#include "Includes.h"
#include "GameObject.h"
#include "Shader.h"
#include "Scene.h"
#include "components/CameraComponent.h"
#include "TestCube.h"
#include "TestCamera.h"
#include "Renderer.h"

using namespace Shak;

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

static std::shared_ptr<Scene> scene;
static TestCamera* camera;
static TestCube* cube;
static Renderer renderer;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("OpenGL 4.6 Learning", "1.0", "com.example.opengl46-learning");

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

    as->window = SDL_CreateWindow("OpenGL 4.6 Learning", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    auto& style = ImGui::GetStyle();
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    style.ScaleAllSizes(main_scale);

    ImGui_ImplSDL3_InitForOpenGL(as->window, as->gl_context);
    ImGui_ImplOpenGL3_Init("#version 460");

    /* Log OpenGL version */
    const GLubyte* glVersion, * glslVersion;
    GL_CHECK(glVersion = glGetString(GL_VERSION));
    GL_CHECK(glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION));
    SDL_Log("OpenGL Version: %s", glVersion);
    SDL_Log("GLSL Version: %s", glslVersion);


    scene = std::make_shared<Scene>();
    camera = scene->CreateGameObject<TestCamera>("camera");
    camera->AddComponent<CameraComponent>("CamComp");
    camera->GetTransform()->SetPosition(glm::vec3(0, 0, 20));

    cube = scene->CreateGameObject<TestCube>("cube");
    camera->cube = cube;

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    ImGui_ImplSDL3_ProcessEvent(event);
    if(event->type == SDL_EVENT_QUIT || (event->type == SDL_EVENT_KEY_DOWN && event->key.scancode == SDL_SCANCODE_ESCAPE)) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

static Uint64 NOW = SDL_GetPerformanceCounter();
static Uint64 LAST = 0;
static float deltaTime = 0;

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    AppState* as = (AppState*)appstate;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    bool showDemo = true;
    ImGui::ShowDemoWindow(&showDemo);
    ImGui::Render();

    GL_CHECK(glEnable(GL_DEPTH_TEST));
    GL_CHECK(glEnable(GL_CULL_FACE));
    GL_CHECK(glCullFace(GL_BACK));
    GL_CHECK(glClearColor(0.0, 0.3, 0.5, 1.0));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);

    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();

    deltaTime = (float)((NOW - LAST) / (float)SDL_GetPerformanceFrequency());

    //Draw scene here
    scene->Update(deltaTime);

    renderer.Setup(static_cast<CameraComponent*>(camera->GetComponentsByType<CameraComponent>()[0]));

    auto validCube = scene->FindGameObjectByName("cube");
    if(validCube)
        ((TestCube*)validCube)->meshComp->Draw(renderer);
    renderer.Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    if(as->gl_context) {
        SDL_GL_DestroyContext(as->gl_context);
    }
    if(as->window) {
        SDL_DestroyWindow(as->window);
    }

    SDL_free(as);
}