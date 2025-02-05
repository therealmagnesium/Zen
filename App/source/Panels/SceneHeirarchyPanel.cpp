#include "SceneHeirarchyPanel.h"

#include <Zen.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

using namespace Core;
using namespace Graphics;

static const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
                                                ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
template <typename T, typename UIFunc>
static void DrawComponent(const char* name, std::shared_ptr<Entity>& entity, UIFunc uiFunction)
{
    if (entity->HasComponent<T>())
    {
        auto& component = entity->GetComponent<T>();
        ImVec2 availableRegion = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.f, 4.f));
        float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.f;
        ImGui::PopStyleVar();

        bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", name);
        ImGui::SameLine(availableRegion.x - lineHeight * .5f);

        if (ImGui::Button("+", ImVec2(20.f, 20.f)))
            ImGui::OpenPopup("Component Settings");

        bool removeComponent = false;
        if (ImGui::BeginPopup("Component Settings"))
        {
            if (ImGui::MenuItem("Remove component"))
                removeComponent = true;

            ImGui::EndPopup();
        }

        if (open)
        {
            uiFunction(component);
            ImGui::TreePop();
        }

        if (removeComponent)
            entity->RemoveComponent<T>();
    }
}

void SceneHeirarchyPanel::SetContext(Scene* scene)
{
    m_context = scene;
}

void SceneHeirarchyPanel::Display()
{
    assert(m_context != NULL);

    ImGui::Begin("Scene Heirarchy Panel");
    {
        for (auto& entity : m_context->GetEntities())
            this->DrawEntityNode(entity);

        if (ImGui::IsWindowHovered() && IsMouseClicked(MOUSE_BUTTON_LEFT))
            m_selectionContext = NULL;

        if (m_selectionContext == NULL)
        {
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Create Empty Entity"))
                    m_context->AddEntity("Entity");

                ImGui::EndPopup();
            }
        }
    }
    ImGui::End();

    ImGui::Begin("Properties Panel");
    {
        if (m_selectionContext != NULL)
        {
            this->DrawComponents(m_selectionContext);
        }
    }
    ImGui::End();

    ImGui::ShowDemoWindow();
}

void SceneHeirarchyPanel::DrawEntityNode(std::shared_ptr<Entity>& entity)
{
    ImGuiTreeNodeFlags flags = ((entity == m_selectionContext) ? ImGuiTreeNodeFlags_Selected : 0) |
                               ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
    bool isOpen = ImGui::TreeNodeEx((void*)entity->GetID(), flags, "%s", entity->GetTag());

    if (ImGui::IsItemClicked())
        m_selectionContext = entity;

    bool entityDeleted = false;
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Delete Entity"))
            entityDeleted = true;

        ImGui::EndPopup();
    }

    if (isOpen)
        ImGui::TreePop();

    if (entityDeleted)
    {
        m_context->DestroyEntity(m_selectionContext);
        m_selectionContext = NULL;
    }
}

void SceneHeirarchyPanel::DrawComponents(std::shared_ptr<Entity>& entity)
{
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, entity->GetTag(), sizeof(buffer));

    if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
        entity->SetTag(buffer);

    ImGui::SameLine();
    ImGui::PushItemWidth(-1.f);

    if (ImGui::Button("Add Component"))
        ImGui::OpenPopup("Add Component");

    if (ImGui::BeginPopup("Add Component"))
    {
        if (ImGui::MenuItem("Transform"))
        {
            m_selectionContext->AddComponent<TransformComponent>();
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::MenuItem("Mesh"))
        {
            m_selectionContext->AddComponent<MeshComponent>();
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::MenuItem("Light Source (Directional)"))
        {
            m_selectionContext->AddComponent<DirectionalLightComponent>();
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::MenuItem("Perspective Camera"))
        {
            m_selectionContext->AddComponent<CameraComponent>();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    ImGui::PopItemWidth();

    DrawComponent<TransformComponent>("Transform", entity, [&](TransformComponent& component) {
        ImGui::DragFloat3("Position", glm::value_ptr(component.position), 0.1f);
        ImGui::DragFloat3("Rotation", glm::value_ptr(component.rotation), 0.1f);
        ImGui::DragFloat3("Scale", glm::value_ptr(component.scale), 0.1f);
    });

    DrawComponent<MeshComponent>("Mesh", entity, [&](MeshComponent& component) {
        std::vector<std::string> meshNameList = AssetManager->GetAllMeshNames();

        if (ImGui::Button("Select Mesh"))
            ImGui::OpenPopup("Mesh Select Popup");

        if (ImGui::BeginPopup("Mesh Select Popup"))
        {
            ImGui::SeparatorText("Mesh list");

            for (u32 i = 0; i < meshNameList.size(); i++)
            {
                if (ImGui::Selectable(meshNameList[i].c_str()))
                    component.mesh = *AssetManager->GetMesh(meshNameList[i].c_str());
            }

            ImGui::EndPopup();
        }

        auto meshName = std::find(meshNameList.begin(), meshNameList.end(), component.mesh.name);
        ImGui::SameLine();

        ImGui::Text("%s", (meshName != meshNameList.end()) ? meshName->c_str() : "None");

        u64 albedoMapID = component.mesh.material.diffuseMap.id;
        ImGui::SeparatorText("Material");

        if (albedoMapID != 0)
        {
            ImGui::ImageButton(component.mesh.material.diffuseMap.path.c_str(), (void*)albedoMapID, ImVec2(64.f, 64.f),
                               ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
            ImGui::ColorEdit3("Albedo", glm::value_ptr(component.mesh.material.diffuse));
        }
        else
            ImGui::ColorEdit3("Albedo", glm::value_ptr(component.mesh.material.diffuse));
    });

    DrawComponent<DirectionalLightComponent>("Directional Light", entity, [&](DirectionalLightComponent& component) {
        ImGui::DragFloat("Intensity", &component.light.intensity, 0.1f);
        ImGui::DragFloat3("Direction", glm::value_ptr(component.light.direction), 0.1f, -1.f, 1.f);
        ImGui::ColorEdit3("Color", glm::value_ptr(component.light.color));
    });

    DrawComponent<CameraComponent>("Perspective Camera", entity, [&](CameraComponent& component) {
        ImGui::Checkbox("Is Primary?", &component.isPrimary);
        ImGui::Checkbox("Is Locked?", &component.camera.isLocked);
        ImGui::DragFloat("FOV", &component.camera.fov);
    });
}
