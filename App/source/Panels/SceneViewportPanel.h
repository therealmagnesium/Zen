#pragma once
#include <Zen.h>
#include <imgui.h>

using namespace Core;
using namespace Graphics;

class SceneViewportPanel
{
public:
    SceneViewportPanel() = default;

    static inline void SetPostFXShader(Graphics::Shader* shader) { s_postProcessingShader = shader; }

    void Display(Framebuffer& framebuffer);

private:
    static void DrawCallback(const ImDrawList*, const ImDrawCmd*);

private:
    static float s_gammaCorrection;
    static Shader* s_postProcessingShader;
    static Framebuffer* s_framebuffer;
};
