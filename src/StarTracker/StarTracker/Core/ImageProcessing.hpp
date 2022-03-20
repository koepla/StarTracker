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
        static inline std::shared_ptr<OpenGL::Shader> stackShader;
        static inline std::shared_ptr<OpenGL::Shader> kernelShader;

    private:
        static void initialize() noexcept;

    public:
        static inline constexpr std::array<float, 9> KernelBlur = { 0.0625f, 0.125f, 0.0625f, 0.125f, 0.5f, 0.125f, 0.0625f, 0.125f, 0.0625f };
        static inline constexpr std::array<float, 9> KernelEdgeDetection = { 1.0f, 1.0f, 1.0f, 1.0f, -8.0f, 1.0f, 1.0f, 1.0f, 1.0f };
        static inline constexpr std::array<float, 9> KernelSharpen = { 0.0f, -1.0f, 0.0f, -1.0f, 5.0f, -1.0f, 0.0f, -1.0f, 0.0f };

    public:
        [[nodiscard]] static bool Stack(const std::shared_ptr<OpenGL::FrameBuffer>& target, const std::vector<std::shared_ptr<OpenGL::Texture>>& textureList) noexcept;
        [[nodiscard]] static bool Kernel(const std::shared_ptr<OpenGL::FrameBuffer>& target, const std::shared_ptr<OpenGL::FrameBuffer>& source, const std::array<float, 9>& kernel) noexcept;
    };
}

#endif // STARTRACKER_CORE_IMAGEPROCESSING_H