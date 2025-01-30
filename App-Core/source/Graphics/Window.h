#pragma once
#include "Core/Base.h"

#include <SDL2/SDL_video.h>
#include <string>

namespace Graphics
{
    struct WindowSpecification
    {
        u32 width;
        u32 height;
        std::string title;
    };

    struct Window
    {
        WindowSpecification specification;
        bool isResizing = false;
        void* context = NULL;
        SDL_Window* handle = NULL;
    };

    Window CreateWindow(const WindowSpecification& spec);
    void HandleWindowEvents(Window& window);
    void DisplayWindow(Window& window);
    void DestroyWindow(Window& window);
}
