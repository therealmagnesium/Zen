#include "Graphics/RendererInternal.h"
#include <glad/glad.h>

namespace Graphics
{
    VertexIdentifier CreateVertexArray()
    {
        VertexIdentifier vao;
        glGenVertexArrays(1, &vao.id);

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
    }

    VertexIdentifier CreateIndexBuffer()
    {
        VertexIdentifier ebo;
        glGenBuffers(1, &ebo.id);

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
    }

    void DeleteBuffer(VertexIdentifier& buffer)
    {
        glDeleteBuffers(1, &buffer.id);
        buffer.isSelected = false;
    }
}
