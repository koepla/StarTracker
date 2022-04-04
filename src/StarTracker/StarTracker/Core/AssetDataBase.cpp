#include "AssetDataBase.hpp"

namespace StarTracker::Core {


	const std::shared_ptr<OpenGL::Shader>& AssetDataBase::LoadShader(const std::filesystem::path &vertexPath, const std::filesystem::path& fragmentPath, bool reload) noexcept {

		static const auto shaderRootPath = assetPath / std::filesystem::path{ "Shaders" };

		if (shaderMap.find(vertexPath.string()) != shaderMap.end() && !reload) {

			return shaderMap[vertexPath.string()];
		}
		else {

			STARTRACKER_INFO("Loading Shader {} {} from disk", vertexPath.string(), fragmentPath.string());
			auto shader = std::make_shared<OpenGL::Shader>();
			shader->LoadFromFile(shaderRootPath / vertexPath, shaderRootPath / fragmentPath);
			shaderMap[vertexPath.string()] = shader;

			return shaderMap[vertexPath.string()];
		}
	}

	const std::shared_ptr<OpenGL::Texture>& AssetDataBase::LoadTexture(const std::filesystem::path &filePath, bool reload) noexcept {

		static const auto textureRootPath = assetPath / std::filesystem::path{ "Textures" };

		if (textureMap.find(filePath.string()) != textureMap.end() && !reload) {

			return textureMap[filePath.string()];
		}
		else {

			STARTRACKER_INFO("Loading Texture {} from disk", filePath.string());
			auto texture = std::make_shared<OpenGL::Texture>();
			texture->LoadFromFile(textureRootPath / filePath);
			textureMap[filePath.string()] = texture;

			return textureMap[filePath.string()];
		}
	}

	const std::shared_ptr<OpenGL::Model>& AssetDataBase::LoadModel(const std::filesystem::path& filePath, bool reload) noexcept {

		static const auto modelRootPath = assetPath / std::filesystem::path{ "Models" };

		if (modelMap.find(filePath.string()) != modelMap.end() && !reload) {

			return modelMap[filePath.string()];
		}
		else {

			STARTRACKER_INFO("Loading Model {} from disk", filePath.string());
			auto model = std::make_shared<OpenGL::Model>();
			model->LoadFromFile(modelRootPath / filePath);
			modelMap[filePath.string()] = model;

			return modelMap[filePath.string()];
		}
	}

	const std::shared_ptr<OpenGL::Model>& AssetDataBase::LoadModel(const std::filesystem::path& filePath, const std::filesystem::path& texturePath, bool reload) noexcept {

		static const auto modelRootPath = assetPath / std::filesystem::path{ "Models" };

		if (modelMap.find(filePath.string()) != modelMap.end() && !reload) {

			return modelMap[filePath.string()];
		}
		else {

			STARTRACKER_INFO("Loading Model {} from disk", filePath.string());
			auto model = std::make_shared<OpenGL::Model>();
			model->LoadFromFile(modelRootPath / filePath, texturePath);
			modelMap[filePath.string()] = model;

			return modelMap[filePath.string()];
		}
	}

	const std::vector<std::shared_ptr<Ephemeris::CelestialBody>>& AssetDataBase::LoadCelestialBodies(const std::filesystem::path& filePath, bool reload) noexcept {

		static const auto celestialBodyRootPath = assetPath / std::filesystem::path{ "Ephemeris" };

		if (celestialBodyMap.find(filePath.string()) != celestialBodyMap.end() && !reload) {

			return celestialBodyMap[filePath.string()];
		}
		else {

			STARTRACKER_INFO("Loading CelestialBodies {} from disk", filePath.string());
			auto celestialBodies = Ephemeris::CelestialBody::LoadFromFile(celestialBodyRootPath / filePath);
			celestialBodyMap[filePath.string()] = celestialBodies;
			
			return celestialBodyMap[filePath.string()];
		}
	}
}