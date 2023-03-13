#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace graphics {

    Texture::Texture() noexcept : nativeHandle{}, width{}, height{}, channels{} {
        glCreateTextures(GL_TEXTURE_2D, 1, &nativeHandle);
        glBindTextureUnit(0, nativeHandle);
    }

    Texture::~Texture() noexcept {
        glDeleteTextures(1, &nativeHandle);
    }

    bool Texture::LoadFromFile(const std::filesystem::path& filePath) noexcept {
        stbi_set_flip_vertically_on_load(1);
        u8* data = stbi_load(filePath.string().c_str(), &width, &height, &channels, 4);
        path = filePath;

        if (data) {
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
        return false;
    }

    void Texture::Bind(u32 slot) const noexcept {
        glBindTextureUnit(slot, nativeHandle);
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
}// namespace graphics