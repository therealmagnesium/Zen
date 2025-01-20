#include "UI/UI.h"

#include "Core/Application.h"
#include "Core/Log.h"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

namespace UI
{
    void SetupContext()
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/rubik/Rubik-Regular.ttf", 18.f);

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowMinSize.x = 380.f;

        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.085f, 0.08f, 0.09f, 1.f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.085f, 0.08f, 0.09f, 1.f);

        style.Colors[ImGuiCol_Header] = ImVec4(0.205f, 0.2f, 0.21f, 1.f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.305f, 0.3f, 0.31f, 1.f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.105f, 0.1f, 0.11f, 1.f);

        style.Colors[ImGuiCol_Button] = ImVec4(0.205f, 0.2f, 0.21f, 1.f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.305f, 0.3f, 0.31f, 1.f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.105f, 0.1f, 0.11f, 1.f);

        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.205f, 0.2f, 0.21f, 1.f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.305f, 0.3f, 0.31f, 1.f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.105f, 0.1f, 0.11f, 1.f);

        style.Colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);
        style.Colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.3805f, 0.381f, 1.f);
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.2805f, 0.281f, 1.f);
        style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.1505f, 0.151f, 1.f);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.205f, 0.21f, 1.f);

        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.085f, 0.08f, 0.09f, 1.f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.085f, 0.08f, 0.09f, 1.f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.085f, 0.08f, 0.09f, 1.f);

        Graphics::Window& window = Core::App->GetWindow();
        ImGui_ImplSDL2_InitForOpenGL(window.handle, window.context);
        ImGui_ImplOpenGL3_Init("#version 450");

        INFO("Succesfully initialized the UI context");
    }

    void DestroyContext()
    {
        INFO("Shutting down UI context...");
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    void ProcessEvent(SDL_Event* event)
    {
        ImGui_ImplSDL2_ProcessEvent(event);
    }

    void BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void EndFrame()
    {
        ImGui::Render();
    }

    void RenderFrame()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
