#pragma once
#include "Core/Base.h"
#include <glm/glm.hpp>

namespace Graphics
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 uvCoord;
        glm::vec3 normal;
    };

    struct VertexIdentifier
    {
        u32 id = 0;
        bool isSelected = false;
    };

    VertexIdentifier CreateVertexArray();
    void BindVertexArray(VertexIdentifier& vao);
    void UnbindVertexArray();
    void DeleteVertexArray(VertexIdentifier& vao);

    VertexIdentifier CreateVertexBuffer();
    void BindVertexBuffer(VertexIdentifier& vbo);
    void UnbindVertexBuffer();
    void DeleteVertexBuffer(VertexIdentifier& vbo);

    VertexIdentifier CreateIndexBuffer();
    void BindIndexBuffer(VertexIdentifier& ebo);
    void UnbindIndexBuffer();
}
