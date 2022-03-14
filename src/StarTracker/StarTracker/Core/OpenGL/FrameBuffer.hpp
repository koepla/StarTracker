#ifndef STARTRACKER_CORE_OPENGL_FRAMEBUFFER_H
#define STARTRACKER_CORE_OPENGL_FRAMEBUFFER_H

#include <StarTracker/Core/Assert.hpp>

#include <glad/glad.h>

#include <string>

namespace StarTracker::Core::OpenGL {

    class FrameBuffer {

    private:
        std::int32_t width;
        std::int32_t height;
        std::uint32_t nativeHandle;
        std::uint32_t nativeTextureHandle;
        std::uint32_t nativeRenderHandle;

    public:
        FrameBuffer(std::int32_t width, std::int32_t height) noexcept;
        ~FrameBuffer() noexcept;

        void Invalidate() noexcept;
        void Resize(std::int32_t width, std::int32_t height) noexcept;

        void Bind() const noexcept;
        void Unbind() const noexcept;

        [[nodiscard]] bool IsValid() const noexcept;
        [[nodiscard]] std::uint32_t GetNativeHandle() const noexcept;
        [[nodiscard]] std::uint32_t GetNativeTextureHandle() const noexcept;
        [[nodiscard]] std::uint32_t GetNativeRenderHandle() const noexcept;
        [[nodiscard]] std::int32_t GetWidth() const noexcept;
        [[nodiscard]] std::int32_t GetHeight() const noexcept;
    };
}

#endif //STARTRACKER_CORE_OPENGL_FRAMEBUFFER_H
