#pragma once

#include "GameObject.h"
#include "Transform.h"

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
            this->GetTransform()->Move(glm::vec3(0, 1, 0) * speed * dt);
        if(key_states[SDL_SCANCODE_S])
            this->GetTransform()->Move(glm::vec3(0, -1, 0) * speed * dt);
    }
};