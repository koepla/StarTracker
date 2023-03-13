#ifndef LIBTRACKER_CORE_OPENGL_RENDERER_H
#define LIBTRACKER_CORE_OPENGL_RENDERER_H

#include "buffer.hpp"
#include "shader.hpp"

namespace graphics {

    enum class PrimitiveMode : u32 { Point = GL_POINTS, Line = GL_LINE_STRIP, Triangle = GL_TRIANGLES };

    class Renderer {
    public:
        /**
         * @brief Initializes the renderer API
         */
        static void Initialize() noexcept;

        /**
         * @brief Clears the render buffer
         */
        static void Clear() noexcept;

        /**
         * @brief Sets the clear color
         * @param color RGBA color
         */
        static void SetClearColor(const glm::vec4& color) noexcept;

        /**
         * @brief Indexed draw call with the specified va and shader
         * @param vertexArray The vertex array whose vertex buffer and index buffer shall be used
         * @param shader Shader for the draw call
         * @param mode Type of primitive [Point | Line | Triangle]
         */
        static void DrawIndexed(std::shared_ptr<VertexArray> vertexArray,
                                std::shared_ptr<Shader> shader,
                                PrimitiveMode mode) noexcept;
    };
}// namespace graphics

#endif// LIBTRACKER_CORE_OPENGL_RENDERER_H