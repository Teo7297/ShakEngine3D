#pragma once

#include "Includes.h"

namespace Shak
{
    class UIManager
    {
    public:
        void Update(float deltaTime);
        void ProcessEvent(SDL_Event event);

    private:
    };
}