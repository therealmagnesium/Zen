#pragma once
#include "Core/Base.h"
#include <SDL2/SDL_events.h>
#include <imgui.h>

namespace UI
{
    void SetupContext();
    void DestroyContext();
    void ProcessEvent(SDL_Event* event);
    void BeginFrame();
    void EndFrame();
    void RenderFrame();
}
