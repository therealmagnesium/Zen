#pragma once
#include "Graphics/RendererInternal.h"

namespace Graphics
{
    enum class BufferType
    {
        Array = 0,
        ElementArray
    };

    namespace RenderCommand
    {
        void SendDataToBuffer(VertexIdentifier& buffer, BufferType type, void* data, u64 size);
        void SetAttributeLocation(VertexIdentifier& vao, u8 location, u8 numComponents, u64 stride, u64 offset);
    }
}
