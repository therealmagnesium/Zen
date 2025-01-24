#pragma once
#include "Graphics/Camera.h"
#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"

#include "Core/Entity.h"

#include <glm/glm.hpp>
#include <memory>

namespace Graphics
{
    enum class FaceCull
    {
        Back = 0,
        Front,
    };

    class RenderManager
    {
    public:
        RenderManager() = default;

        inline glm::mat4& GetProjection() { return m_projection; }
        inline Camera* GetPrimaryCamera() const { return m_primaryCamera; }

        inline void SetClearColor(float r, float g, float b) { m_clearColor = glm::vec3(r, g, b); }
        inline void SetPrimaryCamera(Camera* camera) { m_primaryCamera = camera; }

        void Initialize();
        void Shutdown();

        void CalculateProjection();

        void BeginDrawing();
        void EndDrawing();

        void Clear();
        void CullFace(FaceCull cull);

        void ProcessEntity(std::shared_ptr<Core::Entity>& entity);

        void Prepare(DirectionalLight& directionalLight, Shader& shader);
        void DrawEntities(Shader& shader);

    private:
        void PrepareMesh(Mesh* mesh, Shader& shader);
        void UnprepareMesh();

    private:
        Camera* m_primaryCamera = NULL;

        glm::vec3 m_clearColor;
        glm::mat4 m_projection;

        std::unordered_map<Mesh*, std::vector<glm::mat4>> m_meshTransformsMap;
        std::unordered_map<Mesh*, std::vector<std::shared_ptr<Core::Entity>>> m_meshEntitiesMap;
    };

    extern RenderManager* Renderer;
}
