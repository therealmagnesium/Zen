#include "Graphics/Camera.h"
#include "Core/Input.h"
#include "Core/Log.h"
#include "Core/Time.h"

#include <glm/ext/matrix_transform.hpp>
#include <SDL2/SDL_mouse.h>

namespace Graphics
{

    void UpdateCamera(Camera& camera)
    {
        camera.direction.x = cos(glm::radians(camera.rotation.x)) * cos(glm::radians(camera.rotation.y));
        camera.direction.y = sin(glm::radians(camera.rotation.y));
        camera.direction.z = sin(glm::radians(camera.rotation.x)) * cos(glm::radians(camera.rotation.y));
        camera.direction = glm::normalize(camera.direction);

        camera.view = glm::lookAt(camera.position, camera.position + camera.direction, camera.up);
    }

    void UpdateCameraController(Camera& camera)
    {
        if (!camera.isLocked)
        {
            const glm::vec3 right = glm::normalize(glm::cross(camera.direction, camera.up));

            glm::vec2 walkInput = glm::vec2(0.f);
            walkInput.x = Core::GetInputAxisAlt(Core::INPUT_AXIS_HORIZONTAL);
            walkInput.y = Core::GetInputAxisAlt(Core::INPUT_AXIS_VERTICAL);

            if (glm::length(walkInput) > 0)
                walkInput = glm::normalize(walkInput);

            float flyInput = Core::IsKeyDown(KEY_SPACE) - Core::IsKeyDown(KEY_LEFT_CTRL);
            glm::vec3 movement = right * walkInput.x + camera.up * flyInput + camera.direction * walkInput.y;
            camera.position += movement * camera.speed * Core::Time.delta;

            static glm::vec2 lastMousePosition;
            glm::vec2 mousePosition = Core::GetMousePosition();
            glm::vec2 offset = glm::vec2(0.f);

            if (Core::IsMouseClicked(MOUSE_BUTTON_LEFT))
            {
                lastMousePosition.x = mousePosition.x;
                lastMousePosition.y = mousePosition.y;
            }

            if (Core::IsMouseDown(MOUSE_BUTTON_LEFT))
            {
                offset.x = (mousePosition.x - lastMousePosition.x) * camera.lookSensitivity;
                offset.y = (lastMousePosition.y - mousePosition.y) * camera.lookSensitivity;
            }

            lastMousePosition.x = mousePosition.x;
            lastMousePosition.y = mousePosition.y;

            camera.rotation.x += offset.x;
            camera.rotation.y += offset.y;

            if (camera.rotation.y > 89.f)
                camera.rotation.y = 89.f;
            if (camera.rotation.y < -89.f)
                camera.rotation.y = -89.f;
        }
    }

    void LogCameraInfo(Camera& camera)
    {
        INFO("Camera position   : " V3_FMT, V3_OPEN(camera.position));
        INFO("Camera rotation   : " V3_FMT, V3_OPEN(camera.rotation));
        INFO("Camera direction  : " V3_FMT, V3_OPEN(camera.direction));
        INFO("Camera up vector  : " V3_FMT, V3_OPEN(camera.up));
    }
}
