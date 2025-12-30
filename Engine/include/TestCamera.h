#pragma once

#include "GameObject.h"
#include "Transform.h"
#include "Scene.h"
#include "components/CameraComponent.h"

using namespace Shak;

class TestCamera : public GameObject
{
    void OnAwake() override
    {
        camComp = this->AddComponent<CameraComponent>("CamComp");
        m_scene->SetMainCamera(camComp);
    }

    void OnUpdate(float dt) override
    {
        const bool* key_states = SDL_GetKeyboardState(NULL);
        static float speed = 15.f;
        if(key_states[SDL_SCANCODE_D])
            this->GetTransform()->Move(glm::vec3(1, 0, 0) * speed * dt);
        if(key_states[SDL_SCANCODE_A])
            this->GetTransform()->Move(glm::vec3(-1, 0, 0) * speed * dt);
        if(key_states[SDL_SCANCODE_W])
            this->GetTransform()->Move(glm::vec3(0, 0, -1) * speed * dt);
        if(key_states[SDL_SCANCODE_S])
            this->GetTransform()->Move(glm::vec3(0, 0, 1) * speed * dt);
        if(key_states[SDL_SCANCODE_E])
            this->GetTransform()->Move(glm::vec3(0, 1, 0) * speed * dt);
        if(key_states[SDL_SCANCODE_Q])
            this->GetTransform()->Move(glm::vec3(0, -1, 0) * speed * dt);


        if(key_states[SDL_SCANCODE_M])
        {
            glm::vec3 eulerAngles(glm::radians(0.f), glm::radians(-2.f), glm::radians(0.f));
            this->GetTransform()->Rotate(glm::quat(eulerAngles));
        }

        if(key_states[SDL_SCANCODE_N])
        {
            glm::vec3 eulerAngles(glm::radians(0.f), glm::radians(2.f), glm::radians(0.f));
            this->GetTransform()->Rotate(glm::quat(eulerAngles));
        }
    }

public:
    CameraComponent* camComp;
};
