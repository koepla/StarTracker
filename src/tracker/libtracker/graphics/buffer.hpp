#ifndef LIBTRACKER_CORE_OPENGL_BUFFER_H
#define LIBTRACKER_CORE_OPENGL_BUFFER_H

#include <libtracker/core/core.hpp>

#include <filesystem>
#include <string>
#include <vector>

namespace graphics {

    enum class ShaderDataType { Int, Int2, Int3, Int4, Float, Float2, Float3, Float4, Mat4 };

    class BufferElement {
    public:
        BufferElement(ShaderDataType dataType, std::string_view name, bool normalized = false) noexcept;

        /**
         * @brief Returns the name of the buffer element
         * @return Name
         */
        std::string_view GetName() const noexcept;

        /**
         * @brief Returns the stride of the buffer element
         * @return Stride
         */
        u32 GetStride() const noexcept;

        /**
         * @brief Returns the primitive count of the buffer element
         * @return Primitive count
         */
        u32 GetPrimitiveCount() const noexcept;

        /**
         * @brief Returns the native OpenGL type of the buffer element
         * @return Native OpenGL type
         */
        u32 GetNativeType() const noexcept;

        /**
         * @brief Returns true if the buffer element is normalized
         * @return Normalized
         */
        bool IsNormalized() const noexcept;

    private:
        std::string name;
        u32 stride;
        u32 primitiveCount;
        u32 nativeType;
        bool normalized;
    };

    class BufferLayout {
    public:
        explicit BufferLayout(const std::vector<BufferElement>& bufferElements) noexcept;

        /**
         * @brief Returns all the elements of the buffer layout
         * @return List of buffer elements
         */
        std::vector<BufferElement> GetElements() const noexcept;

        /**
         * @brief Returns the total stride of all buffer elements
         * @return Total stride
         */
        u32 GetTotalStride() const noexcept;

    private:
        friend class VertexBuffer;
        BufferLayout() noexcept = default;

        std::vector<BufferElement> bufferElements{};
    };

    class VertexBuffer {
    public:
        VertexBuffer() noexcept;
        VertexBuffer(const void* data, u32 size) noexcept;
        ~VertexBuffer() noexcept;

        /**
         * @brief Sets the data of the buffer
         * @param data Pointer to the data
         * @param size Size of the data in bytes
         */
        void SetData(const void* data, u32 size) const noexcept;

        /**
         * @brief Sets the layout of the buffer
         * @param bufferLayout Layout of the buffer
         */
        void SetLayout(const BufferLayout& bufferLayout) noexcept;

        /**
         * @brief Binds the buffer to the currently bound va
         */
        void Bind() const noexcept;

        /**
         * @brief Unbinds any buffer
         */
        void Unbind() const noexcept;

        /**
         * @brief Returns the layout of the buffer
         * @return layout
         */
        const BufferLayout& GetLayout() const noexcept;

    private:
        u32 nativeHandle;
        BufferLayout bufferLayout;
    };

    class IndexBuffer {
    public:
        IndexBuffer() noexcept;
        IndexBuffer(const void* data, u32 count) noexcept;
        ~IndexBuffer() noexcept;

        /**
         * @brief Sets the data of the index buffer
         * @param data Pointer to the data (uint32_t buffer)
         * @param count Index count
         */
        void SetData(const void* data, u32 count) noexcept;

        /**
         * @brief Binds the index buffer to the currently bound va
         */
        void Bind() const noexcept;

        /**
         * @brief Unbinds any index buffer
         */
        void Unbind() const noexcept;

        /**
         * @brief Returns the index count
         * @return Index count
         */
        u32 GetIndexCount() const noexcept;

    private:
        u32 nativeHandle;
        u32 indexCount;
    };

    class VertexArray {
    public:
        VertexArray() noexcept;
        ~VertexArray() noexcept;

        /**
         * @brief Sets the index buffer
         * @param indexBuffer Shared pointer to the index buffer
         */
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) noexcept;

        /**
         * @brief Sets the vertex buffer
         * @param vertexBuffer Shared pointer to the vertex buffer
         */
        void SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) noexcept;

        /**
         * @brief Returns the bound index buffer
         * @return Shared pointer to the index buffer
         */
        std::shared_ptr<IndexBuffer> GetIndexBuffer() const noexcept;

        /**
         * @brief Returns the bound vertex buffer
         * @return Shared pointer to the vertex buffer
         */
        std::shared_ptr<VertexBuffer> GetVertexBuffer() const noexcept;

        /**
         * @brief Binds the vertex array
         */
        void Bind() const noexcept;

        /**
         * @brief Unbinds any vertex array
         */
        void Unbind() const noexcept;

    private:
        u32 nativeHandle;
        std::shared_ptr<IndexBuffer> indexBuffer;
        std::shared_ptr<VertexBuffer> vertexBuffer;
    };

    class FrameBuffer {
    public:
        FrameBuffer(s32 width, s32 height) noexcept;
        ~FrameBuffer() noexcept;

        /**
         * @brief Invalidates the frame buffer (reconstructing)
         */
        void Invalidate() noexcept;

        /**
         * @brief Resizes the frame buffer
         * @param width New width
         * @param height New height
         */
        void Resize(s32 width, s32 height) noexcept;

        /**
         * @brief Binds the frame buffer
         */
        void Bind() const noexcept;

        /**
         * @brief Unbinds any frame buffer
         */
        void Unbind() const noexcept;

        /**
         * @brief Checks if the frame buffer is valid
         * @return True if the frame buffer is valid
         */
        bool IsValid() const noexcept;

        /**
         * @brief Returns the OpenGL handle to the frame buffer
         * @return OpenGL frame buffer handle
         */
        u32 GetNativeHandle() const noexcept;

        /**
         * @brief Returns the OpenGL handle to the color attachment texture
         * @return OpenGL texture handle
         */
        u32 GetNativeTextureHandle() const noexcept;

        /**
         * @brief Returns the OpenGL handle to the render buffer
         * @return OpenGL render buffer handle
         */
        u32 GetNativeRenderHandle() const noexcept;

        /**
         * @brief Returns the width of the frame buffer
         * @return Width
         */
        s32 GetWidth() const noexcept;

        /**
         * @brief Returns the height of the frame buffer
         * @return Height
         */
        s32 GetHeight() const noexcept;

        /**
         * Saves the frame buffer as .png
         * @param file Path of the file to write to
         * @return bool
         */
        bool WriteToFile(const std::filesystem::path& file) const noexcept;

    private:
        s32 width;
        s32 height;
        u32 nativeHandle;
        u32 nativeTextureHandle;
        u32 nativeRenderHandle;
    };

}// namespace graphics

#endif// LIBTRACKER_CORE_OPENGL_BUFFER_H
