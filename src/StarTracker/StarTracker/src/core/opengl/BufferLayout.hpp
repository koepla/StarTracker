#ifndef STARTRACKER_CORE_OPENGL_BUFFERLAYOUT_H
#define STARTRACKER_CORE_OPENGL_BUFFERLAYOUT_H

#include "../Assert.hpp"

#include <glad/glad.h>

#include <string>
#include <vector>

namespace StarTracker::Core::OpenGL {

	enum class ShaderDataType {

		Int, Int2, Int3, Int4,
		Float, Float2, Float3, Float4,
		Mat4
	};

	class BufferElement {

	private:
		std::string name;
		std::uint32_t stride;
		std::uint32_t primitiveCount;
		std::uint32_t nativeType;
		bool normalized;

	private:
		BufferElement() noexcept;

	public:
		BufferElement(ShaderDataType dataType, const std::string& name, bool normalized = false) noexcept;

		[[nodiscard]] std::string_view GetName() const noexcept;
		[[nodiscard]] std::uint32_t GetStride() const noexcept;
		[[nodiscard]] std::uint32_t GetPrimitiveCount() const noexcept;
		[[nodiscard]] std::uint32_t GetNativeType() const noexcept;
		[[nodiscard]] bool IsNormalized() const noexcept;
	};

	class BufferLayout {

	private:
		std::vector<BufferElement> bufferElements;

	private:
		friend class VertexBuffer;

	public:
		explicit BufferLayout(const std::vector<BufferElement>& bufferElements) noexcept;

		[[nodiscard]] std::vector<BufferElement> GetElements() const noexcept;
		[[nodiscard]] std::uint32_t GetTotalStride() const noexcept;
	};
}

#endif // STARTRACKER_CORE_OPENGL_BUFFERLAYOUT_H