#include <ShakEngine.h>

#include <SceneManager.h>
#include <scenes/TestScene.h>

// #include <assimp/cimport.h>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>

using namespace Shak;

int main()
{
    // auto scene = aiImportFile("../../assets/corvette/scene.gltf", aiProcessPreset_TargetRealtime_MaxQuality);

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

    engine.Init({}, "test", 800, 600, true, 1);

    auto* sm = engine.GetAppContext().sceneManager;
    sm->RegisterScene<TestScene>("1");
    sm->RegisterScene<TestScene>("2");

    sm->LoadScene("1");

    engine.Start();
}