#pragma once
#include "Core/Base.h"

namespace Core
{
    struct TimeState
    {
        float now = 0.f;
        float delta = 0.f;
        float last = 0.f;
    };

    void UpdateTime();

    extern TimeState Time;
}
