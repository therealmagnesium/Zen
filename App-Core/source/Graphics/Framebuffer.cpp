#include "Graphics/Framebuffer.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderCommand.h"

#include "Core/Log.h"

#include <glad/glad.h>

namespace Graphics
{
    Framebuffer CreateFramebuffer(u8 numAttachments)
    {
        Framebuffer framebuffer;
        framebuffer.attachments.resize(numAttachments);
        glGenFramebuffers(1, &framebuffer.id);

        return framebuffer;
    }

    void ApplyFramebufferAttachments(Framebuffer& framebuffer)
    {
        BindFramebuffer(framebuffer);

        u32 glAttachments[framebuffer.attachments.size()];
        for (u32 i = 0; i < framebuffer.attachments.size(); i++)
        {
            RenderCommand::AttachToFramebuffer(framebuffer, framebuffer.attachments[i],
                                               framebuffer.attachments[i].format);
            glAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
        }

        glDrawBuffers(framebuffer.attachments.size(), glAttachments);

        UnbindFramebuffer();
    }

    void ResizeFramebuffer(Framebuffer& framebuffer, u32 width, u32 height)
    {
        TextureFormat formats[framebuffer.attachments.size()];

        for (u32 i = 0; i < framebuffer.attachments.size(); i++)
        {
            formats[i] = framebuffer.attachments[i].format;
            framebuffer.attachments[i].width = width;
            framebuffer.attachments[i].height = height;
            InvalidateTexture(framebuffer.attachments[i], false);
        }

        ApplyFramebufferAttachments(framebuffer);
    }

    void BindFramebuffer(Framebuffer& framebuffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.id);
        framebuffer.isSelected = true;
    }

    void UnbindFramebuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void DestroyFramebuffer(Framebuffer& framebuffer)
    {
        glDeleteFramebuffers(1, &framebuffer.id);
        framebuffer.isSelected = false;
    }

}
