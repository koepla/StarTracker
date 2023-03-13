#include "buffer.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <libtracker/core/core.hpp>
#include <libtracker/core/window.hpp>

namespace graphics {

    BufferElement::BufferElement(ShaderDataType dataType, std::string_view name, bool normalized) noexcept
        : name{ name },
          stride{},
          primitiveCount{},
          nativeType{},
          normalized{ normalized } {
        switch (dataType) {
            case ShaderDataType::Int: {
                nativeType = GL_INT;
                primitiveCount = 1;
                stride = primitiveCount * sizeof(s32);
                break;
            }
            case ShaderDataType::Int2: {
                nativeType = GL_INT;
                primitiveCount = 2;
                stride = primitiveCount * sizeof(s32);
                break;
            }
            case ShaderDataType::Int3: {
                nativeType = GL_INT;
                primitiveCount = 3;
                stride = primitiveCount * sizeof(s32);
                break;
            }
            case ShaderDataType::Int4: {
                nativeType = GL_INT;
                primitiveCount = 4;
                stride = primitiveCount * sizeof(s32);
                break;
            }
            case ShaderDataType::Float: {
                nativeType = GL_FLOAT;
                primitiveCount = 1;
                stride = primitiveCount * sizeof(s32);
                break;
            }
            case ShaderDataType::Float2: {
                nativeType = GL_FLOAT;
                primitiveCount = 2;
                stride = primitiveCount * sizeof(s32);
                break;
            }
            case ShaderDataType::Float3: {
                nativeType = GL_FLOAT;
                primitiveCount = 3;
                stride = primitiveCount * sizeof(s32);
                break;
            }
            case ShaderDataType::Float4: {
                nativeType = GL_FLOAT;
                primitiveCount = 4;
                stride = primitiveCount * sizeof(s32);
                break;
            }
            case ShaderDataType::Mat4: {
                nativeType = GL_FLOAT;
                primitiveCount = 16;
                stride = primitiveCount * sizeof(s32);
                break;
            }
        }
    }

    std::string_view BufferElement::GetName() const noexcept {
        return name;
    }

    u32 BufferElement::GetStride() const noexcept {
        return stride;
    }

    u32 BufferElement::GetPrimitiveCount() const noexcept {
        return primitiveCount;
    }

    u32 BufferElement::GetNativeType() const noexcept {
        return nativeType;
    }

    bool BufferElement::IsNormalized() const noexcept {
        return normalized;
    }

    BufferLayout::BufferLayout(const std::vector<BufferElement>& bufferElements) noexcept
        : bufferElements{ bufferElements } { }

    std::vector<BufferElement> BufferLayout::GetElements() const noexcept {
        return bufferElements;
    }

    u32 BufferLayout::GetTotalStride() const noexcept {
        u32 totalStride{};
        for (const auto& currentElement : bufferElements) {
            totalStride += currentElement.GetStride();
        }
        return totalStride;
    }

    VertexBuffer::VertexBuffer() noexcept : nativeHandle{} {
        glGenBuffers(1, &nativeHandle);
        glBindBuffer(GL_ARRAY_BUFFER, nativeHandle);
    }

