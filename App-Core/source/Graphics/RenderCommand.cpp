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

        static u32 TextureFormatToGLFramebufferAttachment(TextureFormat format, u8 slot)
        {
            u32 val = 0;
            switch (format)
            {
                case TextureFormat::RGB:
                case TextureFormat::RGBA:
                case TextureFormat::RGB16F:
                case TextureFormat::R:
                    val = GL_COLOR_ATTACHMENT0 + slot;
                    break;

                case TextureFormat::Depth:
                    val = GL_DEPTH_ATTACHMENT;
                    break;

                case TextureFormat::DepthStencil:
                    val = GL_DEPTH_STENCIL_ATTACHMENT;
                    break;

                default:
                    break;
            }
            return val;
        }

        void SetViewport(u32 x, u32 y, u32 width, u32 height)
        {
            glViewport(x, y, width, height);
        }

        void SendDataToBuffer(VertexIdentifier& buffer, BufferType type, void* data, u64 size)
        {
            if (buffer.isSelected)
            {
                u32 glBufferType = BufferTypeToGL(type);
                glBufferData(glBufferType, size, data, GL_STATIC_DRAW);
            }
        }

        void SetAttributeLocation(VertexIdentifier& vao, u8 location, u8 numComponents, u64 stride, u64 offset)
        {
            if (vao.isSelected)
                glVertexAttribPointer(location, numComponents, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        }

        void AttachToFramebuffer(Framebuffer& framebuffer, Texture& attachment, TextureFormat format)
        {
            static u8 slot = 0;
            u32 glType = TextureFormatToGLFramebufferAttachment(format, slot);
            if (framebuffer.isSelected)
                glFramebufferTexture2D(GL_FRAMEBUFFER, glType, GL_TEXTURE_2D, attachment.id, 0);

            if (format == TextureFormat::RGB16F || format == TextureFormat::RGB || format == TextureFormat::RGBA)
                slot++;
        }
    }
}
