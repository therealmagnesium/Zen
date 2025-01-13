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

        for (auto& vao : m_vertexArrays)
            DeleteVertexArray(vao);

        for (auto& vbo : m_vertexBuffers)
            DeleteVertexBuffer(vbo);
    }

    void RenderManager::AddVertexArray(VertexIdentifier& vao)
    {
        m_vertexArrays.push_back(vao);
    }

    void RenderManager::AddVertexBuffer(VertexIdentifier& vbo)
    {
        m_vertexBuffers.push_back(vbo);
    }

    VertexIdentifier RenderManager::StoreIndexBuffer(u32* indices, u32 indexCount)
    {
        VertexIdentifier ebo = CreateIndexBuffer();

        BindIndexBuffer(ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(u32), indices, GL_STATIC_DRAW);
        UnbindIndexBuffer();

        return ebo;
    }

    void RenderManager::ProcessEntity(std::shared_ptr<Core::Entity>& entity)
    {
        if (entity->HasComponent<Core::MeshComponent>())
        {
            auto& mc = entity->GetComponent<Core::MeshComponent>();

            if (mc.mesh != NULL)
            {
                std::vector<std::shared_ptr<Core::Entity>>& batch = m_entitiesToRender[mc.mesh];

                if (batch.size() > 0)
                    batch.push_back(entity);
                else
                {
                    std::vector<std::shared_ptr<Core::Entity>> newBatch;
                    newBatch.push_back(entity);
                    m_entitiesToRender[mc.mesh] = newBatch;
                }
            }
        }
    }

    void RenderManager::StoreDataInAttributeList(u32 location, u32 numComponents, Vertex* data, u32 vertexCount,
                                                 u64 offset)
    {
        VertexIdentifier vbo = CreateVertexBuffer();

        BindVertexBuffer(vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), data, GL_STATIC_DRAW);
        glVertexAttribPointer(location, numComponents, GL_FLOAT, false, sizeof(Vertex), (void*)offset);
        UnbindVertexBuffer();
    }

    void RenderManager::BeginDrawing()
    {
        glEnable(GL_DEPTH_TEST);
        this->Clear();

        Core::ApplicationSpecification& appInfo = Core::App->GetSpecification();
        float aspectRatio = appInfo.windowWidth / (float)appInfo.windowHeight;
        m_projection = glm::perspective(glm::radians(45.f), aspectRatio, 0.1f, 100.f);
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

    void RenderManager::DrawAllMeshes(DirectionalLight& sun)
    {
        if (m_primaryShader != NULL)
        {
            BindShader(*m_primaryShader);
            PrepareLightUniforms(sun);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            for (auto& [mesh, entities] : m_entitiesToRender)
            {
                PrepareMeshUniforms(*mesh);
                for (auto& entity : entities)
                {
                    PrepareEntity(entity);
                    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, NULL);
                }
            }

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);

            UnbindShader();
        }

        m_entitiesToRender.clear();
    }

    void RenderManager::PrepareMeshUniforms(Mesh& mesh)
    {
        if (m_primaryShader != NULL)
        {
            m_primaryShader->SetMat4("viewMatrix", m_primaryCamera->view);
            m_primaryShader->SetMat4("projectionMatrix", m_projection);
            m_primaryShader->SetMaterial("material", mesh.material);
        }

        BindTexture(mesh.material.diffuseMap, mesh.material.diffuseMap.id);
        BindVertexArray(mesh.vertexArray);
        BindIndexBuffer(mesh.indexBuffer);
    }

    void RenderManager::PrepareLightUniforms(DirectionalLight& sun)
    {
        if (m_primaryShader != NULL)
            m_primaryShader->SetLight("directionalLight", sun);
    }

    void RenderManager::PrepareEntity(std::shared_ptr<Core::Entity>& entity)
    {
        if (entity->HasComponent<Core::MeshComponent>())
        {
            auto& tc = entity->GetComponent<Core::TransformComponent>();
            auto& mc = entity->GetComponent<Core::MeshComponent>();

            const glm::vec3 xAxis = glm::vec3(1.f, 0.f, 0.f);
            const glm::vec3 yAxis = glm::vec3(0.f, 1.f, 0.f);
            const glm::vec3 zAxis = glm::vec3(0.f, 0.f, 1.f);

            if (mc.mesh != NULL && m_primaryShader != NULL)
            {
                mc.mesh->transform = glm::mat4(1.f);
                mc.mesh->transform = glm::translate(mc.mesh->transform, tc.position);
                mc.mesh->transform = glm::rotate(mc.mesh->transform, glm::radians(tc.rotation.x), xAxis);
                mc.mesh->transform = glm::rotate(mc.mesh->transform, glm::radians(tc.rotation.y), yAxis);
                mc.mesh->transform = glm::rotate(mc.mesh->transform, glm::radians(tc.rotation.z), zAxis);
                mc.mesh->transform = glm::scale(mc.mesh->transform, tc.scale);

                glm::mat4 normal = glm::mat4(1.f);
                normal = glm::transpose(glm::inverse(mc.mesh->transform));

                m_primaryShader->SetMat4("transformMatrix", mc.mesh->transform);
                m_primaryShader->SetMat4("normalMatrix", normal);
            }
        }
    }

    void RenderManager::UnbindMesh()
    {
        UnbindIndexBuffer();
        UnbindVertexArray();
        UnbindTexture();
    }

}
