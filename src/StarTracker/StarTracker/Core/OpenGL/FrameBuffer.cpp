#include "FrameBuffer.hpp"

namespace StarTracker::Core::OpenGL {

	FrameBuffer::FrameBuffer(std::int32_t width, std::int32_t height) noexcept : width{ width }, height{ height }, nativeHandle{}, nativeTextureHandle{}, nativeRenderHandle{} {

		Invalidate();
	}

	FrameBuffer::~FrameBuffer() noexcept {

		glDeleteFramebuffers(1, &nativeHandle);
	}

	void FrameBuffer::Invalidate() noexcept {

		if(nativeHandle) {

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

		if(!IsValid()) {

			ASSERT(false && "Invalid Framebuffer!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Resize(std::int32_t width, std::int32_t height) noexcept {

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

	std::uint32_t FrameBuffer::GetNativeHandle() const noexcept {

		return nativeHandle;
	}

	std::uint32_t FrameBuffer::GetNativeTextureHandle() const noexcept {

		return nativeTextureHandle;
	}

	std::uint32_t FrameBuffer::GetNativeRenderHandle() const noexcept {

		return nativeRenderHandle;
	}

	std::int32_t FrameBuffer::GetWidth() const noexcept {

		return width;
	}

	std::int32_t FrameBuffer::GetHeight() const noexcept {

		return height;
	}
}