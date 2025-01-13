#pragma once
#include "Graphics/Camera.h"
#include "Graphics/Mesh.h"
#include "Graphics/RendererInternal.h"
#include "Graphics/Shader.h"

#include "Core/Entity.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <unordered_map>

namespace Graphics
{
    typedef std::unordered_map<Mesh*, std::vector<std::shared_ptr<Core::Entity>>> EntityMeshMap;

    class RenderManager
    {
    public:
        RenderManager() = default;

        inline glm::mat4& GetProjection() { return m_projection; }
        inline std::vector<VertexIdentifier>& GetVertexArrays() { return m_vertexArrays; }
        inline std::vector<VertexIdentifier>& GetVertexBuffers() { return m_vertexBuffers; }
        inline Camera* GetPrimaryCamera() const { return m_primaryCamera; }
        inline Shader* GetPrimaryShader() const { return m_primaryShader; }

        inline void SetClearColor(float r, float g, float b) { m_clearColor = glm::vec3(r, g, b); }
        inline void SetPrimaryCamera(Camera* camera) { m_primaryCamera = camera; }
        inline void SetPrimaryShader(Shader* shader) { m_primaryShader = shader; }

        void Initialize();
        void Shutdown();

        void AddVertexArray(VertexIdentifier& vao);
        void AddVertexBuffer(VertexIdentifier& vbo);

        void StoreDataInAttributeList(u32 location, u32 numComponents, Vertex* data, u32 vertexCount, u64 offset);
        VertexIdentifier StoreIndexBuffer(u32* indices, u32 indexCount);

        void ProcessEntity(std::shared_ptr<Core::Entity>& entity);

        void BeginDrawing();
        void EndDrawing();

        void Clear();
        void DrawAllMeshes(DirectionalLight& sun);

    private:
        void PrepareMeshUniforms(Mesh& mesh);
        void PrepareLightUniforms(DirectionalLight& light);
        void PrepareEntity(std::shared_ptr<Core::Entity>& entity);
        void UnbindMesh();

    private:
        Camera* m_primaryCamera = NULL;
        Shader* m_primaryShader = NULL;

        glm::vec3 m_clearColor;
        glm::mat4 m_projection;

        std::vector<VertexIdentifier> m_vertexArrays;
        std::vector<VertexIdentifier> m_vertexBuffers;
        EntityMeshMap m_entitiesToRender;
    };

    extern RenderManager* Renderer;
}
