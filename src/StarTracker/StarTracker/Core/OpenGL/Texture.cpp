#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace StarTracker::Core::OpenGL {

	Texture::Texture() noexcept : path{}, nativeHandle {}, width{}, height{}, channels{} {
	
		glCreateTextures(GL_TEXTURE_2D, 1, &nativeHandle);
		glBindTextureUnit(0, nativeHandle);
	}

	Texture::~Texture() noexcept {

		glDeleteTextures(1, &nativeHandle);
	}

	bool Texture::LoadFromFile(const std::filesystem::path& filePath) noexcept {

		stbi_set_flip_vertically_on_load(1);
		std::uint8_t* data = stbi_load(filePath.string().c_str(), &width, &height, &channels, 4);
		path = filePath;

		if(data) {

			glTextureStorage2D(nativeHandle, 1, GL_RGBA8, width, height);
			glTextureParameteri(nativeHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(nativeHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(nativeHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(nativeHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTextureSubImage2D(nativeHandle, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
			return true;
		}
		else {

			return false;
		}
	}

	void Texture::Bind(std::uint32_t slot) const noexcept {

		glBindTextureUnit(slot, nativeHandle);
	}

	const std::filesystem::path& Texture::GetFilePath() const noexcept {

		return path;
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