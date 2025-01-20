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

        void Prepare(DirectionalLight& directionalLight, Shader& shader);
        void DrawEntity(std::shared_ptr<Core::Entity>& entity, Shader& shader);

    private:
        Camera* m_primaryCamera = NULL;

        glm::vec3 m_clearColor;
        glm::mat4 m_projection;
    };

    extern RenderManager* Renderer;
}
