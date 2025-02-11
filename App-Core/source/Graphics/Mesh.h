#pragma once
#include "Core/Base.h"
#include "Graphics/Material.h"
#include "Graphics/RendererInternal.h"

#include <vector>
#include <glm/glm.hpp>

namespace Graphics
{
    struct Mesh
    {
        VertexIdentifier vertexArray;
        VertexIdentifier vertexBuffer;
        VertexIdentifier instanceBuffer;
        VertexIdentifier indexBuffer;
        Material material;
        u32 vertexCount = 0;
        u32 indexCount = 0;
        std::string name = "Default";
        std::string path = "Default";
        bool shouldCullBackface = true;
        std::vector<Vertex> vertices;
        std::vector<u32> indices;

        static u32 LoadFlags;
    };

    std::vector<Mesh> LoadMeshes(const char* path);
    void UnloadMesh(Mesh& mesh);
}
