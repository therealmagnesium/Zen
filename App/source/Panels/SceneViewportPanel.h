#pragma once
#include <Zen.h>
#include <imgui.h>

class SceneViewportPanel
{
public:
    SceneViewportPanel() = default;

    static inline void SetPostFXShader(Graphics::Shader& shader) { postProcessingShader = shader; }

    void Display();

private:
    static void DrawCallback(const ImDrawList*, const ImDrawCmd*);

private:
    static Graphics::Shader postProcessingShader;
};
