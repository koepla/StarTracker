#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace StarTracker::Core::ImageProcessing {

	Image::Image(const std::filesystem::path& filePath) noexcept : format{}, width{}, height{}, filePath{ filePath }, buffer{} {

		stbi_set_flip_vertically_on_load(1);

		std::int32_t channels{};
		uint8_t* data = stbi_load(filePath.string().c_str(), &width, &height, &channels, 0);
	
		if (data && width > 0 && height > 0) {


			if (channels == 4) {

				format = ImageFormat::RGBA;
			}
			else if (channels == 3) {

				format = ImageFormat::RGB;
			}

			buffer.resize(channels * width * height);
			std::memcpy(buffer.data(), data, channels * width * height);

			stbi_image_free(data);
		}
		else {

			ASSERT(false && "Invalid Image!");
		}
	}

	ImageFormat Image::GetFormat() const noexcept {

		return format;
	}

	std::int32_t Image::GetChannels() const noexcept {

		switch (format) {

			case ImageFormat::RGB: {

				return 3;
			}
			case ImageFormat::RGBA: {

				return 4;
			}
			default: {

				ASSERT(false && "Invalid Image Format!");
				return 0;
			}
		}
	}

	std::int32_t Image::GetWidth() const noexcept {

		return width;
	}

	std::int32_t Image::GetHeight() const noexcept {

		return height;
	}

	std::filesystem::path Image::GetFilePath() const noexcept {

		return filePath;
	}

	const std::uint8_t* Image::GetBuffer() const noexcept {

		return buffer.data();
	}
}