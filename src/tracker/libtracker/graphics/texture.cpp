#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace graphics {

    Texture::Texture() noexcept : nativeHandle{}, width{}, height{}, channels{} {
        glGenTextures(1, &nativeHandle);
        glBindTexture(GL_TEXTURE_2D, nativeHandle);
    }

    Texture::~Texture() noexcept {
        glDeleteTextures(1, &nativeHandle);
    }

    bool Texture::LoadFromFile(const std::filesystem::path& filePath) noexcept {
        stbi_set_flip_vertically_on_load(1);
        u8* data = stbi_load(filePath.string().c_str(), &width, &height, &channels, 4);
        path = filePath;

        if (data) {
            glBindTexture(GL_TEXTURE_2D, nativeHandle);

            // Allocate and upload texture data
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            // Texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // Generate mipmaps
            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(data);
            return true;
        }
        return false;
    }

    void Texture::Bind(u32 slot) const noexcept {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, nativeHandle);
    }

    const std::filesystem::path& Texture::GetFilePath() const noexcept {
        return path;
    }

    u32 Texture::GetNativeHandle() const noexcept {
        return nativeHandle;
    }

    s32 Texture::GetWidth() const noexcept {
        return width;
    }

    s32 Texture::GetHeight() const noexcept {
        return height;
    }

    s32 Texture::GetChannels() const noexcept {
        return channels;
    }

} // namespace graphics