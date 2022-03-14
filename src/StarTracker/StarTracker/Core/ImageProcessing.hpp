#ifndef STARTRACKER_CORE_IMAGEPROCESSING_H
#define STARTRACKER_CORE_IMAGEPROCESSING_H

#include <StarTracker/Core/AssetDataBase.hpp>
#include <StarTracker/Core/OpenGL/FrameBuffer.hpp>
#include <StarTracker/Core/OpenGL/Shader.hpp>
#include <StarTracker/Core/OpenGL/Texture.hpp>
#include <StarTracker/Core/OpenGL/VertexArray.hpp>
#include <StarTracker/Core/OpenGL/Renderer.hpp>

namespace StarTracker::Core {

    class ImageProcessing {

    private:
        static inline std::shared_ptr<OpenGL::VertexArray> vertexArray;
        static inline std::shared_ptr<OpenGL::VertexBuffer> vertexBuffer;
        static inline std::shared_ptr<OpenGL::IndexBuffer> indexBuffer;
        static inline std::shared_ptr<OpenGL::FrameBuffer> stackFrameBuffer;
        static inline std::shared_ptr<OpenGL::Shader> stackShader;

    public:
        [[nodiscard]] static const std::shared_ptr<OpenGL::FrameBuffer>& Stack(const std::vector<std::shared_ptr<OpenGL::Texture>>& textureList) noexcept;
        [[nodiscard]] static const std::shared_ptr<OpenGL::FrameBuffer>& GetStackFrameBuffer() noexcept;
    };
}

#endif // STARTRACKER_CORE_IMAGEPROCESSING_H