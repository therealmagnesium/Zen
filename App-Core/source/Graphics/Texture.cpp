#include "Graphics/Texture.h"
#include "Core/Log.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Graphics
{
    static u32 GetInternalFormat(TextureFormat format, bool isEmptyTexture)
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

    static u32 GetGLFormat(TextureFormat format)
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

    static u32 GetDataType(TextureFormat format)
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

    Texture CreateEmptyTexture(u32 width, u32 height, TextureFormat format)
    {
        Texture texture;
        texture.width = width;
        texture.height = height;
        texture.format = format;

        glGenTextures(1, &texture.id);
        glBindTexture(GL_TEXTURE_2D, texture.id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        u32 glFormat = GetGLFormat(texture.format);
        u32 internalFormat = GetInternalFormat(texture.format, true);
        u32 dataType = GetDataType(texture.format);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texture.width, texture.height, 0, glFormat, dataType, NULL);

        glBindTexture(GL_TEXTURE_2D, 0);

        texture.isValid = true;
        return texture;
    }

    Texture LoadTexture(const char* path, TextureFormat format)
    {
        Texture texture;
        texture.path = path;
        texture.format = format;

        glGenTextures(1, &texture.id);
        glBindTexture(GL_TEXTURE_2D, texture.id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);
        texture.data = stbi_load(path, (s32*)&texture.width, (s32*)&texture.height, (s32*)&texture.channelCount, 0);
        if (texture.data == NULL)
        {
            WARN("Failed to load texture %s!", path);
            return (Texture){.isValid = false};
        }

        u32 glFormat = GetGLFormat(texture.format);
        u32 internalFormat = GetInternalFormat(texture.format, false);
        u32 dataType = GetDataType(texture.format);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texture.width, texture.height, 0, glFormat, dataType,
                     texture.data);

        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

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
}
