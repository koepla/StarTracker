#include "BufferLayout.hpp"

namespace StarTracker::Core::OpenGL {

    BufferElement::BufferElement(ShaderDataType dataType, std::string_view name, bool normalized) noexcept : name{ name }, stride{}, primitiveCount{}, nativeType{}, normalized{ normalized } {

        switch (dataType) {

        case ShaderDataType::Int: {

            nativeType = GL_INT;
            primitiveCount = 1;
            stride = primitiveCount * sizeof(std::int32_t);

            break;
        }
        case ShaderDataType::Int2: {

            nativeType = GL_INT;
            primitiveCount = 2;
            stride = primitiveCount * sizeof(std::int32_t);

            break;
        }
        case ShaderDataType::Int3: {

            nativeType = GL_INT;
            primitiveCount = 3;
            stride = primitiveCount * sizeof(std::int32_t);

            break;
        }
        case ShaderDataType::Int4: {

            nativeType = GL_INT;
            primitiveCount = 4;
            stride = primitiveCount * sizeof(std::int32_t);

            break;
        }
        case ShaderDataType::Float: {

            nativeType = GL_FLOAT;
            primitiveCount = 1;
            stride = primitiveCount * sizeof(std::int32_t);

            break;
        }
        case ShaderDataType::Float2: {

            nativeType = GL_FLOAT;
            primitiveCount = 2;
            stride = primitiveCount * sizeof(std::int32_t);

            break;
        }
        case ShaderDataType::Float3: {

            nativeType = GL_FLOAT;
            primitiveCount = 3;
            stride = primitiveCount * sizeof(std::int32_t);

            break;
        }
        case ShaderDataType::Float4: {

            nativeType = GL_FLOAT;
            primitiveCount = 4;
            stride = primitiveCount * sizeof(std::int32_t);

            break;
        }
        case ShaderDataType::Mat4: {

            nativeType = GL_FLOAT;
            primitiveCount = 16;
            stride = primitiveCount * sizeof(std::int32_t);

            break;
        }
        default: {

            STARTRACKER_ERROR("Unkown ShaderDataType {} for {}", static_cast<int>(dataType), name);
            ASSERT(false && "Unknown ShaderDataType!");
            break;
        }
        }
    }

    std::string_view BufferElement::GetName() const noexcept {

        return name;
    }

    std::uint32_t BufferElement::GetStride() const noexcept {

        return stride;
    }

    std::uint32_t BufferElement::GetPrimitiveCount() const noexcept {

        return primitiveCount;
    }

    std::uint32_t BufferElement::GetNativeType() const noexcept {

        return nativeType;
    }

    bool BufferElement::IsNormalized() const noexcept {

        return normalized;
    }

    BufferLayout::BufferLayout(const std::vector<BufferElement>& bufferElements) noexcept : bufferElements{ bufferElements } {

    }

    std::vector<BufferElement> BufferLayout::GetElements() const noexcept {

        return bufferElements;
    }

    std::uint32_t BufferLayout::GetTotalStride() const noexcept {

        std::uint32_t totalStride{};

        for (const auto& currentElement : bufferElements) {

            totalStride += currentElement.GetStride();
        }

        return totalStride;
    }
}