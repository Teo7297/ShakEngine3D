#pragma once

#include <Scene.h>
#include <UI/UIManager.h>

#include <TestCamera.h>
#include <TestCube.h>
#include <TestSkybox.h>

#include "TestUILayer.h"

class TestScene : public Scene
{
public:
    TestScene() = default;
    ~TestScene() override
    {
        SDL_Log("Destroyed!");
    };

    void Init() override
    {
        auto cubeH = CreateGameObject<TestCube>("cube");
        cubeH.gameObject->GetTransform()->SetPosition(glm::vec3(std::rand() % 5, std::rand() % 5, std::rand() % 5));
        cubeH = CreateGameObject<TestCube>("cube");
        cubeH.gameObject->GetTransform()->SetPosition(glm::vec3(std::rand() % 5, std::rand() % 5, std::rand() % 5));
        cubeH = CreateGameObject<TestCube>("cube");
        cubeH.gameObject->GetTransform()->SetPosition(glm::vec3(std::rand() % 5, std::rand() % 5, std::rand() % 5));
        auto cameraH = CreateGameObject<TestCamera>("camera");
        cameraH.gameObject->GetTransform()->SetPosition(glm::vec3(0, 0, 20));
        CreateGameObject<TestSkybox>("sky");

        auto* layer = m_appContext.uiManager->CreateUILayer<TestUILayer>("TestUILayer");
    }
};
