#pragma once
#include "Graphics/Camera.h"
#include "Graphics/Lights.h"
#include "Graphics/Mesh.h"

#include <glm/glm.hpp>

namespace Core
{
    struct TransformComponent
    {
        bool isEnabled = false;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 transform;

        TransformComponent()
        {
            position = glm::vec3(0.f);
            rotation = glm::vec3(0.f);
            scale = glm::vec3(1.f);
            transform = glm::mat4(1.f);
        }

        TransformComponent(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
        {
            transform = glm::mat4(1.f);

            this->position = position;
            this->rotation = rotation;
            this->scale = scale;
        }

        TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
        {
            transform = glm::mat4(1.f);

            this->position = position;
            this->rotation = rotation;
            this->scale = scale;
        }
    };

    struct MeshComponent
    {
        bool isEnabled = false;
        Graphics::Mesh mesh;

        MeshComponent() = default;
        MeshComponent(Graphics::Mesh* mesh)
        {
            if (mesh != NULL)
                this->mesh = *mesh;
        }
    };

    struct DirectionalLightComponent
    {
        bool isEnabled = false;
        Graphics::DirectionalLight light;

        DirectionalLightComponent() = default;
        DirectionalLightComponent(const glm::vec3& direction) { light.direction = direction; }
    };

    struct CameraComponent
    {
        bool isEnabled = false;
        bool isPrimary = false;
        Graphics::Camera camera;

        CameraComponent() = default;
        CameraComponent(float fov) { camera.fov = fov; }
    };
}
