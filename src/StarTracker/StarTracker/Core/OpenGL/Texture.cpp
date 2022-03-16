#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace StarTracker::Core::OpenGL {

	Texture::Texture() noexcept : nativeHandle{}, width{}, height{}, channels{} {
	
		glGenTextures(1, &nativeHandle);
		glBindTexture(GL_TEXTURE_2D, nativeHandle);
	}

	Texture::~Texture() noexcept {

		glDeleteTextures(1, &nativeHandle);
	}

	void Texture::LoadFromFile(const std::filesystem::path& filePath) noexcept {

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(true);
		std::uint8_t* data = stbi_load(filePath.string().c_str(), &width, &height, &channels, 0);

		const auto format = [&]() -> std::uint32_t {

            if(filePath.string().ends_with("blue.png")) {

                return GL_RED;
            }
			if (channels == 4) {

				return GL_RGBA;
			}
			if (channels == 3) {

				return GL_RGB;
			}
			else {

				ASSERT(false && "Invalid Format!");
				return 0;
			}
		}();

		if (data) {

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
		}
		else {

			ASSERT(false && "Invalid Image!");
		}
	}

	void Texture::Bind(std::uint32_t slot) const noexcept {

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, nativeHandle);
	}

	std::uint32_t Texture::GetNativeHandle() const noexcept {

		return nativeHandle;
	}

	std::int32_t Texture::GetWidth() const noexcept {

		return width;
	}

	std::int32_t Texture::GetHeight() const noexcept {

		return height;
	}

	std::int32_t Texture::GetChannels() const noexcept {

		return channels;
	}
}