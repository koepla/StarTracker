#ifndef STARTRACKER_CORE_OPENGL_INDEXBUFFER_H
#define STARTRACKER_CORE_OPENGL_INDEXBUFFER_H

#include <glad/glad.h>

#include <cstdint>

namespace StarTracker::Core::OpenGL {

	class IndexBuffer {

	private:
		std::uint32_t nativeHandle;
		std::uint32_t indexCount;

	public:
		IndexBuffer() noexcept;
		IndexBuffer(const void* data, std::uint32_t size) noexcept;
		~IndexBuffer() noexcept;

		void SetData(const void* data, std::uint32_t count) noexcept;

		void Bind() const noexcept;
		void Unbind() const noexcept;

		[[nodiscard]] std::uint32_t GetIndexCount() const noexcept;
	};
}

#endif // STARTRACKER_CORE_OPENGL_INDEXBUFFER_H