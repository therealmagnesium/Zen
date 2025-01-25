#include "Graphics/Framebuffer.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderCommand.h"

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

        for (u32 i = 0; i < framebuffer.attachments.size(); i++)
            RenderCommand::AttachToFramebuffer(framebuffer, framebuffer.attachments[i],
                                               framebuffer.attachments[i].format);

        UnbindFramebuffer();
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
