#ifndef STARTRACKER_CORE_OPENGL_VERTEXBUFFER_H
#define STARTRACKER_CORE_OPENGL_VERTEXBUFFER_H

#include "BufferLayout.hpp"

namespace StarTracker::Core::OpenGL {

	class VertexBuffer {

	private:
		std::uint32_t nativeHandle;
		BufferLayout bufferLayout;

	public:
		VertexBuffer() noexcept;
		VertexBuffer(const void* data, std::uint32_t size) noexcept;
		~VertexBuffer() noexcept;

		void SetData(const void* data, std::uint32_t size) noexcept;
		void SetLayout(const BufferLayout& bufferLayout) noexcept;

		void Bind() const noexcept;
		void Unbind() const noexcept;

		[[nodiscard]] const BufferLayout& GetLayout() const noexcept;
	};
}

#endif // STARTRACKER_CORE_OPENGL_VERTEXBUFFER_H