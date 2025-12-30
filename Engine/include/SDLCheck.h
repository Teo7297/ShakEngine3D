#pragma once

#include <SDL3/SDL.h>

inline void CheckSDLError(const char* stmt, const char* fname, int line)
{
    const char* err = SDL_GetError();
    if(std::strlen(err) > 1)
    {
        SDL_LogError(0, "SDL error %s, at %s:%i - for %s\n", err, fname, line, stmt);
        abort();
    }
}

#ifdef _DEBUG
#define SDL_CHECK(stmt) do { \
            stmt; \
            CheckSDLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
#define SDL_CHECK(stmt) stmt
#endif