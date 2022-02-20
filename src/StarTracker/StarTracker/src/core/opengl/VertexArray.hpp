#ifndef STARTRACKER_CORE_OPENGL_VERTEXARRAY_H
#define STARTRACKER_CORE_OPENGL_VERTEXARRAY_H

#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"

#include <memory>

namespace StarTracker::Core::OpenGL {

	class VertexArray {

	private:
		std::uint32_t nativeHandle;
		std::shared_ptr<IndexBuffer> indexBuffer;
		std::shared_ptr<VertexBuffer> vertexBuffer;

	public:
		VertexArray() noexcept;
		~VertexArray() noexcept;

		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) noexcept;
		void SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) noexcept;
	
		void Bind() const noexcept;
		void Unbind() const noexcept;
	};
}

#endif // STARTRACKER_CORE_OPENGL_VERTEXARRAY_H
