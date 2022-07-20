#ifndef STARTRACKER_CORE_ASSETDATABASE_H
#define STARTRACKER_CORE_ASSETDATABASE_H

#include <StarEngine/Ephemeris/CelestialBody.hpp>
#include <StarTracker/Core/BodyLibrary.hpp>
#include <StarTracker/Core/OpenGL/Shader.hpp>
#include <StarTracker/Core/OpenGL/Texture.hpp>
#include <StarTracker/Core/OpenGL/Model.hpp>

namespace StarTracker::Core {

    class AssetDataBase {

    private:
        static inline std::filesystem::path assetPath{ std::filesystem::current_path() / "Assets" };
        static inline std::unordered_map<std::string, std::shared_ptr<OpenGL::Shader>> shaderCache{};
        static inline std::unordered_map<std::string, std::shared_ptr<OpenGL::Texture>> textureCache{};
        static inline std::unordered_map<std::string, std::shared_ptr<OpenGL::Model>> modelCache{};
        static inline std::unordered_map<std::string, std::shared_ptr<BodyLibrary>> bodyLibraryCache{};

    public:
        /**
        * @brief Loads the specified shader
        *
        * @param vertexPath path to the vertex-shader
        *
        * @param fragmentPath path to the fragment-shader
        *
        * @param reload if true, the cache is not used and the shader is being reloaded from disk
        *
        * @return std::shared_ptr to the shader instance
        *
        */
        [[nodiscard]] static std::shared_ptr<OpenGL::Shader> LoadShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath, bool reload = false) noexcept;

        /**
        * @brief Loads the specified texture
        *
        * @param filePath path to the texture
        *
        * @param reload if true, the cache is not used and the texture is being reloaded from disk
        *
        * @return std::shared_ptr to the texture instance
        *
        */
        [[nodiscard]] static std::shared_ptr<OpenGL::Texture> LoadTexture(const std::filesystem::path& filePath, bool reload = false) noexcept;

        /**
        * @brief Loads the specified model
        *
        * @param filePath path to the .obj file of the model
        *
        * @param reload if true, the cache is not used and the model is being reloaded from disk
        *
        * @return std::shared_ptr to the model instance
        *
        */
        [[nodiscard]] static std::shared_ptr<OpenGL::Model> LoadModel(const std::filesystem::path& filePath, bool reload = false) noexcept;

        /**
        * @brief Loads the specified model
        *
        * @param filePath path to the .obj file of the model
        *
        * @param texturePath path to the texture for the model
        *
        * @param reload if true, the cache is not used and the model is being reloaded from disk
        *
        * @return std::shared_ptr to the model instance
        *
        */
        [[nodiscard]] static std::shared_ptr<OpenGL::Model> LoadModel(const std::filesystem::path& filePath, const std::filesystem::path& texturePath, bool reload = false) noexcept;

        /**
        * @brief Loads the specified CelestialBody list
        *
        * @param filePath path to the .json file for the CelestialBody list
        *
        * @param reload if true, the cache is not used and the list is being reloaded from disk
        *
        * @return std::vector of std::shared_ptr to the CelestialBody instances
        *
        */
        [[nodiscard]] static std::shared_ptr<BodyLibrary> LoadBodyLibrary(const std::filesystem::path& filePath, bool reload = false) noexcept;
    };
}

#endif // STARTRACKER_CORE_ASSETDATABASE_H