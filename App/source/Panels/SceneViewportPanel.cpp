#include "SceneViewportPanel.h"

#include <Zen.h>

#include <imgui.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace Core;
using namespace Graphics;

ImDrawList* SceneViewportPanel::s_drawList = NULL;
Shader* SceneViewportPanel::s_postProcessingShader = NULL;
Framebuffer* SceneViewportPanel::s_framebuffer = NULL;
float SceneViewportPanel::s_gammaCorrection = 2.2f;

void SceneViewportPanel::Display(Framebuffer& framebuffer)
{
    s_framebuffer = &framebuffer;

    u64 displayTexture = framebuffer.attachments[0].id;

    ImGui::Begin("Scene Viewport");
    {
        s_drawList = ImGui::GetWindowDrawList();
        s_drawList->AddCallback(DrawCallback, nullptr);

        ImVec2 aspectSize = this->GetLargestViewportSize();
        ImVec2 windowPosition = this->GetCenteredViewportPosition(aspectSize);

        ImGui::SetCursorPos(windowPosition);
        ImGui::Image((void*)displayTexture, aspectSize, ImVec2(0, 1), ImVec2(1, 0));
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

    if (s_postProcessingShader != NULL && s_framebuffer != NULL)
    {
        BindShader(*s_postProcessingShader);
        BindTexture(s_framebuffer->attachments[0], 0);

        s_postProcessingShader->SetInt("screenTexture", 0);
        s_postProcessingShader->SetMat4("projectionMatrix", projection);
        s_postProcessingShader->SetFloat("gamma", s_gammaCorrection);
        s_postProcessingShader->SetFloat("exposure", Renderer->GetExposure());
    }
}

ImVec2 SceneViewportPanel::GetLargestViewportSize()
{
    const ApplicationSpecification& appInfo = App->GetSpecification();
    ImVec2 windowSize = ImGui::GetContentRegionAvail();

    float aspectRatio = appInfo.windowWidth / (float)appInfo.windowHeight;
    ImVec2 aspect = ImVec2(windowSize.x, windowSize.x / aspectRatio);

    if (aspect.y > windowSize.y)
    {
        aspect.y = windowSize.y;
        aspect.x = aspect.y * aspectRatio;
    }

    return aspect;
}

ImVec2 SceneViewportPanel::GetCenteredViewportPosition(ImVec2 aspectSize)
{
    ImVec2 windowSize = ImGui::GetContentRegionAvail();

    ImVec2 viewportPosition;
    viewportPosition.x = (windowSize.x / 2.f) - (aspectSize.x / 2.f);
    viewportPosition.y = (windowSize.y / 2.f) - (aspectSize.y / 2.f);

    return viewportPosition;
}
