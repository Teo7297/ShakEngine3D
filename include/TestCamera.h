#pragma once

#include "GameObject.h"
#include "Transform.h"
#include "Scene.h"

using namespace Shak;

class TestCamera : public GameObject
{
    void OnUpdate(float dt) override
    {
        const bool* key_states = SDL_GetKeyboardState(NULL);
        static float speed = 5.f;
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

        if(key_states[SDL_SCANCODE_SPACE])
        {
            if(!cube) return; // THIS WON'T WORK. EITHER USE HANDLES OR BE CAREFUL WHEN DESTROYING OBJECTS!

            m_scene->DestroyGameObject(cube);
            cube = nullptr;
            SDL_Log("[Camera] done");
        }

    }

public:
    GameObject* cube;
};
