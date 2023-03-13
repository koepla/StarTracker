#ifndef LIBTRACKER_CORE_ASSETDATABASE_H
#define LIBTRACKER_CORE_ASSETDATABASE_H

#include "core/catalog-manager.hpp"
#include "core/settings.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "utility/types.hpp"

class AssetDatabase {
public:
    /**
     * @brief Loads the specified shader
     * @param vertexPath path to the vertex-shader
     * @param fragmentPath path to the fragment-shader
     * @param reload if true, the cache is not used and the shader is being reloaded from disk
     * @return std::shared_ptr to the shader instance
     */
    static std::shared_ptr<graphics::Shader> LoadShader(const std::filesystem::path& vertexPath,
                                                        const std::filesystem::path& fragmentPath,
                                                        bool reload = false) noexcept;

    /**
     * @brief Loads the specified texture
     * @param filePath path to the texture
     * @param reload if true, the cache is not used and the texture is being reloaded from disk
     * @return std::shared_ptr to the texture instance
     */
    static std::shared_ptr<graphics::Texture> LoadTexture(const std::filesystem::path& filePath,
                                                          bool reload = false) noexcept;

    /**
     * Loads the catalog manager
     * @param ngc Name of the `ngc2000.dat` file
     * @param names Name of the `names.dat` file
     * @param planets Name of the `planets.json` file
     */
    static void LoadCatalogManager(const std::filesystem::path& ngc,
                                   const std::filesystem::path& names,
                                   const std::filesystem::path& planets) noexcept;

    /**
     * Loads the specified icon
     * @param fileName Name of the icon file
     * @return GLFWimage instance
     */
    static GLFWimage LoadIcon(const std::filesystem::path& fileName) noexcept;

    /**
     * @brief Loads the specified settings file, note that the current settings are overwritten
     * @param fileName relative path to the .json file
     */
    static void LoadSettings(const std::filesystem::path& fileName) noexcept;

    /**
     * @brief Saves the current settings file
     * @param fileName Name for the file
     */
    static void SaveSettings(const std::filesystem::path& fileName) noexcept;

private:
    static inline std::unordered_map<std::string, std::shared_ptr<graphics::Shader>> shaderCache{};
    static inline std::unordered_map<std::string, std::shared_ptr<graphics::Texture>> textureCache{};
};

#endif// LIBTRACKER_CORE_ASSETDATABASE_H