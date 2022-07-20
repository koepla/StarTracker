#include "VertexBuffer.hpp"

namespace StarTracker::Core::OpenGL {

    VertexBuffer::VertexBuffer() noexcept : nativeHandle{}, bufferLayout{} {

        glGenBuffers(1, &nativeHandle);
        glBindBuffer(GL_ARRAY_BUFFER, nativeHandle);
    }

    VertexBuffer::VertexBuffer(const void* data, std::uint32_t size) noexcept : nativeHandle{}, bufferLayout{} {

        glGenBuffers(1, &nativeHandle);
        glBindBuffer(GL_ARRAY_BUFFER, nativeHandle);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::~VertexBuffer() noexcept {

        glDeleteBuffers(1, &nativeHandle);
    }

    void VertexBuffer::SetData(const void* data, std::uint32_t size) noexcept {

        glBindBuffer(GL_ARRAY_BUFFER, nativeHandle);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    }

    void VertexBuffer::SetLayout(const BufferLayout& bufferLayout) noexcept {

        this->bufferLayout = bufferLayout;
    }

    const BufferLayout& VertexBuffer::GetLayout() const noexcept {

        return bufferLayout;
    }

    void VertexBuffer::Bind() const noexcept {

        glBindBuffer(GL_ARRAY_BUFFER, nativeHandle);
    }

    void VertexBuffer::Unbind() const noexcept {

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}