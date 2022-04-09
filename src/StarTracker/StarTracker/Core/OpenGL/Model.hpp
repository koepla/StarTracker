#ifndef STARTRACKER_CORE_OPENGL_MODEL_H
#define STARTRACKER_CORE_OPENGL_MODEL_H

#include <StarTracker/Core/Core.hpp>
#include <StarTracker/Core/Assert.hpp>
#include <StarTracker/Core/OpenGL/Shader.hpp>
#include <StarTracker/Core/OpenGL/Texture.hpp>
#include <StarTracker/Core/OpenGL/VertexArray.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <filesystem>
#include <unordered_map>

namespace StarTracker::Core::OpenGL {

	struct ModelGeometryInfo {

		std::size_t IndexCount;
		std::size_t VertexCount;
	};

	struct ModelVertex {

		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TextureCoordinates;

		inline bool operator == (const ModelVertex& other) const noexcept {

			return Position == other.Position && Normal == other.Normal && TextureCoordinates == other.TextureCoordinates;
		}
	};

	class Model {

	private:
		bool hasTexture;
		ModelGeometryInfo geometryInfo;
		std::filesystem::path filePath;
		std::shared_ptr<IndexBuffer> indexBuffer;
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<VertexArray> vertexArray;
		std::shared_ptr<Texture> texture;

	public:
		Model() noexcept;
		[[nodiscard]] bool LoadFromFile(const std::filesystem::path& filePath, bool invertedTexCoords = false) noexcept;
		[[nodiscard]] bool LoadFromFile(const std::filesystem::path& filePath, const std::filesystem::path& texturePath, bool invertedTexCoords = false) noexcept;

		[[nodiscard]] const std::filesystem::path& GetFilePath() const noexcept;
		[[nodiscard]] const ModelGeometryInfo& GetGeometryInfo() const noexcept;
		[[nodiscard]] std::shared_ptr<IndexBuffer> GetIndexBuffer() const noexcept;
		[[nodiscard]] std::shared_ptr<VertexBuffer> GetVertexBuffer() const noexcept;
		[[nodiscard]] std::shared_ptr<VertexArray> GetVertexArray() const noexcept;
		[[nodiscard]] std::shared_ptr<Texture> GetTexture() const noexcept;
	};
}

#endif // STARTRACKER_CORE_OPENGL_MODEL_H