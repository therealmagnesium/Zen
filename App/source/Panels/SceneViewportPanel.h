#pragma once
#include <Zen.h>
#include <imgui.h>

using namespace Core;
using namespace Graphics;

class SceneViewportPanel
{
public:
    SceneViewportPanel() = default;

    static inline ImDrawList* GetDrawList() { return s_drawList; }

    inline void SetContext(Scene* scene) { m_context = scene; }
    static inline void SetPostFXShader(Graphics::Shader* shader) { s_postProcessingShader = shader; }

    void Display(Framebuffer& framebuffer);

private:
    static void DrawCallback(const ImDrawList*, const ImDrawCmd*);

private:
    ImVec2 GetLargestViewportSize();
    ImVec2 GetCenteredViewportPosition(ImVec2 aspectSize);

private:
    Scene* m_context = NULL;

private:
    static ImDrawList* s_drawList;
    static float s_gammaCorrection;
    static Shader* s_postProcessingShader;
    static Framebuffer* s_framebuffer;
};
