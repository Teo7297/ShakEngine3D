#pragma once

namespace Shak
{
    class Renderer;
    class IRenderable
    {
    public:
        virtual void Draw(Renderer* renderer) = 0;
    };
}