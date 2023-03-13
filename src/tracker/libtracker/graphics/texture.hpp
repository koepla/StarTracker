#ifndef LIBTRACKER_CORE_OPENGL_TEXTURE_H
#define LIBTRACKER_CORE_OPENGL_TEXTURE_H

#include <libtracker/core/core.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <filesystem>

namespace graphics {

    struct TextureVertex {
        glm::vec3 Position;
        glm::vec3 Color;
        glm::vec2 TextureCoordinates;
    };

    class Texture {
    public:
        Texture() noexcept;
        ~Texture() noexcept;

        /**
         * @brief Loads a texture from the specified path
         * @param filePath Path to the texture
         * @return True if the texture could be loaded
         */
        bool LoadFromFile(const std::filesystem::path& filePath) noexcept;

        /**
         * @brief Binds the texture to a sampler at the specified slot
         * @param slot Sampler slot
         */
        void Bind(u32 slot) const noexcept;

        /**
         * @brief Returns the file path where the texture was loaded from
         * @return Path
         */
        const std::filesystem::path& GetFilePath() const noexcept;

        /**
         * @brief Returns the native handle to the texture (OpenGL TextureID)
         * @return TextureID
         */
        u32 GetNativeHandle() const noexcept;

        /**
         * @brief Returns the width of the texture
         * @return Width of the texture
         */
        s32 GetWidth() const noexcept;

        /**
         * @brief Returns the height of the texture
         * @return Height of the texture
         */
        s32 GetHeight() const noexcept;

        /**
         * @brief Returns the number of color channels
         * @return Color channels
         */
        s32 GetChannels() const noexcept;

    private:
        std::filesystem::path path;
        u32 nativeHandle;
        s32 width;
        s32 height;
        s32 channels;
    };
}// namespace graphics

#endif// LIBTRACKER_CORE_OPENGL_TEXTURE_H