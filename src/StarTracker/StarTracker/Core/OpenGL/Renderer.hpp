#ifndef STARTRACKER_CORE_OPENGL_RENDERER_H
#define STARTRACKER_CORE_OPENGL_RENDERER_H

#include <StarTracker/Core/OpenGL/Model.hpp>
#include <StarTracker/Core/OpenGL/Shader.hpp>
#include <StarTracker/Core/OpenGL/Texture.hpp>
#include <StarTracker/Core/OpenGL/VertexArray.hpp>

namespace StarTracker::Core::OpenGL {

    enum class PrimitiveMode : std::uint32_t {

        Point = GL_POINTS,
        Line = GL_LINE_STRIP,
        Triangle = GL_TRIANGLES
    };

    class Renderer {

    public:
        static void Initialize() noexcept;
        static void Clear() noexcept;
        static void SetClearColor(const glm::vec4& color) noexcept;
        static void DrawIndexed(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<Shader> shader, PrimitiveMode mode) noexcept;
        static void DrawModel(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader) noexcept;
    };
}

#endif // STARTRACKER_CORE_OPENGL_RENDERER_H