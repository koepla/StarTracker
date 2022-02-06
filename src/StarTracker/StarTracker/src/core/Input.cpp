#include "Input.hpp"

namespace StarTracker::Core {

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

    glm::vec2 Input::GetMousePosition() noexcept {

        const auto application = Application::GetInstance();
        auto x = double{ 0.0 };
        auto y = double{ 0.0 };

        glfwGetCursorPos(application->GetWindow().GetNativeHandle(), &x, &y);
        return { static_cast<float>(x), static_cast<float>(y) };
    }
}