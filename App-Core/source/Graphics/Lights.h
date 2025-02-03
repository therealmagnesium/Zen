#pragma once
#include "Core/Base.h"

#include <glm/glm.hpp>

namespace Graphics
{
    struct DirectionalLight
    {
        float intensity = 2.5f;
        glm::vec3 color = glm::vec3(1.f);
        glm::vec3 direction = glm::vec3(0.f, -1.f, 0.f);
    };

}
