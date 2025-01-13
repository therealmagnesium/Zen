#include "Graphics/RendererInternal.h"
#include "Graphics/Renderer.h"
#include <glad/glad.h>

namespace Graphics
{
    VertexIdentifier CreateVertexArray()
    {
        VertexIdentifier vao;
        glGenVertexArrays(1, &vao.id);
        Renderer->AddVertexArray(vao);

        return vao;
    }

    void BindVertexArray(VertexIdentifier& vao)
    {
        glBindVertexArray(vao.id);
        vao.isSelected = true;
    }

    void UnbindVertexArray()
    {
        glBindVertexArray(0);

        for (auto& vao : Renderer->GetVertexArrays())
            vao.isSelected = false;
    }

    void DeleteVertexArray(VertexIdentifier& vao)
    {
        glDeleteVertexArrays(1, &vao.id);
        vao.isSelected = false;
    }

    VertexIdentifier CreateVertexBuffer()
    {
        VertexIdentifier vbo;
        glGenBuffers(1, &vbo.id);
        Renderer->AddVertexBuffer(vbo);

        return vbo;
    }

    void BindVertexBuffer(VertexIdentifier& vbo)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
        vbo.isSelected = true;
    }

    void UnbindVertexBuffer()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        for (auto& vbo : Renderer->GetVertexBuffers())
            vbo.isSelected = false;
    }

    void DeleteVertexBuffer(VertexIdentifier& vbo)
    {
        glDeleteBuffers(1, &vbo.id);
        vbo.isSelected = false;
    }

    VertexIdentifier CreateIndexBuffer()
    {
        VertexIdentifier ebo;
        glGenBuffers(1, &ebo.id);
        Renderer->AddVertexBuffer(ebo);

        return ebo;
    }

    void BindIndexBuffer(VertexIdentifier& ebo)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);
        ebo.isSelected = true;
    }

    void UnbindIndexBuffer()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        for (auto& ebo : Renderer->GetVertexBuffers())
            ebo.isSelected = false;
    }
}
