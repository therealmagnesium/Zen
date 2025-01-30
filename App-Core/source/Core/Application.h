#pragma once
#include "Core/Base.h"

#include "Graphics/Framebuffer.h"
#include "Graphics/Renderer.h"
#include "Graphics/Window.h"

#include <string>

namespace Core
{
    struct ApplicationSpecification
    {
        std::string name = "Untitled";
        std::string author = "";
        u32 windowWidth = 1280;
        u32 windowHeight = 720;
    };

    class Application
    {
    public:
        Application() = default;
        Application(const ApplicationSpecification& spec);
        virtual ~Application();

        inline bool IsRunning() const { return m_isRunning; }
        inline bool IsResizing() const { return m_window.isResizing; }
        inline Graphics::Window& GetWindow() { return m_window; }
        inline ApplicationSpecification& GetSpecification() { return m_specification; }

        void Run();
        void Quit();

    private:
        virtual void OnUpdate() {}
        virtual void OnRender() {}
        virtual void OnRenderUI() {}
        virtual void OnShutdown() {}

    private:
        bool m_isRunning = false;
        ApplicationSpecification m_specification;
        Graphics::RenderManager m_renderer;
        Graphics::Window m_window;
    };

    extern Application* App;
}
