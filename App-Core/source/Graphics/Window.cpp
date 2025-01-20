#include "Graphics/Window.h"

#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/Log.h"
#include "Core/Time.h"

#include "Graphics/Renderer.h"

#include "UI/UI.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>

namespace Graphics
{
    static bool isSDLInitialized = false;

    static void SetupSDL()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            FATAL("Failed to initialize sdl2, cannot run application!");
            Core::App->Quit();
            return;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GL_SetSwapInterval(1);

        isSDLInitialized = true;
    }

    Window CreateWindow(const WindowSpecification& spec)
    {
        Window window;

        if (!isSDLInitialized)
            SetupSDL();

        window.specification = spec;
        window.handle = SDL_CreateWindow(spec.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, spec.width,
                                         spec.height, SDL_WINDOW_OPENGL);

        if (window.handle == NULL)
        {
            FATAL("Cannot start application because the window couldn't be created!");
            Core::App->Quit();
            return window;
        }

        window.context = SDL_GL_CreateContext(window.handle);
        gladLoadGL();

        return window;
    }

    void HandleWindowEvents(Window& window)
    {
        Core::UpdateTime();
        Renderer->CalculateProjection();

        for (int i = 0; i < MOUSE_BUTTON_COUNT; i++)
            Core::Input.mouse.buttonsClicked[i] = false;

        for (int i = 0; i < KEY_COUNT; i++)
            Core::Input.keyboard.keysPressed[i] = false;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            UI::ProcessEvent(&event);

            switch (event.type)
            {
                case SDL_QUIT:
                    Core::App->Quit();
                    break;

                case SDL_KEYDOWN:
                    Core::Input.keyboard.keysPressed[event.key.keysym.scancode] =
                        !Core::Input.keyboard.keysHeld[event.key.keysym.scancode];
                    Core::Input.keyboard.keysHeld[event.key.keysym.scancode] = true;
                    break;

                case SDL_KEYUP:
                    Core::Input.keyboard.keysPressed[event.key.keysym.scancode] = false;
                    Core::Input.keyboard.keysHeld[event.key.keysym.scancode] = false;
                    break;

                case SDL_MOUSEMOTION:
                    Core::Input.mouse.position.x = event.motion.x;
                    Core::Input.mouse.position.y = event.motion.y;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    Core::Input.mouse.buttonsClicked[event.button.button] =
                        !Core::Input.mouse.buttonsHeld[event.button.button];
                    Core::Input.mouse.buttonsHeld[event.button.button] = true;
                    break;

                case SDL_MOUSEBUTTONUP:
                    Core::Input.mouse.buttonsClicked[event.button.button] = false;
                    Core::Input.mouse.buttonsHeld[event.button.button] = false;
                    break;
            }
        }
    }

    void DisplayWindow(Window& window)
    {
        SDL_GL_SwapWindow(window.handle);
    }

    void DestroyWindow(Window& window)
    {
        INFO("Destroying the window...");
        SDL_DestroyWindow(window.handle);
        SDL_GL_DeleteContext(window.context);
    }

}
