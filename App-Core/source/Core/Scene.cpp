#include "Core/Scene.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"

#include "Graphics/Renderer.h"

#include <glm/glm.hpp>

namespace Core
{
    void Scene::Initialize()
    {
        m_editorCamera.position = glm::vec3(12.135f, 1.108f, 14.209f);
        m_editorCamera.rotation = glm::vec3(-141.3f, -2.5f, 0.f);
        Graphics::Renderer->SetPrimaryCamera(&m_editorCamera);
    }

    void Scene::Update()
    {
        m_entityManager.Update();

        Graphics::UpdateCameraController(m_editorCamera);
        Graphics::UpdateCamera(m_editorCamera);

        if (IsKeyPressed(KEY_F8))
            Graphics::LogCameraInfo(m_editorCamera);
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
