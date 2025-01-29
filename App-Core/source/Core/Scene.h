#pragma once
#include "Core/EntityManager.h"
#include "Graphics/Camera.h"

namespace Core
{
    class Scene
    {
    public:
        Scene() = default;

        void Initialize();
        void AddEntity(const char* tag);
        void DestroyEntity(std::shared_ptr<Entity>& entity);

    private:
        EntityManager m_entityManager;
        Graphics::Camera m_editorCamera;
    };
}
