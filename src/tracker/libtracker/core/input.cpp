#include "input.hpp"
#include "application.hpp"
#include "core.hpp"

bool Input::IsKeyPressed(KeyCode keyCode) noexcept {
    const auto application = Application::GetInstance();
    const auto keyState = glfwGetKey(application->GetWindow().GetNativeHandle(), static_cast<int>(keyCode));
    return keyState == GLFW_PRESS;
}

bool Input::IsMousePressed(MouseCode mouseCode) noexcept {
    const auto application = Application::GetInstance();
    const auto mouseState = glfwGetMouseButton(application->GetWindow().GetNativeHandle(), static_cast<int>(mouseCode));
    return mouseState == GLFW_PRESS;
}

bool Input::IsJoystickPressed(JoystickDirection joystickDirection) noexcept {
    const auto application = Application::GetInstance();

    if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GLFW_TRUE) {
        int axesCount = 0;
        auto directions = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
        if (axesCount >= 4 && directions) {
            switch (joystickDirection) {
                case JoystickDirection::Left: return directions[0] == -1.0f || directions[2] == -1.0f;
                case JoystickDirection::Right: return directions[0] == 1.0f || directions[2] == 1.0f;
                case JoystickDirection::Up: return directions[1] == -1.0f || directions[3] == -1.0f;
                case JoystickDirection::Down: return directions[1] == 1.0f || directions[3] == 1.0f;
            }
            return false;
        }
    }
    return false;
}
