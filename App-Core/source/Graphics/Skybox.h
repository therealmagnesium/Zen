#pragma once
#include "Graphics/Texture.h"
#include "Core/Base.h"

#include <stdlib.h>

namespace Graphics
{
    struct Skybox
    {
        u32 id = 0;
        bool isSelected = false;
        s32 textureWidth = 0;
        s32 textureHeight = 0;
        s32 channelCount = 0;
        u8* data[6]{NULL};
    };

    Skybox LoadSkybox(const char** paths, TextureFormat textureFormat);
    void BindSkybox(Skybox& skybox);
    void UnbindSkybox();
    void UnloadSkybox(Skybox& skybox);
}
