#ifndef STARTRACKER_CORE_ASSETDATABASE_H
#define STARTRACKER_CORE_ASSETDATABASE_H

#include <StarEngine/Ephemeris/CelestialBody.hpp>
#include <StarTracker/Core/OpenGL/Shader.hpp>
#include <StarTracker/Core/OpenGL/Texture.hpp>
#include <StarTracker/Core/OpenGL/Model.hpp>

namespace StarTracker::Core {

    class AssetDataBase {

    private:
        static inline std::filesystem::path assetPath{ std::filesystem::current_path() / "Assets" };
        static inline std::unordered_map<std::string, std::shared_ptr<OpenGL::Shader>> shaderMap{};
        static inline std::unordered_map<std::string, std::shared_ptr<OpenGL::Texture>> textureMap{};
        static inline std::unordered_map<std::string, std::shared_ptr<OpenGL::Model>> modelMap{};
        static inline std::unordered_map<std::string, std::vector<std::shared_ptr<Ephemeris::CelestialBody>>> celestialBodyMap{};

    public:
        [[nodiscard]] static const std::shared_ptr<OpenGL::Shader>& LoadShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath, bool reload = false) noexcept;
        [[nodiscard]] static const std::shared_ptr<OpenGL::Texture>& LoadTexture(const std::filesystem::path& filePath, bool reload = false) noexcept;
        [[nodiscard]] static const std::shared_ptr<OpenGL::Model>& LoadModel(const std::filesystem::path& filePath, const glm::vec3& color, bool reload = false) noexcept;
        [[nodiscard]] static const std::shared_ptr<OpenGL::Model>& LoadModel(const std::filesystem::path& filePath, const std::filesystem::path& texturePath, bool reload = false) noexcept;
        [[nodiscard]] static const std::vector<std::shared_ptr<Ephemeris::CelestialBody>>& LoadCelestialBodies(const std::filesystem::path& filePath, bool reload = false) noexcept;
    };
}

#endif // STARTRACKER_CORE_ASSETDATABASE_H