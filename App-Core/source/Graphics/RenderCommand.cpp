#include "Graphics/RenderCommand.h"
#include "Core/Log.h"
#include <glad/glad.h>

namespace Graphics
{
    namespace RenderCommand
    {
        static u32 BufferTypeToGL(BufferType type)
        {
            u32 val = 0;
            switch (type)
            {
                case BufferType::Array:
                    val = GL_ARRAY_BUFFER;
                    break;

                case BufferType::ElementArray:
                    val = GL_ELEMENT_ARRAY_BUFFER;
                    break;
            }

            return val;
        }

        void SendDataToBuffer(VertexIdentifier& buffer, BufferType type, void* data, u64 size)
        {
            if (!buffer.isSelected)
            {
                switch (type)
                {
                    case BufferType::Array:
                        BindVertexBuffer(buffer);
                        break;

                    case BufferType::ElementArray:
                        BindIndexBuffer(buffer);
                        break;
                }
            }

            u32 glBufferType = BufferTypeToGL(type);
            glBufferData(glBufferType, size, data, GL_STATIC_DRAW);
        }

        void SetAttributeLocation(VertexIdentifier& vao, u8 location, u8 numComponents, u64 stride, u64 offset)
        {
            if (vao.isSelected)
                glVertexAttribPointer(location, numComponents, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        }
    }
}
