#include "Graphics/Renderer.h"
#include "Core/Application.h"
#include "Core/Log.h"
#include "Graphics/RenderCommand.h"

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
        m_projection = glm::perspective(glm::radians(45.f), aspectRatio, 0.1f, 100.f);
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

    void RenderManager::ProcessEntity(std::shared_ptr<Core::Entity>& entity)
    {
        if (entity->HasComponent<Core::MeshComponent>())
        {
            auto& tc = entity->GetComponent<Core::TransformComponent>();
            auto& mc = entity->GetComponent<Core::MeshComponent>();

            if (mc.mesh != NULL)
            {
                std::vector<std::shared_ptr<Core::Entity>>& batch = m_meshEntitiesMap[mc.mesh];
                std::vector<glm::mat4>& transformsBatch = m_meshTransformsMap[mc.mesh];
                mc.mesh->normalMatrix = glm::transpose(glm::inverse(tc.transform));

                if (batch.size() != 0)
                    batch.push_back(entity);
                else
                {
                    std::vector<std::shared_ptr<Core::Entity>> newBatch;
                    newBatch.push_back(entity);
                    m_meshEntitiesMap[mc.mesh] = newBatch;
                }

                if (transformsBatch.size() != 0)
                    transformsBatch.push_back(tc.transform);
                else
                {
                    std::vector<glm::mat4> newBatch;
                    newBatch.push_back(tc.transform);
                    m_meshTransformsMap[mc.mesh] = newBatch;
                }
            }
        }
    }

    void RenderManager::Prepare(DirectionalLight& directionalLight, Shader& shader)
    {
        if (m_primaryCamera != NULL)
        {
            shader.SetVec3("cameraPosition", m_primaryCamera->position);
            shader.SetMat4("viewMatrix", m_primaryCamera->view);
            shader.SetMat4("projectionMatrix", m_projection);
            shader.SetLight("directionalLight", directionalLight);
        }
    }

    void RenderManager::DrawEntities(Shader& shader)
    {
        if (m_primaryCamera != NULL)
        {
            Renderer->CullFace(FaceCull::Back);

            for (auto& [mesh, entities] : m_meshEntitiesMap)
            {
                if (mesh != NULL)
                {
                    if (!mesh->shouldCullBackface)
                        Renderer->CullFace(FaceCull::Front);

                    PrepareMesh(mesh, shader);

                    glDrawElementsInstanced(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, NULL,
                                            m_meshTransformsMap[mesh].size());

                    UnprepareMesh();
                }
            }
        }

        m_meshEntitiesMap.clear();
        m_meshTransformsMap.clear();
    }

    void RenderManager::PrepareMesh(Mesh* mesh, Shader& shader)
    {
        if (mesh != NULL)
        {
            std::vector<glm::mat4>& batch = m_meshTransformsMap[mesh];

            BindVertexBuffer(mesh->instanceBuffer);
            RenderCommand::SendDataToBuffer(mesh->instanceBuffer, BufferType::Array, batch.data(),
                                            batch.size() * sizeof(glm::mat4));

            BindVertexArray(mesh->vertexArray);

            RenderCommand::SetAttributeLocation(mesh->vertexArray, 3, 4, 4 * sizeof(glm::vec4), 0 * sizeof(glm::vec4));
            RenderCommand::SetAttributeLocation(mesh->vertexArray, 4, 4, 4 * sizeof(glm::vec4), 1 * sizeof(glm::vec4));
            RenderCommand::SetAttributeLocation(mesh->vertexArray, 5, 4, 4 * sizeof(glm::vec4), 2 * sizeof(glm::vec4));
            RenderCommand::SetAttributeLocation(mesh->vertexArray, 6, 4, 4 * sizeof(glm::vec4), 3 * sizeof(glm::vec4));

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            UnbindVertexArray();

            UnbindVertexBuffer();

            BindVertexArray(mesh->vertexArray);
            BindIndexBuffer(mesh->indexBuffer);
            BindTexture(mesh->material.diffuseMap, 0);

            shader.SetMat4("normalMatrix", mesh->normalMatrix);
            shader.SetMaterial("material", mesh->material);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);
            glEnableVertexAttribArray(4);
            glEnableVertexAttribArray(5);
            glEnableVertexAttribArray(6);
        }
    }

    void RenderManager::UnprepareMesh()
    {
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
        glDisableVertexAttribArray(4);
        glDisableVertexAttribArray(5);
        glDisableVertexAttribArray(6);

        UnbindIndexBuffer();
        UnbindVertexArray();
        UnbindTexture();
    }
}
