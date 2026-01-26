#include <ShakEngine.h>

#include <SceneManager.h>
#include <UIManager.h>
#include <Renderer.h>
#include <scenes/TestScene.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace Shak;

int main()
{
    // auto scene = aiImportFile("../../../Engine/assets/corvette/scene.gltf", aiProcessPreset_TargetRealtime_MaxQuality);

    // if(scene)
    // {
    //     SDL_Log("[Assimp] Successfully loaded model!");
    //     SDL_Log("[Assimp] Meshes: %d", scene->mNumMeshes);
    //     SDL_Log("[Assimp] Materials: %d", scene->mNumMaterials);
    //     SDL_Log("[Assimp] Textures: %d", scene->mNumTextures);
    //     SDL_Log("[Assimp] Animations: %d", scene->mNumAnimations);
    //     aiReleaseImport(scene);
    // }
    // else
    // {
    //     SDL_LogError(0, "[Assimp] Failed to load model: %s", aiGetErrorString());
    // }

    ShakEngine engine;

    engine.Init({}, "test", 1920, 1080, true, 1);
    auto& ctx = engine.GetAppContext();
    ImGui::SetCurrentContext(ctx.imguiContext);

    auto* sm = ctx.sceneManager;
    sm->RegisterScene<TestScene>("1");
    sm->RegisterScene<TestScene>("2");
    sm->LoadScene("1");

    bool editorRunning = true;

    // engine.Start();
    while(editorRunning)
    {
        // Update
        float dt = engine.GetDeltaTime();
        ctx.sceneManager->Update(ctx.renderer, dt);
        ctx.uiManager->Update(dt);

        // Input/Events
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);

            switch(event.type) {
            case SDL_EVENT_QUIT:
                engine.Stop();
                editorRunning = false;
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                SDL_Log("Resized window: width = %i, height = Y: %i", event.window.data1, event.window.data2);
                ctx.sceneManager->GetActiveScene()->GetMainCamera()->SetViewportSize(event.window.data1, event.window.data2);
                break;
            }

            ctx.sceneManager->ProcessEvent(event);
            ctx.uiManager->ProcessEvent(event);
        }

        // Render
        ctx.renderer->RenderScene();
        ctx.renderer->RenderUI();

        // Swap buffers
        SDL_CHECK(SDL_GL_SwapWindow(ctx.window));
    }
    // engine.FreeResources();
}