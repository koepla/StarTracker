#ifndef LIBTRACKER_CORE_CELESTIALBODYLIBRARY_H
#define LIBTRACKER_CORE_CELESTIALBODYLIBRARY_H

#include <memory>
#include <unordered_map>

#include <libengine/ephemeris/catalog.hpp>

#include "core.hpp"
#include "graphics/texture.hpp"
#include "utility/types.hpp"

class CatalogManager {
public:
    /**
     * Loads the specified catalog from disk
     * @param ngc Path to the `ngc2000.dat` file
     * @param names Path to the `names.dat` file
     * @param planets Path toe the `planets.json` file
     * @return bool that indicates success
     */
    static bool LoadCatalog(const std::filesystem::path& ngc,
                            const std::filesystem::path& names,
                            const std::filesystem::path& planets) noexcept;

    /**
     * Load textures from the specified directory, used for texture lookup table
     * @param directory Directory where the textures lie in
     * @return bool that indicates success
     */
    static bool LoadTextures(const std::filesystem::path& directory) noexcept;

    /**
     * Fetch a texture from it's identifier
     * @param identifier (designation for FixedBody, name for Planet)
     * @return instance of OpenGL Texture or null
     */
    static std::shared_ptr<graphics::Texture> FetchTexture(std::string_view identifier) noexcept;

    /**
     * Retrieves the catalog
     * @return catalog reference
     */
    static ephemeris::Catalog& GetCatalog() noexcept;

private:
    static inline ephemeris::Catalog catalog{};
    static inline std::unordered_map<std::string, std::shared_ptr<graphics::Texture>> textures;
};

#endif// LIBTRACKER_CORE_CELESTIALBODYLIBRARY_H