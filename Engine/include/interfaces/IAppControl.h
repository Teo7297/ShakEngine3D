#pragma once

namespace Shak
{
    class IAppControl
    {
    public:
        virtual void Stop() = 0;
        // TODO: other app controls
    };
}