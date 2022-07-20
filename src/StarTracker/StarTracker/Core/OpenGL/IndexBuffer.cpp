#include "IndexBuffer.hpp"

namespace StarTracker::Core::OpenGL {

    IndexBuffer::IndexBuffer() noexcept : nativeHandle{}, indexCount{} {

        glGenBuffers(1, &nativeHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nativeHandle);
    }

    IndexBuffer::IndexBuffer(const void* data, std::uint32_t count) noexcept : nativeHandle{}, indexCount{ count } {

        glGenBuffers(1, &nativeHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nativeHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(std::uint32_t), data, GL_DYNAMIC_DRAW);
    }

    IndexBuffer::~IndexBuffer() noexcept {

        glDeleteBuffers(1, &nativeHandle);
    }

    void IndexBuffer::SetData(const void* data, std::uint32_t count) noexcept {

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nativeHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(std::uint32_t), data, GL_DYNAMIC_DRAW);

        this->indexCount = count;
    }

    void IndexBuffer::Bind() const noexcept {

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nativeHandle);
    }

    void IndexBuffer::Unbind() const noexcept {

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    std::uint32_t IndexBuffer::GetIndexCount() const noexcept {

        return indexCount;
    }
}