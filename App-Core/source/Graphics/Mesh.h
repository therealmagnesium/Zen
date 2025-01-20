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
        VertexIdentifier indexBuffer;
        Material material;
        u32 vertexCount = 0;
        u32 indexCount = 0;
        std::vector<Vertex> vertices;
        std::vector<u32> indices;
    };

    Mesh CreateMesh(Vertex* vertices, u32 vertexCount, u32* indices, u32 indexCount);
    Mesh LoadMesh(const char* path);
    void UnloadMesh(Mesh& mesh);
}
