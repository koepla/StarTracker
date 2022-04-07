#include "Model.hpp"

#include <StarTracker/Core/AssetDataBase.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


namespace std {

	/*
	*   We need this ugly mess in order to use vertex optimization
	*/
	template<> struct hash<StarTracker::Core::OpenGL::ModelVertex> {

		std::size_t operator()(const StarTracker::Core::OpenGL::ModelVertex& vertex) const noexcept {

			return ((std::hash<glm::vec3>()(vertex.Position) ^
				   (std::hash<glm::vec3>()(vertex.Normal) << 1)) >> 1) ^
				   (std::hash<glm::vec2>()(vertex.TextureCoordinates) << 1);
		}
	};
}

namespace StarTracker::Core::OpenGL {

	Model::Model() noexcept : hasTexture{}, geometryInfo{}, filePath{}, indexBuffer{}, vertexBuffer{}, vertexArray{}, texture{} {

	}

	bool Model::LoadFromFile(const std::filesystem::path& filePath, bool invertedTexCoords) noexcept {

		hasTexture = false;
		this->filePath = filePath;

		tinyobj::attrib_t attrib{};
		std::vector<tinyobj::shape_t> shapes{};
		std::vector<tinyobj::material_t> materials{};
		std::vector<ModelVertex> vertices{};
		std::unordered_map<ModelVertex, std::uint32_t> uniqueVertices{};
		std::vector<std::uint32_t> indices{};
		std::string warning{};
		std::string error{};

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, filePath.string().c_str())) {

			STARTRACKER_ERROR("Couldn't load Model {}", filePath.string());
			return false;
		}

		for (const auto& shape : shapes) {

			for (const auto& index : shape.mesh.indices) {

				ModelVertex vertex{};
				vertex.Position = {

					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};
				const auto texCoordsY = attrib.texcoords[2 * index.texcoord_index + 1];
				vertex.Normal = {

					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				};
				vertex.TextureCoordinates = {

					attrib.texcoords[2 * index.texcoord_index + 0],
					invertedTexCoords ? 1.0f - texCoordsY : texCoordsY
				};

				if (uniqueVertices.count(vertex) == 0) {

					uniqueVertices[vertex] = static_cast<std::uint32_t>(vertices.size());
					vertices.emplace_back(vertex);
				}
				indices.emplace_back(uniqueVertices[vertex]);
			}
		}

		geometryInfo.IndexCount = indices.size();
		geometryInfo.VertexCount = vertices.size();

		STARTRACKER_INFO("Found {} indices in Model {}", geometryInfo.IndexCount, filePath.string());
		STARTRACKER_INFO("Found {} vertices in Model {}", geometryInfo.VertexCount, filePath.string());

		const static std::vector<Core::OpenGL::BufferElement> vertexBufferElements = {

			Core::OpenGL::BufferElement{ Core::OpenGL::ShaderDataType::Float3, "aPosition" },
			Core::OpenGL::BufferElement{ Core::OpenGL::ShaderDataType::Float3, "aNormal" },
			Core::OpenGL::BufferElement{ Core::OpenGL::ShaderDataType::Float2, "aTextureCoordinates" }
		};

		// BufferLayout containing the elements
		const static Core::OpenGL::BufferLayout vertexBufferLayout{ vertexBufferElements };

		vertexArray = std::make_shared<VertexArray>();
		vertexBuffer = std::make_shared<VertexBuffer>();
		indexBuffer = std::make_shared<IndexBuffer>();

		// Set up the VertexArray
		vertexArray->Bind();
		vertexBuffer->SetLayout(vertexBufferLayout);
		vertexBuffer->SetData(vertices.data(), static_cast<std::uint32_t>(vertices.size() * sizeof(OpenGL::TextureVertex)));
		indexBuffer->SetData(indices.data(), static_cast<std::uint32_t>(indices.size()));
		vertexArray->SetIndexBuffer(indexBuffer);
		vertexArray->SetVertexBuffer(vertexBuffer);

		return true;
	}

	bool Model::LoadFromFile(const std::filesystem::path& filePath, const std::filesystem::path& texturePath, bool invertedTexCoords) noexcept {

		const auto returnValue = LoadFromFile(filePath, invertedTexCoords);
		texture = Core::AssetDataBase::LoadTexture(texturePath);
		hasTexture = true;

		return returnValue;
	}

	const std::filesystem::path& Model::GetFilePath() const noexcept {

		return filePath;
	}

	const ModelGeometryInfo& Model::GetGeometryInfo() const noexcept {

		return geometryInfo;
	}

	const std::shared_ptr<IndexBuffer>& Model::GetIndexBuffer() const noexcept {

		return indexBuffer;
	}

	const std::shared_ptr<VertexBuffer>& Model::GetVertexBuffer() const noexcept {

		return vertexBuffer;
	}

	const std::shared_ptr<VertexArray>& Model::GetVertexArray() const noexcept {

		return vertexArray;
	}

	const std::shared_ptr<Texture>& Model::GetTexture() const noexcept {

		if (hasTexture) {

			return texture;
		}
		else {

			return nullptr;
		}
	}
}