#pragma once
#include "Graphics/RendererInternal.h"
#include "Graphics/Framebuffer.h"

namespace Graphics
{
    enum class BufferType
    {
        Array = 0,
        ElementArray
    };

    namespace RenderCommand
    {
        void SetViewport(u32 width, u32 height);
        void SendDataToBuffer(VertexIdentifier& buffer, BufferType type, void* data, u64 size);
        void SetAttributeLocation(VertexIdentifier& vao, u8 location, u8 numComponents, u64 stride, u64 offset);
        void AttachToFramebuffer(Framebuffer& framebuffer, Texture& attachment, TextureFormat format);
    }
}
