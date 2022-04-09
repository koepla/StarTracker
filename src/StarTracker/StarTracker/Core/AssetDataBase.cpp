#include "AssetDataBase.hpp"

namespace StarTracker::Core {


	std::shared_ptr<OpenGL::Shader> AssetDataBase::LoadShader(const std::filesystem::path &vertexPath, const std::filesystem::path& fragmentPath, bool reload) noexcept {

		static const auto shaderRootPath = assetPath / std::filesystem::path{ "Shaders" };

		if (shaderCache.find(vertexPath.string()) != shaderCache.end() && !reload) {

			return shaderCache[vertexPath.string()];
		}
		else {

			STARTRACKER_INFO("Loading Shader {} {} from disk", vertexPath.filename().string(), fragmentPath.filename().string());
			auto shader = std::make_shared<OpenGL::Shader>();
			shader->LoadFromFile(shaderRootPath / vertexPath, shaderRootPath / fragmentPath);
			shaderCache[vertexPath.string()] = shader;

			return shaderCache[vertexPath.string()];
		}
	}

	std::shared_ptr<OpenGL::Texture> AssetDataBase::LoadTexture(const std::filesystem::path &filePath, bool reload) noexcept {

		static const auto textureRootPath = assetPath / std::filesystem::path{ "Textures" };

		if (textureCache.find(filePath.string()) != textureCache.end() && !reload) {

			return textureCache[filePath.string()];
		}
		else {

			STARTRACKER_INFO("Loading Texture {} from disk", filePath.filename().string());
			auto texture = std::make_shared<OpenGL::Texture>();
			if (texture->LoadFromFile(textureRootPath / filePath)) {

				textureCache[filePath.string()] = texture;
				return textureCache[filePath.string()];
			}
			else {

				return nullptr;
			}
		}
	}

	std::shared_ptr<OpenGL::Model> AssetDataBase::LoadModel(const std::filesystem::path& filePath, bool reload) noexcept {

		static const auto modelRootPath = assetPath / std::filesystem::path{ "Models" };

		if (modelCache.find(filePath.string()) != modelCache.end() && !reload) {

			return modelCache[filePath.string()];
		}
		else {

			STARTRACKER_INFO("Loading Model {} from disk", filePath.filename().string());
			auto model = std::make_shared<OpenGL::Model>();
			if (model->LoadFromFile(modelRootPath / filePath)) {
				
				modelCache[filePath.string()] = model;
				return modelCache[filePath.string()];
			}
			else {

				return nullptr;
			}
		}
	}

	std::shared_ptr<OpenGL::Model> AssetDataBase::LoadModel(const std::filesystem::path& filePath, const std::filesystem::path& texturePath, bool reload) noexcept {

		static const auto modelRootPath = assetPath / std::filesystem::path{ "Models" };

		if (modelCache.find(filePath.string()) != modelCache.end() && !reload) {

			return modelCache[filePath.string()];
		}
		else {

			STARTRACKER_INFO("Loading Model {} from disk", filePath.filename().string());
			auto model = std::make_shared<OpenGL::Model>();
			if (model->LoadFromFile(modelRootPath / filePath)) {

				modelCache[filePath.string()] = model;
				return modelCache[filePath.string()];
			}
			else {

				return nullptr;
			}
		}
	}

	std::shared_ptr<BodyLibrary> AssetDataBase::LoadBodyLibrary(const std::filesystem::path& filePath, bool reload) noexcept {

		static const auto celestialBodyRootPath = assetPath / std::filesystem::path{ "Ephemeris" };

		if (bodyLibraryCache.find(filePath.string()) != bodyLibraryCache.end() && !reload) {

			return bodyLibraryCache[filePath.string()];
		}
		else {

			STARTRACKER_INFO("Loading CelestialBodyLibrary {} from disk", filePath.filename().string());
			auto library = std::make_shared<BodyLibrary>();
			library->BodyLibrary::LoadFromFile(celestialBodyRootPath / filePath);
			bodyLibraryCache[filePath.string()] = library;
			
			return bodyLibraryCache[filePath.string()];
		}
	}
}