#pragma once

namespace Shak
{
    class IAppControl
    {
    public:
        virtual void Stop() = 0;
        virtual void SetResolution(int w, int h) = 0;
        // TODO: other app controls
    };
}