    VertexBuffer::VertexBuffer(const void* data, u32 size) noexcept : nativeHandle{} {
        glGenBuffers(1, &nativeHandle);
        glBindBuffer(GL_ARRAY_BUFFER, nativeHandle);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::~VertexBuffer() noexcept {
        glDeleteBuffers(1, &nativeHandle);
    }

    void VertexBuffer::SetData(const void* data, u32 size) const noexcept {
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

    IndexBuffer::IndexBuffer() noexcept : nativeHandle{}, indexCount{} {
        glGenBuffers(1, &nativeHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nativeHandle);
    }

    IndexBuffer::IndexBuffer(const void* data, u32 count) noexcept : nativeHandle{}, indexCount{ count } {
        glGenBuffers(1, &nativeHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nativeHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(uint32_t)), data, GL_DYNAMIC_DRAW);
    }

    IndexBuffer::~IndexBuffer() noexcept {
        glDeleteBuffers(1, &nativeHandle);
    }

    void IndexBuffer::SetData(const void* data, u32 count) noexcept {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nativeHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(u32)), data, GL_DYNAMIC_DRAW);
        this->indexCount = count;
    }

    void IndexBuffer::Bind() const noexcept {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nativeHandle);
    }

    void IndexBuffer::Unbind() const noexcept {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    u32 IndexBuffer::GetIndexCount() const noexcept {
        return indexCount;
    }

    VertexArray::VertexArray() noexcept : nativeHandle{ 0 } {
        glGenVertexArrays(1, &nativeHandle);
        glBindVertexArray(nativeHandle);
    }

    VertexArray::~VertexArray() noexcept {
        glDeleteVertexArrays(1, &nativeHandle);
    }

    void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) noexcept {
        glBindVertexArray(nativeHandle);
        indexBuffer->Bind();
        this->indexBuffer = indexBuffer;
    }

    void VertexArray::SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) noexcept {
        glBindVertexArray(nativeHandle);
        vertexBuffer->Bind();

        const auto& bufferLayout = vertexBuffer->GetLayout();
        const auto stride = bufferLayout.GetTotalStride();

        u32 index{ 0 };
        u32 offset{ 0 };

        for (const auto& currentElement : bufferLayout.GetElements()) {
            if (currentElement.GetNativeType() == GL_FLOAT) {
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(index, static_cast<GLint>(currentElement.GetPrimitiveCount()),
                                      currentElement.GetNativeType(),
                                      currentElement.IsNormalized() ? GL_TRUE : GL_FALSE, static_cast<GLint>(stride),
                                      reinterpret_cast<const void*>(static_cast<std::intptr_t>(offset)));
            } else if (currentElement.GetNativeType() == GL_INT) {
                glEnableVertexAttribArray(index);
                glVertexAttribIPointer(index, static_cast<GLint>(currentElement.GetPrimitiveCount()),
                                       currentElement.GetNativeType(), static_cast<GLint>(stride),
                                       reinterpret_cast<const void*>(static_cast<std::intptr_t>(offset)));
            }
            offset += currentElement.GetStride();
            index++;
        }
        this->vertexBuffer = vertexBuffer;
    }

    void VertexArray::Bind() const noexcept {
        glBindVertexArray(nativeHandle);
    }

    void VertexArray::Unbind() const noexcept {
        glBindVertexArray(0);
    }

    std::shared_ptr<IndexBuffer> VertexArray::GetIndexBuffer() const noexcept {
        return indexBuffer;
    }

    std::shared_ptr<VertexBuffer> VertexArray::GetVertexBuffer() const noexcept {
        return vertexBuffer;
    }

    FrameBuffer::FrameBuffer(s32 width, s32 height) noexcept
        : width{ width },
          height{ height },
          nativeHandle{},
          nativeTextureHandle{},
          nativeRenderHandle{} {
        Invalidate();
    }

    FrameBuffer::~FrameBuffer() noexcept {
        glDeleteFramebuffers(1, &nativeHandle);
    }

    void FrameBuffer::Invalidate() noexcept {
        if (nativeHandle) {
            glDeleteFramebuffers(1, &nativeHandle);
            glDeleteTextures(1, &nativeTextureHandle);
            glDeleteRenderbuffers(1, &nativeRenderHandle);
        }

        glGenFramebuffers(1, &nativeHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, nativeHandle);

        glGenTextures(1, &nativeTextureHandle);
        glBindTexture(GL_TEXTURE_2D, nativeTextureHandle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, nativeTextureHandle, 0);

        glGenRenderbuffers(1, &nativeRenderHandle);
        glBindRenderbuffer(GL_RENDERBUFFER, nativeRenderHandle);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, nativeRenderHandle);

        if (!IsValid()) {
            LIBTRACKER_ASSERT(false, "Invalid Framebuffer!");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Resize(s32 width, s32 height) noexcept {
        if (width <= 0 || height <= 0) {
            return;
        }

        this->width = width;
        this->height = height;

        Invalidate();
    }

    void FrameBuffer::Bind() const noexcept {
        glBindFramebuffer(GL_FRAMEBUFFER, nativeHandle);
        glViewport(0, 0, width, height);
    }

    void FrameBuffer::Unbind() const noexcept {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    bool FrameBuffer::IsValid() const noexcept {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

    u32 FrameBuffer::GetNativeHandle() const noexcept {
        return nativeHandle;
    }

    u32 FrameBuffer::GetNativeTextureHandle() const noexcept {
        return nativeTextureHandle;
    }

    u32 FrameBuffer::GetNativeRenderHandle() const noexcept {
        return nativeRenderHandle;
    }

    s32 FrameBuffer::GetWidth() const noexcept {
        return width;
    }

    s32 FrameBuffer::GetHeight() const noexcept {
        return height;
    }

    bool FrameBuffer::WriteToFile(const std::filesystem::path& file) const noexcept {
        utility::OwningBuffer buffer(4 * width * height);

        Bind();
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.Memory<void*>());
        Unbind();

        stbi_flip_vertically_on_write(1);
        return stbi_write_png(file.string().c_str(), width, height, 0, buffer.Memory<void*>(), width * 4);
    }
}// namespace graphics