#include "Graphics/Texture.h"
#include "Core/Log.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Graphics
{
    void InvalidateTexture(Texture& texture, bool isEmptyTexture)
    {
        glBindTexture(GL_TEXTURE_2D, texture.id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        u32 glFormat = GetGLTextureFormat(texture.format);
        u32 internalFormat = GetInternalTextureFormat(texture.format, isEmptyTexture);
        u32 dataType = GetTextureDataType(texture.format);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texture.width, texture.height, 0, glFormat, dataType,
                     texture.data);

        // glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture CreateEmptyTexture(u32 width, u32 height, TextureFormat format)
    {
        Texture texture;
        texture.width = width;
        texture.height = height;
        texture.format = format;

        glGenTextures(1, &texture.id);
        InvalidateTexture(texture, true);

        texture.isValid = true;
        return texture;
    }

    Texture LoadTexture(const char* path, TextureFormat format)
    {
        Texture texture;
        texture.path = path;
        texture.format = format;

        stbi_set_flip_vertically_on_load(true);
        texture.data = stbi_load(path, (s32*)&texture.width, (s32*)&texture.height, (s32*)&texture.channelCount, 0);
        if (texture.data == NULL)
        {
            WARN("Failed to load texture %s!", path);
            return (Texture){.isValid = false};
        }

        glGenTextures(1, &texture.id);
        InvalidateTexture(texture, false);

        texture.isValid = true;
        INFO("Successfully loaded texture %s", path);

        return texture;
    }

    void UnloadTexture(Texture& texture)
    {
        if (texture.data != NULL && texture.isValid)
        {
            INFO("Unloading texture %s...", texture.path.c_str());
            stbi_image_free(texture.data);
            texture.isValid = false;
        }

        glDeleteTextures(1, &texture.id);
    }

    void BindTexture(Texture& texture, u8 slot)
    {
        glBindTexture(GL_TEXTURE_2D, texture.id);
        glActiveTexture(GL_TEXTURE0 + slot);
    }

    void UnbindTexture()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    u32 GetInternalTextureFormat(TextureFormat format, bool isEmptyTexture)
    {
        u32 internalFormat = 0;

        switch (format)
        {
            case TextureFormat::RGB:
                internalFormat = (!isEmptyTexture) ? GL_SRGB : GL_RGB;
                break;

            case TextureFormat::RGB16F:
                internalFormat = GL_RGB16F;
                break;

            case TextureFormat::RGBA:
                internalFormat = (!isEmptyTexture) ? GL_SRGB_ALPHA : GL_RGBA;
                break;

            case TextureFormat::Depth:
                internalFormat = GL_DEPTH_COMPONENT;
                break;

            case TextureFormat::DepthStencil:
                internalFormat = GL_DEPTH24_STENCIL8;
                break;

            default:
                break;
        }

        return internalFormat;
    }

    u32 GetGLTextureFormat(TextureFormat format)
    {
        u32 glFormat = 0;
        switch (format)
        {
            case TextureFormat::RGB:
                glFormat = GL_RGB;
                break;

            case TextureFormat::RGB16F:
                glFormat = GL_RGB;
                break;

            case TextureFormat::RGBA:
                glFormat = GL_RGBA;
                break;

            case TextureFormat::R:
                glFormat = GL_RED;
                break;

            case TextureFormat::Depth:
                glFormat = GL_DEPTH_COMPONENT;

            case TextureFormat::DepthStencil:
                glFormat = GL_DEPTH_STENCIL;
                break;

            default:
                break;
        }

        return glFormat;
    }

    u32 GetTextureDataType(TextureFormat format)
    {
        u32 dataType = 0;

        switch (format)
        {
            case TextureFormat::RGB:
                dataType = GL_UNSIGNED_BYTE;
                break;

            case TextureFormat::Depth:
            case TextureFormat::RGB16F:
                dataType = GL_FLOAT;
                break;

            case TextureFormat::RGBA:
                dataType = GL_UNSIGNED_BYTE;
                break;

            case TextureFormat::DepthStencil:
                dataType = GL_UNSIGNED_INT_24_8;
                break;

            default:
                break;
        }

        return dataType;
    }
}
