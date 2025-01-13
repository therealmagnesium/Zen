#pragma once
#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"

#include <glm/glm.hpp>

namespace Core
{
    struct TransformComponent
    {
        bool isEnabled = false;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        TransformComponent()
        {
            position = glm::vec3(0.f);
            rotation = glm::vec3(0.f);
            scale = glm::vec3(1.f);
        }

        TransformComponent(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
        {
            this->position = position;
            this->rotation = rotation;
            this->scale = scale;
        }
    };

    struct MeshComponent
    {
        bool isEnabled = false;
        Graphics::Mesh* mesh = NULL;

        MeshComponent() = default;
        MeshComponent(Graphics::Mesh* mesh) { this->mesh = mesh; };
    };
}