#include "VertexArray.hpp"

namespace StarTracker::Core::OpenGL {

	VertexArray::VertexArray() noexcept {

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

		std::uint32_t index{ 0 };
		std::uint32_t offset{ 0 };

		for (const auto& currentElement : bufferLayout.GetElements()) {


			if (currentElement.GetNativeType() == GL_FLOAT) {

				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, currentElement.GetPrimitiveCount(), currentElement.GetNativeType(), currentElement.IsNormalized() ? GL_TRUE : GL_FALSE, stride, reinterpret_cast<const void*>(static_cast<std::intptr_t>(offset)));
			}
			else if (currentElement.GetNativeType() == GL_INT) {

				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index, currentElement.GetPrimitiveCount(), currentElement.GetNativeType(), stride, reinterpret_cast<const void*>(static_cast<std::intptr_t>(offset)));
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
}