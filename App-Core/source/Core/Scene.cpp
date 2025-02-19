#include "Core/Scene.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"

#include "Graphics/Renderer.h"

#include <glm/glm.hpp>

namespace Core
{
    void Scene::Initialize(bool addDefaultEntities)
    {
        m_editorCamera.position = glm::vec3(12.135f, 1.108f, 14.209f);
        m_editorCamera.rotation = glm::vec3(-141.3f, -2.5f, 0.f);
        m_editorCamera.speed = 25.f;
        Graphics::Renderer->SetPrimaryCamera(&m_editorCamera);

        if (addDefaultEntities)
        {
            std::shared_ptr<Entity> sceneCamera = m_entityManager.AddEntity("Main Camera");
            sceneCamera->AddComponent<CameraComponent>();

            std::shared_ptr<Entity> directionalLight = m_entityManager.AddEntity("Directional Light");
            directionalLight->AddComponent<DirectionalLightComponent>(glm::vec3(0.2f, -0.86f, -0.95f));
        }
    }

    void Scene::Update()
    {
        m_entityManager.Update();
        Graphics::Renderer->SetPrimaryCamera(&m_editorCamera);

        Graphics::UpdateCameraController(m_editorCamera);
        Graphics::UpdateCamera(m_editorCamera);

        if (IsKeyPressed(KEY_F8))
            Graphics::LogCameraInfo(m_editorCamera);

        for (auto& entity : m_entityManager.GetEntities())
        {
            auto& tc = entity->GetComponent<TransformComponent>();

            if (entity->HasComponent<CameraComponent>())
            {
                auto& cc = entity->GetComponent<CameraComponent>();
                cc.camera.position = tc.position;
                cc.camera.rotation = tc.rotation;

                if (cc.isPrimary)
                    Graphics::Renderer->SetPrimaryCamera(&cc.camera);

                Graphics::UpdateCamera(cc.camera);
            }
        }
    }

    void Scene::DrawEntities(Graphics::Shader& shader)
    {
        for (auto& entity : m_entityManager.GetEntities())
        {
            auto& tc = entity->GetComponent<TransformComponent>();

            if (entity->HasComponent<DirectionalLightComponent>())
            {
                auto& dlc = entity->GetComponent<DirectionalLightComponent>();

                Graphics::BindShader(shader);
                shader.SetLight("directionalLight", dlc.light);
                Graphics::UnbindShader();
            }

            if (entity->HasComponent<MeshComponent>())
            {
                auto& mc = entity->GetComponent<MeshComponent>();
                Graphics::Renderer->DrawMesh(&mc.mesh, tc.transform, shader);
            }
        }
    }

    std::shared_ptr<Entity> Scene::AddEntity(const char* tag)
    {
        std::shared_ptr<Entity> entity = m_entityManager.AddEntity(tag);
        entity->AddComponent<TransformComponent>();

        return entity;
    }

    void Scene::DestroyEntity(std::shared_ptr<Entity>& entity)
    {
        m_entityManager.DestroyEntity(entity);
    }

}
