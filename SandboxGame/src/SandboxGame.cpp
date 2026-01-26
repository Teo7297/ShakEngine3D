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
    ShakEngine engine;

    engine.Init({}, "test", 1920, 1080, true, 1);
    auto& ctx = engine.GetAppContext();
    ImGui::SetCurrentContext(ctx.imguiContext);

    auto* sm = ctx.sceneManager;
    sm->RegisterScene<TestScene>("1");
    sm->RegisterScene<TestScene>("2");
    sm->LoadScene("1");

    engine.Start();
}