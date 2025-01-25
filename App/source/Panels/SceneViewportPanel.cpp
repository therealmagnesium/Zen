#include "SceneViewportPanel.h"

#include <Zen.h>
#include <imgui.h>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace Core;
using namespace Graphics;

Shader* SceneViewportPanel::s_postProcessingShader = NULL;
Framebuffer* SceneViewportPanel::s_framebuffer = NULL;
float SceneViewportPanel::s_gammaCorrection = 2.2f;

void SceneViewportPanel::Display(Framebuffer& framebuffer)
{
    s_framebuffer = &framebuffer;

    u64 displayTexture = framebuffer.attachments[0].id;

    ImGui::Begin("Scene Viewport");
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddCallback(DrawCallback, nullptr);
        ImGui::Image((void*)displayTexture, ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
}

void SceneViewportPanel::DrawCallback(const ImDrawList*, const ImDrawCmd*)
{
    ImDrawData* draw_data = ImGui::GetDrawData();
    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

    glm::mat4 projection = glm::ortho(L, R, B, T);

    if (s_postProcessingShader != NULL)
    {
        BindShader(*s_postProcessingShader);
        BindTexture(s_framebuffer->attachments[0], 0);

        s_postProcessingShader->SetInt("screenTexture", 0);
        s_postProcessingShader->SetMat4("projectionMatrix", projection);
        s_postProcessingShader->SetFloat("gamma", s_gammaCorrection);
        s_postProcessingShader->SetFloat("exposure", Renderer->GetExposure());
    }
}
