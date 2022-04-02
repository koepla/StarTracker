#include "Model.hpp"

#include <StarTracker/Core/AssetDataBase.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


namespace std {

    /*
    *   We need this ugly mess in order to use vertex optimization
    */
    template<> struct hash<StarTracker::Core::OpenGL::TextureVertex> {

        std::size_t operator()(const StarTracker::Core::OpenGL::TextureVertex& vertex) const noexcept {

            return ((std::hash<glm::vec3>()(vertex.Position) ^
                   (std::hash<glm::vec3>()(vertex.Color) << 1)) >> 1) ^
                   (std::hash<glm::vec2>()(vertex.TextureCoordinates) << 1);
        }
    };
}

namespace StarTracker::Core::OpenGL {

    Model::Model() noexcept : hasTexture{}, geometryInfo{}, filePath{}, indexBuffer{}, vertexBuffer{}, vertexArray{}, texture{} {

    }

    void Model::LoadFromFile(const std::filesystem::path& filePath, const glm::vec3& color, bool invertedTexCoords) noexcept {

        hasTexture = false;
        this->filePath = filePath;

        tinyobj::attrib_t attrib{};
        std::vector<tinyobj::shape_t> shapes{};
        std::vector<tinyobj::material_t> materials{};
        std::vector<TextureVertex> vertices{};
        std::unordered_map<TextureVertex, std::uint32_t> uniqueVertices{};
        std::vector<std::uint32_t> indices{};
        std::string warning{};
        std::string error{};

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, filePath.string().c_str())) {

            ASSERT(false && "Couldn't load model!");
        }

        for (const auto& shape : shapes) {

            for (const auto& index : shape.mesh.indices) {

                TextureVertex vertex{};
                vertex.Position = {

                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };
                const auto texCoordsY = attrib.texcoords[2 * index.texcoord_index + 1];
                vertex.TextureCoordinates = {

                    attrib.texcoords[2 * index.texcoord_index + 0],
                    invertedTexCoords ? 1.0f - texCoordsY : texCoordsY
                };
                vertex.Color = color;

                if (uniqueVertices.count(vertex) == 0) {

                    uniqueVertices[vertex] = static_cast<std::uint32_t>(vertices.size());
                    vertices.emplace_back(vertex);
                }
                indices.emplace_back(uniqueVertices[vertex]);
            }
        }

        geometryInfo.IndexCount = indices.size();
        geometryInfo.VertexCount = vertices.size();

        const static std::vector<Core::OpenGL::BufferElement> vertexBufferElements = {

            Core::OpenGL::BufferElement{ Core::OpenGL::ShaderDataType::Float3, "aPosition" },
            Core::OpenGL::BufferElement{ Core::OpenGL::ShaderDataType::Float3, "aColor" },
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
    }

    void Model::LoadFromFile(const std::filesystem::path& filePath, const std::filesystem::path& texturePath, bool invertedTexCoords) noexcept {

        LoadFromFile(filePath, { 1.0f, 1.0f, 1.0f }, invertedTexCoords);
        texture = Core::AssetDataBase::LoadTexture(texturePath);
        hasTexture = true;
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