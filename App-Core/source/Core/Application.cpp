#include "Core/Application.h"
#include "Core/Log.h"

#include "UI/UI.h"

#include <glad/glad.h>
#include <time.h>

namespace Core
{
    static bool isInitialized = false;
    Application* App = NULL;

    Application::Application(const ApplicationSpecification& spec) : m_specification(spec)
    {
        if (isInitialized)
        {
            WARN("Can't initialize application more than once...");
            return;
        }

        srand(time(NULL));

        App = this;
        isInitialized = true;
        m_isRunning = true;

        Graphics::WindowSpecification windowSpec;
        windowSpec.width = spec.windowWidth;
        windowSpec.height = spec.windowHeight;
        windowSpec.title = spec.name;
        m_window = Graphics::CreateWindow(windowSpec);

        m_renderer.Initialize();
        m_renderer.SetClearColor(0.11f, 0.115f, 0.12f);

        m_assetManager.Initialize();

        UI::SetupContext();

        INFO("Successfully initialized the application");
    }

    Application::~Application()
    {
        Graphics::DestroyWindow(m_window);
        UI::DestroyContext();
        m_renderer.Shutdown();
        m_assetManager.Shutdown();
    }

    void Application::Run()
    {
        while (m_isRunning)
        {
            Graphics::HandleWindowEvents(m_window);
            this->OnUpdate();

            m_renderer.BeginDrawing();
            m_renderer.Clear(true, false, false);

            this->OnRender();

            UI::BeginFrame();
            this->OnRenderUI();
            UI::EndFrame();

            UI::RenderFrame();

            m_renderer.EndDrawing();
        }
    }

    void Application::Quit()
    {
        INFO("Closing the application...");
        m_isRunning = false;
        this->OnShutdown();
    }
}
