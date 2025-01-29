#pragma once
#include "Core/EntityManager.h"
#include "Graphics/Camera.h"

namespace Core
{
    class Scene
    {
    public:
        Scene() = default;

        inline EntityManager& GetEntityManager() { return m_entityManager; }
        inline Graphics::Camera& GetEditorCamera() { return m_editorCamera; }
        inline std::vector<std::shared_ptr<Entity>>& GetEntities() { return m_entityManager.GetEntities(); }

        void Initialize();
        void Update();

        std::shared_ptr<Entity> AddEntity(const char* tag);
        void DestroyEntity(std::shared_ptr<Entity>& entity);

    private:
        EntityManager m_entityManager;
        Graphics::Camera m_editorCamera;
    };
}
