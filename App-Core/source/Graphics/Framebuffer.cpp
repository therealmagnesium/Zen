#include "Graphics/Framebuffer.h"
#include "Graphics/Texture.h"
#include <glad/glad.h>

namespace Graphics
{
    Framebuffer CreateFramebuffer(u32 width, u32 height)
    {
        Framebuffer framebuffer;
        framebuffer.colorAttachment = CreateEmptyTexture(width, height, TextureFormat::RGB16F);
        framebuffer.depthAttachment = CreateEmptyTexture(width, height, TextureFormat::DepthStencil);

        glGenFramebuffers(1, &framebuffer.id);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer.id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer.colorAttachment.id, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
                               framebuffer.depthAttachment.id, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        return framebuffer;
    }

    void BindFramebuffer(Framebuffer& framebuffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.id);
    }

    void UnbindFramebuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void DestoryFramebuffer(Framebuffer& framebuffer)
    {
        glDeleteFramebuffers(1, &framebuffer.id);
    }

}
