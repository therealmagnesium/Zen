#include "Core/Time.h"
#include <SDL2/SDL_timer.h>

namespace Core
{
    TimeState Time;

    void UpdateTime()
    {
        Time.now = SDL_GetTicks() / 1000.f;
        Time.delta = Time.now - Time.last;
        Time.last = Time.now;
    }
}
