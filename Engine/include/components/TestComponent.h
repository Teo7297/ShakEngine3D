#pragma once

#include "Component.h"

using namespace Shak;

class TestComponent : public Component
{
public:
    int frame = 0;
    void OnUpdate(float dt) override
    {
        if(frame++ % 100 == 0)
        {
            SDL_Log("comp %d", frame-1);
        }
    }

    void OnDestroy()
    {
        SDL_Log("goodbye");
    }
};