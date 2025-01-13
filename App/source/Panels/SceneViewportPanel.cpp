#include "SceneViewportPanel.h"

#include <Zen.h>
#include <imgui.h>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Graphics::Shader SceneViewportPanel::postProcessingShader;

void SceneViewportPanel::Display()
{
    Graphics::Framebuffer& framebuffer = Core::App->GetFramebuffer();
    u64 displayTexture = framebuffer.colorAttachment.id;

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
    Graphics::Framebuffer& framebuffer = Core::App->GetFramebuffer();

    ImDrawData* draw_data = ImGui::GetDrawData();
    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

    glm::mat4 projection = glm::ortho(L, R, B, T);

    BindShader(postProcessingShader);
    glActiveTexture(GL_TEXTURE0 + framebuffer.colorAttachment.id);
    postProcessingShader.SetInt("screenTexture", framebuffer.colorAttachment.id);
    postProcessingShader.SetMat4("projectionMatrix", projection);
}
