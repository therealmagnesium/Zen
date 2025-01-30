#pragma once
#include "Graphics/Texture.h"
#include "Core/Base.h"

#include <vector>

namespace Graphics
{
    enum class FramebufferAttachmentType
    {
        Color = 0,
        Depth,
        DepthStencil,
    };

    struct Framebuffer
    {
        u32 id = 0;
        u32 width = 0;
        u32 height = 0;
        bool isSelected = false;
        std::vector<Texture> attachments;
    };

    Framebuffer CreateFramebuffer(u8 numAttachments);
    void ApplyFramebufferAttachments(Framebuffer& framebuffer);
    void ResizeFramebuffer(Framebuffer& framebuffer, u32 width, u32 height);
    void BindFramebuffer(Framebuffer& framebuffer);
    void UnbindFramebuffer();
    void DestroyFramebuffer(Framebuffer& framebuffer);
}
