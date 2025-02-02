#pragma once
#include <glm/glm.hpp>

namespace Graphics
{
    struct Camera
    {
        float speed = 8.f;
        float fov = 45.f;
        float lookSensitivity = 0.1f;
        bool isLocked = false;
        glm::vec3 position = glm::vec3(0.f);
        glm::vec3 rotation = glm::vec3(-90.f, 0.f, 0.f);
        glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f);
        glm::mat4 view = glm::mat4(1.f);
    };

    void UpdateCamera(Camera& camera);
    void UpdateCameraController(Camera& camera);
    void LogCameraInfo(Camera& camera);
}
