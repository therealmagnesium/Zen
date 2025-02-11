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
        m_projection = glm::perspective(glm::radians(m_primaryCamera->fov), aspectRatio, 0.1f, 100.f);
    }

    void RenderManager::BeginDrawing()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        this->CalculateProjection();
    }

    void RenderManager::EndDrawing()
    {
        Window& window = Core::App->GetWindow();
        DisplayWindow(window);
    }

    void RenderManager::Clear(bool color, bool depth, bool stencil)
    {
        glClearColor(pow(m_clearColor.x, 2.2f), pow(m_clearColor.y, 2.2f), pow(m_clearColor.z, 2.2f), 1.f);

        u32 bufferBit = 0;
        if (color)
            bufferBit |= (u32)BufferBit::Color;
        if (depth)
            bufferBit |= (u32)BufferBit::Depth;
        if (stencil)
            bufferBit |= (u32)BufferBit::Stencil;

        glClear(bufferBit);
    }

    void RenderManager::CullFace(FaceCull cull)
    {
        u32 glCull = FaceCullToGL(cull);
        glCullFace(glCull);
    }

    void RenderManager::WriteDepth(bool shouldWriteDepth)
    {
        glDepthMask(shouldWriteDepth);
    }

    void RenderManager::Prepare(Shader& shader)
    {
        if (m_primaryCamera != NULL)
        {
            BindShader(shader);

            shader.SetVec3("cameraPosition", m_primaryCamera->position);
            shader.SetMat4("viewMatrix", m_primaryCamera->view);
            shader.SetMat4("projectionMatrix", m_projection);

            UnbindShader();
        }
    }

    void RenderManager::DrawMesh(Mesh* mesh, glm::mat4& transform, Shader& shader)
    {
        if (m_primaryCamera != NULL && mesh != NULL)
        {
            (mesh->shouldCullBackface) ? CullFace(FaceCull::Back) : CullFace(FaceCull::Front);

            BindVertexArray(mesh->vertexArray);
            BindIndexBuffer(mesh->indexBuffer);
            BindTexture(mesh->material.diffuseMap, 0);

            glm::mat4 normalMatrix = glm::transpose(glm::inverse(transform));

            BindShader(shader);

            shader.SetMat4("transformMatrix", transform);
            shader.SetMat4("normalMatrix", normalMatrix);
            shader.SetMaterial("material", mesh->material);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, NULL);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);

            UnbindShader();

            UnbindIndexBuffer();
            UnbindVertexArray();
            UnbindTexture();
        }
    }

    void RenderManager::DrawSkybox(Skybox& skybox, Mesh* cubeMesh, Shader& shader)
    {
        assert(m_primaryCamera != NULL);
        glm::mat4 skyboxView = glm::mat4(glm::mat3(m_primaryCamera->view));

        WriteDepth(false);
        BindShader(shader);

        shader.SetInt("skybox", 0);
        shader.SetMat4("viewMatrix", skyboxView);
        shader.SetMat4("projectionMatrix", m_projection);

        if (cubeMesh != NULL)
        {
            CullFace(FaceCull::Front);

            BindVertexArray(cubeMesh->vertexArray);
            BindIndexBuffer(cubeMesh->indexBuffer);
            BindSkybox(skybox);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            glDrawElements(GL_TRIANGLES, cubeMesh->indexCount, GL_UNSIGNED_INT, NULL);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);

            UnbindSkybox();
            UnbindIndexBuffer();
            UnbindVertexArray();

            CullFace(FaceCull::Back);
        }

        UnbindShader();
        WriteDepth(true);
    }
}
