#pragma once
#include "Core/Base.h"

#include <glad/glad.h>
#include <string>

namespace Graphics
{
    enum class TextureFormat
    {
        None = 0,
        RGB,
        RGBA,
        R,
        DepthStencil
    };

    struct Texture
    {
        u32 id = 0;
        u32 width = 0;
        u32 height = 0;
        u32 channelCount = 0;
        bool isValid = false;
        u8* data = NULL;
        std::string path;
        TextureFormat format;
    };

    Texture CreateEmptyTexture(u32 width, u32 height, TextureFormat format);
    Texture LoadTexture(const char* path, TextureFormat format);
    void BindTexture(Texture& texture, u8 slot);
    void UnbindTexture();
    void UnloadTexture(Texture& texture);
}
