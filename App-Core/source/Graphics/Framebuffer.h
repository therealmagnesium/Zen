#pragma once
#include "Graphics/Texture.h"
#include "Core/Base.h"

namespace Graphics
{
    struct Framebuffer
    {
        u32 id = 0;
        u32 width = 0;
        u32 height = 0;
        Texture colorAttachment;
        Texture depthAttachment;
    };

    Framebuffer CreateFramebuffer(u32 width, u32 height);
    void BindFramebuffer(Framebuffer& framebuffer);
    void UnbindFramebuffer();
    void DestoryFramebuffer(Framebuffer& framebuffer);
}
