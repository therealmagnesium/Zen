#include "Core/Input.h"
#include "Core/KeyCodes.h"

#include <glm/vec2.hpp>

namespace Core
{
    InputState Input;

    bool IsMouseDown(MouseButton button)
    {
        return Input.mouse.buttonsHeld[button];
    }

    bool IsMouseClicked(MouseButton button)
    {
        return Input.mouse.buttonsClicked[button];
    }

    glm::vec2& GetMousePosition()
    {
        return Input.mouse.position;
    }

    bool IsKeyDown(u32 scancode)
    {
        return Input.keyboard.keysHeld[scancode];
    }

    bool IsKeyPressed(u32 scancode)
    {
        return Input.keyboard.keysPressed[scancode];
    }

    float GetInputAxis(InputAxis axis)
    {
        float val = 0.f;
        switch (axis)
        {
            case INPUT_AXIS_HORIZONTAL:
                val = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
                break;
            case INPUT_AXIS_VERTICAL:
                val = IsKeyDown(KEY_UP) - IsKeyDown(KEY_DOWN);
                break;
            default:
                break;
        }

        return val;
    }

    float GetInputAxisAlt(InputAxis axis)
    {
        float val = 0.f;
        switch (axis)
        {
            case INPUT_AXIS_HORIZONTAL:
                val = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
                break;
            case INPUT_AXIS_VERTICAL:
                val = IsKeyDown(KEY_W) - IsKeyDown(KEY_S);
                break;
            default:
                break;
        }

        return val;
    }
}
