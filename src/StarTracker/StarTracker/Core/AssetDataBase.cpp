#include "AssetDataBase.hpp"

namespace StarTracker::Core {


    const std::shared_ptr<OpenGL::Shader>& AssetDataBase::LoadShader(const std::filesystem::path &vertexPath, const std::filesystem::path& fragmentPath) noexcept {

        static const auto shaderRootPath = assetPath / std::filesystem::path{ "Shaders" };

        if(shaderMap.find(vertexPath.string()) != shaderMap.end()) {

            return shaderMap[vertexPath.string()];
        }
        else {

            auto shader = std::make_shared<OpenGL::Shader>();
            shader->LoadFromFile(shaderRootPath / vertexPath, shaderRootPath / fragmentPath);
            shaderMap[vertexPath.string()] = shader;

            return shaderMap[vertexPath.string()];
        }
    }

    const std::shared_ptr<OpenGL::Texture>& AssetDataBase::LoadTexture(const std::filesystem::path &filePath) noexcept {

        static const auto textureRootPath = assetPath / std::filesystem::path{ "Textures" };

        if(textureMap.find(filePath.string()) != textureMap.end()) {

            return textureMap[filePath.string()];
        }
        else {

            auto texture = std::make_shared<OpenGL::Texture>();
            texture->LoadFromFile(textureRootPath / filePath);
            textureMap[filePath.string()] = texture;

            return textureMap[filePath.string()];
        }
    }
}