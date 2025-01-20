#include "Graphics/Renderer.h"
#include "Core/Application.h"
#include "Core/Log.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Graphics
{
    static bool isInitialized = false;
    RenderManager* Renderer = NULL;

    static u32 FaceCullToGL(FaceCull cull)
    {
        u32 val = 0;

        switch (cull)
        {
            case FaceCull::Back:
                val = GL_BACK;
                break;

            case FaceCull::Front:
                val = GL_FRONT;
                break;
        }

        return val;
    }

    void RenderManager::Initialize()
    {
        if (isInitialized)
        {
            WARN("Cannot initialize renderer more than once...");
            return;
        }

        m_projection = glm::mat4(1.f);

        Renderer = this;
        isInitialized = true;

        INFO("Successfully initialized the renderer");
    }

    void RenderManager::Shutdown()
    {
        INFO("Shutting down the renderer...");
    }

    void RenderManager::CalculateProjection()
    {
        Core::ApplicationSpecification& appInfo = Core::App->GetSpecification();
        float aspectRatio = appInfo.windowWidth / (float)appInfo.windowHeight;
        m_projection = glm::perspective(glm::radians(45.f), aspectRatio, 0.1f, 100.f);
    }

    void RenderManager::BeginDrawing()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        this->Clear();
        this->CalculateProjection();
    }

    void RenderManager::EndDrawing()
    {
        Window& window = Core::App->GetWindow();
        DisplayWindow(window);
    }

    void RenderManager::Clear()
    {
        glClearColor(V3_OPEN(m_clearColor), 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderManager::CullFace(FaceCull cull)
    {
        u32 glCull = FaceCullToGL(cull);
        glCullFace(glCull);
    }

    void RenderManager::Prepare(DirectionalLight& directionalLight, Shader& shader)
    {
        if (m_primaryCamera != NULL)
        {
            shader.SetMat4("viewMatrix", m_primaryCamera->view);
            shader.SetMat4("projectionMatrix", m_projection);
            shader.SetLight("directionalLight", directionalLight);
        }
    }

    void RenderManager::DrawEntity(std::shared_ptr<Core::Entity>& entity, Shader& shader)
    {
        if (m_primaryCamera != NULL && entity->HasComponent<Core::MeshComponent>())
        {
            auto& tc = entity->GetComponent<Core::TransformComponent>();
            auto& mc = entity->GetComponent<Core::MeshComponent>();
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(tc.transform));

            shader.SetMat4("transformMatrix", tc.transform);
            shader.SetMat4("normalMatrix", normalMatrix);
            shader.SetMaterial("material", mc.mesh.material);

            BindTexture(mc.mesh.material.diffuseMap, 0);
            BindVertexArray(mc.mesh.vertexArray);
            BindIndexBuffer(mc.mesh.indexBuffer);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            glDrawElements(GL_TRIANGLES, mc.mesh.indexCount, GL_UNSIGNED_INT, NULL);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);

            UnbindIndexBuffer();
            UnbindVertexArray();
            UnbindTexture();
        }
    }
}
