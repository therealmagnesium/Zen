#pragma once
#include "Graphics/Texture.h"
#include <glm/glm.hpp>

namespace Graphics
{
    struct Material
    {
        glm::vec3 diffuse = glm::vec3(1.f);
        Texture diffuseMap;
    };

    void UnloadMaterial(Material& material);
}
