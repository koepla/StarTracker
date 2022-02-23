#ifndef STARTRACKER_CORE_IMAGEPROCESSING_IMAGE_H
#define STARTRACKER_CORE_IMAGEPROCESSING_IMAGE_H

#include "../Assert.hpp"

#include <glad/glad.h>

#include <filesystem>

namespace StarTracker::Core::ImageProcessing {

	enum class ImageFormat : std::uint32_t {
		
		RGB = GL_RGB,
		RGBA = GL_RGBA,
	};

	class Image {

	private:
		ImageFormat format;
		std::int32_t width;
		std::int32_t height;
		std::filesystem::path filePath;
		std::vector<std::uint8_t> buffer;
		
	public:
		explicit Image(const std::filesystem::path& filePath) noexcept;

		[[nodiscard]] ImageFormat GetFormat() const noexcept;
		[[nodiscard]] std::int32_t GetChannels() const noexcept;
		[[nodiscard]] std::int32_t GetWidth() const noexcept;
		[[nodiscard]] std::int32_t GetHeight() const noexcept;
		[[nodiscard]] std::filesystem::path GetFilePath() const noexcept;
		[[nodiscard]] const std::uint8_t* GetBuffer() const noexcept;
	};
}

#endif // STARTRACKER_CORE_IMAGEPROCESSING_IMAGE_H
