#include "Camera.hpp"

namespace StarTracker::Core::OpenGL {


    Camera::Camera(const glm::vec3 &position) noexcept : position{ position }, front{ 0.0f, 0.0f, -1.0f }, up{ 0.0f, 1.0f, 0.0f }, right{ 1.0f, 0.0f, 0.0f }, pitch{ 0.0f }, yaw{ -90.0f }, fov{ 45.0f } {

        const auto application = Application::GetInstance();
        application->RegisterEventHandler([this](const Events::Event& event) -> void {

            const auto mouseScrollEvent = dynamic_cast<const Events::MouseScrollEvent*>(&event);

            if(mouseScrollEvent) {

                fov = glm::clamp(fov - static_cast<float>(mouseScrollEvent->GetDeltaY()), 1.0f, 45.0f);
            }
        });
    }

    glm::mat4 Camera::GetViewMatrix(float deltaTime) noexcept {

        const auto speed = 2.5f * deltaTime;
        if(Input::IsKeyPressed(KeyCode::W)) {

            position += front * speed;
        }
        if(Input::IsKeyPressed(KeyCode::S)) {

            position -= front * speed;
        }
        if(Input::IsKeyPressed(KeyCode::A)) {

            position -= right * speed;
        }
        if(Input::IsKeyPressed(KeyCode::D)) {

            position += right * speed;
        }

        static glm::vec2 lastMousePosition{};
        const auto currentMousePosition = Input::GetMousePosition();

        constexpr auto sensitivity = 0.2f;
        const auto dx = (lastMousePosition.x - currentMousePosition.x) * sensitivity;
        const auto dy = (lastMousePosition.y - currentMousePosition.y) * sensitivity;
        lastMousePosition = currentMousePosition;

        if(Input::IsMousePressed(MouseCode::ButtonRight)) {

            pitch = glm::clamp(pitch + dy, -89.0f, 89.0f);
            yaw -= dx;
        }

        glm::vec3 nonNormalizedFront{};
        nonNormalizedFront.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        nonNormalizedFront.y = std::sin(glm::radians(pitch));
        nonNormalizedFront.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        front = glm::normalize(nonNormalizedFront);

        right = glm::normalize(glm::cross(front, { 0.0f, 1.0f, 0.0f }));
        up = glm::normalize(glm::cross(right, front));

        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 Camera::GetProjectionMatrix() const noexcept {

        const auto application = Application::GetInstance();
        const auto windowWidth = static_cast<float>(application->GetWindow().GetWidth());
        const auto windowHeight = static_cast<float>(application->GetWindow().GetHeight());

        return glm::perspective(glm::radians(fov), windowWidth / windowHeight, 0.1f, 100.0f);
    }
}