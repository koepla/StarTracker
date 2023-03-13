#include "catalog-manager.hpp"
#include "arch/file.hpp"

bool CatalogManager::LoadCatalog(const std::filesystem::path& ngc,
                                 const std::filesystem::path& names,
                                 const std::filesystem::path& planets) noexcept {
    const auto ngcData = arch::ReadFile(ngc);
    const auto nameData = arch::ReadFile(names);
    const auto planetData = arch::ReadFile(planets);
    return catalog.ImportFixed(ngcData, nameData) && catalog.ImportPlanets(planetData);
}

bool CatalogManager::LoadTextures(const std::filesystem::path& directory) noexcept {
    for (const auto& entry : std::filesystem::directory_iterator{ directory }) {
        if (entry.is_regular_file()) {
            const auto path = entry.path();
            auto texture = std::make_shared<graphics::Texture>();
            if (texture->LoadFromFile(path)) {
                const auto name = path.filename().string();
                textures[{ name.c_str(), name.find_last_of('.') }] = std::move(texture);
            }
        }
    }
    return !textures.empty();
}

std::shared_ptr<graphics::Texture> CatalogManager::FetchTexture(std::string_view identifier) noexcept {
    const auto copy = std::string{ identifier };
    if (textures.find(copy) != textures.end()) {
        return textures.at(copy);
    }
    if (textures.find("Default") != textures.end()) {
        return textures.at("Default");
    }
    return nullptr;
}

ephemeris::Catalog& CatalogManager::GetCatalog() noexcept {
    return catalog;
}