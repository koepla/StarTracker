#ifndef STARTRACKER_CORE_OPENGL_CAMERA_H
#define STARTRACKER_CORE_OPENGL_CAMERA_H

#include <StarTracker/Core/Application.hpp>
#include <StarTracker/Core/Input.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace StarTracker::Core::OpenGL {

    class Camera {

    private:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;

        float pitch;
        float yaw;
        float fov;

    public:
        explicit Camera(const glm::vec3& position) noexcept;

        [[nodiscard]] glm::vec3 GetPosition() const noexcept;
        [[nodiscard]] glm::mat4 GetProjectionMatrix(const glm::vec2& viewportSize) const noexcept;
        [[nodiscard]] glm::mat4 GetViewMatrix(float deltaTime, bool blockMovement = false) noexcept;
    };
}

#endif // STARTRACKER_CORE_OPENGL_CAMERA_H