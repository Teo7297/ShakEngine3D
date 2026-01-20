#include "ShakEngine.h"

#include <SDL3/SDL.h>
#include <GL/glew.h>
#include <SDL3/SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include "AssetManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "Renderer.h"

using namespace Shak;

ShakEngine::ShakEngine()
    :
    m_window{ nullptr },
    m_GLContext{ nullptr },
    m_assetManager{ std::make_unique<AssetManager>() },
    m_sceneManager{ std::make_unique<SceneManager>() },
    m_uiManager{ std::make_unique<UIManager>() },
    m_renderer{ std::make_unique<Renderer>() },
    m_shouldStop{ false }
{
    m_appContext =
    {
        .assetManager = m_assetManager.get(),
        .sceneManager = m_sceneManager.get(),
        .uiManager = m_uiManager.get(),
        .renderer = m_renderer.get(),
        .appControl = this
    };
    m_sceneManager->SetAppContext(m_appContext);
}

ShakEngine::~ShakEngine()
{
}

void ShakEngine::Init(const std::vector<AppMetadata>& metadata, const char* windowName, int winWidth, int winHeight, bool resizable, int syncType)
{
    this->InitSDL(metadata, windowName, winWidth, winHeight, resizable, syncType);

    this->InitGlew();

    this->InitImGui();

    this->LogGLVersion();
}

void ShakEngine::Start()
{
    while(!m_shouldStop)
    {
        this->Update();
    }
    this->FreeResources();
}

void ShakEngine::Stop()
{
    m_shouldStop = true;
}

AppContext& ShakEngine::GetAppContext()
{
    return m_appContext;
}

void ShakEngine::Update()
{
    // Update
    float dt = this->GetDeltaTime();
    m_sceneManager->Update(m_renderer.get(), dt);
    m_uiManager->Update(dt);

    // Input/Events
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);

        switch(event.type) {
        case SDL_EVENT_QUIT:
            this->Stop();
            break;
        }

        m_sceneManager->ProcessEvent(event);
        m_uiManager->ProcessEvent(event);
    }

    // Render
    m_renderer->RenderScene();
    m_renderer->RenderUI();

    // Swap buffers
    SDL_CHECK(SDL_GL_SwapWindow(m_window));
}

void ShakEngine::InitSDL(const std::vector<AppMetadata>& metadata, const char* windowName, int winWidth, int winHeight, bool resizable, int syncType)
{
    SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));

    for(int i = 0; i < metadata.size(); i++)
        SDL_CHECK(SDL_SetAppMetadataProperty(metadata[i].key, metadata[i].value));

    /* Set OpenGL 4.6 Core Profile attributes */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_WindowFlags windowFlags = SDL_WINDOW_OPENGL;
    if(resizable)
        windowFlags |= SDL_WINDOW_RESIZABLE;
    SDL_CHECK(m_window = SDL_CreateWindow(windowName, winWidth, winHeight, windowFlags));
    m_appContext.window = m_window;

    SDL_CHECK(m_GLContext = SDL_GL_CreateContext(m_window));
    m_appContext.glContext = &m_GLContext;

    if(syncType < -1 || syncType > 1)
    {
        SDL_Log("Invalid sync type flag %i, setting default value to 1", syncType);
        syncType = 1;
    }
    SDL_GL_SetSwapInterval(syncType);
}

void ShakEngine::InitGlew()
{
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if(glewError != GLEW_OK)
    {
        SDL_Log("Failed to initialize GLEW: %s", glewGetErrorString(glewError));
        return;
    }
}

void ShakEngine::InitImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    m_appContext.imguiContext = ImGui::GetCurrentContext();

    auto& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    auto& style = ImGui::GetStyle();
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    style.ScaleAllSizes(main_scale);

    ImGui_ImplSDL3_InitForOpenGL(m_window, m_GLContext);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void ShakEngine::LogGLVersion()
{
    const GLubyte* glVersion, * glslVersion;
    GL_CHECK(glVersion = glGetString(GL_VERSION));
    GL_CHECK(glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION));
    SDL_Log("OpenGL Version: %s", glVersion);
    SDL_Log("GLSL Version: %s", glslVersion);
}

void ShakEngine::FreeResources()
{
    // Manually clear resource managers before killing SDL resources to avoid dependencies crashes
    m_uiManager.reset();
    m_sceneManager.reset();
    m_assetManager.reset();
    m_renderer.reset();

    // Free ImGui resources
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    // Free SDL resources
    if(m_GLContext)
        SDL_GL_DestroyContext(m_GLContext);
    if(m_window)
        SDL_DestroyWindow(m_window);
}

float ShakEngine::GetDeltaTime()
{
    static Uint64 NOW = SDL_GetPerformanceCounter();
    static Uint64 LAST = 0;

    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();
    return (float)((NOW - LAST) / (float)SDL_GetPerformanceFrequency());
}
