#pragma once
#include "Graphics/Camera.h"
#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"
#include "Graphics/Skybox.h"

#include "Core/Entity.h"

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <memory>

namespace Graphics
{
    enum class FaceCull
    {
        Back = 0,
        Front,
    };

    enum class BufferBit
    {
        Color = GL_COLOR_BUFFER_BIT,
        Depth = GL_DEPTH_BUFFER_BIT,
        Stencil = GL_STENCIL_BUFFER_BIT
    };

    class RenderManager
    {
    public:
        RenderManager() = default;

        inline float GetExposure() const { return m_exposure; }
        inline float GetGammaCorrection() const { return m_gammaCorrection; }
        inline glm::mat4& GetProjection() { return m_projection; }
        inline Camera* GetPrimaryCamera() const { return m_primaryCamera; }

        inline void SetExposure(float exposure) { m_exposure = exposure; }
        inline void SetClearColor(float r, float g, float b) { m_clearColor = glm::vec3(r, g, b); }
        inline void SetPrimaryCamera(Camera* camera) { m_primaryCamera = camera; }

        void Initialize();
        void Shutdown();

        void CalculateProjection();

        void BeginDrawing();
        void EndDrawing();

        void Clear(bool color, bool depth, bool stencil);
        void CullFace(FaceCull cull);
        void WriteDepth(bool shouldWriteDepth);

        void Prepare(Shader& shader);
        void DrawMesh(Mesh* mesh, glm::mat4& transform, Shader& shader);
        void DrawSkybox(Skybox& skybox, Mesh* cubeMesh, Shader& shader);

    private:
        Camera* m_primaryCamera = NULL;

        float m_exposure = 1.f;
        float m_gammaCorrection = 2.2f;
        glm::vec3 m_clearColor = glm::vec3(1.f);
        glm::mat4 m_projection = glm::mat4(1.f);
    };

    extern RenderManager* Renderer;
}
