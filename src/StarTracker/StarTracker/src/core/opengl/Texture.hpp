#ifndef STARTRACKER_CORE_OPENGL_TEXTURE_H
#define STARTRACKER_CORE_OPENGL_TEXTURE_H

#include "../Assert.hpp"

#include <glad/glad.h>

#include <filesystem>

namespace StarTracker::Core::OpenGL {

	class Texture {

	private:
		std::uint32_t nativeHandle;
		std::int32_t width;
		std::int32_t height;
		std::int32_t channels;

	public:
		Texture() noexcept;
		~Texture() noexcept;

		void LoadFromFile(const std::filesystem::path& filePath) noexcept;

		void Bind(std::uint32_t slot) const noexcept;
		
		[[nodiscard]] std::uint32_t GetNativeHandle() const noexcept;
		[[nodiscard]] std::int32_t GetWidth() const noexcept;
		[[nodiscard]] std::int32_t GetHeight() const noexcept;
		[[nodiscard]] std::int32_t GetChannels() const noexcept;
	};
}

#endif // STARTRACKER_CORE_OPENGL_TEXTURE_H