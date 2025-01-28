#include "Graphics/Skybox.h"
#include "Core/Log.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Graphics
{
    Skybox LoadSkybox(const char** paths)
    {
        Skybox skybox;
        glGenTextures(1, &skybox.id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.id);

        for (u8 i = 0; i < LEN(skybox.data); i++)
        {
            stbi_set_flip_vertically_on_load(false);
            skybox.data[i] = stbi_load(paths[i], &skybox.textureWidth, &skybox.textureHeight, &skybox.channelCount, 0);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, skybox.textureWidth, skybox.textureHeight, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, skybox.data[i]);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        INFO("Skybox loaded");

        return skybox;
    }

    void UnloadSkybox(Skybox& skybox)
    {
        INFO("Unloading skybox");

        for (u32 i = 0; i < LEN(skybox.data); i++)
        {
            if (skybox.data[i] != NULL)
                stbi_image_free(skybox.data[i]);
        }

        glDeleteTextures(1, &skybox.id);
    }

    void BindSkybox(Skybox& skybox)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.id);
        glActiveTexture(GL_TEXTURE0);
        skybox.isSelected = true;
    }

    void UnbindSkybox()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}
