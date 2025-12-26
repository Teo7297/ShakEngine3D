#include "ShakEngine.h"

#include "SceneManager.h"
#include "scenes/TestScene.h"

using namespace Shak;

int main()
{
    ShakEngine engine;

    engine.Init({}, "test", 800, 600, true, 1);

    auto* sm = engine.GetAppContext().sceneManager;
    sm->RegisterScene("1", []() {return std::make_unique<TestScene>();});
    sm->RegisterScene("2", []() {return std::make_unique<TestScene>();});

    sm->LoadScene("1");

    engine.Start();
}