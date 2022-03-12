#ifndef STARTRACKER_CORE_ASSETDATABASE_H
#define STARTRACKER_CORE_ASSETDATABASE_H

#include <StarEngine/Ephemeris/CelestialBody.hpp>
#include <StarTracker/Core/OpenGL/Shader.hpp>
#include <StarTracker/Core/OpenGL/Texture.hpp>

namespace StarTracker::Core {

    class AssetDataBase {

    private:
        static inline std::filesystem::path assetPath{ std::filesystem::current_path() / "Assets" };
        static inline std::unordered_map<std::string, std::shared_ptr<OpenGL::Shader>> shaderMap{};
        static inline std::unordered_map<std::string, std::shared_ptr<OpenGL::Texture>> textureMap{};

    public:
        [[nodiscard]] static const std::shared_ptr<OpenGL::Shader>& LoadShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) noexcept;
        [[nodiscard]] static const std::shared_ptr<OpenGL::Texture>& LoadTexture(const std::filesystem::path& filePath) noexcept;
    };
}

#endif // STARTRACKER_CORE_ASSETDATABASE_H