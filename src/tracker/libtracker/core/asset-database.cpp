#include "asset-database.hpp"

#include <stb/stb_image.h>

namespace {

    const std::filesystem::path assetPath{ std::filesystem::current_path() / "assets" };
    const std::filesystem::path shaderRootPath = assetPath / std::filesystem::path{ "shaders" };
    const std::filesystem::path textureRootPath = assetPath / std::filesystem::path{ "textures" };
    const std::filesystem::path ephemerisRootPath = assetPath / std::filesystem::path{ "ephemeris" };
    const std::filesystem::path iconRootPath = assetPath / std::filesystem::path{ "icons" };
}// namespace

std::shared_ptr<graphics::Shader> AssetDatabase::LoadShader(const std::filesystem::path& vertexPath,
                                                            const std::filesystem::path& fragmentPath,
                                                            bool reload) noexcept {
    if (shaderCache.find(vertexPath.string()) != shaderCache.end() && !reload) {
        return shaderCache[vertexPath.string()];
    }

    LIBTRACKER_INFO("Loading Shader {} {} from disk", vertexPath.filename().string(), fragmentPath.filename().string());
    const auto shader = std::make_shared<graphics::Shader>();
    shader->LoadFromFile(shaderRootPath / vertexPath, shaderRootPath / fragmentPath);
    shaderCache[vertexPath.string()] = shader;
    return shaderCache[vertexPath.string()];
}

std::shared_ptr<graphics::Texture> AssetDatabase::LoadTexture(const std::filesystem::path& filePath,
                                                              bool reload) noexcept {
    if (textureCache.find(filePath.string()) != textureCache.end() && !reload) {
        return textureCache[filePath.string()];
    }

    LIBTRACKER_INFO("Loading Texture {} from disk", filePath.filename().string());
    const auto texture = std::make_shared<graphics::Texture>();
    if (texture->LoadFromFile(textureRootPath / filePath)) {
        textureCache[filePath.string()] = texture;
        return textureCache[filePath.string()];
    }
    return nullptr;
}

void AssetDatabase::LoadCatalogManager(const std::filesystem::path& ngc,
                                       const std::filesystem::path& names,
                                       const std::filesystem::path& planets) noexcept {
    CatalogManager::LoadCatalog(ephemerisRootPath / ngc, ephemerisRootPath / names, ephemerisRootPath / planets);
    CatalogManager::LoadTextures(textureRootPath);
}

void AssetDatabase::LoadSettings(const std::filesystem::path& fileName) noexcept {
    LIBTRACKER_INFO("Loading settings {} from disk", fileName.string());
    Settings::LoadFromFile(assetPath / fileName);
}

void AssetDatabase::SaveSettings(const std::filesystem::path& fileName) noexcept {
    const auto data = Settings::GetSerializable();
    if (!arch::WriteFile(assetPath / fileName, data)) {
        LIBTRACKER_ERROR("Could not save settings file");
    }
}

GLFWimage AssetDatabase::LoadIcon(const std::filesystem::path& fileName) noexcept {
    GLFWimage icon{};
    icon.pixels = stbi_load((iconRootPath / fileName).string().c_str(), &icon.width, &icon.height, nullptr, 4);
    return icon;
}
